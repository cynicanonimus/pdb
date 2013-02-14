#include "mnumainwindow.h"
#include "mainwindow.h"
#include "treeleaf.h"
#include "servicescfg.h"
#include <QMenuBar>

MnuMainWindow::MnuMainWindow( MainWindow* parent ) :
    QObject(parent)
{
    //
    Q_ASSERT ( parent );
    m_ptrParent         = parent;
    m_bPasswordExist    = false;
    //
    createTreeControlMenu();
    createNodeControlMenu();
    createAttachmentControlMenu();
    createCryptograficMenu();
    createProtectionMenu();
    //
    createSecurityMenu();
    //
    assemblyAllMenus();
}

MnuMainWindow::~MnuMainWindow()
{
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


    //if ( m_ptrRefreshAttach ) delete  m_ptrRefreshAttach;

}

void enableSaveNodeDescriptor           (bool);

//-------------------------- signal processing begin  ----------------------------------------
void  MnuMainWindow::onCheckPassword ( bool b_password_exist )
{
    m_bPasswordExist = b_password_exist;
    //
    if (m_bPasswordExist)
    {
        if ( ServicesCfg::getInstance().getPassword().length() == 0 )
            m_ptrCreateChangePassword->setText ( "Enter password..." );
        else
            m_ptrCreateChangePassword->setText ( "Change password..." );
    }
    else
        m_ptrCreateChangePassword->setText ( "Create protection password..." );
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
    AbstractDatabaseObject::DB_OBJECT_STATUS node_state = ptr_to_current->getObjectStatus();
    bool b_is_root_node    = (ptr_to_current->getParentID() == 0);
    //
    m_ptrEncryptAttachmentsOfAllNodes       ->setEnabled(false);
    m_ptrDecryptAttachmentsOfAllNodes       ->setEnabled(false);
        //
    m_ptrProtectAttachmentsOfAllNodes       ->setEnabled(false);
    m_ptrUn_ProtectAttachmentsOfAllNodes    ->setEnabled(false);
    //
    switch(node_state)
    {
    case AbstractDatabaseObject::OBJECT_NOT_DEFINED:
        m_ptrDelNode                ->setEnabled(false);
        m_ptrInsertNewNode          ->setEnabled(false);
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

void MnuMainWindow::adjustGlobalProtectionMenu  ( TreeLeaf* ptr_to_current, AttachStatusChecker &checker)
{
    //get all attachments
    const Attachment::AttachmentsList& attachments =  ptr_to_current->getAttachments();
    //analyse attachments
    analyseAttachmentsStatus(attachments,  checker);

    //request child nodes attachment info
    if ( (false == checker.m_bAtLeastOneAttachmentOKAndProtected) ||
         (false == checker.m_bAtLeastOneAttachmentOKAndUnprotected)
       )
    {
        TreeLeaf::ChildList childs = ptr_to_current->getChildList();
        for (unsigned int i = 0; i < childs.size(); i++)
        {
            adjustGlobalProtectionMenu(childs[i], checker);
        }
       // ptr_to_current->get
    };
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
    analyseAttachmentsStatus(attachments,  checker);

    if ( 0 == checker.m_iAttachmentsAmount ) //nothing attached to this node TODO
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

void MnuMainWindow::createAttachmentControlMenu()
{
    m_ptrRestoreAttach  = new QAction(tr("Restore attachment"), this);
    m_ptrRestoreAttach  ->setIconVisibleInMenu(true);
    m_ptrRestoreAttach  ->setIcon(QIcon(":/images/images/restore2.ico"));
    m_ptrRestoreAttach  ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_R));
    m_ptrRestoreAttach  ->setStatusTip(tr("Restore selected attachment"));
    m_ptrRestoreAttach  ->setEnabled(false);
    //
    m_ptrDeleteAttach   = new QAction(tr("Delete attachment"), this);
    m_ptrDeleteAttach   ->setIconVisibleInMenu(true);
    m_ptrDeleteAttach   ->setIcon(QIcon(":/images/images/delete2.ico"));
    m_ptrDeleteAttach   ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_D));
    m_ptrDeleteAttach   ->setStatusTip(tr("Delete selected attachment"));
    m_ptrDeleteAttach   ->setEnabled(false);
    //
    m_ptrExportAttach   = new QAction(tr("Download attachment"), this);
    m_ptrExportAttach   ->setIconVisibleInMenu(true);
    m_ptrExportAttach   ->setIcon(QIcon(":/images/images/download.ico"));
    m_ptrExportAttach   ->setShortcut(QKeySequence (Qt::CTRL + Qt::Key_E));
    m_ptrExportAttach   ->setStatusTip(tr("Export selected attachment"));
    m_ptrExportAttach   ->setEnabled(false);
    //
    m_ptrImportAttach   = new QAction(tr("Import attachment"), this);
    m_ptrImportAttach   ->setIconVisibleInMenu(true);
    m_ptrImportAttach   ->setIcon(QIcon(":/images/images/add2.ico"));
    m_ptrImportAttach   ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_I));
    m_ptrImportAttach   ->setStatusTip(tr("Attach files to the node"));
    m_ptrImportAttach   ->setEnabled(false);
    //
    m_ptrReplaceAttach  = new QAction(tr("Replace current attachment"), this);
    m_ptrReplaceAttach  ->setIconVisibleInMenu(true);
    m_ptrReplaceAttach  ->setIcon(QIcon(":/images/images/replace.ico"));
    m_ptrReplaceAttach  ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_L));
    m_ptrReplaceAttach  ->setStatusTip(tr("Remove the selected attachment and add another one instead of it"));
    m_ptrReplaceAttach  ->setEnabled(false);
    //
    m_ptrCutAttach      = new QAction(tr("Cut attachment"), this);
    m_ptrCutAttach      ->setIconVisibleInMenu(true);
    m_ptrCutAttach      ->setIcon(QIcon(":/images/images/cut.ico"));
    m_ptrCutAttach      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrCutAttach      ->setStatusTip(tr("Cut selected attachments"));
    m_ptrCutAttach      ->setEnabled(false);
    //
    m_ptrPasteAttach    = new QAction(tr("Paste attachment"), this);
    m_ptrPasteAttach    ->setIconVisibleInMenu(true);
    m_ptrPasteAttach    ->setIcon(QIcon(":/images/images/paste.ico"));
    m_ptrPasteAttach    ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_P));
    m_ptrPasteAttach    ->setStatusTip(tr("Paste selected attachments"));
    m_ptrPasteAttach    ->setEnabled(false);
    //
    m_ptrViewAttach    = new QAction(tr("View attachment"), this);
    m_ptrViewAttach    ->setIconVisibleInMenu(true);
    m_ptrViewAttach    ->setIcon(QIcon(":/images/images/view.ico"));
    m_ptrViewAttach    ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_O));
    m_ptrViewAttach    ->setStatusTip(tr("Open and view attachments"));
    m_ptrViewAttach    ->setEnabled(false);
};

