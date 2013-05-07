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
#include "dlgsavescan.h"
#include "ui_dlgsavescan.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QImageWriter>
#include <QTime>
#include <QMessageBox>
#include <QSettings>

DlgSaveScan::DlgSaveScan(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgSaveScan)
{
    ui->setupUi(this);
    //
    fillExtCombobox();
    //
    const QTime c_time = QTime::currentTime();
    const QString str_timestamp = c_time.toString("hh_mm_ss");
    QString str_full_path =QString("document_scan_%1").arg( str_timestamp );
    //
    ui->lineAttachmentName->setText(str_full_path);
    //
    setExtComboboxSel();
    //
    QObject::connect(ui->buttonCancel, SIGNAL(clicked()), this, SLOT(reject() ));
    QObject::connect(ui->buttonSave,   SIGNAL(clicked()), this, SLOT(onSave() ));
    //
    ui->QualitySlider->setMinimum(10);
    ui->QualitySlider->setMaximum(100);
    ui->QualitySlider->setValue(100);
    //
    ui->lineAttachmentName->setFocus();
}

DlgSaveScan::~DlgSaveScan()
{
    delete ui;
}

void DlgSaveScan::fillExtCombobox ()
{
    QList<QByteArray> write_list = QImageWriter::supportedImageFormats();
    //
    QByteArray ba;
     foreach( ba, write_list )
     {
        QString str_ext(ba);
        ui->comboExtentions->addItem(str_ext);
     };
}

void  DlgSaveScan::setExtComboboxSel()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    const QString str_active_ext = settings.value(g_str_SCANNER_DEFAULT_EXT).value<QString>();
    //
    if (str_active_ext.length() == 0)
    {
        ui->comboExtentions->setCurrentIndex(0);
        return;
    };
    //
    for (int i = 0; i < ui->comboExtentions->count(); i++)
    {
        QString str_current_ext = ui->comboExtentions->itemText(i);
        //
        if ( str_current_ext.compare(str_active_ext,Qt::CaseInsensitive) == 0 )
        {
            ui->comboExtentions->setCurrentIndex(i);
            return;
        };
    };
    //
    //nothing found, set 1-st...
    //
    ui->comboExtentions->setCurrentIndex(0);
}

void DlgSaveScan::saveExtComboboxSel ()
{
    if (ui->defaultExtCheckBox->isChecked())
    {
        QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
        settings.setValue( g_str_SCANNER_DEFAULT_EXT, m_strExtention  );
    };
}

void DlgSaveScan::onSave()
{
    m_iImageQuality     = ui->QualitySlider->value();
    m_strAttachmentName = ui->lineAttachmentName->text();
    m_strExtention      = ui->comboExtentions->currentText();
    //
    m_strAttachmentName = m_strAttachmentName.trimmed();
    //
    if (m_strAttachmentName.length() == 0)
    {
        QMessageBox::critical(NULL,
                                  trUtf8("Error!") ,
                                  "Attachment name can not be empty",
                                  QMessageBox::Ok,
                                  QMessageBox::Ok
                                            );
        ui->lineAttachmentName->setFocus();
        return;
    };
    //
    saveExtComboboxSel ();
    //
    m_strAttachmentName = m_strAttachmentName + "." + m_strExtention;
    //
    this->accept();
}
