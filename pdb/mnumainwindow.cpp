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
#include "mnumainwindow.h"
#include "mainwindow.h"
#include "treeleaf.h"
#include "servicescfg.h"
//
#include "../CommonInclude/pdb/pdb_style.h"
#include "../CommonInclude/pdb/DBSettings.h"
//
#include <QMenuBar>
#include <QSettings>

MnuMainWindow::MnuMainWindow( MainWindow* parent ) :
    QObject(parent)
{
    //
    Q_ASSERT ( parent );
    m_ptrParent             = parent;
    m_bPasswordExist        = false;
    //
    m_ptrTreeToolBar        = NULL;
    m_ptrAttachmentToolBar  = NULL;
    m_ptrNodeToolBar        = NULL;
    m_ptrEditorToolBar      = NULL;
    m_ptrSecurityToolBar    = NULL;
    //
    createTreeControlMenu       ();
    createNodeControlMenu       ();
    createEditorMenu            ();
    createTableSubMenu          ();
    createAttachmentControlMenu ();
    createCryptograficMenu      ();
    createProtectionMenu        ();
    createToolBarSubMenu        ();
    createSecurityMenu          ();
    createHelpMenu              ();
    //
    assemblyAllMenus            ();
    //
    QObject::connect(m_ptrMnuTreeToolBar,       SIGNAL(triggered()), this, SLOT (onTriggerTreeToolBox()       ));
    QObject::connect(m_ptrMnuNodeToolBar,       SIGNAL(triggered()), this, SLOT (onTriggerNodeToolBox()       ));
    QObject::connect(m_ptrMnuAttachmentToolBar, SIGNAL(triggered()), this, SLOT (onTriggerAttachmentToolBox() ));
    QObject::connect(m_ptrMnuEditorToolBar,     SIGNAL(triggered()), this, SLOT (onTriggerEditToolBox      () ));
    QObject::connect(m_ptrMnuTableToolBar,      SIGNAL(triggered()), this, SLOT (onTriggerTableToolBox     () ));
    QObject::connect(m_ptrMnuSecurityToolBar,   SIGNAL(triggered()), this, SLOT (onTriggerSecurityToolBox  () ));
}

MnuMainWindow::~MnuMainWindow()
{
/*
    if ( m_ptrCreateNewTree ) delete m_ptrCreateNewTree;
    if ( m_ptrDelTree       ) delete m_ptrDelTree;
    //if ( m_ptrRestoreTree   ) delete m_ptrRestoreTree;
    if ( m_ptrRenameTree    ) delete m_ptrRenameTree;
    if ( m_ptrInfoTree      ) delete m_ptrInfoTree;
    if ( m_ptrSearchInTree  ) delete m_ptrSearchInTree;
    if ( m_ptrExitApp       ) delete m_ptrExitApp;
    //-----
    if ( m_ptrDelNode       ) delete m_ptrDelNode;
    if ( m_ptrRestoreNode   ) delete m_ptrRestoreNode;
    if ( m_ptrRestoreSubtree) delete  m_ptrRestoreSubtree;
    if ( m_ptrInsertNewNode ) delete m_ptrInsertNewNode;
    //
    if ( m_ptrCutNode       ) delete m_ptrCutNode;
    if ( m_ptrPasteNode     ) delete m_ptrPasteNode;
    //
    if ( m_ptrExportNode    ) delete  m_ptrExportNode;
    if ( m_ptrImportNode    ) delete  m_ptrImportNode;
    //
    if ( m_ptrEncryptAttachmentsOfAllNodes ) delete  m_ptrEncryptAttachmentsOfAllNodes;
    if ( m_ptrDecryptAttachmentsOfAllNodes ) delete  m_ptrDecryptAttachmentsOfAllNodes;
    //
    if ( m_ptrProtectAttachmentsOfAllNodes)     delete  m_ptrProtectAttachmentsOfAllNodes;
    if ( m_ptrUn_ProtectAttachmentsOfAllNodes)  delete  m_ptrUn_ProtectAttachmentsOfAllNodes;
    //
    if ( m_ptrSaveNodeDescriptor ) delete m_ptrSaveNodeDescriptor;
    //
    if ( m_ptrExportNode  ) delete m_ptrExportNode;
    if ( m_ptrImportNode  ) delete m_ptrImportNode;
    //---
    if ( m_ptrRestoreAttach ) delete  m_ptrRestoreAttach;
    if ( m_ptrDeleteAttach  ) delete  m_ptrDeleteAttach;
    if ( m_ptrExportAttach  ) delete  m_ptrExportAttach;
    if ( m_ptrImportAttach  ) delete  m_ptrImportAttach;
    if ( m_ptrReplaceAttach ) delete  m_ptrReplaceAttach;
    //
    if ( m_ptrCutAttach     ) delete  m_ptrCutAttach;
    if ( m_ptrPasteAttach   ) delete  m_ptrPasteAttach;
    //
    if ( m_ptrViewAttach   ) delete   m_ptrViewAttach;
    //----
    if ( m_ptrProtectAllAttachmentsOfNode)    delete     m_ptrProtectAllAttachmentsOfNode;
    if ( m_ptrUn_ProtectAllAttachmentsOfNode)   delete   m_ptrUn_ProtectAllAttachmentsOfNode;
    if ( m_ptrProtectSelectedAttachments)       delete   m_ptrProtectSelectedAttachments;
    if ( m_ptrUn_ProtectSelectedAttachments)    delete   m_ptrUn_ProtectSelectedAttachments;
    //
    //if ( m_ptrRefreshAttach ) delete  m_ptrRefreshAttach;
*/

}

void MnuMainWindow::onChangeEditText (bool b_flag)
{
    m_ptrPrintPreview   ->setEnabled(b_flag);
    m_ptrSaveToFile     ->setEnabled(b_flag);
    m_ptrPrint          ->setEnabled(b_flag);
    m_ptrExportToPdf    ->setEnabled(b_flag);
}

void MnuMainWindow::onTriggerTreeToolBox ()
{
    if ( m_ptrTreeToolBar->isVisible() )
    {
        m_ptrTreeToolBar->hide();
        m_ptrMnuTreeToolBar->setChecked(false);
    }else
    {
        m_ptrTreeToolBar->show();
        m_ptrMnuTreeToolBar->setChecked(true);
    };
}

void MnuMainWindow::onTriggerNodeToolBox()
{
    if ( m_ptrNodeToolBar->isVisible() )
    {
        m_ptrNodeToolBar->hide();
        m_ptrMnuNodeToolBar->setChecked(false);
    }else
    {
        m_ptrNodeToolBar->show();
        m_ptrMnuNodeToolBar->setChecked(true);
    };
}

void MnuMainWindow::onTriggerAttachmentToolBox()
{
    if ( m_ptrAttachmentToolBar->isVisible() )
    {
        m_ptrAttachmentToolBar->hide();
        m_ptrMnuAttachmentToolBar->setChecked(false);
    }else
    {
        m_ptrAttachmentToolBar->show();
        m_ptrMnuAttachmentToolBar->setChecked(true);
    };
}

void MnuMainWindow::onTriggerEditToolBox ()
{
    if ( m_ptrEditorToolBar->isVisible() )
    {
        m_ptrEditorToolBar->hide();
        m_ptrMnuEditorToolBar->setChecked(false);
    }else
    {
        m_ptrEditorToolBar->show();
        m_ptrMnuEditorToolBar->setChecked(true);
    };
}

void MnuMainWindow::onTriggerTableToolBox ()
{
    if ( m_ptrTableToolBar->isVisible() )
    {
        m_ptrTableToolBar->hide();
        m_ptrMnuTableToolBar->setChecked(false);
    }else
    {
        m_ptrTableToolBar->show();
        m_ptrMnuTableToolBar->setChecked(true);
    };
}

void MnuMainWindow::onTriggerSecurityToolBox ()
{
    if ( m_ptrSecurityToolBar->isVisible() )
    {
        m_ptrSecurityToolBar->hide();
        m_ptrMnuSecurityToolBar->setChecked(false);
    }else
    {
        m_ptrSecurityToolBar->show();
        m_ptrMnuSecurityToolBar->setChecked(true);
    };
}

//-------------------------- signal processing begin  ----------------------------------------
void  MnuMainWindow::onCheckPassword ( bool b_password_exist )
{
    m_ptrLockAppScreen      ->setEnabled(false);
    m_bPasswordExist = b_password_exist;
    //
    if (m_bPasswordExist)
    {
        if ( ServicesCfg::getInstance().getPassword().length() == 0 )
            m_ptrCreateChangePassword->setText ( "Enter password..." );
        else
        {
            m_ptrLockAppScreen      ->setEnabled(true);
            m_ptrCreateChangePassword->setText ( "Change password..." );
        };
    }
    else
    {
        m_ptrCreateChangePassword->setText ( "Create protection password..." );
    }

}

void MnuMainWindow::showRightClickPopupNodeMenu()
{
    QMenu popup_menu(m_ptrParent);
    //
    assemblyNodeMenu(&popup_menu);
    //
    popup_menu.addSeparator();
    popup_menu.addAction(m_ptrImportAttach);
    //
    popup_menu.addSeparator();
    popup_menu.addMenu(m_ptrNodeCryptoSubMenu); //aready created initially
    //
    popup_menu.addSeparator();
    popup_menu.addMenu(m_ptrNodeProtectSubMenu); //aready created initially
    //
    popup_menu.exec(QCursor::pos());
}

void MnuMainWindow::showRightClickPopupAttachMenu()
{
    QMenu popup_menu(m_ptrParent);
    //
    assemblyAttachMenu(&popup_menu);
    //
    popup_menu.addSeparator();
    popup_menu.addMenu(m_ptrCryptography);
    //
    popup_menu.addSeparator();
    popup_menu.addMenu(m_ptrAttachProtectionSubMenu); //aready created initially
    //
    popup_menu.exec(QCursor::pos());
};

void MnuMainWindow::onSaveDescription()
{
    m_ptrSaveNodeDescriptor->setEnabled(false);
}

void MnuMainWindow::onChangeDescription()
{
    m_ptrSaveNodeDescriptor->setEnabled(true);
}

void  MnuMainWindow::onTreeComboBoxChanged(int index)
{
    if (-1 == index)
    {
        m_ptrDelTree            ->setEnabled(false);
        m_ptrRenameTree         ->setEnabled(false);
        m_ptrInfoTree           ->setEnabled(false);
        m_ptrSearchInTree       ->setEnabled(false);
    }else
    {
        m_ptrDelTree            ->setEnabled(true);
        m_ptrInfoTree           ->setEnabled(true);
        m_ptrRenameTree         ->setEnabled(true);
        m_ptrSearchInTree       ->setEnabled(true);
    };
}
//
void MnuMainWindow::onSelectedAttachmentChanged(Attachment::AttachmentsList v_attachment)
{
    m_ptrRestoreAttach ->setEnabled(false);
    m_ptrDeleteAttach  ->setEnabled(false);
    m_ptrExportAttach  ->setEnabled(false);
    //m_ptrRefreshAttach ->setEnabled(true);
    //m_ptrImportAttach  ->setEnabled(false); defined by node status, not by attachments
    m_ptrReplaceAttach ->setEnabled(false);
    //
    m_ptrCutAttach     ->setEnabled(false);
    m_ptrViewAttach    ->setEnabled(false);
    //
    AttachStatusChecker checker;
    //
    analyseAttachmentsStatus (v_attachment, checker);
    //----------------
    adjustProtectionMenuForSelectedAttacments     ( checker );
    adjustCryptoMenuForSelectedAttacments ( checker );
    //
    //define protection state for all attachments of current node
    if ( v_attachment.size() > 0 )
    {
        TreeLeaf* ptr_parent = v_attachment[0]->getParentLeaf();
        //
        adjustProtectionMenuForAllAttacmentsOfNode  ( ptr_parent );
        adjustCryptographyMenuForAllAttacmentsOfNode( ptr_parent );
        //
        AttachStatusChecker checker;
        adjustGlobalProtectionMenu  (ptr_parent, checker);
        adjustGlobalCryptographyMenu(ptr_parent, checker);
    };
    //
    setAccessExportCutViewAttach  (checker);
    setAccessDelReplRestoreAttach (checker);
    //
    return;
}

