//
/*
 This file is part of project pdb.

    pdb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    pdb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pdb.  If not, see <http://www.gnu.org/licenses/>.
*/
//
#include "searcher.h"
#include "mytree.h"
#include "rootoftree.h"
#include "../CommonInclude/pdb/VariantPtr.h"
//
//Class make a search in the tree(s), nodes and attachments.
//
#include <QMutexLocker>


Searcher::Searcher() : QThread()
{
    m_bContinueSearch = true;
}

void Searcher::setPtrToCombo      ( QComboBox* ptr_combo )
{
    m_ptrCombo = ptr_combo;
}

void Searcher::setPtrToTree       ( MyTree* ptr_tree )
{
    m_ptrTree = ptr_tree;
}

void  Searcher::setSearchConditions(const SearchRequest& request)
{
    m_SearchRequest = request;
}

void Searcher::cancelSearch       ()
{
    QMutexLocker locker (&m_mContinueLocker);
    m_bContinueSearch = false;
}

bool Searcher::resetContinueFlag  ()
{
    QMutexLocker locker (&m_mContinueLocker);
    m_bContinueSearch = true;
    return true;
}

bool Searcher::isOKToContinueSearch()
{
    QMutexLocker locker (&m_mContinueLocker);
    //
    return ( true == m_bContinueSearch);
}

bool Searcher::isOKToStartSearch()
{
    if (
        ( isOKToContinueSearch() == true )                  &&
        (m_SearchRequest.getSearchPhrase().length() > 0)    &&
        m_SearchRequest.isSearchAreaDefined()               &&
        ( NULL != m_ptrCombo)
       )
    {
        return true;
    };

    return false;
}

void Searcher::run ()
{
    if (isOKToStartSearch() == false)
    {
        resetContinueFlag();
        return;
    };
    //
    emit beginOfSearch();
    //
    unsigned int ui_nodes_amount = getTotalNodesAmount();
    emit getTotalNodesAmount(ui_nodes_amount);
    //
    search();
    //
    resetContinueFlag();
    emit endOfSearch();
}

void Searcher::search ()
{
    QRegExp rx;
    //
    if ( m_SearchRequest.isRegularExp() )
    {
        rx.setPattern       ( m_SearchRequest.getSearchPhrase() );
        rx.setPatternSyntax ( (QRegExp::PatternSyntax) m_SearchRequest.getRegExpSyntax() );
        rx.setMinimal       ( m_SearchRequest.isEnableMinMatching() );
    };
    //
    //
    if ( m_SearchRequest.isCurrentTreeOnly() ) // seacrh only in the current tree
    {
        //get the actual tree root node
        QVariant root_back_data     = m_ptrCombo->itemData( m_ptrCombo->currentIndex() );
        RootOfTree* ptr_root        = VariantPtr<RootOfTree>::asPtr( root_back_data );
        Q_ASSERT ( ptr_root );
        TreeLeaf* ptr_root_node     = ptr_root->getChildLeaf();
        //
        recursiveSearchInTree(ptr_root_node, rx);
    }else
    {
        for (int i = 0; i < m_ptrCombo->count(); i++)
        {
            QVariant root_back_data = m_ptrCombo->itemData(i);
            RootOfTree* ptr_root = VariantPtr<RootOfTree>::asPtr( root_back_data );
            Q_ASSERT ( ptr_root );
            TreeLeaf* ptr_root_node     = ptr_root->getChildLeaf();
            Q_ASSERT ( ptr_root_node );
            //
            recursiveSearchInTree(ptr_root_node, rx);
            if (isOKToContinueSearch() == false)
                break;
        };
    };
}

bool Searcher::isTimingOKToSearchInThisObject (const AbstractDatabaseObject *ptr_object) const
{
    bool b_is_ok_to_search = true;
    //
    if (NULL == ptr_object)
        return false;
    //
    const QDateTime* ptr_begin_time   = m_SearchRequest.getBeginSearchDateTime();
    const QDateTime* ptr_end_time     = m_SearchRequest.getEndSearchDateTime  ();
    //
    if (NULL != ptr_begin_time )
    {
        if (ptr_object->getDateTime() < (*ptr_begin_time))
            b_is_ok_to_search = false;
    };
    //
    if ( ( true == b_is_ok_to_search) &&(NULL != ptr_end_time) )
    {
        if (ptr_object->getDateTime() > (*ptr_end_time))
            b_is_ok_to_search = false;
    };
    //
    return b_is_ok_to_search;
}

void Searcher::recursiveSearchInTree (TreeLeaf* ptr_root, QRegExp &rx)
{
    //
    //first of all we check time interval. if node is not in - do not make search
    //in the node (names and descriptions)
    //
    bool b_search_in_this_node = isTimingOKToSearchInThisObject(ptr_root);
    //
    //
    if (b_search_in_this_node)
    {
        if (m_SearchRequest.isSearchInNodeNames())
            searchInNodeName(ptr_root, rx);
        //
        if (m_SearchRequest.isSearchInNodeDescriptor())
            searchInNodeDescriptor(ptr_root, rx);
    };
    //
    // timing for attachments checked per every attachment
    //
    if (m_SearchRequest.isSearchInAttachmentNames())
        searchInAttachNames(ptr_root, rx);
    //
    if (m_SearchRequest.isSearchInAtttachmentBody())
        searchInAttachBody(ptr_root, rx);
    //
    if (isOKToContinueSearch() == false)
        return;
    //
    //call for childs
    for (int i = 0; i < ptr_root->childCount(); i++)
    {
        TreeLeaf* ptr_actual = (TreeLeaf*) ptr_root->child(i);
        Q_ASSERT ( ptr_actual );
        //
        emit jumpToNextNode();
        //
        recursiveSearchInTree(ptr_actual, rx);
        //
        if (isOKToContinueSearch() == false)
            break;
    };
};

