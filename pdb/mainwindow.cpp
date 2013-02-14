#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "treeleaf.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include "connectionmanager.h"
#include "mnumainwindow.h"
#include "GDefinitions.h"
#include "databasecleaner.h"
#include "rootoftree.h"
#include "tmpfilecleaner.h"
#include "treesearchdlg.h"
#include "servicescfg.h"
//
#include "VariantPtr.h"
//
#include "logger.h"
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql>
#include <QDir>
#include <QtDebug>
#include <QMutexLocker>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //
    setWindowTitle(QObject::trUtf8("Personal database"));
    //
    //make SSH tunnel, if need
    //
    m_Tunnel.makeTunnel();
    //
    m_bEditorTextChanged    = false;
    m_uiConnectionsInUse    = 0;
    m_bIsDBUsageIconVisible = false;
    m_ptrDbInUseIcon        = new QPixmap(":/images/images/db_connection.png");
    m_ptrDbInUseLabel       = new QLabel();
    m_ptrDbInUseLabel->setPixmap(*m_ptrDbInUseIcon);
    m_ptrDbInUseLabel->setToolTip(tr("Database in use now"));
    //
    m_ptrSearchDlg = new TreeSearchDlg();
    //
    // create menu object
    //
    m_pMainMenu = new MnuMainWindow( this );
    //
    //
    statusBar()->addPermanentWidget(m_ptrDbInUseLabel);
    //
    restoreWindowParams();
    //
    initComboBoxTreeVSplitter();
    initEditorServiceBarHSplitter();
    initMainVSplitter();
    //
    m_dlgWaiting.setModal(false);
    //
    this->setCentralWidget(m_pMainVSplitter);
    //pass pointer on combobox into the tree
    ui->m_TreeOfNodes->setTreeComboBox(ui->m_DBNameList);
    //pass pointer on combobox into the tree global functions object
    m_G_TreeActions.setPtrToComboBox(ui->m_DBNameList);
    m_G_TreeActions.setPtrToTree(ui->m_TreeOfNodes);
    m_G_TreeActions.setPtrToStatusBar(statusBar());
    //
    m_ptrSearchDlg->setPtrToComboBox(ui->m_DBNameList);
    m_ptrSearchDlg->setPtrToTree(ui->m_TreeOfNodes);
    //
    //pass pointer on the status bar into the tree
    ui->m_TreeOfNodes->setStatusBar( statusBar() );
    ui->m_Service_Tab->setStatusBar( statusBar() );
    //
    if (ConnectionManager::getInstance().getManagerStatus() == ConnectionManager::STATE_OK)
    {
        QString str_header = tr("Pdb: host: [");
        str_header += ConnectionManager::getInstance().getHostName();
        str_header += "] database: [";
        str_header += ConnectionManager::getInstance().getDBName();
        str_header += "]";
        //
        setWindowTitle(str_header);
    }else
    {
        QMessageBox box;
        box.setText(tr("Can not establish connection to database. Check settings."));
        box.exec();
        return;
    };
    //
    //for enabling menu after initial restore tree
    QObject::connect( ui->m_TreeOfNodes,                SIGNAL( endOfInit           ( int )), m_pMainMenu,       SLOT( onTreeComboBoxChanged(int) ) );
    //
    //initialize node reading
    //
    ui->m_TreeOfNodes->init();
    //move later in connectSignalsAndSlots()
    QObject::connect( &(ServicesCfg::getInstance()),                     SIGNAL(PasswordExists(bool)),   this,         SLOT( onCheckPassword(bool) ) );
    //if parameter is false - creation password, if true - change ppassword
    QObject::connect( &(ServicesCfg::getInstance()),                     SIGNAL(PasswordExists(bool)),   m_pMainMenu,  SLOT( onCheckPassword(bool) ) );
    //this one we use when password is created 1-st time, for notfication of the main menu class (change menu text)
    QObject::connect( &(ServicesCfg::getInstance()),                     SIGNAL(PasswordCreated(bool)),  m_pMainMenu,  SLOT( onCheckPassword(bool) ) );
    //
    //------------------------------------------------ connect signals and slots ------------------------------------------------
    connectSignalsAndSlots ();
    //
    ServicesCfg::getInstance().getDataAndCheckInstance();
    //
    statusBar()->showMessage(tr("Ready"));
    //
    return;
    //**********************************************
    //             this is debug code
    //**********************************************
    QStringList str_paths =  QApplication::libraryPaths();
    //
    QString str_out;
    for (int i = 0; i < str_paths.size(); ++i)
    {
        str_out += str_paths[i];
        str_out += "; ";
    };
    statusBar()->showMessage(str_out);
    //
    return;
}