void MnuMainWindow::setAccessDelReplRestoreAttach ( const AttachStatusChecker& checker )
{
    if (
            //(true == checker.m_bAtLeastOneAttachmentIsOK)           &&
            (false == checker.m_bAtLeastOneAttachmentOKAndProtected)
       )
    {
        m_ptrDeleteAttach  ->setEnabled(true);
        //
        if (1  == checker.m_iAttachmentsAmount)
        {
            m_ptrReplaceAttach->setEnabled(true);
        };
    };
    //
    if (checker.m_bAtLeastOneAttachmentIsDeleted & checker.m_bParentNodeHasOkStatus)
        m_ptrRestoreAttach  ->setEnabled(true);
}

void MnuMainWindow::setAccessExportCutViewAttach(const AttachStatusChecker &checker)
{
    if ( checker.m_iAttachmentsAmount > 0 )
    {
        m_ptrExportAttach  ->setEnabled(true);
        //
        if (    checker.m_bAtLeastOneAttachmentIsOK                     &&
                (false == checker.m_bAtLeastOneAttachmentIsDeleted)     &&
                (false == checker.m_bAtLeastOneAttachmentOKAndProtected)&&
                checker.m_bParentNodeHasOkStatus
           )
        {
            m_ptrCutAttach     ->setEnabled(true);
        }else
        {
            m_ptrCutAttach     ->setEnabled(false);
        };
        //
        if (false == checker.m_bAtLeastOneAttachmentIsNotReady)
        {
            m_ptrViewAttach     ->setEnabled(true);
        }else
        {
            m_ptrViewAttach     ->setEnabled(false);
        };
    };
}

void MnuMainWindow::analyseAttachmentsStatus (const Attachment::AttachmentsList&  v_attachment,
                                              AttachStatusChecker&              checker)
{
    bool b_parent_node_status_chacked = false;
    //
    Attachment::AttachmentsList::const_iterator itr;
    //
    for (itr = v_attachment.begin(); itr != v_attachment.end(); ++itr )
    {
        Attachment* ptr_to_attach = (*itr);
        Q_ASSERT (ptr_to_attach);
        //
        if (ptr_to_attach->getObjectStatus() == AbstractDatabaseObject::OBJECT_DELETED)
        {
            checker.m_bAtLeastOneAttachmentIsDeleted = true;

        } else if (ptr_to_attach->getObjectStatus() ==  AbstractDatabaseObject::OBJECT_OK)
        {
            checker.m_bAtLeastOneAttachmentIsOK = true;
            //---------------------------------
            if (ptr_to_attach->is_protected())
                checker.m_bAtLeastOneAttachmentOKAndProtected = true;
            else
                checker.m_bAtLeastOneAttachmentOKAndUnprotected = true;
            //---------------------------------
            if( ptr_to_attach->is_crypted()      == true)
            {
                checker.m_bAtLeastOneAttachmentOKAndEncrypted = true;
            };
            //
            if( ptr_to_attach->is_crypted()       == false)
            {
                checker.m_bAtLeastOneAttachmentOKAndDecrypted = true;
            };

        } else // status NOT OBJECT_DELETED and NOT OBJECT_OK
        {
            checker.m_bAtLeastOneAttachmentIsNotReady  = true;
        };
        //
        if (false == b_parent_node_status_chacked)
        {
            checker.m_bParentNodeHasOkStatus = (ptr_to_attach->getParentLeaf()->getObjectStatus() == AbstractDatabaseObject::OBJECT_OK);
            b_parent_node_status_chacked = true;
        };
    };
    //
    checker.m_iAttachmentsAmount = v_attachment.size();
}

void MnuMainWindow::onSelectedNodeChanged(TreeLeaf* ptr_to_current, bool has_cutting_node, bool has_cutting_attach)
{
    m_ptrEncryptAttachmentsOfAllNodes       ->setEnabled(false);
    m_ptrDecryptAttachmentsOfAllNodes       ->setEnabled(false);
    //
    m_ptrProtectAttachmentsOfAllNodes       ->setEnabled(false);
    m_ptrUn_ProtectAttachmentsOfAllNodes    ->setEnabled(false);
    //
    m_ptrDelNode                ->setEnabled(false);
    m_ptrInsertNewNode          ->setEnabled(false);
    m_ptrChangeNodeColor        ->setEnabled(false);
    m_ptrSetNodeIcon            ->setEnabled(false);
    m_ptrImportAttach           ->setEnabled(false);
    m_ptrRestoreNode            ->setEnabled(false);
    m_ptrRestoreSubtree         ->setEnabled(false);
    m_ptrExportNode             ->setEnabled(false);
    m_ptrImportNode             ->setEnabled(false);
    //
    m_ptrCutNode                ->setEnabled(false);
    m_ptrPasteNode              ->setEnabled(false);
    //
    m_ptrPasteAttach            ->setEnabled(false);
    //
    m_ptrChangeTextColor        ->setEnabled(false);
    m_ptrChangeBackgroundColor  ->setEnabled(false);
    //
    m_ptrLoadFromFile           ->setEnabled(false);
    //
    m_ptrInsertTable            ->setEnabled(false);
    //
    m_ptrBold                   ->setEnabled(false);
    m_ptrUnderline              ->setEnabled(false);
    m_ptrItalic                 ->setEnabled(false);
    //
    m_ptrTextAlignLeft          ->setEnabled(false);
    m_ptrTextAlignRight         ->setEnabled(false);
    m_ptrTextAlignCenter        ->setEnabled(false);
    m_ptrTextAlignJustify       ->setEnabled(false);
    //
    m_ptrInsertImageLink       ->setEnabled(false);
    m_ptrInsertHTML            ->setEnabled(false);
    //
    if (NULL == ptr_to_current)
        return;
    //
    AbstractDatabaseObject::DB_OBJECT_STATUS node_state = ptr_to_current->getObjectStatus();
    bool b_is_root_node    = (ptr_to_current->getParentID() == 0);
    //

    //
    bool b_allow_expand     = false;
    bool b_allow_collapse   = false;
    //
    adjustExpandCollapseMenus( ptr_to_current, b_allow_expand, b_allow_collapse );
    //
    m_ptrExpandSubtree  ->setEnabled(b_allow_expand);
    m_ptrCollapseSubtree->setEnabled(b_allow_collapse);
    //
    switch(node_state)
    {
    case AbstractDatabaseObject::OBJECT_NOT_DEFINED:
        m_ptrDelNode                ->setEnabled(false);
        m_ptrInsertNewNode          ->setEnabled(false);
        m_ptrChangeNodeColor        ->setEnabled(false);
        m_ptrSetNodeIcon            ->setEnabled(false);
        m_ptrImportAttach           ->setEnabled(false);
        m_ptrRestoreNode            ->setEnabled(false);
        m_ptrRestoreSubtree         ->setEnabled(false);
        m_ptrExportNode             ->setEnabled(false);
        m_ptrImportNode             ->setEnabled(false);
        //
        m_ptrCutNode                ->setEnabled(false);
        m_ptrPasteNode              ->setEnabled(false);
        //
        m_ptrPasteAttach            ->setEnabled(false);
        break;
    case AbstractDatabaseObject::OBJECT_OK:
        if (b_is_root_node)
            m_ptrDelNode            ->setEnabled(false);
        else
            m_ptrDelNode            ->setEnabled(true);
        //
        m_ptrRestoreNode        ->setEnabled(false);  
        //
        if ( ptr_to_current->isPossibleToExportNode() == true )
            m_ptrExportNode         ->setEnabled(true);
        //
        m_ptrImportNode         ->setEnabled(true);
        m_ptrInsertNewNode      ->setEnabled(true);
        m_ptrChangeNodeColor    ->setEnabled(true);
        m_ptrSetNodeIcon        ->setEnabled(true);
        m_ptrImportAttach       ->setEnabled(true);
        //
        if (b_is_root_node)
            m_ptrCutNode            ->setEnabled(false);
        else
            m_ptrCutNode            ->setEnabled(true);
        //
        if (has_cutting_node)
            m_ptrPasteNode          ->setEnabled(true);
        else
            m_ptrPasteNode          ->setEnabled(false);
        //
        if (has_cutting_attach)
            m_ptrPasteAttach->setEnabled(true);
        else
            m_ptrPasteAttach->setEnabled(false);
        //
        adjustProtectionMenuForAllAttacmentsOfNode   ( ptr_to_current );
        adjustCryptographyMenuForAllAttacmentsOfNode ( ptr_to_current );
        //
        {
            AttachStatusChecker checker;
            adjustGlobalProtectionMenu  (ptr_to_current, checker);
            adjustGlobalCryptographyMenu(ptr_to_current, checker);
        }
        //
        //if selected node exist, user is able to load description of this node from file
        //
        m_ptrLoadFromFile   ->setEnabled(true);
        //
        m_ptrInsertTable    ->setEnabled(true);
        //
        m_ptrBold           ->setEnabled(true);
        m_ptrUnderline      ->setEnabled(true);
        m_ptrItalic         ->setEnabled(true);
        //
        m_ptrTextAlignLeft     ->setEnabled(true);
        m_ptrTextAlignRight    ->setEnabled(true);
        m_ptrTextAlignCenter   ->setEnabled(true);
        m_ptrTextAlignJustify  ->setEnabled(true);
        //
        m_ptrChangeTextColor        ->setEnabled(true);
        m_ptrChangeBackgroundColor  ->setEnabled(true);
        //
        m_ptrInsertImageLink        ->setEnabled(true);
        m_ptrInsertHTML             ->setEnabled(true);
        //
        break;
    case AbstractDatabaseObject::OBJECT_DELETED:
        m_ptrDelNode            ->setEnabled(false);
        m_ptrRestoreNode        ->setEnabled(true);
        //
        if (ptr_to_current->childCount() > 0)
            m_ptrRestoreSubtree     ->setEnabled(true);
        else
            m_ptrRestoreSubtree     ->setEnabled(false);

        //
        m_ptrExportNode         ->setEnabled(true);
        m_ptrImportNode         ->setEnabled(false);
        //
        m_ptrCutNode            ->setEnabled(false);
        m_ptrPasteNode          ->setEnabled(false);
        //
        m_ptrCutNode            ->setEnabled(false);
        m_ptrPasteNode          ->setEnabled(false);
        //
        m_ptrImportAttach       ->setEnabled(false);
        //
        m_ptrPasteAttach        ->setEnabled(false);
        break;
    default: //different update statuses are here
        m_ptrDelNode            ->setEnabled(false);
        m_ptrRestoreNode        ->setEnabled(false);
        m_ptrRestoreSubtree     ->setEnabled(false);
        //
        m_ptrCutNode            ->setEnabled(false);
        m_ptrPasteNode          ->setEnabled(false);
        //
        m_ptrExportNode         ->setEnabled(false);
        m_ptrImportNode         ->setEnabled(false);
        //
        m_ptrImportAttach       ->setEnabled(true);
        m_ptrInsertNewNode      ->setEnabled(true);
        m_ptrPasteAttach        ->setEnabled(true);
    };
}
//-------------------------- signal processing end ---------------------------------------------

