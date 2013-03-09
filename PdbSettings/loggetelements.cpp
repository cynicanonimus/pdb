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
#include "loggetelements.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QSettings>
#include <QFileDialog>
//
LoggetElements::LoggetElements(QGroupBox *ptr_parent_frame, QGridLayout *ptr_layout) :
    AbstractGraficeElements(ptr_parent_frame, ptr_layout, 0)
{
    m_ptrEnableLog      = NULL;
    //
    m_pLabelLogPath     = NULL;
    m_pLineLogPath      = NULL;
    m_pLogPathChoose    = NULL;
    //
    m_ptrLogErrors      = NULL;
    m_ptrLogSSHTunnel   = NULL;
    //
    m_bInitialLoadFlag = true;
    //
    m_strFrameName = tr("Log settings");
    //
    deleteElementsOnExit(true);
    //
    /*
    for (unsigned int i = 0; i < en_BITARRAY_SIZE; i++)
    {
        m_bitFlags.setBit(i,false);
    };
    */
    //
    readData();
    //
    createLayout();
    //
    createLinks();
    //
    updateData(false);
}

LoggetElements::~LoggetElements()
{
    if (m_ptrNodeGridbox) delete  m_ptrNodeGridbox;
    //
    if (m_ptrLogCreateNode)     delete m_ptrLogCreateNode;
    if (m_ptrLogUpdateNode)     delete m_ptrLogUpdateNode;
    if (m_ptrLogDeleteNode)     delete m_ptrLogDeleteNode;
    if (m_ptrLogMoveNode )      delete m_ptrLogMoveNode;
    if (m_ptrLogNodeExport )    delete m_ptrLogNodeExport;
    if (m_ptrLogNodeDescriptor) delete m_ptrLogNodeDescriptor;
    //----------------------------------------------------------------
    if (m_ptrAttGridbox)        delete  m_ptrAttGridbox;
    if (m_ptrLogAttachDel)      delete  m_ptrLogAttachDel;
    if (m_ptrLogAttachCreate)   delete  m_ptrLogAttachCreate;
    if (m_ptrLogAttachChange)   delete  m_ptrLogAttachChange;
    if (m_ptrLogAttachRename)   delete  m_ptrLogAttachRename;
    if (m_ptrLogAttachMove)     delete  m_ptrLogAttachMove;
    if (m_ptrLogAttachView)     delete  m_ptrLogAttachView;
    if (m_ptrLogAttachExport)   delete  m_ptrLogAttachExport;
    if (m_ptrLogAtachCrypt)    delete  m_ptrLogAtachCrypt;
    //----------------------------------------------------------------
    if (m_ptrTreeGridbox)       delete  m_ptrTreeGridbox;
    if (m_ptrLogTreeCreate)     delete  m_ptrLogTreeCreate;
    if (m_ptrLogTreeDel)        delete  m_ptrLogTreeDel;
    if (m_ptrLogTreeRename)     delete  m_ptrLogTreeRename;
};

