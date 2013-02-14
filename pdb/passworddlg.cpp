#include "passworddlg.h"
#include "ui_passworddlg.h"
#include "servicescfg.h"
#include "dbacccesssafe.h"
#include "logger.h"
//
#include <QCryptographicHash>
#include <QByteArray>
#include <QMessageBox>
#include <QtSql>
//

PasswordDlg::PasswordDlg( QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::PasswordDlg)
{
    m_ptrMainLayout = NULL;
    ui->setupUi(this);
    //
    createLayout();
    //
    createLinks();
    //
    m_enMode = CHANGE_PASSWORD;
    m_ptrAppCombobox = NULL;
    //
    m_ptrEncrypter = NULL;
    m_ptrProgress  = NULL;
    //
    setDlgMode(ENTER_PASSWORD);
}

PasswordDlg::~PasswordDlg()
{
    delete ui;
    //
    if (m_ptrEncrypter) delete m_ptrEncrypter;
    if (m_ptrProgress)  delete m_ptrProgress;
}

void PasswordDlg::createLayout ()
{
    m_ptrMainLayout         = new QGridLayout(this);
    this->setLayout(m_ptrMainLayout);
    ///m_pGridLayout->addWidget(ptr_widget, row,column,rowSpan,columnSpan, aligment);
    int i_row               = 0;
    m_ptrMainLayout->addWidget(ui->m_LblPassword1, i_row,0,1,2);
    //...
    m_ptrMainLayout->addWidget(ui->m_EnterPasswdLineEdit1, i_row,2,1,4);
    //
    i_row++;
    m_ptrMainLayout->addWidget(ui->m_LblPassword2, i_row,0,1,2);
    //...
    m_ptrMainLayout->addWidget(ui->m_EnterPasswdLineEdit2, i_row,2,1,4);
    //
    i_row++;
    m_ptrMainLayout->addWidget(ui->m_LblPassword3, i_row,0,1,2);
    //...
    m_ptrMainLayout->addWidget(ui->m_EnterPasswdLineEdit3, i_row,2,1,4);
    //
    i_row++;
    m_ptrMainLayout->addWidget(ui->m_LeftButton, i_row,0,1,3);
    //...
    m_ptrMainLayout->addWidget(ui->m_RightButton, i_row,3,1,3);
    //
    i_row++;
    m_ptrMainLayout->addWidget(ui->m_msgLabel, i_row, 0, 1, 6);
}

void PasswordDlg::createLinks ()
{
    QObject::connect( ui->m_EnterPasswdLineEdit1,   SIGNAL(textChanged(QString)),       this,    SLOT(onChangePassword1(QString) ));
    QObject::connect( ui->m_EnterPasswdLineEdit2,   SIGNAL(textChanged(QString)),       this,    SLOT(onChangePassword2(QString) ));
    QObject::connect( ui->m_EnterPasswdLineEdit3,   SIGNAL(textChanged(QString)),       this,    SLOT(onChangePassword3(QString) ));

    QObject::connect( ui->m_LeftButton,             SIGNAL(clicked()),                  this,    SLOT( onLeftButtonClick()       ));
    QObject::connect( ui->m_RightButton,            SIGNAL(clicked()),                  this,    SLOT( onRightButtonClick()      ));
}

void PasswordDlg::setOKAvailabilityOnReplace ()
{
    if ( isPasswordMatchWithExisting() == false )
    {
        ui->m_msgLabel->setText("old password is wrong");
        ui->m_LeftButton            ->setEnabled(false);
        return;
    };
    ui->m_msgLabel->setText("");
    //
    QString str_pwd_new_1 = ui->m_EnterPasswdLineEdit1->text();
    QString str_pwd_new_2 = ui->m_EnterPasswdLineEdit2->text();
    QString str_pwd_new_3 = ui->m_EnterPasswdLineEdit3->text();
    //
    if (str_pwd_new_2.compare(str_pwd_new_3) != 0)
    {
        ui->m_msgLabel->setText("new password is not typed correct");
        ui->m_LeftButton            ->setEnabled(false);
        return;
    };
    ui->m_msgLabel->setText("");
    //
    if (str_pwd_new_2.length() == 0)
    {
        ui->m_msgLabel->setText("new password is not typed yet");
        ui->m_LeftButton            ->setEnabled(false);
        return;
    };
    //
    if (str_pwd_new_1.compare(str_pwd_new_2) == 0)
    {
        ui->m_msgLabel->setText("new password and old are the same");
        ui->m_LeftButton            ->setEnabled(false);
        return;
    };
    //
    ui->m_LeftButton            ->setEnabled(true);
}