void MnuMainWindow::adjustExpandCollapseMenus (TreeLeaf* ptr_to_current, bool& b_allow_expand, bool& b_allow_collapse )
{
    if (NULL == ptr_to_current)
    {
        b_allow_expand      = false;
        b_allow_collapse    = false;
        return;
    };
    //
    TreeLeaf::ChildList childs = ptr_to_current->getChildList();
    //
    if (childs.size() > 0 )
    {
        if ( ptr_to_current->isExpanded() )
            b_allow_collapse = true;
        else
            b_allow_expand  = true;
    }else
        return; //no child - no interest
    //
    if (b_allow_collapse && b_allow_expand)
        return; //nothing more to check.
    //
    for (unsigned int i = 0; i < childs.size(); i++)
    {
        TreeLeaf* ptr_current = (TreeLeaf*) childs[i];
        //check nested elements
        adjustExpandCollapseMenus( ptr_current, b_allow_expand, b_allow_collapse );
        //
        if (b_allow_collapse && b_allow_expand)
            return; //nothing more to check.
    };
    //
}

void MnuMainWindow::adjustGlobalProtectionMenu  ( TreeLeaf* ptr_to_current, AttachStatusChecker &checker)
{
    //get all attachments
    const Attachment::AttachmentsList& attachments =  ptr_to_current->getAttachments();
    //analyse attachments
    analyseAttachmentsStatus(attachments,  checker);
    //do not analyse the rest
    if ( checker.m_bAtLeastOneAttachmentOKAndProtected && checker.m_bAtLeastOneAttachmentOKAndUnprotected )
        return;
    //
    //we do not analyse all attachments recursively, because it is too slow. We just accept "OK", if node has childs.
    //
    if ( ptr_to_current->getChildList().size() > 0 )
    {
        checker.m_bAtLeastOneAttachmentOKAndProtected   = true;
        checker.m_bAtLeastOneAttachmentOKAndUnprotected = true;
    };
/*
    //request child nodes attachment info
    if ( (false == checker.m_bAtLeastOneAttachmentOKAndProtected) ||
         (false == checker.m_bAtLeastOneAttachmentOKAndUnprotected)
       )
    {
        TreeLeaf::ChildList childs = ptr_to_current->getChildList();
        for (unsigned int i = 0; i < childs.size(); i++)
        {
            adjustGlobalProtectionMenu(childs[i], checker);
        };
    };
  */
    //set menu status
    if (checker.m_bAtLeastOneAttachmentOKAndProtected)
        m_ptrUn_ProtectAttachmentsOfAllNodes->setEnabled(true);
    else
        m_ptrUn_ProtectAttachmentsOfAllNodes->setEnabled(false);
    //
    if (checker.m_bAtLeastOneAttachmentOKAndUnprotected)
        m_ptrProtectAttachmentsOfAllNodes->setEnabled(true);
    else
        m_ptrProtectAttachmentsOfAllNodes->setEnabled(false);
    //
}

void MnuMainWindow::adjustGlobalCryptographyMenu( TreeLeaf* ptr_to_current, AttachStatusChecker& checker)
{
    //set menu status if we have no password
    if ( ServicesCfg::getInstance().getPassword().length() == 0 )
    {
        m_ptrDecryptAttachmentsOfAllNodes->setEnabled(false);
        m_ptrEncryptAttachmentsOfAllNodes->setEnabled(false);
        return;
    }
    //get all attachments
    const Attachment::AttachmentsList& attachments =  ptr_to_current->getAttachments();
    //analyse attachments
    analyseAttachmentsStatus(attachments,  checker);
    //do not analyse the rest
    if ( checker.m_bAtLeastOneAttachmentOKAndEncrypted && checker.m_bAtLeastOneAttachmentOKAndDecrypted )
        return;
    //
    if ( ptr_to_current->getChildList().size() > 0 )
    {
        checker.m_bAtLeastOneAttachmentOKAndEncrypted = true;
        checker.m_bAtLeastOneAttachmentOKAndDecrypted = true;
    };
/*
    //request child nodes attachment info
    if ( (false == checker.m_bAtLeastOneAttachmentOKAndEncrypted) ||
         (false == checker.m_bAtLeastOneAttachmentOKAndDecrypted)
       )
    {
        TreeLeaf::ChildList childs = ptr_to_current->getChildList();
        for (unsigned int i = 0; i < childs.size(); i++)
        {
            adjustGlobalProtectionMenu(childs[i], checker);
        }
       // ptr_to_current->get
    };
*/
    //set menu status
    if ( checker.m_bAtLeastOneAttachmentOKAndEncrypted )
        m_ptrDecryptAttachmentsOfAllNodes->setEnabled(true);
    else
        m_ptrDecryptAttachmentsOfAllNodes->setEnabled(false);
    //
    if ( checker.m_bAtLeastOneAttachmentOKAndDecrypted )
        m_ptrEncryptAttachmentsOfAllNodes->setEnabled(true);
    else
        m_ptrEncryptAttachmentsOfAllNodes->setEnabled(false);
}

void MnuMainWindow::adjustCryptographyMenuForAllAttacmentsOfNode ( TreeLeaf* ptr_parent )
{
    const Attachment::AttachmentsList& attachments =  ptr_parent->getAttachments();
    AttachStatusChecker checker;
    analyseAttachmentsStatus(attachments,  checker);
    //
    if ( (0 == checker.m_iAttachmentsAmount )                     ||        //nothing attached to this node
         (ServicesCfg::getInstance().getPassword().length() == 0)          //no password
       )
    {
        m_ptrEncryptAllAttachmentsOfNode ->setEnabled(false);
        m_ptrDecryptAllAttachmentsOfNode ->setEnabled(false);
        return;
    };
    //
    m_ptrDecryptAllAttachmentsOfNode->setEnabled ( checker.m_bAtLeastOneAttachmentOKAndEncrypted );
    m_ptrEncryptAllAttachmentsOfNode->setEnabled ( checker.m_bAtLeastOneAttachmentOKAndDecrypted );
}

void MnuMainWindow::adjustProtectionMenuForAllAttacmentsOfNode  ( TreeLeaf* ptr_parent )
{
    const Attachment::AttachmentsList& attachments =  ptr_parent->getAttachments();
    AttachStatusChecker checker;
    //
    analyseAttachmentsStatus(attachments,  checker);

    if ( 0 == checker.m_iAttachmentsAmount ) //nothing attached to this node
    {
        m_ptrProtectAllAttachmentsOfNode  ->setEnabled(false);
        m_ptrUn_ProtectAllAttachmentsOfNode->setEnabled(false);
        return;
    };
    //
    m_ptrUn_ProtectAllAttachmentsOfNode->setEnabled ( checker.m_bAtLeastOneAttachmentOKAndProtected );
    m_ptrProtectAllAttachmentsOfNode->setEnabled ( checker.m_bAtLeastOneAttachmentOKAndUnprotected );
}

void MnuMainWindow::adjustProtectionMenuForSelectedAttacments ( const AttachStatusChecker& checker)
{
    if (0 == checker.m_iAttachmentsAmount) //nothing attached to this node
    {
        m_ptrProtectSelectedAttachments ->setEnabled(false);
        m_ptrUn_ProtectSelectedAttachments->setEnabled(false);
        return;
    };
    //
    m_ptrUn_ProtectSelectedAttachments->setEnabled(checker.m_bAtLeastOneAttachmentOKAndProtected);
    m_ptrProtectSelectedAttachments->setEnabled(checker.m_bAtLeastOneAttachmentOKAndUnprotected);
}

void MnuMainWindow::adjustCryptoMenuForSelectedAttacments (  const AttachStatusChecker& checker )
{
    if (
            (0 == checker.m_iAttachmentsAmount)                         || //nothing attached to this node
            (ServicesCfg::getInstance().getPassword().length() == 0)       //no password
       )
    {
        m_ptrEncryptSelectedAttachments->setEnabled(false);
        m_ptrDecryptSelectedAttachments->setEnabled(false);
        return;
    };
    //
    m_ptrEncryptSelectedAttachments->setEnabled ( checker.m_bAtLeastOneAttachmentOKAndDecrypted );
    m_ptrDecryptSelectedAttachments->setEnabled ( checker.m_bAtLeastOneAttachmentOKAndEncrypted );
}

void MnuMainWindow::createHelpMenu ()
{
    m_ptrAbout  = new QAction(tr("About..."), this);
    m_ptrAbout  ->setIconVisibleInMenu(true);
    m_ptrAbout  ->setIcon(QIcon(":/images/images/about.ico"));
    m_ptrAbout  ->setShortcut(QKeySequence (Qt::Key_F1));
    m_ptrAbout  ->setStatusTip(tr("About the program"));
    m_ptrAbout  ->setEnabled(true); //always enabled
 }

