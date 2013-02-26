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
#include "dbgraficelements.h"
#include "dbstructuredescriptor.h"
//
#include <QtSql>
#include <QtDebug>
#include <QMessageBox>
#include <QVariant>
#include <QThread>
//
#include "../CommonInclude/pdb/pdb_style.h"
//
DBGraficElements::DBGraficElements(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout, QObject* parent) :
    AbstractGraficeElements(ptr_parent_frame, ptr_layout, parent)
{
    //
    m_pPrevButton       = NULL;
    m_pNextButton       = NULL;
    m_pAddButton        = NULL;
    m_pDeleteButton     = NULL;
    //
    m_pLabelPageName    = NULL;
    m_pLineEditPageName = NULL;
    //
    m_pLabelHost        = NULL;
    m_pLineEditHost     = NULL;
    //
    m_pLabelDBName      = NULL;
    m_pLineEditDBName   = NULL;
    //
    m_pLabelUserName    = NULL;
    m_pLineEditUserName = NULL;
    //
    m_pLabelPwd         = NULL;
    m_pLineEditPwd      = NULL;
    //
    m_pLabelPort        = NULL;
    m_pLineEditPort     = NULL;
    //
    m_pLabelConnAmount  = NULL;
    m_pLineEditConnAmount = NULL;
    //
    m_pcheckMakeTunnel  = NULL;
    m_pLabelMakeTunnel  = NULL;
    m_pLineMakeTunnel   = NULL;
    //
    m_pTestDbConnButton = NULL;
    //
    m_pSetCurrentCgfDefault= NULL;
    //
    m_ptrTunnelProcess  = NULL;
    //
    m_strFrameName = tr("Database settings");
    //
    deleteElementsOnExit(true);
    //
    m_dbCreator.setDbSettings(&m_stDBSettings);
    m_bDBCreatorIsOK = m_dbCreator.readDbFileDescription();
    //
    if (m_bDBCreatorIsOK)
        readData();
    //
    createLayout();
    //
    if (m_bDBCreatorIsOK)
        createLinks();
    //
    if (m_bDBCreatorIsOK)
        updateData(false);
    //
    if (m_bDBCreatorIsOK)
        setNavButtonsState();
    //
    if (m_bDBCreatorIsOK)
        onTunnelCheckboxClick();
}

DBGraficElements::~DBGraficElements()
{

}

void DBGraficElements::readData ()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    QVariant var = settings.value(g_str_DB_SETTINGS);
    //
    if (( var.isNull()== false) &&(true == var.isValid()) )
    {
        m_stDBSettings = var.value<DBSettings>();
    }else
    {
        m_stDBSettings.addDefaultPage(m_dbCreator.getDefaultDbDriverName());
        m_stDBSettings.setCurrentPage(0);
        m_stDBSettings.setCurrentPageAsActive();
        //
    };
    //
    //temporary code, remove it after 1-st launch
    //
    while( (unsigned int) m_stDBSettings.getPagesAmount() > m_stDBSettings.m_vMakeTunnel.size() )
    {
        m_stDBSettings.m_vMakeTunnel.push_back(false);
    };
    //
    while ( m_stDBSettings.getPagesAmount() > m_stDBSettings.m_TunnelList.size() )
    {
        m_stDBSettings.m_TunnelList<<"";
    };
    //
    return;
}

void DBGraficElements::writeData()
{
    updateData(true);
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    settings.setValue(g_str_DB_SETTINGS, qVariantFromValue(m_stDBSettings));
}

void DBGraficElements::createLinks()
{
//    QObject::connect(ui->m_TreeOfSettings, SIGNAL( currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(onTreeCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)) );
    QObject::connect(m_pTestDbConnButton,       SIGNAL(clicked()), this, SLOT( onTestBtnClick()   ) );
    QObject::connect(m_pSetCurrentCgfDefault,   SIGNAL(clicked()), this, SLOT(onDefaultBtnClick() ) );
    //
    QObject::connect(m_pPrevButton,             SIGNAL(clicked()), this, SLOT(onPrevClick()       ) );
    QObject::connect(m_pNextButton,             SIGNAL(clicked()), this, SLOT(onNextClick()       ) );
    QObject::connect(m_pAddButton,              SIGNAL(clicked()), this, SLOT(onCreateNewClick()  ) );
    QObject::connect(m_pDeleteButton,           SIGNAL(clicked()), this, SLOT(onDeleteClick()     ) );
    //create link to the checkbox "make tunnel"
    QObject::connect(m_pcheckMakeTunnel,        SIGNAL(clicked()), this, SLOT(onTunnelCheckboxClick()));
}