void LoggetElements::createLinks ()
{
    QObject::connect(m_pLogPathChoose,  SIGNAL(clicked()),          this, SLOT ( onChooseLogPath() ) );
    //
    QObject::connect(m_ptrEnableLog,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickEnableLog(int) ) );     //en_ENABLE_LOG
    QObject::connect(m_ptrLogErrors,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );    //en_LOG_ERRORS
    QObject::connect(m_ptrLogSSHTunnel, SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );    //
    //
    QObject::connect(m_ptrLogCreateNode,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogUpdateNode,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogDeleteNode,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogMoveNode,      SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogNodeExport,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogNodeDescriptor,SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    //
    QObject::connect(m_ptrLogAttachDel,     SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogAttachCreate,  SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogAttachChange,  SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogAttachRename,  SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogAttachMove,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogAttachView,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogAttachExport,  SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogAtachCrypt,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    //
    QObject::connect(m_ptrLogTreeCreate,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogTreeDel,       SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
    QObject::connect(m_ptrLogTreeRename,    SIGNAL(stateChanged(int)),  this, SLOT ( onClickAllCheckboxes() ) );
};

void LoggetElements::createLayout()
{
    int i_row = 0;
    //
    m_ptrEnableLog = new QCheckBox(tr("Enable logging"));
    addAndRegisterElement(m_ptrEnableLog, i_row,0,1,4);
    //
    i_row++;
    m_pLabelLogPath = new QLabel ("Path for log files:");
    addAndRegisterElement(m_pLabelLogPath, i_row,0,1,1);
    //
    m_pLineLogPath = new QLineEdit( m_pParentFrame );
    addAndRegisterElement(m_pLineLogPath, i_row,1,1,2);
    //
    m_pLogPathChoose = new QPushButton("Change...",  m_pParentFrame);
    addAndRegisterElement(m_pLogPathChoose, i_row,3,1,1);
    //
    i_row++;
    m_ptrLogErrors = new QCheckBox(tr("Log any error"));
    addAndRegisterElement(m_ptrLogErrors, i_row,0,1,4);
    m_ptrLogSSHTunnel= new QCheckBox(tr("Log SSH tunneling"));
    addAndRegisterElement(m_ptrLogSSHTunnel, i_row,2,1,4);
    //================================================
    i_row++;
    //
    m_ptrNodeGroupBox   = new QGroupBox("Log node activities");
    m_ptrNodeGridbox = new QGridLayout(m_ptrNodeGroupBox);
    m_ptrNodeGroupBox->setLayout(m_ptrNodeGridbox);
    addAndRegisterElement(m_ptrNodeGroupBox, i_row,0,1,4);
    //
    //------------------------------------------------
    //
    int i_node_row               = 0;
    m_ptrLogCreateNode = new QCheckBox(tr("Create node"));
    m_ptrNodeGridbox->addWidget(m_ptrLogCreateNode, i_node_row,0,1,2);
    //
    m_ptrLogUpdateNode = new QCheckBox(tr("Update node name"));
    m_ptrNodeGridbox->addWidget(m_ptrLogUpdateNode, i_node_row,2,1,2);
    //
    i_node_row++;
    m_ptrLogDeleteNode = new QCheckBox(tr("Delete node"));
    m_ptrNodeGridbox->addWidget(m_ptrLogDeleteNode, i_node_row,0,1,2);
    //
    m_ptrLogMoveNode = new QCheckBox(tr("Move node"));
    m_ptrNodeGridbox->addWidget(m_ptrLogMoveNode, i_node_row,2,1,2);
    //
    i_node_row++;
    m_ptrLogNodeExport = new QCheckBox(tr("Export node"));
    m_ptrNodeGridbox->addWidget(m_ptrLogNodeExport , i_node_row,0,1,2);
    //
    m_ptrLogNodeDescriptor = new QCheckBox(tr("Change node description"));
    m_ptrNodeGridbox->addWidget(m_ptrLogNodeDescriptor, i_node_row,2,1,2);
    //================================================
    i_row++;
    //
    m_ptrAttGroupBox   = new QGroupBox("Log attachment activities");
    m_ptrAttGridbox = new QGridLayout(m_ptrAttGroupBox);
    m_ptrAttGroupBox->setLayout(m_ptrAttGridbox);
    addAndRegisterElement(m_ptrAttGroupBox, i_row,0,1,4);
    //
    //------------------------------------------------
    //
    int i_att_row               = 0;
    m_ptrLogAttachDel = new QCheckBox(tr("Delete attachment"));
    m_ptrAttGridbox->addWidget(m_ptrLogAttachDel, i_att_row,0,1,2);
    //
    m_ptrLogAttachCreate = new QCheckBox(tr("Create attachment"));
    m_ptrAttGridbox->addWidget(m_ptrLogAttachCreate, i_att_row,2,1,2);
    //--
    i_att_row++;
    m_ptrLogAttachChange = new QCheckBox(tr("Change attachment"));
    m_ptrAttGridbox->addWidget(m_ptrLogAttachChange, i_att_row,0,1,2);
    //
    m_ptrLogAttachRename = new QCheckBox(tr("Rename attachment "));
    m_ptrAttGridbox->addWidget(m_ptrLogAttachRename, i_att_row,2,1,2);
    //--
    i_att_row++;
    m_ptrLogAttachMove = new QCheckBox(tr("Move attachment"));
    m_ptrAttGridbox->addWidget(m_ptrLogAttachMove, i_att_row,0,1,2);
    //
    m_ptrLogAttachView = new QCheckBox(tr("View attachment"));
    m_ptrAttGridbox->addWidget(m_ptrLogAttachView, i_att_row,2,1,2);
    //--
    i_att_row++;
    m_ptrLogAttachExport = new QCheckBox(tr("Export attachment"));
    m_ptrAttGridbox->addWidget(m_ptrLogAttachExport, i_att_row,0,1,2);
    //
    m_ptrLogAtachCrypt = new QCheckBox(tr("Encrypt/decrypt attachment"));
    m_ptrAttGridbox->addWidget(m_ptrLogAtachCrypt, i_att_row,2,1,2);
    //================================================
    i_row++;
    //
    m_ptrTreeGroupBox   = new QGroupBox("Log tree activities");
    m_ptrTreeGridbox = new QGridLayout(m_ptrTreeGroupBox);
    m_ptrTreeGroupBox->setLayout(m_ptrTreeGridbox);
    addAndRegisterElement(m_ptrTreeGroupBox, i_row,0,1,4);
    //
    //------------------------------------------------
    //
    int i_tree_row               = 0;
    //
    m_ptrLogTreeCreate = new QCheckBox(tr("Create tree"));
    m_ptrTreeGridbox->addWidget(m_ptrLogTreeCreate, i_tree_row,0,1,2);
    //
    m_ptrLogTreeDel = new QCheckBox(tr("Delete tree"));
    m_ptrTreeGridbox->addWidget(m_ptrLogTreeDel, i_tree_row,2,1,2);
    //--
    i_tree_row++;
    m_ptrLogTreeRename = new QCheckBox(tr("Rename tree"));
    m_ptrTreeGridbox->addWidget(m_ptrLogTreeRename, i_att_row,0,1,2);
};

void    LoggetElements::writeData()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    settings.setValue( g_str_LOG_PATH,      m_strLogPath );
    //
    QVariant flags(m_bitFlags);
    settings.setValue(g_str_LOG_FLAGS, flags);
};

void    LoggetElements::readData()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    m_strLogPath      = settings.value(g_str_LOG_PATH).toString();
    //
    QVariant flags =     settings.value(g_str_LOG_FLAGS);
    //
    m_bitFlags = flags .toBitArray();
    //
    if ( (m_bitFlags.size() == 0) || (m_bitFlags.size() < en_BITARRAY_SIZE) )
    {
        m_bitFlags.resize(en_BITARRAY_SIZE);
    };
};

