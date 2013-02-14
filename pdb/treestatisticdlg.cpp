#include "treestatisticdlg.h"
#include "ui_treestatisticdlg.h"
//
#include <QKeyEvent>

TreeStatisticDlg::TreeStatisticDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TreeStatisticDlg)
{
    ui->setupUi(this);
    QObject::connect(ui->m_bOK,  SIGNAL(clicked()),  this, SLOT( onOK ()  ));
}

TreeStatisticDlg::~TreeStatisticDlg()
{
    delete ui;
}

void TreeStatisticDlg::keyPressEvent (QKeyEvent * event)
{
    int key_code = event->key();
    //
    switch( key_code )
    {
    case Qt::Key_Escape:
    case Qt::Key_Return:
        close();
        return;
    default:
        break;
    };
    //
    QDialog::keyPressEvent(event);
};

void TreeStatisticDlg::setNodeAmount          (unsigned int ui_node_amount)
{
    ui->m_EditNodesAmount->setText(QString::number(ui_node_amount));
};

void TreeStatisticDlg::setAttachmentsAmount   (unsigned int ui_attachment_amount)
{
    ui->m_editAttachmentsAmount->setText(QString::number(ui_attachment_amount));
};

void TreeStatisticDlg::setAttachmentsSize     (unsigned int ui_attachment_size)
{
    const QLocale& cLocale = QLocale::c();
    QString str_size = cLocale.toString( ui_attachment_size );

    ui->m_EditSizeofAttachments->setText(str_size);
};

void TreeStatisticDlg::onOK()
{
    close();
};