void PasswordDlg::setOKAvailabilityOnCreate ()
{
    QString str_pwd_new_1 = ui->m_EnterPasswdLineEdit1->text();
    QString str_pwd_new_2 = ui->m_EnterPasswdLineEdit2->text();
    //
    if (str_pwd_new_1.length() ==  0)
    {
        ui->m_LeftButton            ->setEnabled(false);
        return;
    };
    //
    if (str_pwd_new_1.compare(str_pwd_new_2) != 0)
    {
        ui->m_LeftButton            ->setEnabled(false);
        return;
    };
    //
    ui->m_LeftButton            ->setEnabled(true);
    return;
}

bool PasswordDlg::isPasswordMatchWithExisting ()
{
    //
    QString     str_entered_password    = ui->m_EnterPasswdLineEdit1->text();
    QByteArray  ba_entered_password     = str_entered_password.toUtf8();
    QString     str_new_pwd_hash        = QCryptographicHash::hash(ba_entered_password, QCryptographicHash::Sha1).toBase64();
    //
    if ( str_new_pwd_hash == ServicesCfg::getInstance().getPasswordHash() )
        return true;
    //
    return false;
}

void PasswordDlg::onChangePassword2   (QString)
{
    switch(m_enMode)
    {
    case CHANGE_PASSWORD:
        setOKAvailabilityOnReplace();
        break;
    case CREATE_PASSWORD:
        setOKAvailabilityOnCreate();
        break;
    default:
        break;
    };
}

void PasswordDlg::onChangePassword3   (QString)
{
  switch(m_enMode)
  {
  case CHANGE_PASSWORD:
      setOKAvailabilityOnReplace();
      break;
  default:
      break;
  };
}

void PasswordDlg::onChangePassword1  (QString str_text)
{
    switch(m_enMode)
    {
    case ENTER_PASSWORD:
        if (str_text.length() == 0)
            ui->m_LeftButton->setEnabled(false);
        else
            ui->m_LeftButton->setEnabled(true);
        //
        break;
    case CHANGE_PASSWORD:
        setOKAvailabilityOnReplace();
        break;
    case CREATE_PASSWORD:
        setOKAvailabilityOnCreate();
        break;
    };
}

void PasswordDlg::onLeftButtonClick  () //ok usially
{
    switch(m_enMode)
    {
    case ENTER_PASSWORD:
        if ( isPasswordMatchWithExisting () == true )
        {
            ServicesCfg::getInstance().enterExistingPassword( ui->m_EnterPasswdLineEdit1->text() );
            this->close();
        }else
        {
            QMessageBox::critical(NULL, "Error", "Password is wrong, try again.", QMessageBox::Ok);
            ui->m_EnterPasswdLineEdit1->setText("");
        };
        //
        break;
    case CHANGE_PASSWORD:
        //
        //TODO make all change password operations here
        changePassword ();
        ui->m_EnterPasswdLineEdit1->setText("");
        ui->m_EnterPasswdLineEdit2->setText("");
        ui->m_EnterPasswdLineEdit3->setText("");
        this->close ();
        break;
    case CREATE_PASSWORD:
        createPassword();
        break;
    };

    return;
}