void DBGraficElements::onTunnelCheckboxClick()
{
    if (m_pcheckMakeTunnel->checkState())
        m_pLineMakeTunnel->setEnabled(true);
    else
        m_pLineMakeTunnel->setEnabled(false);
}

void DBGraficElements::onTestBtnClick()
{
        QMessageBox             box;
        QSqlDatabase            db;
        //
        bool b_make_tunnel = ( m_pcheckMakeTunnel->checkState() == Qt::Checked );
        //
        if (b_make_tunnel)
        {
            m_bTunnelIsActiveNow = true;
            makeTunnel();
            QTime dieTime= QTime::currentTime().addSecs(1);
            while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        };
        //
        QVariant v_combo_data = m_pDatabaseType->itemData(m_pDatabaseType->currentIndex());
        QString str_driver_name = v_combo_data.toString();
        //
        db = QSqlDatabase::addDatabase ( str_driver_name,"conn");   // "QOCI"  // ORACLE for example
        db.setHostName        ( m_pLineEditHost->text()     );      // localhost   "192.168.178.3"
        db.setDatabaseName    ( m_pLineEditDBName->text()   );      // the name of my database
        db.setUserName        ( m_pLineEditUserName->text() );      // the name of my database user
        db.setPassword        ( m_pLineEditPwd->text()      );      // the password of my database user. DO NOT use ONLY from notebook
        //
        const int i_port_number = m_pLineEditPort->text().toInt();
        if ( 0 != i_port_number )
            db.setPort ( i_port_number );
        //
        if ( !db.open() )
        {
            QSqlError err = db.lastError();
            QString str_msg = err.text();
            qDebug() << err;
            box.setText(str_msg);
            box.exec();
        }else
        {
            QStringList str_existing_tables;
            bool b_TablesOK = isDBTablesOK(&db, str_existing_tables);
            //
            if (false == b_TablesOK)
            {
                QString str_message = QString ("Required tables not found in this database. Do you want to create them now?");
                if (str_existing_tables.size() > 0)
                {
                    str_message += "\nExistings tables: ";
                    for (unsigned int k = 0; k < (unsigned int) str_existing_tables.size(); k++)
                    {
                        if (k > 0)
                            str_message += ", ";
                        //
                        str_message += str_existing_tables.at(k);
                    };
                    //
                    str_message += " will be dropped (data will be lost)! ";
                }
                //
                const int i_res = QMessageBox::warning(NULL,
                                                 trUtf8("WARNING!") ,
                                                 str_message,
                                                 QMessageBox::Yes|QMessageBox::No,
                                                 QMessageBox::Yes
                                                );
                if (QMessageBox::Yes == i_res)
                {
                    if ( createDBTables(&db) == true)
                    {
                        box.setText(tr("Tables successfuly created."));
                        box.exec();
                    };
                };
            }else
            {
                box.setText(tr("Connection to db OK, objects OK"));
                box.exec();
            };
        };
        //
        if (b_make_tunnel)
        {
            destroyTunnel();
        };
}

