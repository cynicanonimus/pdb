#include "dlgexportparams.h"
#include "ui_dlgexportparams.h"
//
#include <QFileDialog>

DlgExportParams::DlgExportParams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgExportParams)
{
    ui->setupUi(this);
    //
    this->ui->m_bAll->setEnabled(false);
    this->ui->m_bOnlySelected->setEnabled(false);
    //
    QObject::connect( this->ui->m_bAll,             SIGNAL(clicked()),              this, SLOT( onClickExportAll     ()  ));
    QObject::connect( this->ui->m_bOnlySelected,    SIGNAL(clicked()),              this, SLOT(onClickExportCurrent  ()  ));
    QObject::connect( this->ui->m_bCancel,          SIGNAL(clicked()),              this, SLOT(onClickCancel         ()  ));
    QObject::connect( this->ui->m_ExportPath,       SIGNAL(textChanged(QString)),   this, SLOT(onChangeExportPath(QString)));
    QObject::connect( this->ui->m_bChangePath,      SIGNAL(clicked()),              this, SLOT(onClickChangeExportPath() ));
}

DlgExportParams::~DlgExportParams()
{
    delete ui;
}

void DlgExportParams::reset()
{
    m_strExportPath = tr("");
    m_enUserChoice = en_DO_NOTHING;
};

bool DlgExportParams::isExportAll() const
{
    return ( en_EXPORT_ALL == m_enUserChoice );
};

bool DlgExportParams::isExportCurrent() const
{
    return ( en_EXPORT_CURRENT == m_enUserChoice );
};
//**********************************************************************************
const QString&  DlgExportParams::getExportPath  () const
{
    return m_strExportPath;
};

void DlgExportParams::onChangeExportPath   (QString str_path)
{
    if (QFile::exists(str_path))
    {
        this->ui->m_bAll->setEnabled(true);
        this->ui->m_bOnlySelected->setEnabled(true);
    }else
    {
        this->ui->m_bAll->setEnabled(false);
        this->ui->m_bOnlySelected->setEnabled(false);
    };
    //
    return;
}

void DlgExportParams::onClickChangeExportPath()
{
    QString str_dir_name = QFileDialog::getExistingDirectory(NULL,
                                                             tr("Choose tree export directory"),
                                                             NULL,
                                                             QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks
                                                             );



    if (str_dir_name.length() > 0 )
        this->ui->m_ExportPath->setText(str_dir_name);
};


void  DlgExportParams::onClickExportAll()
{
    m_enUserChoice = en_EXPORT_ALL;
    m_strExportPath = this->ui->m_ExportPath->text();
    close();
};

void  DlgExportParams::onClickExportCurrent()
{
    m_enUserChoice = en_EXPORT_CURRENT;
    m_strExportPath = this->ui->m_ExportPath->text();
    close();
};

void  DlgExportParams::onClickCancel ()
{
    reset();
    close();
};


void DlgExportParams::keyPressEvent(QKeyEvent * event)
{
    int key_code = event->key();
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
};
