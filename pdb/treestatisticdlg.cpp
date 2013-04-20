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
}

void TreeStatisticDlg::setNodeAmount          (unsigned int ui_node_amount)
{
    ui->m_EditNodesAmount->setText(QString::number(ui_node_amount));
}

void TreeStatisticDlg::setAttachmentsAmount   (unsigned int ui_attachment_amount)
{
    ui->m_editAttachmentsAmount->setText(QString::number(ui_attachment_amount));
}

void TreeStatisticDlg::setAttachmentsSize     (unsigned int ui_attachment_size)
{
    const QLocale& cLocale = QLocale::c();
    QString str_size = cLocale.toString( ui_attachment_size );

    ui->m_EditSizeofAttachments->setText(str_size);
}

void TreeStatisticDlg::onOK()
{
    close();
}