bool DBGraficElements::isDBTablesOK ( QSqlDatabase* ptr_db, QStringList& already_existing_tables )
{
    if (NULL == ptr_db)
        return false;
    //
    const QString str_actual_db_type = m_pDatabaseType->currentText();
    const DbDescriptors&  descriptors = m_dbCreator.getDescriptorsList();
    //
    if ( descriptors.contains(str_actual_db_type) == false)
    {
        return false;
    };
    //
    DbStructureDescriptor* ptr_db_descriptor = descriptors.value(str_actual_db_type);
    //
    if (NULL == ptr_db_descriptor)
        return false;
    //
    // --------------------------------------------- step 1: get tables list ---------------------------------------------
    //
    QSqlQuery qry(*ptr_db);
    //
    bool b_sql = qry.prepare( ptr_db_descriptor->getShowTablesRequest() );
    //
    if ( false == b_sql )
    {
        QString str_message = QString("Can't parse query '%1' ").arg(ptr_db_descriptor->getShowTablesRequest());
        QMessageBox::critical(NULL,"Error in syntax?",str_message, QMessageBox::Ok);
        return true; //nothing to do in this situation, no reason to try to create tables
    };
    //
    b_sql = qry.exec();
    //
    if( false == b_sql )
    {
        QString str_message = QString("Can't execute query '%1' ").arg(ptr_db_descriptor->getShowTablesRequest());
        QMessageBox::critical(NULL,"Error in syntax?",str_message, QMessageBox::Ok);
        return true; //nothing to do in this situation, no reason to try to create tables
    };
    //
    QStringList str_exist_tables;
    //
    while( qry.next() )
    {
        QString     str_table_name   = qry.value(0).toString();
        str_exist_tables<<str_table_name ;
    };
    //------------------ step 2: compare mandatory list and existing list ------------------------
    const QStringList& mandatory_tables_list = m_dbCreator.getMandatoryTablesList();
    //
    bool b_all_tables_exist = true;
    //
    for ( unsigned int i = 0; i < (unsigned int) mandatory_tables_list.size(); ++i )
    {
        if (str_exist_tables.contains( mandatory_tables_list.at(i) ) == false )
        {
            b_all_tables_exist = false;
        }else
        {
            already_existing_tables<<mandatory_tables_list.at(i);
        };
    };
    //
    return b_all_tables_exist;
}

bool DBGraficElements::createDBTables ( QSqlDatabase* ptr_db )
{
    if (NULL == ptr_db)
        return false;
    //
    //bool b_support_transaction      = ptr_db->driver()->hasFeature(QSqlDriver::Transactions);
    //
    const QString str_db_type = m_pDatabaseType->currentText();
    DbStructureDescriptor* ptr_descriptor = NULL;
    //
    DbDescriptors::ConstIterator itr = m_dbCreator.getDescriptorsList().begin();
    //
    while ( itr != m_dbCreator.getDescriptorsList().end() )
    {
        if (itr.key().compare (str_db_type) == 0)
        {
            ptr_descriptor  = itr.value();
            break;
        }
        itr++;
    };
    //
    if (NULL == ptr_descriptor)
        return false;
    //
    const StatementsMap& statements = ptr_descriptor->getStatements();
    //
    const unsigned int ui_statements_amount = statements.size();

    if (0 == ui_statements_amount)
        return false;
    //
    unsigned int ui_statements_counter      = 0;
    unsigned int ui_current_statement_num   = 0;
    bool         b_exec_sql                 = true;
    //
    QSqlQuery qry(*ptr_db);
    //
    //begin transaction
    //
/*
    if (b_support_transaction)
    {
        b_exec_sql = qry.exec("START TRANSACTION;");
        //
        if (false == b_exec_sql)
        {
            QString str_err = QString("can not execute 'START TRANSACTION;' Stop.");
            QMessageBox::critical(NULL,"Error during table creation", str_err, QMessageBox::Ok);
            return false;
        };
    };
*/
    //
    while (ui_statements_counter < ui_statements_amount)
    {
        if (statements.contains(ui_current_statement_num) == false)
        {
            ui_current_statement_num++;
            continue;
        };
        //
        //we found next statement
        //execute it
        //
        const QString str_current_statement = statements.value(ui_current_statement_num);
        b_exec_sql = qry.exec(str_current_statement);
        if (false == b_exec_sql)
        {
            QString str_err = QString("Can't execute query '%1'\n Error: '%2'.\n Stop.").arg(str_current_statement).arg(qry.lastError().text());
            QMessageBox::critical(NULL,"Error during table creation", str_err, QMessageBox::Ok);
            break;
        };
        //
        ui_current_statement_num++;
        ui_statements_counter++;
    };
    //
    //end transaction
    //
    return true;
}