bool PasswordDlg::changePassword()
{
    this->hide();
    //get new password from dialog
    bool b_res = false;
    QMessageBox box;

    QString str_new_password    = ui->m_EnterPasswdLineEdit3->text();
    //
    double d_encryted_bytes_totally = 0;
    //request summary size of the crypted attachments
    if (requestEncryptedAttachmentsInfo_DB(d_encryted_bytes_totally) == false)
        return false;
    //
    if (d_encryted_bytes_totally < 1) //because of double
    {
        //just change password and inform user;
        if (ServicesCfg::getInstance().setNewPassword(str_new_password) == true)
        {
            box.setText("Password changed succesfully");
            b_res = false;
        }else
        {
            box.setText("Can not change password");
            b_res = true;
        };
        box.exec();
        return b_res;
    };
    // something is already encrypted, ask user start process or not
    const QLocale& cLocale = QLocale::c();
    unsigned int ui_size = (unsigned int) d_encryted_bytes_totally;
    QString str_size = cLocale.toString( ui_size );

    QString str_message_text = "Needs to re-ecnrypt ";
    str_message_text += str_size;
    str_message_text += " bytes. Do you want to do it now?";
    const int i_res = QMessageBox::question(NULL,
                                     trUtf8("Re-encrypt request") ,
                                     str_message_text,
                                     QMessageBox::Yes|QMessageBox::No,
                                     QMessageBox::No
                                    );
    if (QMessageBox::No == i_res)
        return true;
    //
    //drop all attachments of all nodes. "forget" it. It makes for cleaning the status of nodes .
    emit DropAttachments();
    //
    Attachment::AttachmentsList list_of_attachments;
    //
    if ( requestEncryptedAttachmentsList (list_of_attachments) == false)
    {
        clearTmpArray(list_of_attachments);
        return false;
    };
    //
    if ( ecrypt_decrypt_attachments ( CryptoThread::DECRYPT_IT, list_of_attachments ) == false )
    {
        clearTmpArray(list_of_attachments);
        return false;
    };
    //now change password
    if (ServicesCfg::getInstance().setNewPassword(str_new_password) == false)
    {
        QMessageBox::critical(NULL, "Error", "Can not set new password.\nOld password still active.\nAll attachments are in decrypted state now!\nStop.", QMessageBox::Ok);
        clearTmpArray(list_of_attachments);
        return false;
    };
    //and encrypt again
    if ( ecrypt_decrypt_attachments ( CryptoThread::ENCRYPT_IT, list_of_attachments ) == false )
    {
        clearTmpArray(list_of_attachments);
        return false;
    };
    //
    clearTmpArray(list_of_attachments);
    //
    emit ReencryptFinished();
    QMessageBox::information(NULL, "Finished", "Password successfuly changed. All attachments are re-encrypted.", QMessageBox::Ok);
    //
    return true;
}

void PasswordDlg::clearTmpArray ( Attachment::AttachmentsList& list_of_attachments )
{
    for (unsigned int i = 0; i < list_of_attachments.size(); ++i)
    {
        delete list_of_attachments[i];
    };
};

bool PasswordDlg::ecrypt_decrypt_attachments ( CryptoThread::REQESTED_ACTION en_action, Attachment::AttachmentsList list_of_attachments )
{
    initOnDemand();
    //
    QString str_title;
    //
    switch(en_action)
    {
    case CryptoThread::ENCRYPT_IT:
        str_title = "";
        break;
    case CryptoThread::DECRYPT_IT:
        str_title = "Decrypt all encrypted attachments";
        break;
    default:
        Q_ASSERT(FALSE);
        return false;
    };
    //
    m_ptrEncrypter->setAction(en_action);
    m_ptrProgress->setWindowTitle(str_title);
    //
    m_ptrEncrypter->addAttacmentsToProcessList(list_of_attachments);
    //
    m_ptrProgress->setMinimum(0);
    m_ptrProgress->setMaximum( list_of_attachments.size() );
    //
    m_ptrEncrypter->start(QThread::IdlePriority);
    m_ptrProgress->exec();
    m_ptrEncrypter->wait();
    //
    if (m_ptrEncrypter->isFinishedCorrectly() == false)
    {
        onReEncryptError(CryptoThread::DECRYPT_IT);
        return false;
    };
    //
    return true;
}