void LoggetElements::updateData  (bool b_from_dialog)
{
    if(b_from_dialog)
    {
        m_strLogPath    = m_pLineLogPath->text();

    }else
    {
        m_pLineLogPath->setText(m_strLogPath);
    };
    //
    updateFlags(b_from_dialog);
};

void    LoggetElements::updateFlags (bool b_from_dialog)
{
    if (b_from_dialog)
    {
       m_bitFlags.setBit( en_ENABLE_LOG, m_ptrEnableLog->checkState() );
       //
       m_bitFlags.setBit( en_LOG_ERRORS, m_ptrLogErrors->checkState() );
       //
       m_bitFlags.setBit( en_LOG_CREATE_NODE,       m_ptrLogCreateNode->checkState() );
       m_bitFlags.setBit( en_LOG_UPDATE_NODE,       m_ptrLogUpdateNode->checkState() );
       m_bitFlags.setBit( en_LOG_DELETE_NODE,       m_ptrLogDeleteNode->checkState() );
       m_bitFlags.setBit( en_LOG_MOVE_NODE,         m_ptrLogMoveNode->checkState() );
       m_bitFlags.setBit( en_LOG_EXPORT_NODE,       m_ptrLogNodeExport->checkState() );
       m_bitFlags.setBit( en_LOG_DESCRIPTOR_NODE,   m_ptrLogNodeDescriptor->checkState() );
       //--------------------------------------------
       m_bitFlags.setBit( en_LOG_CREATE_ATTACH,     m_ptrLogAttachCreate->checkState() );
       m_bitFlags.setBit( en_LOG_DELETE_ATTACH,     m_ptrLogAttachDel->checkState() );
       m_bitFlags.setBit( en_LOG_UPDATE_ATTACH,     m_ptrLogAttachChange->checkState() );
       m_bitFlags.setBit( en_LOG_RENAME_ATTACH,     m_ptrLogAttachRename->checkState() );
       m_bitFlags.setBit( en_LOG_MOVE_ATTACH,       m_ptrLogAttachMove->checkState() );
       m_bitFlags.setBit( en_LOG_VIEW_ATTACH,       m_ptrLogAttachView->checkState() );
       m_bitFlags.setBit( en_LOG_EXPORT_ATTACH,     m_ptrLogAttachExport->checkState() );
       m_bitFlags.setBit( en_LOG_CRYPT_ATTACH,      m_ptrLogAtachCrypt->checkState() );
       //--------------------------------------------
       m_bitFlags.setBit( en_LOG_CREATE_TREE,       m_ptrLogTreeCreate->checkState() );
       m_bitFlags.setBit( en_LOG_DELETE_TREE,       m_ptrLogTreeDel->checkState() );
       m_bitFlags.setBit( en_LOG_RENAME_TREE,       m_ptrLogTreeRename->checkState() );
       //--------------------------------------------
       m_bitFlags.setBit( en_LOG_TUNNELING,         m_ptrLogSSHTunnel->checkState() );

    }else
    {
        if (m_bitFlags[en_ENABLE_LOG])
        {
            m_ptrEnableLog->setCheckState(Qt::Checked);
            enableInterface(true);
        }
        else
        {
            m_ptrEnableLog->setCheckState(Qt::Unchecked);
            enableInterface(false);
        };
        //
        if (m_bitFlags[en_LOG_ERRORS])
            m_ptrLogErrors->setCheckState(Qt::Checked);
        else
            m_ptrLogErrors->setCheckState(Qt::Unchecked);
        //--------------------------------------------
        if ( m_bitFlags[ en_LOG_CREATE_NODE] )
            m_ptrLogCreateNode->setCheckState(Qt::Checked);
        else
            m_ptrLogCreateNode->setCheckState(Qt::Unchecked);
        //
        if ( m_bitFlags[ en_LOG_UPDATE_NODE] )
            m_ptrLogUpdateNode->setCheckState(Qt::Checked);
        else
            m_ptrLogUpdateNode->setCheckState(Qt::Unchecked);
        //
        if ( m_bitFlags[ en_LOG_DELETE_NODE] )
            m_ptrLogDeleteNode->setCheckState(Qt::Checked);
        else
            m_ptrLogDeleteNode->setCheckState(Qt::Unchecked);
        //
        if ( m_bitFlags[ en_LOG_MOVE_NODE] )
            m_ptrLogMoveNode->setCheckState(Qt::Checked);
        else
            m_ptrLogMoveNode->setCheckState(Qt::Unchecked);
        //
        if ( m_bitFlags[ en_LOG_EXPORT_NODE] )
            m_ptrLogNodeExport->setCheckState(Qt::Checked);
        else
            m_ptrLogNodeExport->setCheckState(Qt::Unchecked);
        //
        if ( m_bitFlags[ en_LOG_DESCRIPTOR_NODE] )
            m_ptrLogNodeDescriptor->setCheckState(Qt::Checked);
        else
            m_ptrLogNodeDescriptor->setCheckState(Qt::Unchecked);
        //--------------------------------------------
        if ( m_bitFlags[ en_LOG_CREATE_ATTACH] )
            m_ptrLogAttachCreate->setCheckState(Qt::Checked);
        else
            m_ptrLogAttachCreate->setCheckState(Qt::Unchecked);
        //
        if ( m_bitFlags[ en_LOG_DELETE_ATTACH] )
            m_ptrLogAttachDel->setCheckState(Qt::Checked);
        else
            m_ptrLogAttachDel->setCheckState(Qt::Unchecked);
        //
        if ( m_bitFlags[ en_LOG_UPDATE_ATTACH] )
            m_ptrLogAttachChange->setCheckState(Qt::Checked);
        else
            m_ptrLogAttachChange->setCheckState(Qt::Unchecked);
        if ( m_bitFlags[ en_LOG_RENAME_ATTACH] )
            m_ptrLogAttachRename->setCheckState(Qt::Checked);
        else
            m_ptrLogAttachRename->setCheckState(Qt::Unchecked);
        if ( m_bitFlags[ en_LOG_MOVE_ATTACH] )
            m_ptrLogAttachMove->setCheckState(Qt::Checked);
        else
            m_ptrLogAttachMove->setCheckState(Qt::Unchecked);
        if ( m_bitFlags[ en_LOG_VIEW_ATTACH] )
            m_ptrLogAttachView->setCheckState(Qt::Checked);
        else
            m_ptrLogAttachView->setCheckState(Qt::Unchecked);
        if ( m_bitFlags[ en_LOG_EXPORT_ATTACH] )
            m_ptrLogAttachExport->setCheckState(Qt::Checked);
        else
            m_ptrLogAttachExport->setCheckState(Qt::Unchecked);
        if ( m_bitFlags[ en_LOG_CRYPT_ATTACH] )
            m_ptrLogAtachCrypt->setCheckState(Qt::Checked);
        else
            m_ptrLogAtachCrypt->setCheckState(Qt::Unchecked);
        //--------------------------------------------
        if ( m_bitFlags[ en_LOG_CREATE_TREE] )
            m_ptrLogTreeCreate->setCheckState(Qt::Checked);
        else
            m_ptrLogTreeCreate->setCheckState(Qt::Unchecked);
        if ( m_bitFlags[ en_LOG_DELETE_TREE] )
            m_ptrLogTreeDel->setCheckState(Qt::Checked);
        else
            m_ptrLogTreeDel->setCheckState(Qt::Unchecked);
        if ( m_bitFlags[ en_LOG_RENAME_TREE] )
            m_ptrLogTreeRename->setCheckState(Qt::Checked);
        else
            m_ptrLogTreeRename->setCheckState(Qt::Unchecked);
        //--------------------------------------------
        if ( m_bitFlags[ en_LOG_TUNNELING] )
            m_ptrLogSSHTunnel->setCheckState(Qt::Checked);
        else
            m_ptrLogSSHTunnel->setCheckState(Qt::Unchecked);
        //
        m_bInitialLoadFlag = false;
    };
}