void DBGraficElements::makeTunnel()
{
    QString str_raw_command = m_pLineMakeTunnel->text();
    QStringList str_split_list = str_raw_command.split(" ",QString::SkipEmptyParts);
    //
    QString program = str_split_list.at(0);
    str_split_list.removeAt(0);
    QStringList arguments = str_split_list;
    //
    m_ptrTunnelProcess = new QProcess(this);
    //
    QObject::connect(m_ptrTunnelProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(onTunnelError(QProcess::ProcessError)) );

    m_ptrTunnelProcess->start(program, arguments);
    //
};

void DBGraficElements::onTunnelError ( QProcess::ProcessError err)
{
    if (false == m_bTunnelIsActiveNow)
        return;
    //
    QString str_msg;
    QMessageBox box;
    switch(err)
    {
    case QProcess::FailedToStart:
        str_msg = "Can not make tunnel. Process failed to start.";
        break;
    case QProcess::Crashed:
        str_msg = "Tunnel crashed";
        break;
    case QProcess::Timedout:
        str_msg = "Process timeout expired";
        break;
    case QProcess::WriteError:
        str_msg = "Can not write to process";
        break;
    case QProcess::ReadError:
        str_msg = "Can not read from process";
        break;
    default:
        str_msg = "Unknown error. Process stopped.";
        break;
    };
    //
    box.setText(str_msg);
    box.exec();
}

void  DBGraficElements::destroyTunnel ()
{
    if (m_ptrTunnelProcess)
    {
        m_bTunnelIsActiveNow = false;
        //m_ptrTunnelProcess->tryTerminate();
        m_ptrTunnelProcess->terminate();
        m_ptrTunnelProcess->kill();
        //
        QTime dieTime= QTime::currentTime().addSecs(3);
        while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        //
        //QProcess::ProcessState st = m_ptrTunnelProcess->state();
        delete m_ptrTunnelProcess;
        m_ptrTunnelProcess = NULL;
    };
}

void DBGraficElements::onDefaultBtnClick()
{
    m_stDBSettings.setCurrentPageAsActive();
    setNavButtonsState();
}

void DBGraficElements::onPrevClick ()
{
    updateData(true);
    m_stDBSettings.setPreviousAsCurrent();
    setNavButtonsState();
    updateData(false);
    //set right status oftunnel string
    onTunnelCheckboxClick();
}

void DBGraficElements::onNextClick ()
{
    updateData(true); //remember old
    m_stDBSettings.setNextAsCurrent();
    setNavButtonsState();
    updateData(false); //set new
    //set right status oftunnel string
    onTunnelCheckboxClick();
}

void DBGraficElements::onCreateNewClick ()
{
    updateData(true);
    m_stDBSettings.addDefaultPage(m_dbCreator.getDefaultDbDriverName());
    setNavButtonsState();
    updateData(false);
}

void DBGraficElements::onDeleteClick()
{
    m_stDBSettings.delCurrentPage();
    setNavButtonsState();
    updateData(false);
}