bool PasswordDlg::requestEncryptedAttachmentsList (Attachment::AttachmentsList& list_of_attachments)
{

    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    Q_ASSERT(ptr_db);
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_select_str = QString("select id_attach, attach_name, crypted, is_binary from attachments WHERE crypted <> 0;");
    //
    if (!qry.prepare( str_select_str ))
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;

    } else if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    }
    //
    while( qry.next() )
    {
        int         id_attach         = qry.value(0).toInt();
        QString     str_attach_name   = qry.value(1).toString();
        int         i_attach_crypted  = qry.value(2).toInt();
        bool        b_attach_binary   = qry.value(3).toBool();
        //
        Attachment* ptr_new_attachment = new Attachment(id_attach,
                                                        str_attach_name,
                                                        0,
                                                        true,
                                                        i_attach_crypted,
                                                        false,
                                                        b_attach_binary,
                                                        QDateTime::currentDateTime(),
                                                        NULL);
        //
        list_of_attachments.push_back(ptr_new_attachment);
    }; // while( qry.next() )

    return true;
}

bool PasswordDlg::requestEncryptedAttachmentsInfo_DB(double& d_bytes_amount)
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    Q_ASSERT(ptr_db);
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_select_str = "select sum(attach_size) from attachments where crypted <> 0;";
    //
    bool b_sql_result = qry.prepare( str_select_str );
    //
    if (false == b_sql_result)
    {
        const QString str_msg = QString("Unable prepare query %1").arg( qry.lastError().text() );
        Logger::getInstance().logIt(en_LOG_ERRORS,str_msg);
        return false;
    };
    //
    b_sql_result = qry.exec();
    //
    if( false == b_sql_result )
    {
        const QString str_msg = QString("Unable exec query %1").arg( qry.lastError().text() );
        Logger::getInstance().logIt(en_LOG_ERRORS,str_msg);
        return false;
    };
    //
    while( qry.next() )
    {
        d_bytes_amount    = qry.value(0).toDouble();
    };

    //
    return true;
}

void PasswordDlg::createPassword ()
{
    QString     str_entered_password    = ui->m_EnterPasswdLineEdit1->text();
    //
    bool b_res = ServicesCfg::getInstance().setNewPassword ( str_entered_password );
    //
    if (b_res)
    {
        QMessageBox box;
        box.setText("Password successfuly created.");
        box.exec();
        close();
    };
}

void PasswordDlg::onRightButtonClick () //cancel usially
{
    switch(m_enMode)
    {
    case ENTER_PASSWORD:
        //emit Shutdown();
        close();
        break;
    default:
        //reset fields
        ui->m_EnterPasswdLineEdit1->setText("");
        ui->m_EnterPasswdLineEdit2->setText("");
        ui->m_EnterPasswdLineEdit3->setText("");
        close();
    };
}

