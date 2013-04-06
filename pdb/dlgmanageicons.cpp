//
#include    <QGridLayout>
//
#include    "dlgmanageicons.h"
#include    "ui_dlgmanageicons.h"

DlgManageIcons::DlgManageIcons(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgManageIcons)
{
    ui->setupUi(this);
    //
    makeLayout();
    //
    ui->buttonDelete->setEnabled(false);
    ui->buttonSave->setEnabled(false);
    ui->buttonLoad->setEnabled(false);
    ui->buttonSelected->setEnabled(false);
    //
    QObject::connect(ui->buttonDelete,      SIGNAL(clicked()), this, SLOT(onDelete  ()     ));
    QObject::connect(ui->buttonSave,        SIGNAL(clicked()), this, SLOT(onSave    ()     ));
    QObject::connect(ui->buttonLoad,        SIGNAL(clicked()), this, SLOT(onLoad    ()     ));
    QObject::connect(ui->buttonSelected,    SIGNAL(clicked()), this, SLOT(onSelect  ()     ));
    QObject::connect(ui->buttonSetNothing,  SIGNAL(clicked()), this, SLOT(onSetNothing  () ));
    //
    QObject::connect(ui->buttonCancel,      SIGNAL(clicked()), this, SLOT(reject    ()     ));
    //
}

DlgManageIcons::~DlgManageIcons()
{
    delete ui;
}

void DlgManageIcons::makeLayout()
{
    m_ptrLayout = new QGridLayout(this);
    this->setLayout(m_ptrLayout);
    //
    int i_row               = 0;
    //
    m_ptrLayout->addWidget(ui->labelQuickSearch, i_row,0,1,2);
    m_ptrLayout->addWidget(ui->lineEditQuickSearch, i_row,2,1,3);
    //
    i_row++;
    m_ptrLayout->addWidget(ui->listIcons,i_row,0,6,5);
    //
    m_ptrLayout->addWidget(ui->buttonDelete,i_row,6,1,2);
    i_row++;
    m_ptrLayout->addWidget(ui->buttonSave,i_row,6,1,2);
    i_row++;
    m_ptrLayout->addWidget(ui->buttonLoad,i_row,6,1,2);
    i_row++;
    i_row++;
    m_ptrLayout->addWidget(ui->buttonSelected,i_row,6,1,2);
    i_row++;
    m_ptrLayout->addWidget(ui->buttonSetNothing,i_row,6,1,2);
    //
    i_row++;
    m_ptrLayout->addWidget(ui->buttonCancel,i_row,0,1,7);

    //ui->labelQuickSearch

}

void DlgManageIcons::onDelete       ()
{

}

void DlgManageIcons::onSave         ()
{

}

void DlgManageIcons::onLoad         ()
{

}

void DlgManageIcons::onSelect       ()
{
    this->accept();
}
void DlgManageIcons::onSetNothing   ()
{

}
