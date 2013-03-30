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
#include "globaltreeoperationshandler.h"
#include "treerenamecreatedlg.h"
#include "rootoftree.h"
#include "treeleaf.h"
#include "../CommonInclude/pdb/VariantPtr.h"
#include "dbacccesssafe.h"
#include "treestatisticdlg.h"
//
#include "../CommonInclude/pdb/pdb_style.h"
#include "logger.h"

//
#include <QMessageBox>
#include <QtSql>
//
GlobalTreeOperationsHandler::GlobalTreeOperationsHandler(QObject *parent) :
    QObject(parent)
{

}

void GlobalTreeOperationsHandler::setPtrToComboBox(QComboBox* ptr_combo)
{
    m_ptrCombo = ptr_combo;
}

void GlobalTreeOperationsHandler::setPtrToTree (MyTree* ptr_to_tree)
{
    m_ptrTree = ptr_to_tree;
}

void GlobalTreeOperationsHandler::setPtrToStatusBar (QStatusBar*    ptr_to_statusbar)
{
    m_ptrStatusBar = ptr_to_statusbar;
}

void    GlobalTreeOperationsHandler::onCreateNewTree()
{
    TreeRenameCreateDlg dlg;
    dlg.setDlgMode(TreeRenameCreateDlg::CREATE_DLG);
    RootOfTree*     p_root_of_tree = NULL;
    //
    if ( QDialog::Accepted == dlg.exec() )
    {
        p_root_of_tree = createNewTree_DB( dlg.getTreeName(), dlg.getRootNodeName() );
    };
    //
    if ( NULL != p_root_of_tree )
    {
        QVariant new_tree_root = VariantPtr<RootOfTree>::asQVariant(p_root_of_tree);
        m_ptrCombo->addItem(p_root_of_tree->getName(), new_tree_root);
        m_ptrCombo->setCurrentIndex(m_ptrCombo->count() - 1);
    };
}

void GlobalTreeOperationsHandler::onRenameCurrentTree()
{
    TreeRenameCreateDlg dlg;
    dlg.setDlgMode(TreeRenameCreateDlg::RENAME_DLG);
    //
    dlg.setTreeName(m_ptrCombo->currentText());
    int             i_res = dlg.exec();
    //
    if (QDialog::Accepted == i_res)
    {
        //rename root object
        QVariant root_back_data = m_ptrCombo->itemData(m_ptrCombo->currentIndex());
        RootOfTree* ptr_root = VariantPtr<RootOfTree>::asPtr( root_back_data );
        if (ptr_root)
        {
            ptr_root->rename_it( dlg.getTreeName() );
            //change text in combobox
            m_ptrCombo->setItemText(m_ptrCombo->currentIndex(), dlg.getTreeName());
        };
    };
}

void GlobalTreeOperationsHandler::onDeleteTree()
{
    const QString str_tree_name = m_ptrCombo->currentText();
    //
    QString str_message_text = tr("<P align=\"center\"><FONT COLOR='#800000'>If you click 'Yes' now,");
    str_message_text += tr("<br>the entire tree <br><br>");
    str_message_text += str_tree_name;
    str_message_text += tr("<br><br>will be deleted. Undo is not possible.");
    str_message_text += tr("<br>Are you REALLY sure?</FONT> </P>");

    const int i_res = QMessageBox::warning(NULL,
                                     trUtf8("WARNING!") ,
                                     str_message_text,
                                     QMessageBox::Yes|QMessageBox::No,
                                     QMessageBox::No
                                    );
    if (QMessageBox::Yes != i_res)
        return;
    //
    //delete it
    //
    QVariant root_back_data = m_ptrCombo->itemData( m_ptrCombo->currentIndex() );
    RootOfTree* ptr_new_root= VariantPtr<RootOfTree>::asPtr( root_back_data );
    //
    if (ptr_new_root)
    {
        //detach root node from the tree
        m_ptrTree->takeTopLevelItem( 0 );
        //
        ptr_new_root->delete_it();
        //delete from combobox
        m_ptrCombo->removeItem(m_ptrCombo->currentIndex());
        //
        delete (ptr_new_root);
        m_ptrStatusBar->showMessage(tr("tree ") + str_tree_name + tr(" deleted."));
    };
}