MainWindow::~MainWindow()
{
    delete ui;
    if ( m_pMainMenu        ) delete m_pMainMenu;
    if ( m_ptrDbInUseIcon   ) delete m_ptrDbInUseIcon;
    if ( m_ptrDbInUseLabel  ) delete m_ptrDbInUseLabel;
    //
    if ( m_ptrSearchDlg     ) delete m_ptrSearchDlg;
}

void MainWindow::connectSignalsAndSlots ()
{
    //node menu:
    QObject::connect( m_pMainMenu->m_ptrDelNode,        SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onDelCurrentItem    () ) );
    QObject::connect( m_pMainMenu->m_ptrRestoreNode,    SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onRestoreCurrentItem() ) );
    QObject::connect( m_pMainMenu->m_ptrRestoreSubtree, SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onRestoreSubtree    () ) );
    QObject::connect( m_pMainMenu->m_ptrInsertNewNode,  SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onInsertNewItem     () ) );
    //
    QObject::connect( m_pMainMenu->m_ptrCutNode,        SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onCutNode()            ) );
    QObject::connect( m_pMainMenu->m_ptrPasteNode,      SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onPasteNode()          ) );
    //
    QObject::connect( m_pMainMenu->m_ptrImportNode,     SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onImportNode()         ) );
    QObject::connect( m_pMainMenu->m_ptrExportNode,     SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onExportNode()         ) );
    //global attachments operations
    QObject::connect( m_pMainMenu->m_ptrProtectAttachmentsOfAllNodes,   SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT(OnProtectAttachmentsGlobal()   ));
    QObject::connect( m_pMainMenu->m_ptrUn_ProtectAttachmentsOfAllNodes, SIGNAL(triggered()),ui->m_TreeOfNodes, SLOT(OnUnProtectAttachmentsGlobal() ));
    //
    QObject::connect( m_pMainMenu->m_ptrEncryptAttachmentsOfAllNodes,   SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT(OnEncryptAttachmentsGlobal()   ));
    QObject::connect( m_pMainMenu->m_ptrDecryptAttachmentsOfAllNodes,   SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT(OnDecryptAttachmentsGlobal()   ));
    //
    QObject::connect( m_pMainMenu->m_ptrSaveNodeDescriptor,             SIGNAL(triggered()), this,              SLOT(saveCurrentModeDescriptor()    ));
    //
    //attachments menu status change
    //
    QObject::connect( ui->m_Service_Tab,                SIGNAL(selectionChanged(Attachment::AttachmentsList)), m_pMainMenu, SLOT (onSelectedAttachmentChanged(Attachment::AttachmentsList) ));
    //
    // when attachment cutted
    //
    QObject::connect( ui->m_Service_Tab,                SIGNAL(attachmentForCutSelected(Attachment::AttachmentsList)), ui->m_TreeOfNodes, SLOT (onAcceptCutAttachment(Attachment::AttachmentsList) ));
    //TreeLeaf::AttachmentsList
    //
    //attachments menu:
    //
    QObject::connect(m_pMainMenu->m_ptrImportAttach,    SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onImportAttach () ) );
    QObject::connect(m_pMainMenu->m_ptrDeleteAttach,    SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onDeleteAttachment()  ));
    QObject::connect(m_pMainMenu->m_ptrRestoreAttach,   SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onRestoreAttachment() ));
    QObject::connect(m_pMainMenu->m_ptrExportAttach,    SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onExportAttachment()  ));
    QObject::connect(m_pMainMenu->m_ptrReplaceAttach,   SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onReplaceAttachment() ));
    //
    QObject::connect(m_pMainMenu->m_ptrCutAttach,       SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onCutAttachment()     ));
    QObject::connect(m_pMainMenu->m_ptrPasteAttach,     SIGNAL(triggered()), ui->m_TreeOfNodes, SLOT( onPasteAttachments()  ));
    //
    QObject::connect(m_pMainMenu->m_ptrViewAttach,      SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onViewAttachment  ()  ));
    //protection operations with attachment(s)
    QObject::connect(m_pMainMenu->m_ptrProtectAllAttachmentsOfNode,     SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onProtectAllAttachments() ));      //TODO
    QObject::connect(m_pMainMenu->m_ptrUn_ProtectAllAttachmentsOfNode,  SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onDropProtectAllAttachments() ));  //TODO
    QObject::connect(m_pMainMenu->m_ptrProtectSelectedAttachments,      SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onProtectSelected()        ));
    QObject::connect(m_pMainMenu->m_ptrUn_ProtectSelectedAttachments,   SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onDropProtectSelected()    ));
    //encryption/decryption
    QObject::connect(m_pMainMenu->m_ptrEncryptAllAttachmentsOfNode,     SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onEncryptAllAttachments()  ));     //TODO
    QObject::connect(m_pMainMenu->m_ptrDecryptAllAttachmentsOfNode,     SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onDecryptAllAttachments()  ));     //TODO
    QObject::connect(m_pMainMenu->m_ptrEncryptSelectedAttachments,      SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onEncryptSelected()        ));
    QObject::connect(m_pMainMenu->m_ptrDecryptSelectedAttachments,      SIGNAL(triggered()), ui->m_Service_Tab, SLOT( onDecryptSelected()        ));
    //
    //QObject::connect(m_pMainMenu->m_ptrRefreshAttach,   SIGNAL(triggered()), ui->m_Service_Tab, SLOT( refreshCurrentAttachmentsList() ));
    //
    //node menu status change
    //
    QObject::connect( ui->m_TreeOfNodes,                SIGNAL (treeSelectionChanged(TreeLeaf*, bool, bool) ), m_pMainMenu, SLOT ( onSelectedNodeChanged(TreeLeaf*, bool, bool) ) );
    //
    //show description
    //
    QObject::connect( ui->m_TreeOfNodes,                SIGNAL (treeSelectionChanged(TreeLeaf*, TreeLeaf*) ), this, SLOT(onSelectedNodeChanged(TreeLeaf*,TreeLeaf*) ) );
    //
    // global tree operations. Handle tree menu
    //
    QObject::connect(m_pMainMenu->m_ptrCreateNewTree,   SIGNAL(triggered()),  &m_G_TreeActions, SLOT ( onCreateNewTree      () ));
    QObject::connect(m_pMainMenu->m_ptrDelTree,         SIGNAL(triggered()),  &m_G_TreeActions, SLOT ( onDeleteTree         () ));
    QObject::connect(m_pMainMenu->m_ptrRenameTree,      SIGNAL(triggered()),  &m_G_TreeActions, SLOT ( onRenameCurrentTree  () ));
    QObject::connect(m_pMainMenu->m_ptrInfoTree,        SIGNAL(triggered()),  &m_G_TreeActions, SLOT ( onInfoAboutTree      () ));
    //search
    QObject::connect(m_pMainMenu->m_ptrSearchInTree,    SIGNAL(triggered()), this,              SLOT (  onSearch() ));
    QObject::connect(m_pMainMenu->m_ptrExitApp,         SIGNAL(triggered()), this,              SLOT (  close   () ));
    //enable-disable "Save Description" menu item
    QObject::connect(this,                              SIGNAL(changeDescription()), m_pMainMenu, SLOT(onChangeDescription() ));
    QObject::connect(this,                              SIGNAL(saveDescription()),  m_pMainMenu, SLOT (onSaveDescription()   ));
    //
    //operate with attachments initiated from from tree side
    //
    QObject::connect( ui->m_TreeOfNodes,                SIGNAL (treeSelectionChanged(TreeLeaf*, TreeLeaf*) ), ui->m_Service_Tab, SLOT(onSelectedNodeChanged(TreeLeaf*,TreeLeaf*)) );
    QObject::connect( ui->m_TreeOfNodes,                SIGNAL (updateAttachmentList() ), ui->m_Service_Tab, SLOT(onAttachmentUpdated()) );
    //
    // whow pop-up menu on the right button mouse click
    //
    QObject::connect(ui->m_TreeOfNodes,                 SIGNAL( showPopupMenu()    ), m_pMainMenu, SLOT (showRightClickPopupNodeMenu()   ));
    QObject::connect(ui->m_Service_Tab,                 SIGNAL( showPopupMenu()    ), m_pMainMenu, SLOT (showRightClickPopupAttachMenu() ));
    //
    // when combobox selection changed
    //
    QObject::connect( ui->m_DBNameList,                 SIGNAL( currentIndexChanged ( int )), ui->m_TreeOfNodes, SLOT( onCurrentDBChanged(int) ) );
    QObject::connect( ui->m_DBNameList,                 SIGNAL( currentIndexChanged ( int )), m_pMainMenu,       SLOT( onTreeComboBoxChanged(int) ) );
    //
    //when text in the text editor changed
    //
    QObject::connect( ui->m_textEditor,                 SIGNAL(textChanged() ), this, SLOT (onEditorTextChanged() ));
    //
    QObject::connect( &(ServicesCfg::getInstance()),    SIGNAL (DbInUse() ), this, SLOT (onDbAlreadyInUse() ));
    //support "show/hide database access icon"
    QObject::connect( &(ConnectionManager::getInstance()), SIGNAL(takeConn()),   this,              SLOT(onTakeConn()       ));
    QObject::connect( &(ConnectionManager::getInstance()), SIGNAL(releaseConn()),this,              SLOT(onReleaseConn()    ));
    //if user does not type password buc click "cancel"
    QObject::connect( &m_PwdDlg,                        SIGNAL(Shutdown()),      this,              SLOT(close()            ));
    //security menu item
    QObject::connect(m_pMainMenu->m_ptrCreateChangePassword, SIGNAL(triggered()), this, SLOT(onCreateChangePassword()       ));
    //change password stuff
    QObject::connect(&m_PwdDlg,                              SIGNAL(DropAttachments()), ui->m_TreeOfNodes, SLOT(onDropAttachments() ));
    //
    QObject::connect(&m_PwdDlg,                              SIGNAL(ReencryptFinished()), ui->m_Service_Tab, SLOT(onAttachmentUpdated()) );

}

