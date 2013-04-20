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
#include "dlginserttable.h"
#include "ui_dlginserttable.h"
//
#include <QTextFrameFormat>
#include <QColorDialog>
//
DlgInsertTable::DlgInsertTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgInsertTable)
{
    ui->setupUi(this);
    //
    ui->comboBoxFrameStyle->addItem( "No border" );
    ui->comboBoxFrameStyle->addItem( "Dashed" );
    ui->comboBoxFrameStyle->addItem( "Dotted" );
    ui->comboBoxFrameStyle->addItem( "Solid" );
    ui->comboBoxFrameStyle->addItem( "Double" );
    ui->comboBoxFrameStyle->addItem( "DotDash" );
    ui->comboBoxFrameStyle->addItem( "DotDotDash" );
    ui->comboBoxFrameStyle->addItem( "Groove" );
    ui->comboBoxFrameStyle->addItem( "Ridge" );
    ui->comboBoxFrameStyle->addItem( "Inset" );
    ui->comboBoxFrameStyle->addItem( "Outset" );
    //
    ui->comboBoxFrameStyle->setCurrentIndex(0);
    //
    ui->tableWidth->setValue(100);
    //
    m_uiColumnsNumber   = 1;
    m_uiRowsNumber      = 1;
    //
    //QColor col = QColorDialog::getColor(Qt::black, this);
    m_cFrameColor.setRgb(0,0,0);
    //QColor col(Qt::black);
    //if(col.isValid())
    //{
    QString qss = QString("background-color: %1").arg(m_cFrameColor.name());
    ui->buttonBackColor->setStyleSheet(qss);
    //};
    //
    QObject::connect(ui->cancelButton,          SIGNAL(clicked()),                  this, SLOT( reject             ()    ));
    QObject::connect(ui->OKButton,              SIGNAL(clicked()),                  this, SLOT( onOK               ()    ));
    QObject::connect(ui->buttonBackColor,       SIGNAL(clicked()),                  this, SLOT( onChangeFrameColor ()    ));
}

DlgInsertTable::~DlgInsertTable()
{
    delete ui;
}
//
void DlgInsertTable::setTableFormat ( const QTextTableFormat& tbl_format )
{
    ui->borderWidth->setValue( tbl_format.border() );
    QTextFrameFormat::BorderStyle style = tbl_format.borderStyle();
    //
    QTextLength length = tbl_format.width();

    ui->tableWidth->setValue((int)length.value(100));
    //
    int i_current_index = 0;
    switch (style)
    {
    case QTextFrameFormat::BorderStyle_None:
        i_current_index = 0;
        break;
    case QTextFrameFormat::BorderStyle_Dashed:
        i_current_index = 1;
        break;
    case QTextFrameFormat::BorderStyle_Dotted:
        i_current_index = 2;
        break;
    case QTextFrameFormat::BorderStyle_Solid:
        i_current_index = 3;
        break;
    case QTextFrameFormat::BorderStyle_Double:
        i_current_index = 4;
        break;
    case QTextFrameFormat::BorderStyle_DotDash:
        i_current_index = 5;
        break;
    case QTextFrameFormat::BorderStyle_DotDotDash:
        i_current_index = 6;
        break;
    case QTextFrameFormat::BorderStyle_Groove:
        i_current_index = 7;
        break;
    case QTextFrameFormat::BorderStyle_Ridge:
        i_current_index = 8;
        break;
    case QTextFrameFormat::BorderStyle_Inset:
        i_current_index = 9;
        break;
    case QTextFrameFormat::BorderStyle_Outset:
        i_current_index = 10;
        break;
    default:
        Q_ASSERT(FALSE);
        i_current_index = 0;
        break;
    };
    //
    ui->comboBoxFrameStyle->setCurrentIndex(i_current_index);
    //
    QBrush brush = tbl_format.borderBrush();
    //

    QColor col = brush.color();
    //
    if(col.isValid())
    {
        m_cFrameColor = col;
        QString qss = QString("background-color: %1").arg(m_cFrameColor.name());
        ui->buttonBackColor->setStyleSheet(qss);
    };
    //
    ui->columsNumber->setEnabled(false);
    ui->rowsNumber->setEnabled(false);
}
//----------------------------------------------------------------------------------------
QTextFrameFormat::BorderStyle DlgInsertTable::getBorderStyle() const
{
    switch(ui->comboBoxFrameStyle->currentIndex())
    {
    case 0:
        return QTextFrameFormat::BorderStyle_None;
    case 1:
        return QTextFrameFormat::BorderStyle_Dashed;
    case 2:
        return QTextFrameFormat::BorderStyle_Dotted;
    case 3:
        return QTextFrameFormat::BorderStyle_Solid;
    case 4:
        return QTextFrameFormat::BorderStyle_Double;
    case 5:
        return QTextFrameFormat::BorderStyle_DotDash;
    case 6:
        return QTextFrameFormat::BorderStyle_DotDotDash;
    case 7:
        return QTextFrameFormat::BorderStyle_Groove;
    case 8:
        return QTextFrameFormat::BorderStyle_Ridge;
    case 9:
        return QTextFrameFormat::BorderStyle_Inset;
    case 10:
        return QTextFrameFormat::BorderStyle_Outset;
    default:
        Q_ASSERT(FALSE);
        break;
    };
    //
    return QTextFrameFormat::BorderStyle_Solid;
}

void DlgInsertTable::onChangeFrameColor ()
{
    QColor col = QColorDialog::getColor(m_cFrameColor, this);
    //
    if(col.isValid())
    {
        m_cFrameColor = col;
        QString qss = QString("background-color: %1").arg(m_cFrameColor.name());
        ui->buttonBackColor->setStyleSheet(qss);
    };
}

void DlgInsertTable::onOK()
{
    m_TableFormat.setBorderStyle (this->getBorderStyle());
    m_TableFormat.setBorder(ui->borderWidth->value());
    QBrush brush (m_cFrameColor);
    m_TableFormat.setBorderBrush(brush);
    //
    m_TableFormat.setWidth(QTextLength(QTextLength::PercentageLength, ui->tableWidth->value()));
    //
    m_uiColumnsNumber = ui->columsNumber->value();
    m_uiRowsNumber    = ui->rowsNumber->value();
    //
    this->accept();
    //this->close();
}