void Searcher::searchInNodeName       (TreeLeaf* ptr_root, QRegExp& rx)
{
    const QString str_name = ptr_root->text(0);
    //
    bool b_res = searchInAnyText (str_name, rx);
    //
    if (true == b_res)
        emit foundNode (ptr_root,  enNODE_NAMES);
};

void Searcher::searchInNodeDescriptor (TreeLeaf* ptr_root, QRegExp& rx)
{
    const QString str_descriptor = ptr_root->getDescriptor().toPlainText();
    //
    bool b_res = searchInAnyText (str_descriptor, rx);
    //
    if (true == b_res)
        emit foundNode (ptr_root,  enNODE_DESCRIPTOR);
};

void Searcher::searchInAttachNames    (TreeLeaf* ptr_root, QRegExp& rx)
{
    const Attachment::AttachmentsList& attachments = ptr_root->getAttachments();
    //
    Attachment::AttachmentsList::const_iterator itr = attachments.begin();
    //
    for (; itr != attachments.end(); ++itr)
    {
        const QString& str_att_name = (*itr)->getName();
        //
        bool b_res = isTimingOKToSearchInThisObject( (*itr) );
        //
        if (b_res)
            b_res = searchInAnyText (str_att_name, rx);
        else
            continue;
        //
        if (true == b_res)
            emit foundNode (ptr_root,  enATT_NAMES);
    }
}

void Searcher::searchInAttachBody     (TreeLeaf* ptr_root, QRegExp &rx)
{
    const Attachment::AttachmentsList& attachments = ptr_root->getAttachments();
    //
    Attachment::AttachmentsList::const_iterator itr = attachments.begin();
    //
    for (; itr != attachments.end(); ++itr)
    {
        if ((*itr)->is_binary() == false)
        {
            bool b_res = isTimingOKToSearchInThisObject( (*itr) );
            //
            if (b_res)
            {
                const QString str_attachment_body ( (*itr)->getBlob() );
                b_res = searchInAnyText (str_attachment_body, rx);
            }else
                continue;
            //
            if (true == b_res)
                emit foundNode (ptr_root,  enATT_BODY);
        };
    };
}

bool Searcher::searchInAnyText (const QString &str_target_pfrase, QRegExp& rx)
{
    if ( m_SearchRequest.isRegularExp() )
    {
        if (m_SearchRequest.getRegExpSyntax() == QRegExp::Wildcard)
        {
            bool b_res = rx.exactMatch(str_target_pfrase);
            return b_res;
        }else
        {
            int i_index = rx.indexIn(str_target_pfrase);
            if (-1 != i_index)
                return true;
        };

        //
        return false;
    };
    // ------------------------------------ change it late completely. ------------------------------------
    if (m_SearchRequest.isWholeWord())
    {
        if (str_target_pfrase.compare(m_SearchRequest.getSearchPhrase(),m_SearchRequest.getCaseSensitivity()) == 0 )
        {
            return true;
        };
        //
        return false;
    };
    // ------------------------------------ change it later completely. ------------------------------------
    //if it is not regexp and it is t not wholeword search (change it later)
    //
    int i_index = str_target_pfrase.indexOf(m_SearchRequest.getSearchPhrase(),0,m_SearchRequest.getCaseSensitivity());
    //
    if (-1 != i_index)
    {
        return true;
    };
    //
    return false;
};

unsigned int Searcher::getTotalNodesAmount  ()
{
    unsigned int ui_nodes_amount = 0;
    //
    if ( m_SearchRequest.isCurrentTreeOnly() ) // seacrh only in the current tree
    {
        //get the actual tree root node
        QVariant root_back_data     = m_ptrCombo->itemData( m_ptrCombo->currentIndex() );
        RootOfTree* ptr_root        = VariantPtr<RootOfTree>::asPtr( root_back_data );
        Q_ASSERT ( ptr_root );
        TreeLeaf* ptr_root_node     = ptr_root->getChildLeaf();
        Q_ASSERT ( ptr_root_node );
        //
        getNodesAmountPerTree(ptr_root_node, ui_nodes_amount);
        ui_nodes_amount++; //because of root nodeui_nodes_amount
    }else
    {
        for (int i = 0; i < m_ptrCombo->count(); i++)
        {
            QVariant root_back_data = m_ptrCombo->itemData(i);
            RootOfTree* ptr_root = VariantPtr<RootOfTree>::asPtr( root_back_data );
            Q_ASSERT ( ptr_root );
            TreeLeaf* ptr_root_node     = ptr_root->getChildLeaf();
            Q_ASSERT ( ptr_root_node );
            //
            getNodesAmountPerTree(ptr_root_node, ui_nodes_amount);
            ui_nodes_amount++; //because of root nodeui_nodes_amount
        };
    };
    //
    return ui_nodes_amount;
};

void Searcher::getNodesAmountPerTree        ( TreeLeaf* ptr_root, unsigned int& ui_already_calc )
{
    ui_already_calc += ptr_root->childCount();
    //
    for (int i = 0; i < ptr_root->childCount(); i++)
    {
        TreeLeaf* ptr_actual = (TreeLeaf*) ptr_root->child(i);
        Q_ASSERT ( ptr_actual );
        getNodesAmountPerTree(ptr_actual, ui_already_calc);
    };
};