void DBGraficElements::createLayout()
{
    const int i_edit_width  = 230;
    const int i_edit_height = 20;
    //
    const int i_button_width  = 130;
    const int i_button_height = 20;
    //-------------------------------
    int i_row               = 0;
    //
    if (false == m_bDBCreatorIsOK)
    {
        m_pLabelPageName = new QLabel ( m_pParentFrame );
        QString str_message = QString("File %1 contains errors or does not exist.").arg(m_dbCreator.getDescriptorFileName());
        m_pLabelPageName->setText( str_message );
        m_pLabelPageName->setAlignment(Qt::AlignCenter);
        addAndRegisterElement(m_pLabelPageName,i_row,0,1,4);
        //
        i_row++;
        m_pLabelDatabaseType = new QLabel ( m_pParentFrame );
        m_pLabelDatabaseType->setText("Unable to make DB settings");
        m_pLabelDatabaseType->setAlignment(Qt::AlignCenter);
        addAndRegisterElement(m_pLabelDatabaseType, i_row,0,1,4);
        return;
    };
    //
    //---------------
    //
    m_pPrevButton = new QPushButton(QObject::trUtf8("< Prev"), m_pParentFrame);
    m_pPrevButton->setFixedSize(i_button_width,i_button_height);
    addAndRegisterElement(m_pPrevButton, i_row,0,1,1);
    //---------------
    m_pNextButton = new QPushButton(QObject::trUtf8("Next >"), m_pParentFrame);
    m_pNextButton->setFixedSize(i_button_width,i_button_height);
    addAndRegisterElement(m_pNextButton, i_row,1,1,1);
    //---------------
    m_pAddButton = new QPushButton(QObject::trUtf8("Create new"), m_pParentFrame);
    m_pAddButton->setFixedSize(i_button_width,i_button_height);
    addAndRegisterElement(m_pAddButton, i_row,2,1,1);
    //---------------
    m_pDeleteButton = new QPushButton(QObject::trUtf8("Delete current"), m_pParentFrame);
    m_pDeleteButton->setFixedSize(i_button_width,i_button_height);
    addAndRegisterElement(m_pDeleteButton, i_row,3,1,1);
    //---------------
    i_row++;
    m_pLabelPageName = new QLabel ( m_pParentFrame );
    m_pLabelPageName->setText( QObject::trUtf8("Configuration name:") );
    addAndRegisterElement(m_pLabelPageName,i_row,0,1,2);
    //
    m_pLineEditPageName = new QLineEdit( m_pParentFrame );
    addAndRegisterElement(m_pLineEditPageName, i_row,2,1,2);
    //---------------
    i_row++;
    m_pLabelDatabaseType = new QLabel ( m_pParentFrame );
    m_pLabelDatabaseType->setText( QObject::trUtf8("Database type:") );
    m_pLabelDatabaseType->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_pLabelDatabaseType,i_row,0,1,2);
    //
    m_pDatabaseType     = new QComboBox( m_pParentFrame );
    //new databases defined here
    //add all supported DB types
    //
    DbDescriptors::ConstIterator itr = m_dbCreator.getDescriptorsList().begin();
    //
    while ( itr != m_dbCreator.getDescriptorsList().end() )
    {
        m_pDatabaseType->addItem(itr.key(), itr.value()->getDriverName());
        itr++;
    };
    //
    addAndRegisterElement(m_pDatabaseType, i_row,2,1,2);
    //---------------
    i_row++;
    //
    m_pLabelHost = new QLabel ( m_pParentFrame );
    m_pLabelHost->setText( QObject::trUtf8("Host:") );
    m_pLabelHost->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_pLabelHost,i_row,0,1,2);
    //
    m_pLineEditHost = new QLineEdit( m_pParentFrame );
    addAndRegisterElement(m_pLineEditHost, i_row,2,1,2);
    //---------------
    i_row++;
    m_pLabelDBName = new QLabel ( m_pParentFrame );
    m_pLabelDBName->setText( QObject::trUtf8("Database name:") );
    m_pLabelDBName->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_pLabelDBName,i_row,0,1,2);
    //
    m_pLineEditDBName = new QLineEdit( m_pParentFrame );
    addAndRegisterElement(m_pLineEditDBName, i_row,2,1,2);
    //---------------
    i_row++;
    m_pLabelUserName = new QLabel ( m_pParentFrame );
    m_pLabelUserName->setText( QObject::trUtf8("User name:") );
    m_pLabelUserName->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_pLabelUserName,i_row,0,1,2);
    //
    m_pLineEditUserName = new QLineEdit( m_pParentFrame );
    addAndRegisterElement(m_pLineEditUserName, i_row,2,1,2);
    //---------------
    i_row++;
    m_pLabelPwd = new QLabel ( m_pParentFrame );
    m_pLabelPwd->setText( QObject::trUtf8("Password:") );
    m_pLabelPwd->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_pLabelPwd,i_row,0,1,2);
    //
    m_pLineEditPwd = new QLineEdit( m_pParentFrame );
    addAndRegisterElement(m_pLineEditPwd, i_row,2,1,2);
    //---------------
    i_row++;
    m_pLabelPort = new QLabel ( m_pParentFrame );
    m_pLabelPort->setText( QObject::trUtf8("Port:") );
    m_pLabelPort->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_pLabelPort,i_row,0,1,2);
    //
    m_pLineEditPort = new QLineEdit( m_pParentFrame );
    m_pLineEditPort->setAlignment(Qt::AlignRight);
    addAndRegisterElement(m_pLineEditPort,i_row,2,1,2);
    //---------------
    i_row++;
    m_pLabelConnAmount = new QLabel ( m_pParentFrame );
    m_pLabelConnAmount->setText( QObject::trUtf8("Number of database connections:") );
    m_pLabelConnAmount->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_pLabelConnAmount,i_row,0,1,2);
    //
    m_pLineEditConnAmount = new QLineEdit( m_pParentFrame );
    m_pLineEditConnAmount->setAlignment(Qt::AlignRight);
    addAndRegisterElement(m_pLineEditConnAmount,i_row,2,1,2);
    //
    i_row++;
    m_ptrBackupLabel    = new QLabel( m_pParentFrame );
    m_ptrBackupLabel    ->setText( QObject::trUtf8("DB backup command:") );
    m_ptrBackupLabel    ->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_ptrBackupLabel,i_row,0,1,2);
    //
    m_ptrEditBackup = new QLineEdit( m_pParentFrame );
    m_ptrEditBackup->setAlignment(Qt::AlignRight);
    addAndRegisterElement(m_ptrEditBackup,i_row,2,1,2);
    //
    i_row++;
    m_pcheckMakeTunnel = new QCheckBox( tr("Create SSH tunnel before connecting to server") );
    addAndRegisterElement(m_pcheckMakeTunnel,i_row,0,1,3);
    //
    i_row++;
    m_pLabelMakeTunnel = new QLabel ( m_pParentFrame );
    m_pLabelMakeTunnel->setText( QObject::trUtf8("Tunnel command:") );
    m_pLabelMakeTunnel->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_pLabelMakeTunnel,i_row,0,1,2);
    //
    m_pLineMakeTunnel = new QLineEdit( m_pParentFrame );
    m_pLineMakeTunnel->setAlignment(Qt::AlignLeft);
    m_pLineMakeTunnel->setToolTip("ssh user@host -L 3307:127.0.0.1:3306 -N - for example");
    addAndRegisterElement(m_pLineMakeTunnel,i_row,2,1,2);
    //
    i_row++;
    m_pTestDbConnButton = new QPushButton(QObject::trUtf8("Test connection and objects in DB"), m_pParentFrame);
    addAndRegisterElement(m_pTestDbConnButton, i_row,0,1,2);
    //
    m_pSetCurrentCgfDefault = new QPushButton(QObject::trUtf8("Set current settigns as default"), m_pParentFrame);
    addAndRegisterElement(m_pSetCurrentCgfDefault, i_row,2,1,2);

};