void PasswordDlg::setDlgMode (PASSWORD_MODE en_mode)
{
    if (m_enMode == en_mode)
        return;
    //
    m_enMode = en_mode;
    //
    switch(m_enMode)
    {
    case ENTER_PASSWORD:
        this                        ->setWindowTitle("Enter application password");
        ui->m_LblPassword1          ->setText("Enter password:");
        ui->m_EnterPasswdLineEdit1  ->setEchoMode(QLineEdit::Password);
        ui->m_EnterPasswdLineEdit1  ->clear();
        //
        ui->m_LeftButton            ->setEnabled(false);
        ui->m_LeftButton            ->setText("OK");
        ui->m_RightButton           ->setText("Cancel");
        //
        //ui->m_msgLabel->setText("Enter password and click 'OK' or click 'Exit application'");
        //
        ui->m_LblPassword2          ->hide();
        ui->m_EnterPasswdLineEdit2  ->hide();
        ui->m_LblPassword3          ->hide();
        ui->m_EnterPasswdLineEdit3  ->hide();
        this->resize(405, 90);
        break;
    case CHANGE_PASSWORD:
        this->setWindowTitle("Change existing password");
        //
        ui->m_LblPassword1->setText("Enter old password:");
        ui->m_EnterPasswdLineEdit1->setEchoMode(QLineEdit::Password);
        ui->m_EnterPasswdLineEdit1->setText("");
        //
        ui->m_LblPassword2          ->setText("Enter new password:");
        ui->m_LblPassword2          ->show();
        ui->m_EnterPasswdLineEdit2->setEchoMode(QLineEdit::Password);
        ui->m_EnterPasswdLineEdit2->setText("");
        ui->m_EnterPasswdLineEdit2  ->show();
        //
        ui->m_LblPassword3          ->setText("Enter new password again:");
        ui->m_LblPassword3          ->show();
        ui->m_EnterPasswdLineEdit3->setEchoMode(QLineEdit::Password);
        ui->m_EnterPasswdLineEdit3->setText("");
        ui->m_EnterPasswdLineEdit3  ->show();
        //
        ui->m_LeftButton            ->setEnabled(false);
        ui->m_LeftButton            ->setText("Change");
        ui->m_RightButton           ->setText("Cancel");
        //
        this->resize(405, 110);
        break;
    case CREATE_PASSWORD:
        this->setWindowTitle("Enter new password");
        //
        ui->m_LblPassword1->setText("Enter new password:");
        ui->m_EnterPasswdLineEdit1->setEchoMode(QLineEdit::Password);
        ui->m_EnterPasswdLineEdit1->setText("");
        //
        ui->m_LblPassword2          ->setText("Enter new password again:");
        ui->m_LblPassword2          ->show();
        ui->m_EnterPasswdLineEdit2->setEchoMode(QLineEdit::Password);
        ui->m_EnterPasswdLineEdit2->setText("");
        ui->m_EnterPasswdLineEdit2  ->show();
        //
        //ui->m_LblPassword3          ->setText("");
        //ui->m_LblPassword3          ->hide();
        //ui->m_EnterPasswdLineEdit3->setEchoMode(QLineEdit::Password);
        //ui->m_EnterPasswdLineEdit3->setText("");
        ui->m_EnterPasswdLineEdit3  ->hide();
        //
        ui->m_LeftButton            ->setEnabled(false);
        ui->m_LeftButton            ->setText("Enter");
        ui->m_RightButton           ->setText("Cancel");
        //
        this->resize(405, 90);
        break;
//405x225
        break;
    };
}

void PasswordDlg::initOnDemand ()
{
    if (NULL != m_ptrEncrypter)
        return;
    //
    m_ptrEncrypter  = new CryptoThread;
    m_ptrProgress   = new QProgressDialog;
    //
    QObject::connect( m_ptrEncrypter,        SIGNAL(setTotalAttachmentsNumber(unsigned int)),       m_ptrProgress,  SLOT( setMaximum(int)                                   ) );
    QObject::connect( m_ptrEncrypter,        SIGNAL(setCurrentLabel(QString)),                      m_ptrProgress,  SLOT( setLabelText(QString)                             ) );
    QObject::connect( m_ptrEncrypter,        SIGNAL(setValue(int)),                                 m_ptrProgress,  SLOT( setValue(int)                                     ) );
    QObject::connect( m_ptrEncrypter,        SIGNAL(stopThis()),                                    this,           SLOT( onCancelAsyncOperations()                         ) );
    //QObject::connect( m_ptrProgress,         SIGNAL(canceled()),                                m_ptrEncrypter, SLOT( OnTerminate()             ) );
    //
    m_ptrProgress->setAutoClose(true);
    m_ptrProgress->setAutoReset(true);
    m_ptrProgress->setMinimumWidth(380);
    m_ptrProgress->setCancelButton(NULL);
}

void PasswordDlg::onReEncryptError (CryptoThread::REQESTED_ACTION action)
{
    switch(action)
    {
    case CryptoThread::ENCRYPT_IT:
        QMessageBox::critical(NULL, "Error", "Can not encrypt attachment. Stop.", QMessageBox::Ok);
        break;
    case CryptoThread::DECRYPT_IT:
        QMessageBox::critical(NULL, "Error", "Can not decrypt attachment. Stop.", QMessageBox::Ok);
        break;
    default:
        Q_ASSERT(FALSE);
    };
}

void PasswordDlg::onCancelAsyncOperations()
{
    m_ptrProgress->close();
}
