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
#include "securityelements.h"
#include "../CommonInclude/pdb/cryptosupport.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QSettings>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

//
SecurityElements::SecurityElements(QGroupBox *ptr_parent_frame, QGridLayout *ptr_layout) :
       AbstractGraficeElements(ptr_parent_frame, ptr_layout, NULL)
{
    //
    deleteElementsOnExit(true);
    //
    m_strFrameName = tr("Security settings");
    //
    m_ptrAttachmentGroupBox     = NULL;
    m_ptrAttachmentGrid         = NULL;
    //
    m_ptrProtectionByDefault    = NULL;
    //
    readData();
    //
    createLayout();
    //
    createLinks();
    //
    updateData(false);
    //
    onClickShred();
}

SecurityElements::~SecurityElements()
{

}

void SecurityElements::createLinks()
{
    QObject::connect(m_ptrCheckboxUseShredder,  SIGNAL(stateChanged(int)),          this,   SLOT ( onClickShred        ()    ));
    QObject::connect(m_ptrEncryptMode,          SIGNAL(currentIndexChanged(int)),   this,   SLOT ( onEncryptModeChanged(int) ));
}

void SecurityElements::onEncryptModeChanged(int index)
{
    //TODO
    if (CryptoSupport::TRIPLEDES_CBC == index)
        m_ptrEncryptMode->setCurrentIndex(CryptoSupport::BLOWFISH_OFB);
    //
    return;
}

void SecurityElements::onClickShred()
{
    if (m_ptrCheckboxUseShredder->checkState() == Qt::Checked)
        m_ptrEditShredderCmdLine->setEnabled(true);
    else
        m_ptrEditShredderCmdLine->setEnabled(false);
}

void SecurityElements::writeData()
{
    updateData(true);
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    settings.setValue(g_str_SEC_PROTECT,    m_bProtectByDefault);
    //
    settings.setValue(g_str_SEC_SCHRED,     m_bShred);
    settings.setValue(g_str_SEC_SCHRED_CMD, m_strSchredCommandLine);
    //
    settings.setValue(g_str_SEC_TEC_CODE,   m_uiEncryptMode);
    //

    settings.setValue(g_str_SEC_MARK_CRYPT, m_bMarkCryptByDefault);
}

void SecurityElements::readData()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    m_bProtectByDefault     = settings.value(g_str_SEC_PROTECT).value<bool>();
    //
    m_uiEncryptMode         = settings.value(g_str_SEC_TEC_CODE).value<unsigned int>();
    //
    m_bShred                = settings.value(g_str_SEC_SCHRED).value<bool>();
    m_strSchredCommandLine  = settings.value(g_str_SEC_SCHRED_CMD).value<QString>();
    //
    m_bMarkCryptByDefault   = settings.value(g_str_SEC_MARK_CRYPT).value<bool>();
}

void    SecurityElements::updateData  (bool b_from_dialog)
{
    if (b_from_dialog)
    {
        m_uiEncryptMode = m_ptrEncryptMode->currentIndex();

        if (m_ptrProtectionByDefault->checkState() == Qt::Checked)
            m_bProtectByDefault = true;
        else
            m_bProtectByDefault  = false;
        //
        if (m_ptrCheckboxUseShredder->checkState() == Qt::Checked)
            m_bShred = true;
        else
            m_bShred = false;
        //
        m_strSchredCommandLine  = m_ptrEditShredderCmdLine->text();
        //
        if (m_ptrMarkCryptByDefault->checkState() == Qt::Checked)
            m_bMarkCryptByDefault = true;
        else
            m_bMarkCryptByDefault  = false;

    }else
    {
        m_ptrEncryptMode->setCurrentIndex(m_uiEncryptMode);
        //
        if (true == m_bProtectByDefault)
        {
            m_ptrProtectionByDefault->setChecked(true);
        }else
        {
            m_ptrProtectionByDefault->setChecked(false);
        };
        //
        if (true == m_bShred)
        {
            m_ptrCheckboxUseShredder->setChecked(true);
        }else
        {
            m_ptrCheckboxUseShredder->setChecked(false);
        };
        //
        m_ptrEditShredderCmdLine->setText(m_strSchredCommandLine);
        //
        if (true == m_bMarkCryptByDefault)
        {
            m_ptrMarkCryptByDefault->setChecked(true);
        }else
        {
            m_ptrMarkCryptByDefault->setChecked(false);
        };
    };
}

