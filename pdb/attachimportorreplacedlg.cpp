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
#include "attachimportorreplacedlg.h"
#include "servicescfg.h"
#include "../CommonInclude/pdb/pdb_style.h"
#include "../CommonInclude/pdb/cryptosupport.h"
//
#include <QFileDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QFileInfo>
#include <QSettings>
//
AttachImportOrReplaceDlg::AttachImportOrReplaceDlg(const QString &str_header, bool b_allow_multiple_files, QObject *parent) :
    QObject(parent)
{
    m_bAllowMultipleFiles    = b_allow_multiple_files;
    m_strDlgHeader           = str_header;
    m_dlgFileDlg             = NULL;
    m_ptrEncryptType         = NULL;
    m_ptrEncryptUploadEnable = NULL;
    m_uiDefaultTypeOfEncrypt = CryptoSupport::NO_ENCRYPT;
}

AttachImportOrReplaceDlg::~AttachImportOrReplaceDlg()
{
    if (m_dlgFileDlg) delete m_dlgFileDlg;
}

bool AttachImportOrReplaceDlg::exec(const QString&  str_path,
                                    const QString   str_action_button_text,
                                    bool            b_delete_files_after_attachment,
                                    bool            b_protect_attachment,
                                    bool            b_encrypt_attachment)
{
    if (str_path.length() == 0)
        return false;
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    m_uiDefaultTypeOfEncrypt                = settings.value(g_str_SEC_TEC_CODE).value<unsigned int>();
    const unsigned int ui_password_length   = ServicesCfg::getInstance().getPassword().length();
    //
    bool b_res = true;
    //
    //this, tr("Import file(s)"), m_strLastAttachmentFromPath, "*.*"
    m_dlgFileDlg = new QFileDialog(NULL, m_strDlgHeader, str_path, "*");

    m_dlgFileDlg->setOption(QFileDialog::DontUseNativeDialog);
    if (m_bAllowMultipleFiles)
    {
        m_dlgFileDlg->setFileMode(QFileDialog::ExistingFiles);
    }else
    {
        m_dlgFileDlg->setFileMode(QFileDialog::ExistingFile);
    };

    m_dlgFileDlg->setLabelText(QFileDialog::Accept, str_action_button_text);
    //
    QLayout* layout = m_dlgFileDlg->layout();
    QGridLayout* gridbox = qobject_cast<QGridLayout*>(layout);
    //
    if (NULL == gridbox)
        return false;
    //
    const QString str_origin_attach_size = settings.value(g_str_ATTACH_MAX_SIZE_MB).toString();
    //
    QCheckBox ctrl_delete_after_upload  ( tr("Delete file(s) after upload to the database") , NULL);
    QCheckBox ctrl_protect_upload       ( tr("Protect attachment(s)") , NULL);
    QLineEdit ctrl_max_attach_size      ( str_origin_attach_size );
    ctrl_max_attach_size.setMaxLength (2);//not more than 99 Mb.
    ctrl_max_attach_size.setInputMask("99");
    //
    m_ptrEncryptUploadEnable    = new QCheckBox( tr("Encrypt attachment(s)") , NULL );
    m_ptrEncryptType            = new QComboBox();
    //
    QObject::connect( m_ptrEncryptType,             SIGNAL(currentIndexChanged(int)),   this, SLOT( onChangeEncryptType (int)     ) );
    QObject::connect( m_ptrEncryptUploadEnable,     SIGNAL(stateChanged(int)),          this, SLOT( onChangeEncryptCheckBox (int) ) );
    //
    fillEncryptionTypes();
    //set actual default type of encrypt
    m_ptrEncryptType->setCurrentIndex(m_uiDefaultTypeOfEncrypt);
    //
    ctrl_delete_after_upload.setChecked(b_delete_files_after_attachment);
    ctrl_protect_upload.setChecked(b_protect_attachment);
    //
    if ( 0 == m_uiDefaultTypeOfEncrypt )
        m_ptrEncryptUploadEnable->setToolTip("Default encrypt method is not defined. Check your settings.");
    else if ( 0 == ui_password_length )
        m_ptrEncryptUploadEnable->setToolTip("Password is not defined. Define password first.");
    //
    if ( ui_password_length > 0 )
    {
        m_ptrEncryptUploadEnable->setEnabled(true);
    }
    else
    {
        m_ptrEncryptUploadEnable->setEnabled(false);
        b_encrypt_attachment = false;
    };
    //
    m_ptrEncryptType->setEnabled(b_encrypt_attachment);
    //
    m_ptrEncryptUploadEnable->setChecked(b_encrypt_attachment);
    //add additional elements to the layout
    gridbox->addWidget ( &ctrl_delete_after_upload, gridbox->rowCount(), 0,1,1);
    gridbox->addWidget ( &ctrl_protect_upload,      gridbox->rowCount(), 0,1,1);
    gridbox->addWidget ( m_ptrEncryptUploadEnable,  gridbox->rowCount(), 0,1,1);
    gridbox->addWidget ( m_ptrEncryptType,          gridbox->rowCount()-1, 1,1,1);
    gridbox->addWidget ( new QLabel("Actual max. size of attaments, Mb:"), gridbox->rowCount(), 0,1,1);
    gridbox->addWidget ( &ctrl_max_attach_size,     gridbox->rowCount()-1, 1,1,1);
    //
    m_dlgFileDlg->setLayout(gridbox);
    //
    if (!m_dlgFileDlg->exec()) // if user click "cancel"
    {
        b_res =  false;
        return b_res;
    };
    //
    m_FileList = m_dlgFileDlg->selectedFiles();
    //
    QString str_new_attach_size = ctrl_max_attach_size.text();
    //
    if (str_origin_attach_size != str_new_attach_size)
    {
        unsigned int ui_attach_size = str_new_attach_size.toInt();
        if (ui_attach_size < 1)
            ui_attach_size = 1;
        settings.setValue( g_str_ATTACH_MAX_SIZE_MB, ui_attach_size );
    };
    //
    if ( m_FileList.size() > 0 )
    {
        QFileInfo pathInfo( m_FileList[0] );
        m_strLastAttachmentFromPath = pathInfo.path();
        m_strLastAttachmentFromPath += g_strPATH_DELIMETER;
        //
        Qt::CheckState c_state = ctrl_delete_after_upload.checkState();
        //
        m_bDeleteFilesAfterAttachment = false;
        //
        if (Qt::Checked == c_state)
        {
            m_bDeleteFilesAfterAttachment = true;
        };
        //
        c_state  = ctrl_protect_upload.checkState();
        m_bProtectAttachment = false;
        //
        if (Qt::Checked == c_state)
        {
            m_bProtectAttachment = true;
        };
        //
        c_state  = m_ptrEncryptUploadEnable->checkState();
        m_bEncryptAttachment = false;
        //
        if (Qt::Checked == c_state)
        {
            m_bEncryptAttachment = true;
            m_uiDefaultTypeOfEncrypt = m_ptrEncryptType->currentIndex();
        }else
        {
            m_uiDefaultTypeOfEncrypt = 0;
        };
    };
    //
    return b_res;
}