void MnuMainWindow::createSecurityMenu()
{
    m_ptrCreateChangePassword   = new QAction("", this);
    m_ptrCreateChangePassword   ->setIconVisibleInMenu(true);
    //m_ptrCreateChangePassword      ->setIcon(QIcon(":/images/images/no_shield.png"));
    //m_ptrCreateChangePassword      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrCreateChangePassword      ->setStatusTip(tr("Create password for encrypt/decrypt attachments"));
    m_ptrCreateChangePassword      ->setEnabled(true);
};

void MnuMainWindow::createCryptograficMenu ()
{
    m_ptrEncryptAllAttachmentsOfNode      = new QAction(tr("Encrypt all attachments"), this);
    m_ptrEncryptAllAttachmentsOfNode      ->setIconVisibleInMenu(true);
    m_ptrEncryptAllAttachmentsOfNode      ->setIcon(QIcon(":/images/images/lock.ico"));
    //m_ptrEncryptListOfAttachments      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrEncryptAllAttachmentsOfNode      ->setStatusTip(tr("Encrypt all attachments of this node"));
    m_ptrEncryptAllAttachmentsOfNode      ->setEnabled(false);
    //
    m_ptrDecryptAllAttachmentsOfNode      = new QAction(tr("Decrypt all attachments"), this);
    m_ptrDecryptAllAttachmentsOfNode      ->setIconVisibleInMenu(true);
    m_ptrDecryptAllAttachmentsOfNode      ->setIcon(QIcon(":/images/images/lock_open.ico"));
    //m_ptrDecryptAll      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrDecryptAllAttachmentsOfNode      ->setStatusTip(tr("Decrypt all attachments of this node"));
    m_ptrDecryptAllAttachmentsOfNode      ->setEnabled(false);
    //
    m_ptrEncryptSelectedAttachments        = new QAction(tr("Encrypt selected attachment"), this);
    m_ptrEncryptSelectedAttachments        ->setIconVisibleInMenu(true);
    //m_ptrEncryptSelectedAttachments        ->setIcon(QIcon(":/images/images/no_shield.png"));
    //m_ptrEncryptSelectedAttachments   ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrEncryptSelectedAttachments        ->setStatusTip(tr("Encrypt selected attachment"));
    m_ptrEncryptSelectedAttachments        ->setEnabled(false);
    //
    m_ptrDecryptSelectedAttachments       = new QAction(tr("Decrypt selected attachment"), this);
    m_ptrDecryptSelectedAttachments        ->setIconVisibleInMenu(true);
    //m_ptrDecryptSelectedAttachments        ->setIcon(QIcon(":/images/images/no_shield.png"));
    //m_ptrDecryptSelectedAttachments   ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrDecryptSelectedAttachments        ->setStatusTip(tr("Decrypt selected attachment"));
    m_ptrDecryptSelectedAttachments        ->setEnabled(false);

}