void  MainWindow::onCreateChangePassword ()
{
    //
    m_PwdDlg.setDbCombobox(ui->m_DBNameList); //for iterating list of nodes and attachments
    //
    //if (ServicesCfg::getInstance().getPasswordHash().length() == 0) //we create new password
    if (ServicesCfg::getInstance().getPasswordHash().length() == 0) //we create new password
    {
        m_PwdDlg.setDlgMode(PasswordDlg::CREATE_PASSWORD);
    }else
    {
        if (ServicesCfg::getInstance().getPassword().length() > 0)
            m_PwdDlg.setDlgMode(PasswordDlg::CHANGE_PASSWORD); // we change existings password
        else
            m_PwdDlg.setDlgMode(PasswordDlg::ENTER_PASSWORD);  // we enter password
    };
    //
    m_PwdDlg.exec();
    //
    return;
}

void MainWindow::onCheckPassword (bool b_password_exist)
{
    if (b_password_exist)
    {
        m_PwdDlg.setDlgMode(PasswordDlg::ENTER_PASSWORD);
        m_PwdDlg.exec();
    };
}

void MainWindow::onSelectedNodeChanged(TreeLeaf* ptr_to_current, TreeLeaf* ptr_to_previous)
{
    //save text of description of previous node if exists
    //add flag "text was changed" to allow save empty descriptors
    saveTextOfDescription   (ptr_to_previous);
    //show text of description of new actual node if exists.
    showTextOfDescription   (ptr_to_current);
}