void GlobalTreeOperationsHandler::onInfoAboutTree()
{
    unsigned int    node_amount         = 0;
    unsigned int    attachments_amount  = 0;
    unsigned int    attachments_size    = 0;
    //
    QVariant root_back_data = m_ptrCombo->itemData(m_ptrCombo->currentIndex());
    RootOfTree* ptr_root = VariantPtr<RootOfTree>::asPtr( root_back_data );
    //
    requestTreeInfo_DB ( node_amount, attachments_amount, attachments_size, ptr_root->getID() );
    //create dialog window, show it.
    TreeStatisticDlg dlg;
    //
    dlg.setNodeAmount(node_amount);
    dlg.setAttachmentsAmount(attachments_amount);
    dlg.setAttachmentsSize(attachments_size);
    //
    dlg.exec();
    //
    return;
}

void GlobalTreeOperationsHandler::requestTreeInfo_DB (unsigned int& node_amount,
                                                      unsigned int& attachments_amount,
                                                      unsigned int& attachments_size,
                                                      unsigned int tree_id)
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    Q_ASSERT(ptr_db);
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //not const because used twice (later)
    QString str_select_str = QString ("select SUM(attachments.attach_size), count(attachments.attach_size) from attachments, node_tbl WHERE attachments.id_parent=node_tbl.id_node AND node_tbl.id_tree = %1;").
                                   arg(tree_id);
    //
    if (!qry.prepare( str_select_str ))
    {
        const QString str_err = qry.lastError().text();
        Logger::getInstance().logIt( en_LOG_ERRORS, str_err, &str_select_str );
        return;

    } else if( !qry.exec() )
    {
        const QString str_err = qry.lastError().text();
        Logger::getInstance().logIt( en_LOG_ERRORS, str_err, &str_select_str );
        return;
    }else
    {
        while( qry.next() )
        {
            attachments_size     = qry.value(0).toInt();
            attachments_amount   = qry.value(1).toInt();
            break;
        }; // while( qry.next() )
    }; //if (!qry.prepare( str_select_str ))
    //
    str_select_str = QString("SELECT count(id_node) FROM node_tbl WHERE id_tree = %1;").arg(tree_id);
    //
    if (!qry.prepare( str_select_str ))
    {
        const QString str_err = qry.lastError().text();
        Logger::getInstance().logIt( en_LOG_ERRORS, str_err, &str_select_str );
        return;

    } else if( !qry.exec() )
    {
        const QString str_err = qry.lastError().text();
        Logger::getInstance().logIt( en_LOG_ERRORS, str_err, &str_select_str );
        return;
    }else
    {
        while( qry.next() )
        {
            node_amount     = qry.value(0).toInt();
            break;
        }; // while( qry.next() )
    }; //if (!qry.prepare( str_select_str ))
    //
    return;
}

RootOfTree* GlobalTreeOperationsHandler::createNewTree_DB(const QString& str_tree_name, const QString& str_root_node_name)
{
    //
    QMessageBox box;
    QString     str_msg;
    //create the new RootOfTree
    RootOfTree* ptr_root = NULL;
    ptr_root = new RootOfTree(str_tree_name, -1, QDateTime::currentDateTime(), true);
    //get the ID
    int i_tree_id = ptr_root->getID();
    if (-1 == i_tree_id)
    {
        str_msg = tr("Can not create new tree. Database error.");
        Logger::getInstance().logIt(en_LOG_ERRORS, str_msg);
        box.setText(str_msg);
        box.exec();
        return NULL;
    };
    //create new TreeLeaf (use constructor "create from interface"
    TreeLeaf* ptr_new_leaf = new TreeLeaf(NULL, -1, i_tree_id, str_root_node_name, m_ptrTree );
    if (ptr_new_leaf->getID() == -1)
    {
        str_msg = "Can not create new tree. Database error.";
        Logger::getInstance().logIt(en_LOG_ERRORS, str_msg);
        box.setText(str_msg);
        box.exec();
        return NULL;
    }
    //link root_of_tree and root node addChildLeaf
    ptr_root->addChildLeaf(ptr_new_leaf);
    return ptr_root;
}