void LoggetElements::onClickEnableLog(int i_state)
{
    if (Qt::Checked == i_state)
    {
        m_bEnableLog   = true;
        enableInterface(true);

    }else
    {
        m_bEnableLog   = false;
        enableInterface(false);
    };
    //
    if (false == m_bInitialLoadFlag)
        updateFlags(true);
};

void LoggetElements::enableInterface(bool b_enable)
{
    m_ptrLogSSHTunnel->setEnabled(b_enable);
    //
    m_pLineLogPath->setEnabled(b_enable);
    m_pLogPathChoose->setEnabled(b_enable);
    m_ptrLogErrors->setEnabled(b_enable);
    //
    m_ptrNodeGroupBox->setEnabled(b_enable);
    m_ptrAttGroupBox->setEnabled(b_enable);
    m_ptrTreeGroupBox->setEnabled(b_enable);
}

void LoggetElements::onClickAllCheckboxes()
{
    if (false == m_bInitialLoadFlag)
        updateFlags(true);
};

void  LoggetElements::onChooseLogPath()
{
    QString str_dir_name = QFileDialog::getExistingDirectory(NULL,
                                                             tr("Choose default export directory"),
                                                             m_strLogPath,
                                                             QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks
                                                             );



    if (str_dir_name.length() > 0 )
        m_strLogPath = str_dir_name;
    //
    updateData(false);
    return;
};