void MnuMainWindow::createAttachmentControlMenu()
{
    m_ptrRestoreAttach  = new QAction(tr("Restore attachment"), this);
    m_ptrRestoreAttach  ->setIconVisibleInMenu(true);
    m_ptrRestoreAttach  ->setIcon(QIcon(":/images/images/cube_green_new.png"));
    m_ptrRestoreAttach  ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_R));
    m_ptrRestoreAttach  ->setStatusTip(tr("Restore selected attachment"));
    m_ptrRestoreAttach  ->setEnabled(false);
    //
    m_ptrDeleteAttach   = new QAction(tr("Delete attachment"), this);
    m_ptrDeleteAttach   ->setIconVisibleInMenu(true);
    m_ptrDeleteAttach   ->setIcon(QIcon(":/images/images/delete2.png"));
    m_ptrDeleteAttach   ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_D));
    m_ptrDeleteAttach   ->setStatusTip(tr("Delete selected attachment"));
    m_ptrDeleteAttach   ->setEnabled(false);
    //
    m_ptrExportAttach   = new QAction(tr("Download attachment"), this);
    m_ptrExportAttach   ->setIconVisibleInMenu(true);
    m_ptrExportAttach   ->setIcon(QIcon(":/images/images/down_plus.png"));
    m_ptrExportAttach   ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_E));
    m_ptrExportAttach   ->setStatusTip(tr("Export selected attachment"));
    m_ptrExportAttach   ->setEnabled(false);
    //
    m_ptrImportAttach   = new QAction(tr("Import attachment"), this);
    m_ptrImportAttach   ->setIconVisibleInMenu(true);
    m_ptrImportAttach   ->setIcon(QIcon(":/images/images/up_plus.png"));
    m_ptrImportAttach   ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_A));
    m_ptrImportAttach   ->setStatusTip(tr("Attach files to the node"));
    m_ptrImportAttach   ->setEnabled(false);
    //
    m_ptrReplaceAttach  = new QAction(tr("Replace current attachment"), this);
    m_ptrReplaceAttach  ->setIconVisibleInMenu(true);
    m_ptrReplaceAttach  ->setIcon(QIcon(":/images/images/replace.png"));
    m_ptrReplaceAttach  ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_L));
    m_ptrReplaceAttach  ->setStatusTip(tr("Remove the selected attachment and add another one instead of it"));
    m_ptrReplaceAttach  ->setEnabled(false);
    //
    m_ptrCutAttach      = new QAction(tr("Cut attachment"), this);
    m_ptrCutAttach      ->setIconVisibleInMenu(true);
    m_ptrCutAttach      ->setIcon(QIcon(":/images/images/cut.png"));
    m_ptrCutAttach      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrCutAttach      ->setStatusTip(tr("Cut selected attachments"));
    m_ptrCutAttach      ->setEnabled(false);
    //
    m_ptrPasteAttach    = new QAction(tr("Paste attachment"), this);
    m_ptrPasteAttach    ->setIconVisibleInMenu(true);
    m_ptrPasteAttach    ->setIcon(QIcon(":/images/images/paste.png"));
    m_ptrPasteAttach    ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_P));
    m_ptrPasteAttach    ->setStatusTip(tr("Paste selected attachments"));
    m_ptrPasteAttach    ->setEnabled(false);
    //
    m_ptrViewAttach    = new QAction(tr("View attachment"), this);
    m_ptrViewAttach    ->setIconVisibleInMenu(true);
    m_ptrViewAttach    ->setIcon(QIcon(":/images/images/view.png"));
    m_ptrViewAttach    ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_O));
    m_ptrViewAttach    ->setStatusTip(tr("Open and view attachments"));
    m_ptrViewAttach    ->setEnabled(false);
    //
    m_ptrDirectScan    = new QAction(tr("Scan..."), this);
    m_ptrDirectScan    ->setIconVisibleInMenu(true);
    m_ptrDirectScan    ->setIcon(QIcon(":/images/images/scanner.png"));
    //m_ptrDirectScan    ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_O));
    m_ptrDirectScan    ->setStatusTip(tr("Scan new document and add it as attachment"));
    //TODO
    //m_ptrDirectScan    ->setEnabled(false);

    //
    if (NULL == m_ptrAttachmentToolBar)
        m_ptrAttachmentToolBar = new QToolBar;
    //
    m_ptrAttachmentToolBar->addAction(m_ptrImportAttach);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrExportAttach);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrDeleteAttach);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrReplaceAttach);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrRestoreAttach);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrReplaceAttach);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrCutAttach);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrPasteAttach);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrViewAttach);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrDirectScan);
    //
    m_ptrAttachmentToolBar->setWindowTitle(tr("Operations with attachments"));
}

void MnuMainWindow::createSecurityMenu()
{
    m_ptrCreateChangePassword   = new QAction("", this);
    m_ptrCreateChangePassword   ->setIconVisibleInMenu(true);
    m_ptrCreateChangePassword      ->setIcon(QIcon(":/images/images/keys.png"));
    //m_ptrCreateChangePassword      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrCreateChangePassword      ->setStatusTip(tr("Create password for encrypt/decrypt attachments"));
    m_ptrCreateChangePassword      ->setEnabled(true);
    //
    m_ptrLockAppScreen   = new QAction("Lock screen", this);
    m_ptrLockAppScreen   ->setIconVisibleInMenu(true);
    m_ptrLockAppScreen      ->setIcon(QIcon(":/images/images/lock_screen.png"));
    m_ptrLockAppScreen      ->setShortcut(QKeySequence (Qt::Key_F10));
    m_ptrLockAppScreen      ->setStatusTip(tr("Lock application screen"));
    m_ptrLockAppScreen      ->setEnabled(false);
    //
    if (NULL == m_ptrSecurityToolBar)
        m_ptrSecurityToolBar = new QToolBar;
    //
    m_ptrSecurityToolBar->addAction(m_ptrCreateChangePassword);
    m_ptrSecurityToolBar->addSeparator();
    m_ptrSecurityToolBar->addAction(m_ptrLockAppScreen);
    //
    m_ptrSecurityToolBar->setWindowTitle(tr("Security toolbar"));

}

void MnuMainWindow::createToolBarSubMenu()
{
    m_ptrMnuNodeToolBar  = new QAction("Node toolbar", this);
    m_ptrMnuNodeToolBar  ->setCheckable(true);
    //
    m_ptrMnuTreeToolBar = new QAction("Tree toolbar", this);
    m_ptrMnuTreeToolBar ->setCheckable(true);
    //
    m_ptrMnuAttachmentToolBar = new QAction("Attachment toolbar", this);
    m_ptrMnuAttachmentToolBar ->setCheckable(true);
    //
    m_ptrMnuEditorToolBar = new QAction("Editor toolbar", this);
    m_ptrMnuEditorToolBar ->setCheckable(true);
    //
    m_ptrMnuTableToolBar = new QAction("Table toolbar", this);
    m_ptrMnuTableToolBar ->setCheckable(true);
    //
    m_ptrMnuSecurityToolBar= new QAction("Security toolbar", this);
    m_ptrMnuSecurityToolBar ->setCheckable(true);
}

void MnuMainWindow::createCryptograficMenu ()
{
    m_ptrEncryptAllAttachmentsOfNode      = new QAction(tr("Encrypt all attachments"), this);
    m_ptrEncryptAllAttachmentsOfNode      ->setIconVisibleInMenu(true);
    m_ptrEncryptAllAttachmentsOfNode      ->setIcon(QIcon(":/images/images/lock.png"));
    //m_ptrEncryptAllAttachmentsOfNode      ->setShortcut(QKeySequence("Ctrl+Alt+N"));
    m_ptrEncryptAllAttachmentsOfNode      ->setShortcut(QKeySequence("Ctrl+Alt+N"));
    m_ptrEncryptAllAttachmentsOfNode      ->setStatusTip(tr("Encrypt all attachments of this node"));
    m_ptrEncryptAllAttachmentsOfNode      ->setEnabled(false);
    //
    m_ptrDecryptAllAttachmentsOfNode      = new QAction(tr("Decrypt all attachments"), this);
    m_ptrDecryptAllAttachmentsOfNode      ->setIconVisibleInMenu(true);
    m_ptrDecryptAllAttachmentsOfNode      ->setIcon(QIcon(":/images/images/lock_open.png"));
    m_ptrDecryptAllAttachmentsOfNode      ->setShortcut(QKeySequence("Ctrl+Alt+U"));
    m_ptrDecryptAllAttachmentsOfNode      ->setStatusTip(tr("Decrypt all attachments of this node"));
    m_ptrDecryptAllAttachmentsOfNode      ->setEnabled(false);
    //
    m_ptrEncryptSelectedAttachments        = new QAction(tr("Encrypt selected attachment"), this);
    m_ptrEncryptSelectedAttachments        ->setIconVisibleInMenu(true);
    m_ptrEncryptSelectedAttachments        ->setIcon(QIcon(":/images/images/lock_add.png"));
    m_ptrEncryptSelectedAttachments        ->setShortcut( QKeySequence ("Ctrl+Shift+N") );
    m_ptrEncryptSelectedAttachments        ->setStatusTip(tr("Encrypt selected attachment"));
    m_ptrEncryptSelectedAttachments        ->setEnabled(false);
    //
    m_ptrDecryptSelectedAttachments       = new QAction(tr("Decrypt selected attachment"), this);
    m_ptrDecryptSelectedAttachments        ->setIconVisibleInMenu(true);
    m_ptrDecryptSelectedAttachments        ->setIcon(QIcon(":/images/images/lock_delete.png"));
    m_ptrDecryptSelectedAttachments        ->setShortcut( QKeySequence ("Ctrl+Shift+U") );
    m_ptrDecryptSelectedAttachments        ->setStatusTip(tr("Decrypt selected attachment"));
    m_ptrDecryptSelectedAttachments        ->setEnabled(false);
    //
    if (NULL == m_ptrAttachmentToolBar)
        m_ptrAttachmentToolBar = new QToolBar;
    else
        m_ptrAttachmentToolBar->addSeparator();
    //
    m_ptrAttachmentToolBar->addAction(m_ptrEncryptAllAttachmentsOfNode);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrDecryptAllAttachmentsOfNode);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrEncryptSelectedAttachments);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrDecryptSelectedAttachments);
    //
}

void MnuMainWindow::createProtectionMenu()
{
    m_ptrProtectAllAttachmentsOfNode= new QAction(tr("Protect all attachments "), this);
    m_ptrProtectAllAttachmentsOfNode      ->setIconVisibleInMenu(true);
    m_ptrProtectAllAttachmentsOfNode      ->setIcon(QIcon(":/images/images/shield_green.png"));
    //m_ptrProtectAllAttachmentsOfNode      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrProtectAllAttachmentsOfNode      ->setStatusTip(tr("Protect all attachments of this node"));
    m_ptrProtectAllAttachmentsOfNode      ->setEnabled(false);
    //
    m_ptrUn_ProtectAllAttachmentsOfNode= new QAction(tr("Remove protection of all attachments"), this);
    m_ptrUn_ProtectAllAttachmentsOfNode      ->setIconVisibleInMenu(true);
    m_ptrUn_ProtectAllAttachmentsOfNode      ->setIcon(QIcon(":/images/images/shield_delete.png"));
    //m_ptrUn_ProtectAllAttachmentsOfNode      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrUn_ProtectAllAttachmentsOfNode      ->setStatusTip(tr("Protect all attachments of this node"));
    m_ptrUn_ProtectAllAttachmentsOfNode      ->setEnabled(false);
    //
    m_ptrProtectSelectedAttachments= new QAction(tr("Protect selected attachment"), this);
    m_ptrProtectSelectedAttachments      ->setIconVisibleInMenu(true);
    m_ptrProtectSelectedAttachments      ->setIcon(QIcon(":/images/images/shield_add.png"));
    //m_ptrProtectSelectedAttachments      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrProtectSelectedAttachments      ->setStatusTip(tr("Set protection for selected attachment"));
    m_ptrProtectSelectedAttachments      ->setEnabled(false);
    //
    m_ptrUn_ProtectSelectedAttachments= new QAction(tr("Remove protection of selected attachment"), this);
    m_ptrUn_ProtectSelectedAttachments      ->setIconVisibleInMenu(true);
    m_ptrUn_ProtectSelectedAttachments      ->setIcon(QIcon(":/images/images/shield_red.png"));
    //m_ptrDropThisProtect      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrUn_ProtectSelectedAttachments      ->setStatusTip(tr("Drop protection for selected attachment"));
    m_ptrUn_ProtectSelectedAttachments      ->setEnabled(false);
    //
    if (NULL == m_ptrAttachmentToolBar)
        m_ptrAttachmentToolBar = new QToolBar();
    else
        m_ptrAttachmentToolBar->addSeparator();
    //
    m_ptrAttachmentToolBar->addAction(m_ptrProtectAllAttachmentsOfNode);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrUn_ProtectAllAttachmentsOfNode);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrProtectSelectedAttachments);
    m_ptrAttachmentToolBar->addSeparator();
    m_ptrAttachmentToolBar->addAction(m_ptrUn_ProtectSelectedAttachments);
    m_ptrAttachmentToolBar->addSeparator();
}