void MnuMainWindow::createProtectionMenu()
{
    m_ptrProtectAllAttachmentsOfNode= new QAction(tr("Protect all attachments "), this);
    m_ptrProtectAllAttachmentsOfNode      ->setIconVisibleInMenu(true);
    m_ptrProtectAllAttachmentsOfNode      ->setIcon(QIcon(":/images/images/no_shield.png"));
    //m_ptrProtectAllAttachmentsOfNode      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrProtectAllAttachmentsOfNode      ->setStatusTip(tr("Protect all attachments of this node"));
    m_ptrProtectAllAttachmentsOfNode      ->setEnabled(false);
    //
    m_ptrUn_ProtectAllAttachmentsOfNode= new QAction(tr("Remove protection of all attachments"), this);
    m_ptrUn_ProtectAllAttachmentsOfNode      ->setIconVisibleInMenu(true);
    m_ptrUn_ProtectAllAttachmentsOfNode      ->setIcon(QIcon(":/images/images/shield_add.png"));
    //m_ptrUn_ProtectAllAttachmentsOfNode      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrUn_ProtectAllAttachmentsOfNode      ->setStatusTip(tr("Protect all attachments of this node"));
    m_ptrUn_ProtectAllAttachmentsOfNode      ->setEnabled(false);
    //
    m_ptrProtectSelectedAttachments= new QAction(tr("Protect selected attachment"), this);
    m_ptrProtectSelectedAttachments      ->setIconVisibleInMenu(true);
    m_ptrProtectSelectedAttachments      ->setIcon(QIcon(":/images/images/no_shield.png"));
    //m_ptrProtectSelectedAttachments      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrProtectSelectedAttachments      ->setStatusTip(tr("Set protection for selected attachment"));
    m_ptrProtectSelectedAttachments      ->setEnabled(false);
    //
    m_ptrUn_ProtectSelectedAttachments= new QAction(tr("Remove protection of selected attachment"), this);
    m_ptrUn_ProtectSelectedAttachments      ->setIconVisibleInMenu(true);
    m_ptrUn_ProtectSelectedAttachments      ->setIcon(QIcon(":/images/images/shield.png"));
    //m_ptrDropThisProtect      ->setShortcut(QKeySequence (Qt::CTRL +Qt::Key_M));
    m_ptrUn_ProtectSelectedAttachments      ->setStatusTip(tr("Drop protection for selected attachment"));
    m_ptrUn_ProtectSelectedAttachments      ->setEnabled(false);
}