void   MainWindow::onReleaseConn          ()
{
    QMutexLocker locker (&m_ConnCounterProtector);
    //
    m_uiConnectionsInUse--;
    if (
            (true == m_bIsDBUsageIconVisible) &&
            (0 == m_uiConnectionsInUse)         //no any connections are in use, but icon is visible
       )
    {
        showDBUsageIcon(false); //switch icon off
    };
    //
    m_ConnCounterProtector.unlock();
}

void   MainWindow::onTakeConn             ()
{
    QMutexLocker locker (&m_ConnCounterProtector);
    m_uiConnectionsInUse++;
    //
    if ( false == m_bIsDBUsageIconVisible)      //if icon is invisible now
    {
        showDBUsageIcon(true); //switch icon on
    };
}

void MainWindow::showDBUsageIcon (bool b_show)
{
    m_bIsDBUsageIconVisible = b_show;
    //show-hide icon here
    if (m_bIsDBUsageIconVisible)
    {
        m_ptrDbInUseLabel->show();
    }else
    {
       m_ptrDbInUseLabel->hide();
    };
};

void  MainWindow::onSearch()
{
    m_ptrSearchDlg->show();
}

void MainWindow::onDbAlreadyInUse() const
{
    QString str_msg = tr("Another instance of Pdb is working now with this database. Please, stop the work now.");
    QMessageBox box;
    box.setText(str_msg);
    box.exec();
}

