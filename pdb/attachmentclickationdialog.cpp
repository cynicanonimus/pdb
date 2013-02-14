#include "attachmentclickationdialog.h"
#include "ui_attachmentclickationdialog.h"
//
#include <QKeyEvent>

AttachmentClickAtionDialog::AttachmentClickAtionDialog(bool b_rename_enabled, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AttachmentClickAtionDialog)
{
    ui->setupUi(this);
    //
    reset();
    //
    QObject::connect( this->ui->pushButtonOpen,     SIGNAL(clicked()), this, SLOT( onClickOpen    () ) );
    QObject::connect( this->ui->pushButtonDownload, SIGNAL(clicked()), this, SLOT( onClickDownload() ) );
    QObject::connect( this->ui->pushButtonRename,   SIGNAL(clicked()), this, SLOT( onClickRename  () ) );
    QObject::connect( this->ui->pushButtonCancel,   SIGNAL(clicked()), this, SLOT( onClickCancel  () ) );
    //TODO: make it optional in settings
    this->ui->pushButtonOpen->setFocus();
    //#
    this->ui->pushButtonRename->setEnabled(b_rename_enabled);
}

AttachmentClickAtionDialog::~AttachmentClickAtionDialog()
{
    delete ui;
}

void  AttachmentClickAtionDialog::reset()
{
    m_enUserChoice = en_DO_NOTHING;
}

void  AttachmentClickAtionDialog::onClickOpen()
{
    m_enUserChoice = en_OPEN_ATTACH;
    close();
}

void  AttachmentClickAtionDialog::onClickDownload()
{
    m_enUserChoice = en_DOWNLOAD_ATTACH;
    close();
}

void  AttachmentClickAtionDialog::onClickRename()
{
    m_enUserChoice = en_RENAME_ATTACH;
    close();
}

void  AttachmentClickAtionDialog::onClickCancel ()
{
    reset();
    close();
}

bool  AttachmentClickAtionDialog::isDownload()
{
    return (en_DOWNLOAD_ATTACH == m_enUserChoice);
}

bool  AttachmentClickAtionDialog::isOpen()
{
    return (en_OPEN_ATTACH == m_enUserChoice);
}

bool  AttachmentClickAtionDialog::isRename()
{
    return (en_RENAME_ATTACH == m_enUserChoice);
}

void  AttachmentClickAtionDialog::keyPressEvent(QKeyEvent *event)
{
    const int key_code = event->key();
    //
    switch( key_code )
    {
    case Qt::Key_Escape:
        onClickCancel();
        return;
    default:
        break;
    };
    //
    QDialog::keyPressEvent(event);
}
