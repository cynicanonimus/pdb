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
#include "dlgexportparams.h"
#include "ui_dlgexportparams.h"
//
#include <QFileDialog>
#include <QSettings>

DlgExportParams::DlgExportParams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExportParams)
{
    ui->setupUi(this);
    //
    //do not change the order, because of "onOK"
    //
    ui->comboBoxExportFormats->addItem("txt");
    ui->comboBoxExportFormats->addItem("html");
    ui->comboBoxExportFormats->addItem("odt");
    ui->comboBoxExportFormats->addItem("pdf");
    //
    ui->comboBoxExportFormats->setCurrentIndex(0);
    //
    ui->radioButtonOnlyCurrent->setChecked(true);
    m_bExportOnlyCurrentNode = true;
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    m_strExportPath = settings.value(g_str_ATTACH_EXPORT_PATH).toString();
    this->ui->m_ExportPath->setText(m_strExportPath);
    //
    QObject::connect( this->ui->m_bChangePath,      SIGNAL(clicked()),              this, SLOT(onClickChangeExportPath()    ));
    QObject::connect( this->ui->ButtonOK,           SIGNAL(clicked()),              this, SLOT(onOK  ()                     ));
    QObject::connect( this->ui->ButtonCancel,       SIGNAL(clicked()),              this, SLOT(reject()                     ));
}

DlgExportParams::~DlgExportParams()
{
    delete ui;
}

//**********************************************************************************

void DlgExportParams::onClickChangeExportPath()
{
    QString str_dir_name = QFileDialog::getExistingDirectory(NULL,
                                                             tr("Choose tree export directory"),
                                                             m_strExportPath,
                                                             QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks
                                                             );
    if (str_dir_name.length() > 0 )
    {
        this->ui->m_ExportPath->setText(str_dir_name);
        m_strExportPath = str_dir_name;
    };
}

void DlgExportParams::keyPressEvent(QKeyEvent * event)
{
    int key_code = event->key();
    //
    switch( key_code )
    {
    case Qt::Key_Escape:
        this->reject();
        return;
    default:
        break;
    };
    //
    QDialog::keyPressEvent(event);
}

void DlgExportParams::onOK()
{
    if ( true == ui->radioButtonOnlyCurrent->isChecked() )
        m_bExportOnlyCurrentNode = true;
    else
        m_bExportOnlyCurrentNode = false;
    //
    //m_enExportFormat
    switch (ui->comboBoxExportFormats->currentIndex())
    {
    case 0:
        m_enExportFormat = en_TXT;
        break;
    case 1:
        m_enExportFormat = en_HTML;
        break;
    case 2:
        m_enExportFormat = en_ODT;
        break;
    case 3:
        m_enExportFormat = en_PDF;
        break;
    default:
        Q_ASSERT(FALSE);
        m_enExportFormat = en_TXT;
    };
    //
    if ( true == ui->checkBoxExportAttachments->isChecked() )
        m_bExportAttachments = true;
    else
        m_bExportAttachments = false;
    //
    if ( true == ui->checkBoxNoExportEncrypted->isChecked() )
        m_bExportEncrypted = false;
    else
        m_bExportEncrypted = true;
    //
    this->accept();
}