void MainWindow::onEditorTextChanged()
{
    setTextChangeSignal(true);
}

void MainWindow::setTextChangeSignal (bool b_changed)
{
    m_bEditorTextChanged = b_changed;
    //
    //because when we change the database in the combobox, we loose pointer to previous node.
    //
    ui->m_DBNameList->setEnabled(!m_bEditorTextChanged);
    //

    //
    if (b_changed)
        emit changeDescription();
    else
        emit saveDescription();
}

void MainWindow::saveTextOfDescription          (TreeLeaf* ptr_to_node)
{
    if (NULL == ptr_to_node)
        return;
    //
    if (false == m_bEditorTextChanged)
        return;
    //
    ptr_to_node->setDescriptor( ui->m_textEditor->toHtml() );
    //
    setTextChangeSignal(false);
}

void MainWindow::showTextOfDescription          (TreeLeaf* ptr_to_node)
{
    if (NULL == ptr_to_node)
    {
        ui->m_textEditor->setText("");
        setTextChangeSignal(false);
        ui->m_textEditor->setEnabled(false);
        return;
    };
    //
    ui->m_textEditor->setHtml(ptr_to_node->getDescriptor().toHtml());
    ui->m_textEditor->setEnabled(true);
    setTextChangeSignal(false);
}

void MainWindow::initMainVSplitter()
{
    m_pMainVSplitter = new QSplitter(Qt::Horizontal);
    //
    m_pMainVSplitter->addWidget(m_pComboBoxTreeVSplitter);
    m_pMainVSplitter->addWidget(m_pEditorServiceBarHSplitter);
    //
    m_pMainVSplitter->setStretchFactor(m_pMainVSplitter->indexOf(m_pComboBoxTreeVSplitter),1);
    m_pMainVSplitter->setStretchFactor(m_pMainVSplitter->indexOf(m_pEditorServiceBarHSplitter),1);
}