void    SecurityElements::createLayout()
{
    int i_row = 0;
    //--------------------------------------------------------------------

    m_ptrAttachmentGroupBox     = new QGroupBox( tr("Encrypting and decrypting attachments") );
    m_ptrAttachmentGrid         = new QGridLayout(m_ptrAttachmentGroupBox);
    m_ptrAttachmentGroupBox->setLayout(m_ptrAttachmentGrid);
    addAndRegisterElement(m_ptrAttachmentGroupBox, i_row,0,1,4);
    //
    int i_attach_row               = 0;
    //
    m_ptrEncryptModeLabel = new QLabel(tr("Choose default encrypt/decrypt method:"));
    m_ptrAttachmentGrid->addWidget(m_ptrEncryptModeLabel, i_attach_row,0,1,2);
    //
    m_ptrEncryptMode = new QComboBox( m_pParentFrame );
    //
    CryptoSupport encrypt_decrypt;
    //
    QStringList str_list_tecs = encrypt_decrypt.getSuppotedTec();
    //
    m_ptrEncryptMode->addItem("Do not use cryptography", 0);
    //
    for (int i = 0; i < str_list_tecs.size(); i++)
    {
        m_ptrEncryptMode->addItem(str_list_tecs.at(i), i+1);
    };
    //
    m_ptrAttachmentGrid->addWidget(m_ptrEncryptMode, i_attach_row,2,1,2);
    //
    i_attach_row++;
    //
    m_ptrMarkCryptByDefault= new QCheckBox( tr("Always select \"Encrypt attachment(s)\" in upload and replace dialogs") );
    m_ptrAttachmentGrid->addWidget(m_ptrMarkCryptByDefault, i_attach_row,0,1,4);
/*
  Always select \"crypt attachment(s)\" for upload and replace
    m_ptrClientSide = new QRadioButton(tr("crypt on client side"));
    m_ptrAttachmentGrid->addWidget(m_ptrClientSide, i_attach_row,0,1,2);
    //...
    m_ptrServerSide  = new QRadioButton(tr("crypt on server side"));
    m_ptrAttachmentGrid->addWidget(m_ptrServerSide, i_attach_row,2,1,2);
    //
    i_attach_row++;
    //
    m_ptrLblClientCrypt = new QLabel(tr("client side command"));
    m_ptrAttachmentGrid->addWidget(m_ptrLblClientCrypt, i_attach_row,0,1,2);
    //---
*/
    //
    i_row++;
    //
    m_ptrProtectionGroupBox     = new QGroupBox( tr("Protecting attachments") );
    m_ptrProtectionGrid         = new QGridLayout(m_ptrProtectionGroupBox);
    m_ptrProtectionGroupBox->setLayout(m_ptrProtectionGrid);
    addAndRegisterElement(m_ptrProtectionGroupBox, i_row,0,1,4);
    //--------------------------------------------------------------------
    //
    int i_protection_row               = 0;
    //
    m_ptrProtectionByDefault= new QCheckBox( tr("Protect attachment by default") );
    m_ptrProtectionByDefault->setToolTip("Protects attachments from unintetional deletion");
    m_ptrProtectionGrid->addWidget(m_ptrProtectionByDefault, i_protection_row,0,1,4);
    //-----------------------------------------------------------------------------
    i_row++;
    //
    m_ptrTmpFilesGroupBox       = new QGroupBox( tr("Deleting temporary files") );
    m_ptrTmpFilesGrid           = new QGridLayout(m_ptrTmpFilesGroupBox);
    m_ptrTmpFilesGroupBox->setLayout(m_ptrTmpFilesGrid);
    addAndRegisterElement(m_ptrTmpFilesGroupBox, i_row,0,1,4);
    //
    int i_tmp_files_row               = 0;
    //
    m_ptrCheckboxUseShredder = new QCheckBox( tr("Use shredder instead of standard OS delete command for tmp-files") );
    m_ptrCheckboxUseShredder->setToolTip("NOT recommended for SSD drives");
    m_ptrTmpFilesGrid->addWidget(m_ptrCheckboxUseShredder, i_tmp_files_row,0,1,4);
    //
    i_tmp_files_row++;
    //
    m_ptrShredderLabel = new QLabel(tr("Shredder command:"));
    m_ptrTmpFilesGrid->addWidget(m_ptrShredderLabel, i_tmp_files_row,0,1,1);
    //shred  --iterations=N  -u???? -z %file
    m_ptrEditShredderCmdLine = new QLineEdit();
    m_ptrEditShredderCmdLine->setToolTip("For example: shred --iterations=2 -u -z %file \n '%file' is mandatory part of the command");
    m_ptrTmpFilesGrid->addWidget(m_ptrEditShredderCmdLine, i_tmp_files_row,1,1,3);
}