void AttachImportOrReplaceDlg::fillEncryptionTypes ()
{
    if (NULL == m_ptrEncryptType)
        return;
    //
    CryptoSupport encrypt_decrypt;
    //
    QStringList str_list_tecs = encrypt_decrypt.getSuppotedTec();
    //
    m_ptrEncryptType->addItem("Do not use cryptography", 0);
    //
    for (int i = 0; i < str_list_tecs.size(); i++)
    {
        m_ptrEncryptType->addItem(str_list_tecs.at(i), i+1);
    };
}

void AttachImportOrReplaceDlg::onChangeEncryptType (int index)
{
    if (CryptoSupport::TRIPLEDES_CBC == index)
        m_ptrEncryptType->setCurrentIndex(CryptoSupport::BLOWFISH_OFB);
    else if (CryptoSupport::NO_ENCRYPT == index)
        m_ptrEncryptUploadEnable->setChecked(false);
    //
    return;
}

void AttachImportOrReplaceDlg::onChangeEncryptCheckBox (int state)
{
    if (Qt::Unchecked == state)
    {
        if (m_ptrEncryptType)
            m_ptrEncryptType->setEnabled(false);
    }else //checked
    {
        if (m_ptrEncryptType)
            m_ptrEncryptType->setEnabled(true);
    };
}