void MainWindow::initEditorServiceBarHSplitter()
{
    m_pEditorServiceBarHSplitter = new QSplitter(Qt::Vertical);
    //
    m_pEditorServiceBarHSplitter->addWidget(ui->m_EditorLabel);
    m_pEditorServiceBarHSplitter->addWidget(ui->m_textEditor);
    m_pEditorServiceBarHSplitter->addWidget(ui->m_AttachmentLabel);
    m_pEditorServiceBarHSplitter->addWidget(ui->m_Service_Tab);
    //
    //
    m_pEditorServiceBarHSplitter->setStretchFactor(m_pEditorServiceBarHSplitter->indexOf(ui->m_EditorLabel),0);
    m_pEditorServiceBarHSplitter->setStretchFactor(m_pEditorServiceBarHSplitter->indexOf(ui->m_textEditor), 1);
    m_pEditorServiceBarHSplitter->setStretchFactor(m_pEditorServiceBarHSplitter->indexOf(ui->m_AttachmentLabel),0);
    m_pEditorServiceBarHSplitter->setStretchFactor(m_pEditorServiceBarHSplitter->indexOf(ui->m_Service_Tab),1);
}

void MainWindow::initComboBoxTreeVSplitter()
{
    m_pComboBoxTreeVSplitter = new QSplitter(Qt::Vertical);
    //fix the size of elements (prevent stretching)
    //ui->m_DBNameList->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    //
    m_pComboBoxTreeVSplitter->addWidget(ui->m_DBNameList);
    m_pComboBoxTreeVSplitter->addWidget(ui->m_TreeOfNodes);
    //
    //
    m_pComboBoxTreeVSplitter->setStretchFactor( m_pComboBoxTreeVSplitter->indexOf(ui->m_DBNameList), 0 );
    //m_pComboBoxTreeVSplitter->setStretchFactor( 0, 0 );
    m_pComboBoxTreeVSplitter->setStretchFactor( m_pComboBoxTreeVSplitter->indexOf(ui->m_TreeOfNodes), 1 );
}

void MainWindow::saveCurrentModeDescriptor ()
{
    //save current node descriptor, if need
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) ui->m_TreeOfNodes->currentItem();
    //
    if (ptr_actual_item)
        saveTextOfDescription(ptr_actual_item);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    saveCurrentModeDescriptor ();
    //
    //setCursor(Qt::BusyCursor);
    statusBar()->showMessage(tr("Save node state..."));
    ui->m_TreeOfNodes->saveExpandStateOfAllTrees();
    statusBar()->showMessage(tr("Save last ID..."));
    ui->m_TreeOfNodes->setLastUsedRootID_DB();
    statusBar()->showMessage(tr("Clean temporary files..."));
    TmpFileCleaner::getInstance().clearAll();
    //
    m_dlgWaiting.show();
    //
    DatabaseCleaner* ptr_cleaner = new DatabaseCleaner();
    //
    statusBar()->showMessage(tr("DB cleaner...."));
    QThreadPool::globalInstance()->start(ptr_cleaner);
    //mark actual db is not in use now
    ServicesCfg::getInstance().dropInstance();
    //
    statusBar()->showMessage(tr("Stop engine...."));
    QThreadPool::globalInstance()->waitForDone();
    //
    m_dlgWaiting.hide();
    //
    statusBar()->showMessage(tr("Stop logging...."));
    Logger::getInstance().stopLog();
    //
    statusBar()->showMessage(tr("destroy tunnel..."));
    m_Tunnel.destroyTunnel();
    //
    QByteArray ba_geometry = saveGeometry();
    QByteArray ba_state = saveState();
    //
    QSettings settings (g_strCOMPANY, g_str_CNF_APP_NAME);
    //
    settings.setValue( g_str_WINDOW_GEOMETRY,    ba_geometry );
    settings.setValue( g_str_WINDOW_STATE,       ba_state    );
    //
    //setCursor(Qt::ArrowCursor);
    statusBar()->showMessage(tr("End."));
    e->accept();
    //
    exit(0);
}

void MainWindow::restoreWindowParams()
{
    QSettings settings (g_strCOMPANY, g_str_CNF_APP_NAME);
    //
    restoreGeometry(settings.value(g_str_WINDOW_GEOMETRY).toByteArray());
    restoreState(settings.value(g_str_WINDOW_STATE).toByteArray());
}

