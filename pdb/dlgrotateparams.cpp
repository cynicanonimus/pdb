#include "dlgrotateparams.h"
#include "ui_dlgrotateparams.h"

DlgRotateParams::DlgRotateParams(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgRotateParams)
{
    ui->setupUi(this);
    //
    m_bClockWise = true;
    ui->buttonClockwise->setChecked(true);
    //
    ui->spinBox->setMinimum(0);
    ui->spinBox->setMaximum(180);
    ui->spinBox->setValue(90);
    //
    QObject::connect(ui->buttonCancel,      SIGNAL(clicked()), this, SLOT(reject()            ));
    QObject::connect(ui->buttonOK,          SIGNAL(clicked()), this, SLOT(onOK()              ));
}

DlgRotateParams::~DlgRotateParams()
{
    delete ui;
}

void DlgRotateParams::onOK()
{
    m_iAngle = ui->spinBox->value();
    //
    if (ui->buttonClockwise->isChecked())
        m_bClockWise = true;
    else
        m_bClockWise = false;
    //
    this->accept();
}
