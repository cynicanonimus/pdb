#ifndef PASSWORDDLG_H
#define PASSWORDDLG_H
//
#include <QDialog>
#include <QGridLayout>
#include <QComboBox>
#include <QProgressDialog>
//
#include "attachment.h"
#include "cryptothread.h"
//
// class is responsible for all user operations with password.
//

namespace Ui
{
    class PasswordDlg;
}

class PasswordDlg : public QDialog
{
    Q_OBJECT

public:
    enum PASSWORD_MODE
    {
        ENTER_PASSWORD,
        CREATE_PASSWORD,
        CHANGE_PASSWORD
    };
    
public:
    explicit PasswordDlg(QWidget *parent = 0);
    ~PasswordDlg();
    
public:
    void    setDbCombobox  ( QComboBox* ptr_combo )        {m_ptrAppCombobox = ptr_combo;}
    void    setDlgMode     ( PASSWORD_MODE en_mode );

signals:
    void    Shutdown           ();
    void    DropAttachments    ();
    void    ReencryptFinished  ();

private slots:
    void    onLeftButtonClick   ();
    void    onRightButtonClick  ();
    void    onChangePassword1   (QString);
    void    onChangePassword2   (QString);
    void    onChangePassword3   (QString);
    void    onCancelAsyncOperations();


private:
    void    createPassword              ();
    bool    changePassword              ();
    //
    //create thread and progress dialog on demand, also mak a link between it
    bool    ecrypt_decrypt_attachments  ( CryptoThread::REQESTED_ACTION en_action, Attachment::AttachmentsList list_of_attachments );
    void    clearTmpArray               ( Attachment::AttachmentsList& list_of_attachments );
    //
    void    initOnDemand                ();
    void    onReEncryptError            ( CryptoThread::REQESTED_ACTION action );

    //
    bool    isPasswordMatchWithExisting ();
    void    setOKAvailabilityOnReplace  ();
    void    setOKAvailabilityOnCreate   ();
    void    createLayout                ();
    void    createLinks                 ();
    bool    requestEncryptedAttachmentsInfo_DB(double &d_bytes_amount);
    bool    requestEncryptedAttachmentsList (Attachment::AttachmentsList& list_of_attachments);
private:

    QGridLayout*        m_ptrMainLayout;
    Ui::PasswordDlg*    ui;
    PASSWORD_MODE       m_enMode;
    QComboBox*          m_ptrAppCombobox;
    //
    CryptoThread*       m_ptrEncrypter;
    QProgressDialog*    m_ptrProgress;
};

#endif // PASSWORDDLG_H