void MnuMainWindow::createTreeControlMenu()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    QVariant var = settings.value(g_str_DB_SETTINGS);
    DBSettings db_settings = var.value<DBSettings>();
    const QString str_backup_cmd = db_settings.m_Backups [db_settings.getCurrentPage()];
    //for the m_ptrTreeControlMenu
    //
    m_ptrBackupDatabase = new QAction(tr("Backup database"), this);

    m_ptrBackupDatabase  ->setIconVisibleInMenu(true);
    m_ptrBackupDatabase  ->setIcon(QIcon(":/images/images/data_disk.png"));
    m_ptrBackupDatabase  ->setShortcut(QKeySequence (Qt::Key_F5));
    if ( str_backup_cmd.length() > 0 )
    {
        m_ptrBackupDatabase  ->setStatusTip(tr("Immediately backup database"));
        m_ptrBackupDatabase  ->setEnabled(true);
    }else
    {
        m_ptrBackupDatabase  ->setStatusTip(tr("Database backup not possible: command line not defined."));
        m_ptrBackupDatabase  ->setEnabled(false);
    };
    //m_ptrBackupDatabase  ->setToolTip("tooltip");
    //
    m_ptrCreateNewTree  = new QAction(tr("Create new tree"), this);
    m_ptrCreateNewTree  ->setIconVisibleInMenu(true);
    m_ptrCreateNewTree  ->setIcon(QIcon(":/images/images/tree.png"));
    m_ptrCreateNewTree  ->setShortcut(QKeySequence (Qt::Key_F3));
    m_ptrCreateNewTree  ->setStatusTip(tr("Create new tree"));
    m_ptrCreateNewTree  ->setEnabled(true);
    //
    m_ptrDelTree        = new QAction(tr("Delete current tree"), this);
    m_ptrDelTree        ->setIconVisibleInMenu(true);
    m_ptrDelTree        ->setIcon(QIcon(":/images/images/tree_delete.png"));
    m_ptrDelTree        ->setShortcut(QKeySequence (Qt::Key_F8));
    m_ptrDelTree        ->setStatusTip(tr("Delete current tree"));
    m_ptrDelTree        ->setEnabled(false);
    //
    m_ptrRenameTree     = new QAction(tr("Rename current tree"), this);
    m_ptrRenameTree     ->setIconVisibleInMenu(true);
    m_ptrRenameTree     ->setIcon(QIcon(":/images/images/tree_rename.png"));
    m_ptrRenameTree     ->setShortcut( QKeySequence (Qt::Key_F6) );
    m_ptrRenameTree     ->setStatusTip(tr("Change name of the current tree"));
    m_ptrRenameTree     ->setEnabled(false);
    //
    m_ptrInfoTree       = new QAction(tr("Info about current tree"), this);
    m_ptrInfoTree       ->setIconVisibleInMenu(true);
    m_ptrInfoTree      ->setIcon(QIcon(":/images/images/information2.png"));
    m_ptrInfoTree      ->setShortcut( QKeySequence (Qt::Key_F7) );
    m_ptrInfoTree      ->setStatusTip(tr("Get statistic info about current tree"));
    m_ptrInfoTree      ->setEnabled(false);
    //
    m_ptrSearchInTree   = new QAction(tr("Search..."), this);
    m_ptrSearchInTree   ->setIconVisibleInMenu(true);
    m_ptrSearchInTree   ->setIcon(QIcon(":/images/images/find.png"));
    m_ptrSearchInTree   ->setShortcut( QKeySequence (Qt::CTRL + Qt::Key_F) );
    m_ptrSearchInTree   ->setStatusTip(tr("Search in tree(s)"));
    m_ptrSearchInTree   ->setEnabled(false);
    //
    m_ptrExitApp   = new QAction(tr("Exit"), this);
    m_ptrExitApp   ->setIcon(QIcon(":/images/images/exit.png"));
    m_ptrExitApp   ->setShortcut( QKeySequence (Qt::CTRL + Qt::Key_Q) );
    m_ptrExitApp   ->setStatusTip(tr("Leave the app"));
    m_ptrExitApp   ->setEnabled(true);
    //
    if (NULL == m_ptrTreeToolBar)
        m_ptrTreeToolBar = new QToolBar();
    //
    m_ptrTreeToolBar->addAction(m_ptrCreateNewTree);
    m_ptrTreeToolBar->addSeparator();
    m_ptrTreeToolBar->addAction(m_ptrDelTree);
    m_ptrTreeToolBar->addSeparator();
    m_ptrTreeToolBar->addAction(m_ptrRenameTree);
    m_ptrTreeToolBar->addSeparator();
    m_ptrTreeToolBar->addAction(m_ptrInfoTree);
    m_ptrTreeToolBar->addSeparator();
    m_ptrTreeToolBar->addAction(m_ptrSearchInTree);
    m_ptrTreeToolBar->addSeparator();
    m_ptrTreeToolBar->addAction(m_ptrBackupDatabase);
    m_ptrTreeToolBar->addSeparator();
    m_ptrTreeToolBar->addAction(m_ptrExitApp);
    //
    m_ptrTreeToolBar->setWindowTitle(tr("Operations with tree"));

}
//
void MnuMainWindow::createNodeControlMenu()
{
    m_ptrDelNode        = new QAction(tr("Delete current node"), this);
    m_ptrDelNode        ->setIconVisibleInMenu(true);
    m_ptrDelNode        ->setIcon(QIcon(":/images/images/delete.png"));
    m_ptrDelNode        ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_D));
    m_ptrDelNode        ->setStatusTip(tr("Delete current node"));
    m_ptrDelNode        ->setEnabled(false);
    //
    m_ptrRestoreNode    = new QAction(tr("Restore current node"), this);
    m_ptrRestoreNode    ->setIconVisibleInMenu(true);
    m_ptrRestoreNode    ->setIcon(QIcon(":/images/images/cube_green.png"));
    m_ptrRestoreNode    ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_R));
    m_ptrRestoreNode    ->setStatusTip(tr("Restore current node"));
    m_ptrRestoreNode    ->setEnabled(false);
    //
    m_ptrRestoreSubtree = new QAction(tr("Completely restore subtree"), this);
    m_ptrRestoreSubtree ->setIconVisibleInMenu(true);
    m_ptrRestoreSubtree ->setIcon(QIcon(":/images/images/cube_green_add.png"));
    m_ptrRestoreSubtree ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_G));
    m_ptrRestoreSubtree ->setStatusTip(tr("Restore current subtree: all nodes and attachments"));
    m_ptrRestoreSubtree ->setEnabled(false);
    //
    m_ptrInsertNewNode  = new QAction(tr("Insert new node"), this);
    m_ptrInsertNewNode  ->setIconVisibleInMenu(true);
    m_ptrInsertNewNode  ->setIcon(QIcon(":/images/images/add.png"));
    m_ptrInsertNewNode  ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_Insert));
    m_ptrInsertNewNode  ->setStatusTip(tr("Insert new node"));
    m_ptrInsertNewNode  ->setEnabled(false);
    //
    m_ptrChangeNodeColor  = new QAction(tr("Change node color"), this);
    m_ptrChangeNodeColor  ->setIconVisibleInMenu(true);
    m_ptrChangeNodeColor  ->setIcon(QIcon(":/images/images/colorwheel.png"));
    //m_ptrChangeNodeColor  ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_Insert));
    m_ptrChangeNodeColor  ->setStatusTip(tr("Change color of the current node"));
    m_ptrChangeNodeColor  ->setEnabled(false);
    //
    m_ptrSetNodeIcon  = new QAction(tr("Node icon..."), this);
    m_ptrSetNodeIcon  ->setIconVisibleInMenu(true);
    m_ptrSetNodeIcon  ->setIcon(QIcon(":/images/images/photo.png"));
    //m_ptrSetNodeIcon  ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_Insert));
    m_ptrSetNodeIcon  ->setStatusTip(tr("Set, remove or change icon of the node"));
    m_ptrSetNodeIcon  ->setEnabled(false);

    //
    m_ptrCutNode        = new QAction(tr("Cut node"), this);
    m_ptrCutNode        ->setIconVisibleInMenu(true);
    m_ptrCutNode        ->setIcon(QIcon(":/images/images/cut.png"));
    m_ptrCutNode        ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_M));
    m_ptrCutNode        ->setStatusTip(tr("Cut node"));
    m_ptrCutNode        ->setEnabled(false);
    //----  alternate menu item
    m_ptrPasteNode      = new QAction(tr("Paste node"), this);
    m_ptrPasteNode      ->setIconVisibleInMenu(true);
    m_ptrPasteNode      ->setIcon(QIcon(":/images/images/paste.png"));
    m_ptrPasteNode      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrPasteNode      ->setStatusTip(tr("Paste node"));
    m_ptrPasteNode      ->setEnabled(false);
    //
    m_ptrExportNode     = new QAction(tr("Export node..."), this);
    m_ptrExportNode     ->setIconVisibleInMenu(true);
    m_ptrExportNode     ->setIcon(QIcon(":/images/images/export.png"));
    m_ptrExportNode     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_E));
    m_ptrExportNode     ->setStatusTip(tr("Export current node or subtree to the disk"));
    m_ptrExportNode     ->setEnabled(false);
    //
    m_ptrImportNode     = new QAction(tr("Import directory..."), this);
    m_ptrImportNode     ->setIconVisibleInMenu(true);
    m_ptrImportNode     ->setIcon(QIcon(":/images/images/import.png"));
    m_ptrImportNode     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_I));
    m_ptrImportNode     ->setStatusTip(tr("Import from disk under current node"));
    m_ptrImportNode     ->setEnabled(false);
    //
    m_ptrEncryptAttachmentsOfAllNodes     = new QAction(tr("Encrypt all attachments of current node(s)..."), this);
    m_ptrEncryptAttachmentsOfAllNodes     ->setIconVisibleInMenu(true);
    m_ptrEncryptAttachmentsOfAllNodes     ->setIcon(QIcon(":/images/images/lock_add.png"));
    //m_ptrEncryptAttachmentsOfAllNodes     ->setShortcut(QKeySequence (Qt::SHIFT + Qt::ALT + Qt::Key_P));
    m_ptrEncryptAttachmentsOfAllNodes     ->setStatusTip(tr("Encrypt all attachments of the node (with childs optionally"));
    m_ptrEncryptAttachmentsOfAllNodes     ->setEnabled(false);
    //
    m_ptrDecryptAttachmentsOfAllNodes     = new QAction(tr("Decrypt all attachments of current node(s)..."), this);
    m_ptrDecryptAttachmentsOfAllNodes     ->setIconVisibleInMenu(true);
    m_ptrDecryptAttachmentsOfAllNodes     ->setIcon(QIcon(":/images/images/lock_delete.png"));
    //m_ptrDecryptAttachmentsOfAllNodes     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_I));
    m_ptrDecryptAttachmentsOfAllNodes     ->setStatusTip(tr("Decrypt all attachments of the node (with childs optionally"));
    m_ptrDecryptAttachmentsOfAllNodes     ->setEnabled(false);
    //....... not added to the toolbar yet
    m_ptrProtectAttachmentsOfAllNodes     = new QAction(tr("Protect all attachment of current node(s)..."), this);
    m_ptrProtectAttachmentsOfAllNodes     ->setIconVisibleInMenu(true);
    m_ptrProtectAttachmentsOfAllNodes     ->setIcon(QIcon(":/images/images/shield_green.png"));
    //m_ptrProtectAttachmentsOfAllNodes     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_I));
    m_ptrProtectAttachmentsOfAllNodes     ->setStatusTip(tr("Protect all attachments of the node (with childs optionally"));
    m_ptrProtectAttachmentsOfAllNodes     ->setEnabled(false);
    //
    m_ptrUn_ProtectAttachmentsOfAllNodes     = new QAction(tr("Unprotect all attachments of current node(s)..."), this);
    m_ptrUn_ProtectAttachmentsOfAllNodes     ->setIconVisibleInMenu(true);
    m_ptrUn_ProtectAttachmentsOfAllNodes     ->setIcon(QIcon(":/images/images/shield_delete.png"));
    //m_ptrUn_ProtectAttachmentsOfAllNodes     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_I));
    m_ptrUn_ProtectAttachmentsOfAllNodes     ->setStatusTip(tr("Protect all attachments of the node (with childs optionally"));
    m_ptrUn_ProtectAttachmentsOfAllNodes     ->setEnabled(false);
    //
    m_ptrExpandSubtree      = new QAction(tr("Expand subtree"), this);
    m_ptrExpandSubtree      ->setIconVisibleInMenu(true);
    m_ptrExpandSubtree      ->setIcon(QIcon(":/images/images/expand_subtree.png"));
    m_ptrExpandSubtree      ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_Plus));
    m_ptrExpandSubtree      ->setStatusTip(tr("Expand this element and all childs"));
    m_ptrExpandSubtree      ->setEnabled(false);
    //
    m_ptrCollapseSubtree      = new QAction(tr("Collapse subtree"), this);
    m_ptrCollapseSubtree      ->setIconVisibleInMenu(true);
    m_ptrCollapseSubtree      ->setIcon(QIcon(":/images/images/collapse_subtree.png"));
    m_ptrCollapseSubtree      ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_Minus));
    m_ptrCollapseSubtree      ->setStatusTip(tr("Collapse this element and all childs"));
    m_ptrCollapseSubtree      ->setEnabled(false);
    //
    m_ptrNodeToolBar    = new QToolBar;
    //
    m_ptrNodeToolBar->addAction(m_ptrInsertNewNode);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrChangeNodeColor);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrSetNodeIcon);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrDelNode);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrRestoreNode);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrRestoreSubtree);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrCutNode);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrPasteNode);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrExportNode);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrImportNode);
    ///---------------
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrEncryptAttachmentsOfAllNodes);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrDecryptAttachmentsOfAllNodes);
    m_ptrNodeToolBar->addSeparator();
    //----
    m_ptrNodeToolBar->addAction(m_ptrProtectAttachmentsOfAllNodes);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrUn_ProtectAttachmentsOfAllNodes);
    //-------
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrExpandSubtree);
    m_ptrNodeToolBar->addSeparator();
    m_ptrNodeToolBar->addAction(m_ptrCollapseSubtree);
    //
    m_ptrNodeToolBar->setWindowTitle(tr("Operations with nodes"));
}

