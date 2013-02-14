#include "askchangesdetailsdlg.h"
#include "ui_askchangesdetailsdlg.h"

AskChangesDetailsDlg::AskChangesDetailsDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskChangesDetailsDlg)
{
    ui->setupUi(this);
    //
    m_enMode = DO_NOTHING;
    //
    QObject::connect( ui->pushButtonOnlyOne,    SIGNAL(clicked()),   this,    SLOT( onClickOnlyOne()       ));
    QObject::connect( ui->pushButtonAll,        SIGNAL(clicked()),   this,    SLOT( onClickChildsAlso()    ));
    QObject::connect( ui->pushButtonCancel,     SIGNAL(clicked()),   this,    SLOT( onClickCancel()        ));
}

AskChangesDetailsDlg::~AskChangesDetailsDlg()
{
    delete ui;
}


void AskChangesDetailsDlg::onClickOnlyOne     ()
{
    m_enMode = AskChangesDetailsDlg::ONLY_ONE_NODE;
    this->close();
}

void AskChangesDetailsDlg::onClickChildsAlso  ()
{
    m_enMode = AskChangesDetailsDlg::CHILDS_ALSO;
    this->close();
}

void AskChangesDetailsDlg::onClickCancel      ()
{
    m_enMode = AskChangesDetailsDlg::DO_NOTHING;
    this->close();
}

void  AskChangesDetailsDlg::keyPressEvent(QKeyEvent *event)
{
    const int key_code = event->key();
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
}