void DBGraficElements::updateData (bool b_from_dialog)
{
    if (b_from_dialog) //from dialog to class members
    {
        m_stDBSettings.m_NameofSet[m_stDBSettings.getCurrentPage()] = m_pLineEditPageName->text();
        //
        m_stDBSettings.m_DbHosts    [m_stDBSettings.getCurrentPage()] = m_pLineEditHost->text();
        //m_stDBSettings.m_DbTypes    [m_stDBSettings.getCurrentPage()] = m_pDatabaseType->currentText();
        QVariant v_combo_data = m_pDatabaseType->itemData(m_pDatabaseType->currentIndex());
        const QString str_driver = v_combo_data.toString();
        //
        m_stDBSettings.m_DbTypes    [m_stDBSettings.getCurrentPage()] = str_driver;
        m_stDBSettings.m_DbNames    [m_stDBSettings.getCurrentPage()] = m_pLineEditDBName   ->text();
        m_stDBSettings.m_UserNames  [m_stDBSettings.getCurrentPage()] = m_pLineEditUserName->text();
        m_stDBSettings.m_Passwords  [m_stDBSettings.getCurrentPage()] = m_pLineEditPwd      ->text();
        m_stDBSettings.m_Backups    [m_stDBSettings.getCurrentPage()] = m_ptrEditBackup     ->text();
        m_stDBSettings.m_Ports      [m_stDBSettings.getCurrentPage()] = m_pLineEditPort     ->text().toInt();
        m_stDBSettings.m_ConnNumbers[m_stDBSettings.getCurrentPage()] = m_pLineEditConnAmount->text().toInt();
        //checkbox
        if (m_pcheckMakeTunnel->checkState() == Qt::Checked)
            m_stDBSettings.m_vMakeTunnel[m_stDBSettings.getCurrentPage()] = true;
        else
            m_stDBSettings.m_vMakeTunnel[m_stDBSettings.getCurrentPage()] = false;
        //
        m_stDBSettings.m_TunnelList[m_stDBSettings.getCurrentPage()] = m_pLineMakeTunnel->text();
    }
    else //to dialog from class members
    {
        m_pLineEditPageName ->setText( m_stDBSettings.m_NameofSet[m_stDBSettings.getCurrentPage()] );
        //
        m_pLineEditHost     ->setText( m_stDBSettings.m_DbHosts[m_stDBSettings.getCurrentPage()] );
        //
        QString str_actual_driver = m_stDBSettings.m_DbTypes    [m_stDBSettings.getCurrentPage()];

        for (unsigned int i = 0; i < (unsigned int) m_pDatabaseType->count(); i++)
        {
            QVariant vr_combo_driver = m_pDatabaseType->itemData(i);
            QString str_combo_driver = vr_combo_driver.toString();
            //
            if (str_combo_driver.compare(str_actual_driver) == 0)
            {
                m_pDatabaseType->setCurrentIndex(i);
                break;
            };
        };
        // m_pDatabaseType- later, when have mroe db types
        //
        m_pLineEditDBName   ->setText( m_stDBSettings.m_DbNames  [m_stDBSettings.getCurrentPage()] );
        m_pLineEditUserName ->setText( m_stDBSettings.m_UserNames[m_stDBSettings.getCurrentPage()] );
        m_pLineEditPwd      ->setText( m_stDBSettings.m_Passwords[m_stDBSettings.getCurrentPage()] );
        m_ptrEditBackup     ->setText( m_stDBSettings.m_Backups  [m_stDBSettings.getCurrentPage()] );
        m_pLineEditPort     ->setText( QString::number( m_stDBSettings.m_Ports[m_stDBSettings.getCurrentPage()] ) );
        m_pLineEditConnAmount->setText ( QString::number( m_stDBSettings.m_ConnNumbers[m_stDBSettings.getCurrentPage()] ) );
        //
        if (true == m_stDBSettings.m_vMakeTunnel[m_stDBSettings.getCurrentPage()])
        {
            m_pcheckMakeTunnel->setChecked(true);
        }else
        {
            m_pcheckMakeTunnel->setChecked(false);
        };
        //
        //m_stDBSettings.
        //
        m_pLineMakeTunnel->setText( m_stDBSettings.m_TunnelList[m_stDBSettings.getCurrentPage()] );
    };
}

void DBGraficElements::setNavButtonsState()
{
    if ( (m_stDBSettings.isCurrentPageTheFirst() == false))
    {
        m_pPrevButton->setEnabled(true);
    }else
    {
        m_pPrevButton->setEnabled(false);
    };
    //
    if ( m_stDBSettings.isCurrentPageTheLast() == false )
    {
        m_pNextButton->setEnabled(true);
    }else
    {
        m_pNextButton->setEnabled(false);
    };
    //
    if ( ( m_stDBSettings.getPagesAmount() > 1 ) && (m_stDBSettings.getActivePage() != m_stDBSettings.getCurrentPage()) )
    {
        m_pDeleteButton->setEnabled(true);
    }else
    {
        m_pDeleteButton->setEnabled(false);
    };
    //
    if (m_stDBSettings.getActivePage() != m_stDBSettings.getCurrentPage())
    {
        m_pSetCurrentCgfDefault->setEnabled(true);
        m_pLineEditPageName->setStyleSheet("border: 1px solid grey");
    }else
    {
        m_pLineEditPageName->setStyleSheet("border: 3px solid green");
        m_pSetCurrentCgfDefault->setEnabled(false);
    };
}