void MnuMainWindow::createEditorMenu()
{
    m_ptrLoadFromFile      = new QAction(tr("Load from file"), this);
    m_ptrLoadFromFile      ->setIconVisibleInMenu(true);
    m_ptrLoadFromFile      ->setIcon(QIcon(":/images/images/hand_paper.png"));
    //m_ptrLoadFromFile      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrLoadFromFile      ->setStatusTip(tr("Load node descriptor from file"));
    m_ptrLoadFromFile      ->setEnabled(false);
    //
    m_ptrSaveToFile      = new QAction(tr("Save to file"), this);
    m_ptrSaveToFile      ->setIconVisibleInMenu(true);
    m_ptrSaveToFile      ->setIcon(QIcon(":/images/images/save_as.png"));
    //m_ptrLoadFromFile      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrSaveToFile      ->setStatusTip(tr("Save node descriptor to file"));
    m_ptrSaveToFile      ->setEnabled(false);
    //
    m_ptrSaveNodeDescriptor     = new QAction(tr("Save node descriptor"), this);
    m_ptrSaveNodeDescriptor     ->setIconVisibleInMenu(true);
    m_ptrSaveNodeDescriptor     ->setIcon(QIcon(":/images/images/disk_green.png"));
    m_ptrSaveNodeDescriptor     ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_S));
    m_ptrSaveNodeDescriptor     ->setStatusTip(tr("Save changed node descriptor"));
    m_ptrSaveNodeDescriptor     ->setEnabled(false);
    //
    m_ptrUndo      = new QAction(tr("Undo"), this);
    m_ptrUndo      ->setIconVisibleInMenu(true);
    m_ptrUndo      ->setIcon(QIcon(":/images/images/undo.png"));
    m_ptrUndo      ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_Z));
    m_ptrUndo      ->setStatusTip(tr("Undo last action"));
    m_ptrUndo      ->setEnabled(false);
    //
    m_ptrRedo      = new QAction(tr("Redo"), this);
    m_ptrRedo      ->setIconVisibleInMenu(true);
    m_ptrRedo      ->setIcon(QIcon(":/images/images/redo.png"));
    m_ptrRedo      ->setShortcut(QKeySequence ("Ctrl+Shift+Z") );
    m_ptrRedo      ->setStatusTip(tr("Redo last action"));
    m_ptrRedo      ->setEnabled(false);
    //
    m_ptrPrint      = new QAction(tr("Print"), this);
    m_ptrPrint      ->setIconVisibleInMenu(true);
    m_ptrPrint      ->setIcon(QIcon(":/images/images/printer.png"));
    m_ptrPrint      ->setShortcut(QKeySequence ("Ctrl+Shift+P"));
    m_ptrPrint      ->setStatusTip(tr("Print node descriptor"));
    m_ptrPrint      ->setEnabled(false);
    //
    m_ptrPrintPreview      = new QAction(tr("Print preview"), this);
    m_ptrPrintPreview      ->setIconVisibleInMenu(true);
    m_ptrPrintPreview      ->setIcon(QIcon(":/images/images/printer_view.png"));
    m_ptrPrintPreview      ->setShortcut(QKeySequence ("Ctrl+Shift+V"));
    m_ptrPrintPreview      ->setStatusTip(tr("Print preview for node descriptor"));
    m_ptrPrintPreview      ->setEnabled(false);
    //
    m_ptrExportToPdf      = new QAction(tr("Export to PDF"), this);
    m_ptrExportToPdf      ->setIconVisibleInMenu(true);
    m_ptrExportToPdf      ->setIcon(QIcon(":/images/images/document_into.png"));
    //m_ptrLoadFromFile      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrExportToPdf      ->setStatusTip(tr("Export node descriptor to PDF file"));
    m_ptrExportToPdf      ->setEnabled(false);
    //
    m_ptrChangeTextColor      = new QAction(tr("Text"), this);
    m_ptrChangeTextColor      ->setIconVisibleInMenu(true);
    //we set icon dynamically later
    //m_ptrChangeTextColor      ->setIcon(QIcon(":/images/images/document_into.png"));
    //m_ptrLoadFromFile      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrChangeTextColor      ->setStatusTip(tr("Set text color"));
    m_ptrChangeTextColor      ->setEnabled(false);
    //
    m_ptrChangeBackgroundColor      = new QAction(tr("Background"), this);
    m_ptrChangeBackgroundColor      ->setIconVisibleInMenu(true);
    //we set icon dynamically later
    //m_ptrChangeBackgroundColor    ->setIcon(QIcon(":/images/images/document_into.png"));
    //m_ptrChangeBackgroundColor    ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrChangeBackgroundColor      ->setStatusTip(tr("Set background color"));
    m_ptrChangeBackgroundColor      ->setEnabled(false);
    //
    m_ptrInsertHTML      = new QAction(tr("Insert HTML code"), this);
    m_ptrInsertHTML      ->setIconVisibleInMenu(true);
    m_ptrInsertHTML    ->setIcon(QIcon(":/images/images/code_add.png"));
    //m_ptrInsertHTML    ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrInsertHTML      ->setStatusTip(tr("Insert HTML-code in the document"));
    m_ptrInsertHTML      ->setEnabled(false);
    //
    m_ptrInsertImageLink      = new QAction(tr("Insert image as link to the file"), this);
    m_ptrInsertImageLink      ->setIconVisibleInMenu(true);
    m_ptrInsertImageLink      ->setIcon(QIcon(":/images/images/picture.png"));
    //m_ptrInsertImageLink    ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrInsertImageLink      ->setStatusTip(tr("Insert link to the image file"));
    m_ptrInsertImageLink      ->setEnabled(false);
    //
    m_ptrSentenceCase      = new QAction(tr("Sentence case"), this);
    //m_ptrSentenceCase    ->setIconVisibleInMenu(true);
    //m_ptrSentenceCase    ->setIcon(QIcon(":/images/images/document_into.png"));
    //m_ptrSentenceCase    ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrSentenceCase      ->setStatusTip(tr("Set words case as in sentence"));
    m_ptrSentenceCase      ->setEnabled(false);
    //
    m_ptrUpperCase      = new QAction(tr("UPPER CASE"), this);
    //m_ptrUpperCase    ->setIconVisibleInMenu(true);
    //m_ptrUpperCase    ->setIcon(QIcon(":/images/images/document_into.png"));
    m_ptrUpperCase    ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_Up));
    m_ptrUpperCase      ->setStatusTip(tr("SET UPPER CASE"));
    m_ptrUpperCase      ->setEnabled(false);
    //
    m_ptrLowerCase      = new QAction(tr("lower case"), this);
    //m_ptrLowerCase    ->setIconVisibleInMenu(true);
    //m_ptrLowerCase    ->setIcon(QIcon(":/images/images/document_into.png"));
    m_ptrLowerCase    ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_Down));
    m_ptrLowerCase      ->setStatusTip(tr("set lower case"));
    m_ptrLowerCase      ->setEnabled(false);
    //
    m_ptrToggleCase      = new QAction(tr("tOGGLE cASE"), this);
    //m_ptrToggleCase    ->setIconVisibleInMenu(true);
    //m_ptrToggleCase    ->setIcon(QIcon(":/images/images/document_into.png"));
    //m_ptrToggleCase    ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrToggleCase      ->setStatusTip(tr("toggle case for selection"));
    m_ptrToggleCase      ->setEnabled(false);
    //
    m_ptrBold      = new QAction(tr("Make bold"), this);
    m_ptrBold      ->setIconVisibleInMenu(true);
    m_ptrBold      ->setIcon(QIcon(":/images/images/textbold.png"));
    m_ptrBold      ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_B));
    m_ptrBold      ->setStatusTip(tr("Make text bold"));
    m_ptrBold      ->setEnabled(false);
    //
    m_ptrUnderline      = new QAction(tr("Make underline"), this);
    m_ptrUnderline      ->setIconVisibleInMenu(true);
    m_ptrUnderline      ->setIcon(QIcon(":/images/images/textunder.png"));
    m_ptrUnderline      ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_U));
    m_ptrUnderline      ->setStatusTip(tr("Make text underline"));
    m_ptrUnderline      ->setEnabled(false);
    //
    m_ptrItalic      = new QAction(tr("Make italic"), this);
    m_ptrItalic      ->setIconVisibleInMenu(true);
    m_ptrItalic      ->setIcon(QIcon(":/images/images/textitalic.png"));
    m_ptrItalic      ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_I));
    m_ptrItalic      ->setStatusTip(tr("Make text italic"));
    m_ptrItalic      ->setEnabled(false);
    //
    m_ptrTextAlignLeft      = new QAction(tr("Left"), this);
    m_ptrTextAlignLeft      ->setIconVisibleInMenu(true);
    m_ptrTextAlignLeft      ->setIcon(QIcon(":/images/images/textleft.png"));
    //m_ptrTextAlignLeft      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrTextAlignLeft      ->setStatusTip(tr("Align text left"));
    m_ptrTextAlignLeft      ->setEnabled(false);
    //
    m_ptrTextAlignRight      = new QAction(tr("Right"), this);
    m_ptrTextAlignRight      ->setIconVisibleInMenu(true);
    m_ptrTextAlignRight      ->setIcon(QIcon(":/images/images/textright.png"));
    //m_ptrTextAlignRight      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrTextAlignRight      ->setStatusTip(tr("Align text right"));
    m_ptrTextAlignRight      ->setEnabled(false);
    //
    m_ptrTextAlignCenter      = new QAction(tr("Center"), this);
    m_ptrTextAlignCenter      ->setIconVisibleInMenu(true);
    m_ptrTextAlignCenter      ->setIcon(QIcon(":/images/images/textcenter.png"));
    //m_ptrTextAlignRight      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrTextAlignCenter      ->setStatusTip(tr("Align text center"));
    m_ptrTextAlignCenter      ->setEnabled(false);
    //
    m_ptrTextAlignJustify      = new QAction(tr("Justify"), this);
    m_ptrTextAlignJustify      ->setIconVisibleInMenu(true);
    m_ptrTextAlignJustify      ->setIcon(QIcon(":/images/images/textjustify.png"));
    //m_ptrTextAlignJustify      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrTextAlignJustify      ->setStatusTip(tr("Align text center"));
    m_ptrTextAlignJustify      ->setEnabled(false);
    //