void MnuMainWindow::createTreeControlMenu()
{
    //for the m_ptrTreeControlMenu
    //
    m_ptrCreateNewTree  = new QAction(tr("Create new tree"), this);
    m_ptrCreateNewTree  ->setIconVisibleInMenu(true);
    m_ptrCreateNewTree  ->setIcon(QIcon(":/images/images/tree.ico"));
    m_ptrCreateNewTree  ->setShortcut(QKeySequence (Qt::Key_F3));
    m_ptrCreateNewTree  ->setStatusTip(tr("Create new tree"));
    m_ptrCreateNewTree  ->setEnabled(true);
    //
    m_ptrDelTree        = new QAction(tr("Delete current tree"), this);
    m_ptrDelTree        ->setShortcut(QKeySequence (Qt::Key_F8));
    m_ptrDelTree        ->setStatusTip(tr("Delete current tree"));
    m_ptrDelTree        ->setEnabled(false);
    //
    m_ptrRenameTree     = new QAction(tr("Rename current tree"), this);
    m_ptrRenameTree     ->setShortcut( QKeySequence (Qt::Key_F6) );
    m_ptrRenameTree     ->setStatusTip(tr("Change name of the current tree"));
    m_ptrRenameTree     ->setEnabled(false);
    //
    m_ptrInfoTree       = new QAction(tr("Info about current tree"), this);
    m_ptrInfoTree      ->setIcon(QIcon(":/images/images/information2.ico"));
    m_ptrInfoTree      ->setShortcut( QKeySequence (Qt::Key_F7) );
    m_ptrInfoTree      ->setStatusTip(tr("Get statistic info about current tree"));
    m_ptrInfoTree      ->setEnabled(false);
    //
    m_ptrSearchInTree   = new QAction(tr("Search..."), this);
    m_ptrSearchInTree   ->setShortcut( QKeySequence (Qt::CTRL + Qt::Key_F) );
    m_ptrSearchInTree   ->setStatusTip(tr("Search in tree(s)"));
    m_ptrSearchInTree   ->setEnabled(false);
    //
    m_ptrExitApp   = new QAction(tr("Exit"), this);
    m_ptrExitApp   ->setIcon(QIcon(":/images/images/exit.ico"));
    m_ptrExitApp   ->setShortcut( QKeySequence (Qt::CTRL + Qt::Key_Q) );
    m_ptrExitApp   ->setStatusTip(tr("Leave the app"));
    m_ptrExitApp   ->setEnabled(true);
};
//
void MnuMainWindow::createNodeControlMenu()
{
    m_ptrDelNode        = new QAction(tr("Delete current node"), this);
    m_ptrDelNode        ->setIconVisibleInMenu(true);
    m_ptrDelNode        ->setIcon(QIcon(":/images/images/delete.ico"));
    m_ptrDelNode        ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_D));
    m_ptrDelNode        ->setStatusTip(tr("Delete current node"));
    m_ptrDelNode        ->setEnabled(false);
    //
    m_ptrRestoreNode    = new QAction(tr("Restore current node"), this);
    m_ptrRestoreNode    ->setIconVisibleInMenu(true);
    m_ptrRestoreNode    ->setIcon(QIcon(":/images/images/restore.ico"));
    m_ptrRestoreNode    ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_R));
    m_ptrRestoreNode    ->setStatusTip(tr("Restore current node"));
    m_ptrRestoreNode    ->setEnabled(false);
    //
    m_ptrRestoreSubtree = new QAction(tr("Completely restore subtree"), this);
    m_ptrRestoreSubtree ->setIconVisibleInMenu(true);
    m_ptrRestoreSubtree ->setIcon(QIcon(":/images/images/restore3.ico"));
    m_ptrRestoreSubtree ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_G));
    m_ptrRestoreSubtree ->setStatusTip(tr("Restore current subtree: all nodes and attachments"));
    m_ptrRestoreSubtree ->setEnabled(false);
    //
    m_ptrInsertNewNode  = new QAction(tr("Insert new node"), this);
    m_ptrInsertNewNode  ->setIconVisibleInMenu(true);
    m_ptrInsertNewNode  ->setIcon(QIcon(":/images/images/add.ico"));
    m_ptrInsertNewNode  ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_Insert));
    m_ptrInsertNewNode  ->setStatusTip(tr("Insert new node"));
    m_ptrInsertNewNode  ->setEnabled(false);
    //
    m_ptrCutNode        = new QAction(tr("Cut node"), this);
    m_ptrCutNode        ->setIconVisibleInMenu(true);
    m_ptrCutNode        ->setIcon(QIcon(":/images/images/cut.ico"));
    m_ptrCutNode        ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_M));
    m_ptrCutNode        ->setStatusTip(tr("Cut node"));
    m_ptrCutNode        ->setEnabled(false);
    //----  alternate menu item
    m_ptrPasteNode      = new QAction(tr("Paste node"), this);
    m_ptrPasteNode      ->setIconVisibleInMenu(true);
    m_ptrPasteNode      ->setIcon(QIcon(":/images/images/paste.ico"));
    m_ptrPasteNode      ->setShortcut(QKeySequence (Qt::ALT + Qt::Key_P));
    m_ptrPasteNode      ->setStatusTip(tr("Paste node"));
    m_ptrPasteNode      ->setEnabled(false);
    //
    m_ptrExportNode     = new QAction(tr("Export..."), this);
    m_ptrExportNode     ->setIconVisibleInMenu(true);
    m_ptrExportNode     ->setIcon(QIcon(":/images/images/export.ico"));
    m_ptrExportNode     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_E));
    m_ptrExportNode     ->setStatusTip(tr("Export current node or subtree to the disk"));
    m_ptrExportNode     ->setEnabled(false);
    //
    m_ptrImportNode     = new QAction(tr("Import..."), this);
    m_ptrImportNode     ->setIconVisibleInMenu(true);
    m_ptrImportNode     ->setIcon(QIcon(":/images/images/import.ico"));
    m_ptrImportNode     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_I));
    m_ptrImportNode     ->setStatusTip(tr("Import from disk under current node"));
    m_ptrImportNode     ->setEnabled(false);
    //
    m_ptrEncryptAttachmentsOfAllNodes     = new QAction(tr("Encrypt all attachments.."), this);
    //m_ptrEncryptAttachmentsOfAllNodes     ->setIconVisibleInMenu(true);
    //m_ptrEncryptAttachmentsOfAllNodes     ->setIcon(QIcon(":/images/images/import.ico"));
    //m_ptrEncryptAttachmentsOfAllNodes     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_I));
    m_ptrEncryptAttachmentsOfAllNodes     ->setStatusTip(tr("Encrypt all attachments of the node (with childs optionally"));
    m_ptrEncryptAttachmentsOfAllNodes     ->setEnabled(false);
    //
    m_ptrDecryptAttachmentsOfAllNodes     = new QAction(tr("Decrypt all attachments.."), this);
    //m_ptrDecryptAttachmentsOfAllNodes     ->setIconVisibleInMenu(true);
    //m_ptrDecryptAttachmentsOfAllNodes     ->setIcon(QIcon(":/images/images/import.ico"));
    //m_ptrDecryptAttachmentsOfAllNodes     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_I));
    m_ptrDecryptAttachmentsOfAllNodes     ->setStatusTip(tr("Decrypt all attachments of the node (with childs optionally"));
    m_ptrDecryptAttachmentsOfAllNodes     ->setEnabled(false);
    //
    m_ptrProtectAttachmentsOfAllNodes     = new QAction(tr("Protect all attachments.."), this);
    //m_ptrProtectAttachmentsOfAllNodes     ->setIconVisibleInMenu(true);
    //m_ptrProtectAttachmentsOfAllNodes     ->setIcon(QIcon(":/images/images/import.ico"));
    //m_ptrProtectAttachmentsOfAllNodes     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_I));
    m_ptrProtectAttachmentsOfAllNodes     ->setStatusTip(tr("Protect all attachments of the node (with childs optionally"));
    m_ptrProtectAttachmentsOfAllNodes     ->setEnabled(false);
    //
    m_ptrUn_ProtectAttachmentsOfAllNodes     = new QAction(tr("Unprotect all attachments.."), this);
    //m_ptrUn_ProtectAttachmentsOfAllNodes     ->setIconVisibleInMenu(true);
    //m_ptrUn_ProtectAttachmentsOfAllNodes     ->setIcon(QIcon(":/images/images/import.ico"));
    //m_ptrUn_ProtectAttachmentsOfAllNodes     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_I));
    m_ptrUn_ProtectAttachmentsOfAllNodes     ->setStatusTip(tr("Protect all attachments of the node (with childs optionally"));
    m_ptrUn_ProtectAttachmentsOfAllNodes     ->setEnabled(false);
    //
    m_ptrSaveNodeDescriptor     = new QAction(tr("Save node descriptor"), this);
    m_ptrSaveNodeDescriptor     ->setIconVisibleInMenu(true);
    m_ptrSaveNodeDescriptor     ->setIcon(QIcon(":/images/images/save.ico"));
    m_ptrSaveNodeDescriptor     ->setShortcut(QKeySequence (Qt::ALT +Qt::Key_S));
    m_ptrSaveNodeDescriptor     ->setStatusTip(tr("Save changed node descriptor"));
    m_ptrSaveNodeDescriptor     ->setEnabled(false);
};

