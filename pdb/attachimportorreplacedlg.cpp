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
//
//
#include <QFileDialog>
#include <QGridLayout>
#include <QCheckBox>
#include <QFileInfo>
#include <QSettings>
//
AttachImportOrReplaceDlg::AttachImportOrReplaceDlg(const QString &str_header, bool b_allow_multiple_files, QObject *parent) :
    QObject(parent)
{
    m_bAllowMultipleFiles = b_allow_multiple_files;
    m_strDlgHeader = str_header;
    m_dlgFileDlg  = NULL;
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
    QCheckBox ctrl_delete_after_upload  ( tr("Delete file(s) after upload to the database") , NULL);
    QCheckBox ctrl_protect_upload       ( tr("Protect attachment(s)") , NULL);
    QCheckBox ctrl_crypt_upload         ( tr("Enrypt attachment(s)") , NULL);
    //
    ctrl_delete_after_upload.setChecked(b_delete_files_after_attachment);
    ctrl_protect_upload.setChecked(b_protect_attachment);
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    const unsigned int ui_default_encrypt_method = settings.value(g_str_SEC_TEC_CODE).value<unsigned int>();
    const unsigned int ui_password_length        = ServicesCfg::getInstance().getPassword().length();
    //
    //TODO: insert combobox with encrypt methods
    //
    if ( 0 == ui_default_encrypt_method )
        ctrl_crypt_upload.setToolTip("Default encrypt method is not defined. Check your settings.");
    else if ( 0 == ui_password_length )
        ctrl_crypt_upload.setToolTip("Password is not defined. Define password first.");
    //
    if ( (ui_password_length > 0) && (0 != ui_default_encrypt_method))
    {
        ctrl_crypt_upload.setEnabled(true);
    }
    else
    {
        ctrl_crypt_upload.setEnabled(false);
        b_encrypt_attachment = false;
    };
    //
    ctrl_crypt_upload.setChecked(b_encrypt_attachment);
    //
    gridbox->addWidget(&ctrl_delete_after_upload, gridbox->rowCount(), 0,1,1);
    gridbox->addWidget(&ctrl_protect_upload, gridbox->rowCount(), 0,1,1);
    gridbox->addWidget(&ctrl_crypt_upload, gridbox->rowCount(), 0,1,1);
    //gridbox->addWidget(combo);
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
        c_state  = ctrl_crypt_upload.checkState();
        m_bEncryptAttachment = false;
        //
        if (Qt::Checked == c_state)
        {
            m_bEncryptAttachment = true;
        };
    };
    //
    return b_res;
}