/*
    m_ptrZoomIn      = new QAction(tr("Zoom In"), this);
    m_ptrZoomIn      ->setIconVisibleInMenu(true);
    //m_ptrZoomIn      ->setIcon(QIcon(":/images/images/textjustify.png"));
    //m_ptrZoomIn      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrZoomIn      ->setStatusTip(tr("Zoom in"));
    m_ptrZoomIn      ->setEnabled(true);
*/
    //
    m_ptrEditorToolBar = new QToolBar();
    //
    m_ptrEditorToolBar->addAction(m_ptrLoadFromFile);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrSaveToFile);
    m_ptrEditorToolBar->addSeparator();
    //
    m_ptrEditorToolBar->addAction(m_ptrSaveNodeDescriptor);
    m_ptrEditorToolBar->addSeparator();
    //
    m_ptrEditorToolBar->addAction(m_ptrUndo);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrRedo);
    //
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrPrint);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrPrintPreview);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrExportToPdf);
    //
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrBold);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrUnderline);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrItalic);
    //
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrTextAlignLeft);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrTextAlignRight);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrTextAlignCenter);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrTextAlignJustify);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrInsertImageLink);
    m_ptrEditorToolBar->addSeparator();
    m_ptrEditorToolBar->addAction(m_ptrInsertHTML);
    //
    m_ptrEditorToolBar->setWindowTitle(tr("Editor toolbar"));
    //m_ptrEditorToolBar->addSeparator();
    //m_ptrEditorToolBar->addAction(m_ptrZoomIn);
}

void MnuMainWindow::createTableSubMenu ()
{
    m_ptrInsertTable      = new QAction(tr("Insert table"), this);
    m_ptrInsertTable      ->setIconVisibleInMenu(true);
    m_ptrInsertTable      ->setIcon(QIcon(":/images/images/table_add.png"));
    //m_ptrInsertTable      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrInsertTable      ->setStatusTip(tr("Insert table in the document"));
    m_ptrInsertTable      ->setEnabled(false);
    //
    m_ptrInsertRow      = new QAction(tr("Insert row"), this);
    m_ptrInsertRow      ->setIconVisibleInMenu(true);
    m_ptrInsertRow      ->setIcon(QIcon(":/images/images/row_add.png"));
    //m_ptrInsertRow      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrInsertRow      ->setStatusTip(tr("Insert raw(s) in the table"));
    m_ptrInsertRow      ->setEnabled(false);
    //
    m_ptrInsertColumn      = new QAction(tr("Insert column"), this);
    m_ptrInsertColumn      ->setIconVisibleInMenu(true);
    m_ptrInsertColumn      ->setIcon(QIcon(":/images/images/coll_add.png"));
    //m_ptrInsertColumn      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrInsertColumn      ->setStatusTip(tr("Insert column(s) in the table"));
    m_ptrInsertColumn      ->setEnabled(false);
    //
    m_ptrRemoveRow      = new QAction(tr("Remove row"), this);
    m_ptrRemoveRow      ->setIconVisibleInMenu(true);
    m_ptrRemoveRow      ->setIcon(QIcon(":/images/images/row_remove.png"));
    //m_ptrRemoveRow      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrRemoveRow      ->setStatusTip(tr("Remove current raw from the table"));
    m_ptrRemoveRow      ->setEnabled(false);
    //
    m_ptrRemoveColumn      = new QAction(tr("Remove column"), this);
    m_ptrRemoveColumn      ->setIconVisibleInMenu(true);
    m_ptrRemoveColumn      ->setIcon(QIcon(":/images/images/col_remove.png"));
    //m_ptrRemoveColumn      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrRemoveColumn      ->setStatusTip(tr("Remove current column from the table"));
    m_ptrRemoveColumn      ->setEnabled(false);
    //
    m_ptrCellsMerge      = new QAction(tr("Merge cells"), this);
    m_ptrCellsMerge      ->setIconVisibleInMenu(true);
    m_ptrCellsMerge      ->setIcon(QIcon(":/images/images/cell_merge.png"));
    //m_ptrCellsMerge      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrCellsMerge      ->setStatusTip(tr("Merge selected cells"));
    m_ptrCellsMerge      ->setEnabled(false);
    //
    m_ptrCellSplit      = new QAction(tr("Split cell"), this);
    m_ptrCellSplit      ->setIconVisibleInMenu(true);
    m_ptrCellSplit      ->setIcon(QIcon(":/images/images/cell_split.png"));
    //m_ptrCellSplit      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrCellSplit      ->setStatusTip(tr("Split current cell"));
    m_ptrCellSplit      ->setEnabled(false);
    //
    m_ptrTableSettings      = new QAction(tr("Change table settings"), this);
    m_ptrTableSettings      ->setIconVisibleInMenu(true);
    m_ptrTableSettings      ->setIcon(QIcon(":/images/images/table_edit.png"));
    //m_ptrTableSettings      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrTableSettings      ->setStatusTip(tr("Change settings of the current table"));
    m_ptrTableSettings      ->setEnabled(false);
    //
    m_ptrTableToolBar  = new QToolBar();
    m_ptrTableToolBar ->addAction(m_ptrInsertTable);
    m_ptrTableToolBar ->addSeparator();
    m_ptrTableToolBar ->addAction(m_ptrInsertRow);
    m_ptrTableToolBar ->addSeparator();
    m_ptrTableToolBar ->addAction(m_ptrInsertColumn);
    m_ptrTableToolBar ->addSeparator();
    m_ptrTableToolBar ->addAction(m_ptrRemoveRow);
    m_ptrTableToolBar ->addSeparator();
    m_ptrTableToolBar ->addAction(m_ptrRemoveColumn);
    m_ptrTableToolBar ->addSeparator();
    m_ptrTableToolBar ->addAction(m_ptrCellsMerge);
    m_ptrTableToolBar ->addSeparator();
    m_ptrTableToolBar ->addAction(m_ptrCellSplit);
    m_ptrTableToolBar ->addSeparator();
    m_ptrTableToolBar ->addAction(m_ptrTableSettings);
    m_ptrTableToolBar ->addSeparator();
    m_ptrTableToolBar->setWindowTitle(tr("Tables toolbar"));
}

void MnuMainWindow::assemblyTreeMenu(QMenu* ptr_node_menu)
{
    if (NULL == ptr_node_menu)
        return;
    //
    ptr_node_menu->addAction(m_ptrCreateNewTree);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrRenameTree);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrDelTree);
    ptr_node_menu->addSeparator();
    //m_ptrTreeControlMenu->addAction(m_ptrRestoreTree);
    //m_ptrTreeControlMenu->addSeparator();
    ptr_node_menu->addAction(m_ptrInfoTree);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrSearchInTree);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrBackupDatabase);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrExitApp);
}

void MnuMainWindow::assemblyNodeMenu(QMenu* ptr_node_menu)
{
    if (NULL == ptr_node_menu)
        return;
    //
    ptr_node_menu->addAction(m_ptrInsertNewNode);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrChangeNodeColor);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrSetNodeIcon);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrDelNode);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrRestoreNode);
    ptr_node_menu->addAction(m_ptrRestoreSubtree);
    ptr_node_menu->addSeparator();
    //
    ptr_node_menu->addAction (m_ptrCutNode);
    ptr_node_menu->addAction (m_ptrPasteNode);
    //
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrExportNode);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrImportNode);
    //
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrExpandSubtree);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrCollapseSubtree);
}

void MnuMainWindow::assemblyAttachMenu( QMenu* ptr_node_menu )
{
    if (NULL == ptr_node_menu)
        return;
    //
    ptr_node_menu->addAction(m_ptrRestoreAttach);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrDeleteAttach);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrExportAttach);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrImportAttach);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrReplaceAttach);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrCutAttach);
    ptr_node_menu->addAction(m_ptrPasteAttach);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrViewAttach);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrDirectScan);
}

void MnuMainWindow::assemblyNodeCryptoSubMenu(QMenu* ptr_menu)
{
    if (NULL == ptr_menu)
        return;
    //
    ptr_menu->addAction(m_ptrEncryptAttachmentsOfAllNodes);
    ptr_menu->addSeparator();
    ptr_menu->addAction(m_ptrDecryptAttachmentsOfAllNodes);
}

void MnuMainWindow::assemblyNodeProtectSubMenu (QMenu* ptr_menu)
{
    if (NULL == ptr_menu)
        return;
    //
    ptr_menu->addAction(m_ptrProtectAttachmentsOfAllNodes);
    ptr_menu->addSeparator();
    ptr_menu->addAction(m_ptrUn_ProtectAttachmentsOfAllNodes);
}

void MnuMainWindow::assemblyTextColorSubMenu  ( QMenu* ptr_set_text_color_menu )
{
    ptr_set_text_color_menu->addAction(m_ptrChangeTextColor);
    ptr_set_text_color_menu->addSeparator();
    ptr_set_text_color_menu->addAction(m_ptrChangeBackgroundColor);
}