void MnuMainWindow::assemblyTreeMenu(QMenu* ptr_node_menu)
{
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
    ptr_node_menu->addAction(m_ptrExitApp);
}

void MnuMainWindow::assemblyNodeMenu(QMenu* ptr_node_menu)
{
    ptr_node_menu->addAction(m_ptrInsertNewNode);
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

    //
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrSaveNodeDescriptor);
}

void MnuMainWindow::assemblyAttachMenu( QMenu* ptr_node_menu )
{
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
}

void MnuMainWindow::assemblyNodeCryptoSubMenu(QMenu* ptr_menu)
{
    ptr_menu->addAction(m_ptrEncryptAttachmentsOfAllNodes);
    ptr_menu->addSeparator();
    ptr_menu->addAction(m_ptrDecryptAttachmentsOfAllNodes);
}

void MnuMainWindow::assemblyNodeProtectSubMenu (QMenu* ptr_menu)
{
    ptr_menu->addAction(m_ptrProtectAttachmentsOfAllNodes);
    ptr_menu->addSeparator();
    ptr_menu->addAction(m_ptrUn_ProtectAttachmentsOfAllNodes);
}

void MnuMainWindow::assemblyAttachProtectionSubMenu( QMenu* ptr_node_menu )
{
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
    ptr_node_menu->addAction(m_ptrEncryptAllAttachmentsOfNode);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrDecryptAllAttachmentsOfNode);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrEncryptSelectedAttachments);
    ptr_node_menu->addSeparator();
    ptr_node_menu->addAction(m_ptrDecryptSelectedAttachments);
}

void MnuMainWindow::assemblySecurityMenu( QMenu* ptr_node_menu )
{
    ptr_node_menu->addAction(m_ptrCreateChangePassword);
}

void MnuMainWindow::assemblyAllMenus()
{
    m_ptrTreeControlMenu = m_ptrParent->menuBar()->addMenu(tr("&Tree"));
    //
    assemblyTreeMenu(m_ptrTreeControlMenu);
    //--------
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
    assemblyNodeProtectSubMenu(m_ptrNodeProtectSubMenu);
    //--------
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
    //--------
    m_ptrSecurity = m_ptrParent->menuBar()->addMenu(tr("Security"));
    //
    assemblySecurityMenu(m_ptrSecurity);
};
