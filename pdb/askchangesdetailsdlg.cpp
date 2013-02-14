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