void MnuMainWindow::assemblyTableSubMenu ( QMenu* ptr_table_menu )
{
    ptr_table_menu->addAction(m_ptrInsertTable);
    ptr_table_menu->addSeparator();
    ptr_table_menu->addAction(m_ptrInsertRow);
    ptr_table_menu->addSeparator();
    ptr_table_menu->addAction(m_ptrInsertColumn);
    ptr_table_menu->addSeparator();
    ptr_table_menu->addAction(m_ptrRemoveRow);
    ptr_table_menu->addSeparator();
    ptr_table_menu->addAction(m_ptrRemoveColumn);
    ptr_table_menu->addSeparator();
    ptr_table_menu->addAction(m_ptrCellsMerge);
    ptr_table_menu->addSeparator();
    ptr_table_menu->addAction(m_ptrCellSplit);
    ptr_table_menu->addSeparator();
    ptr_table_menu->addAction(m_ptrTableSettings);
}

void MnuMainWindow::assemblyInsertInTextSubMenu ( QMenu* ptr_insert_in_text_menu )
{
    ptr_insert_in_text_menu->addAction(m_ptrInsertHTML);
    ptr_insert_in_text_menu->addSeparator();
    ptr_insert_in_text_menu->addAction(m_ptrInsertImageLink);
    //ptr_insert_in_text_menu->addSeparator();
    //ptr_insert_in_text_menu->addAction();
}

void MnuMainWindow::assemblyAlignSubMenu ( QMenu* ptr_align_menu )
{
    ptr_align_menu->addAction(m_ptrTextAlignLeft);
    ptr_align_menu->addSeparator();
    ptr_align_menu->addAction(m_ptrTextAlignRight);
    ptr_align_menu->addSeparator();
    ptr_align_menu->addAction(m_ptrTextAlignCenter);
    ptr_align_menu->addSeparator();
    ptr_align_menu->addAction(m_ptrTextAlignJustify);
}

void MnuMainWindow::assemblyTextAttribSubMenu       ( QMenu* ptr_attrib_menu)
{
    ptr_attrib_menu->addAction(m_ptrBold);
    ptr_attrib_menu->addSeparator();
    ptr_attrib_menu->addAction(m_ptrUnderline);
    ptr_attrib_menu->addSeparator();
    ptr_attrib_menu->addAction(m_ptrItalic);
}

void MnuMainWindow::assemblyChangeCaseSubMenu ( QMenu* ptr_case_menu )
{
    ptr_case_menu->addAction(m_ptrSentenceCase);
    ptr_case_menu->addSeparator();
    ptr_case_menu->addAction(m_ptrUpperCase);
    ptr_case_menu->addSeparator();
    ptr_case_menu->addAction(m_ptrLowerCase);
    ptr_case_menu->addSeparator();
    ptr_case_menu->addAction(m_ptrToggleCase);
}

void MnuMainWindow::assemblyAttachProtectionSubMenu( QMenu* ptr_node_menu )
{
    if (NULL == ptr_node_menu)
        return;
    //
    ptr_node_menu->addAction(m_ptrProtectAllAttachmentsOfNode);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrUn_ProtectAllAttachmentsOfNode);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrProtectSelectedAttachments);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrUn_ProtectSelectedAttachments);
}

void MnuMainWindow::assemblyAttachCryptographySubMenu ( QMenu* ptr_node_menu )
{
    if (NULL == ptr_node_menu)
        return;
    //
    ptr_node_menu->addAction(m_ptrEncryptAllAttachmentsOfNode);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrDecryptAllAttachmentsOfNode);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrEncryptSelectedAttachments);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrDecryptSelectedAttachments);
}

void MnuMainWindow::assemblyViewMenu( QMenu* ptr_node_menu )
{
    m_ptrToolbarsMenu = ptr_node_menu->addMenu("Toolbars");
    //
    m_ptrToolbarsMenu->addAction(m_ptrMnuTreeToolBar);
    m_ptrToolbarsMenu->addSeparator();
    //
    m_ptrToolbarsMenu->addAction(m_ptrMnuNodeToolBar);
    m_ptrToolbarsMenu->addSeparator();
    //
    m_ptrToolbarsMenu->addAction(m_ptrMnuAttachmentToolBar);
    m_ptrToolbarsMenu->addSeparator();
    //
    m_ptrToolbarsMenu->addAction(m_ptrMnuEditorToolBar);
    m_ptrToolbarsMenu->addSeparator();
    //
    m_ptrToolbarsMenu->addAction(m_ptrMnuTableToolBar);
    m_ptrToolbarsMenu->addSeparator();
    //
    m_ptrToolbarsMenu->addAction(m_ptrMnuSecurityToolBar);
    //
    ptr_node_menu->addSeparator();
    //
    m_ptrLogWindow  = new QAction("Show log", this);
    m_ptrLogWindow  ->setCheckable(true);
    m_ptrLogWindow  ->setToolTip(tr("Show/hide log window with service messages"));
    //
    ptr_node_menu->addAction(m_ptrLogWindow);
}

void MnuMainWindow::assemblySecurityMenu( QMenu* ptr_node_menu )
{
    ptr_node_menu->addAction(m_ptrCreateChangePassword);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrLockAppScreen);
}

void MnuMainWindow::assemblyHelpMenu()
{
    m_ptrHelpMenu->addAction(m_ptrAbout);
}

void MnuMainWindow::assemblyEditorMenu ( QMenu* ptr_node_menu )
{
    if (NULL == ptr_node_menu)
        return;
    //TODO
    ptr_node_menu->addAction(m_ptrLoadFromFile);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrSaveToFile);
    ptr_node_menu->addSeparator();
    //
    ptr_node_menu->addAction(m_ptrSaveNodeDescriptor);
    ptr_node_menu->addSeparator();
    //
    ptr_node_menu->addAction(m_ptrUndo);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrRedo);
    ptr_node_menu->addSeparator();
    //
    ptr_node_menu->addAction(m_ptrPrint);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrPrintPreview);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrExportToPdf);
}

void MnuMainWindow::assemblyAllMenus()
{
    m_ptrTreeControlMenu = m_ptrParent->menuBar()->addMenu(tr("&Tree"));
    //======================== tree menu ========================
    assemblyTreeMenu(m_ptrTreeControlMenu);
    //======================== node menu ========================
    m_ptrNodeControlMenu = m_ptrParent->menuBar()->addMenu(tr("&Node"));
    //
    assemblyNodeMenu(m_ptrNodeControlMenu);
    //
    m_ptrNodeControlMenu->addSeparator();
    m_ptrNodeCryptoSubMenu = m_ptrNodeControlMenu->addMenu(tr("Encrypt/Decrypt..."));
    //
    assemblyNodeCryptoSubMenu(m_ptrNodeCryptoSubMenu);
    //
    m_ptrNodeControlMenu->addSeparator();
    m_ptrNodeProtectSubMenu= m_ptrNodeControlMenu->addMenu(tr("Protect/Unprotect..."));
    //
    assemblyNodeProtectSubMenu(m_ptrNodeProtectSubMenu);
    //======================== editor menu ========================
    m_ptrEditorMenu = m_ptrParent->menuBar()->addMenu(tr("&Editor"));
    assemblyEditorMenu(m_ptrEditorMenu);
    //
    m_ptrEditorMenu ->addSeparator();
    m_ptrTextColorMenu = m_ptrEditorMenu->addMenu(tr("Set color"));
    assemblyTextColorSubMenu(m_ptrTextColorMenu);
    //
    m_ptrEditorMenu ->addSeparator();
    m_ptrTableMenu = m_ptrEditorMenu->addMenu(tr("Table"));
    assemblyTableSubMenu(m_ptrTableMenu);
    //
    m_ptrEditorMenu ->addSeparator();
    m_ptrInsertInTextMenu = m_ptrEditorMenu->addMenu(tr("Insert..."));
    assemblyInsertInTextSubMenu(m_ptrInsertInTextMenu);
    //
    m_ptrEditorMenu ->addSeparator();
    m_ptrCaseMenu = m_ptrEditorMenu->addMenu(tr("Change case"));
    assemblyChangeCaseSubMenu(m_ptrCaseMenu);
    //
    m_ptrEditorMenu ->addSeparator();
    m_ptrMakeTextAttribSubMenu = m_ptrEditorMenu->addMenu(tr("Font attributes"));
    assemblyTextAttribSubMenu(m_ptrMakeTextAttribSubMenu);
    //
    m_ptrEditorMenu ->addSeparator();
    m_ptrAlignSubMenu = m_ptrEditorMenu->addMenu(tr("Alignment"));
    assemblyAlignSubMenu(m_ptrAlignSubMenu);
    //======================== attachment menu ========================
    m_ptrAttachmentMenu = m_ptrParent->menuBar()->addMenu(tr("&Attachment"));
    //
    assemblyAttachMenu(m_ptrAttachmentMenu);
    //
    m_ptrAttachmentMenu->addSeparator();
    m_ptrCryptography = m_ptrAttachmentMenu->addMenu(tr("Encrypt/Decrypt"));
    assemblyAttachCryptographySubMenu(m_ptrCryptography);
    //
    m_ptrAttachmentMenu->addSeparator();
    m_ptrAttachProtectionSubMenu = m_ptrAttachmentMenu->addMenu(tr("Protection..."));
    //
    assemblyAttachProtectionSubMenu(m_ptrAttachProtectionSubMenu);
    //======================== view menu ========================
    m_ptrViewMenu = m_ptrParent->menuBar()->addMenu(tr("View"));
    //
    assemblyViewMenu(m_ptrViewMenu);
    //======================== security menu ========================
    m_ptrSecurity = m_ptrParent->menuBar()->addMenu(tr("Security"));
    //
    assemblySecurityMenu(m_ptrSecurity);
    //
    //======================== help menu ========================
    //
    m_ptrHelpMenu = m_ptrParent->menuBar()->addMenu(tr("&Help"));
    assemblyHelpMenu();
}

void MnuMainWindow::syncToolbarsVisibilityAndMenu ()
{
    if ( m_ptrTreeToolBar )
        m_ptrTreeToolBar->isVisible() ? m_ptrMnuTreeToolBar->setChecked(true) : m_ptrMnuTreeToolBar->setChecked(false);
    //
    if ( m_ptrNodeToolBar )
        m_ptrNodeToolBar->isVisible() ? m_ptrMnuNodeToolBar->setChecked(true) : m_ptrMnuNodeToolBar->setChecked(false);
    //
    if ( m_ptrAttachmentToolBar )
        m_ptrAttachmentToolBar->isVisible() ? m_ptrMnuAttachmentToolBar->setChecked(true) : m_ptrMnuAttachmentToolBar->setChecked(false);
    //
    if ( m_ptrEditorToolBar )
        m_ptrEditorToolBar->isVisible() ? m_ptrMnuEditorToolBar->setChecked(true) : m_ptrMnuEditorToolBar->setChecked(false);
    //
    if ( m_ptrTableToolBar)
        m_ptrTableToolBar->isVisible() ? m_ptrMnuTableToolBar->setChecked(true) : m_ptrMnuTableToolBar->setChecked(false);
    //
    if (m_ptrSecurityToolBar)
        m_ptrSecurityToolBar->isVisible() ? m_ptrMnuSecurityToolBar->setChecked(true) : m_ptrMnuSecurityToolBar->setChecked(false);
    //
    return;
}
