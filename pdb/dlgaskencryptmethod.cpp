#include "dlgaskencryptmethod.h"
#include "ui_dlgaskencryptmethod.h"
//
#include "../CommonInclude/pdb/cryptosupport.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QSettings>

DlgAskEncryptMethod::DlgAskEncryptMethod(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgAskEncryptMethod)
{
    ui->setupUi(this);
    //
    QObject::connect(ui->pushButtonOK,      SIGNAL(clicked()),                  this, SLOT (onOK   ()   ));
    QObject::connect(ui->pushButtonCancel,  SIGNAL(clicked()),                  this, SLOT (reject ()   ));
    QObject::connect(ui->m_ptrEncryptMode,  SIGNAL(currentIndexChanged(int)),   this, SLOT (onEncryptModeChanged(int) ));
    //
    m_uiInitialEncryptMode  = 0;
    m_uiEncryptMode         = 0;
    //
    fillEncryptMethodList();
    //
    if (0 != m_uiEncryptMode)
        ui->pushButtonOK->setFocus();
    else
        ui->m_ptrEncryptMode->setFocus();
}

DlgAskEncryptMethod::~DlgAskEncryptMethod()
{
    delete ui;
}

void DlgAskEncryptMethod::fillEncryptMethodList()
{
    CryptoSupport encrypt_decrypt;
    //
    QStringList str_list_tecs = encrypt_decrypt.getSuppotedTec();
    //
    ui->m_ptrEncryptMode->addItem("No encryption", 0);
    //
    for (int i = 0; i < str_list_tecs.size(); i++)
    {
        ui->m_ptrEncryptMode->addItem(str_list_tecs.at(i), i+1);
    };
    //check default ecryption method, set active index
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    m_uiInitialEncryptMode         = settings.value(g_str_SEC_TEC_CODE).value<unsigned int>();
    //
    ui->m_ptrEncryptMode->setCurrentIndex(m_uiInitialEncryptMode);
    //
    //onEncryptModeChanged(m_uiEncryptMode);
}

void DlgAskEncryptMethod::onEncryptModeChanged(int index)
{
    if (0 == index)
        ui->pushButtonOK->setEnabled(false);
    else
        ui->pushButtonOK->setEnabled(true);
    //
    m_uiEncryptMode = index;
}

void DlgAskEncryptMethod::onOK()
{
    if (m_uiEncryptMode != m_uiInitialEncryptMode)
    {
        QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
        settings.setValue(g_str_SEC_TEC_CODE,   m_uiEncryptMode);
    };
    //
    this->accept();
}
