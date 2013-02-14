#ifndef ATTACHMENTCLICKATIONDIALOG_H
#define ATTACHMENTCLICKATIONDIALOG_H

#include <QDialog>

namespace Ui {
class AttachmentClickAtionDialog;
}

class AttachmentClickAtionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AttachmentClickAtionDialog(bool b_rename_enabled, QWidget *parent = 0);
    ~AttachmentClickAtionDialog();
    //
    //if want use more than once
    void    reset();
    bool    isDownload();
    bool    isOpen();
    bool    isRename();
    //
    void    keyPressEvent(QKeyEvent * event);
    //
private slots:
    void    onClickOpen     ();
    void    onClickDownload ();
    void    onClickCancel   ();
    void    onClickRename   ();

private:
    enum en_ATTACH_USER_CHOICE
    {
        en_OPEN_ATTACH,
        en_DOWNLOAD_ATTACH,
        en_RENAME_ATTACH,
        en_DO_NOTHING
    };
    //
    en_ATTACH_USER_CHOICE m_enUserChoice;
    //
    Ui::AttachmentClickAtionDialog *ui;
};

#endif // ATTACHMENTCLICKATIONDIALOG_H
