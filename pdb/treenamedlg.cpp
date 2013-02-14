#include "treenamedlg.h"
#include "ui_treenamedlg.h"
//
#include <QMessageBox>

TreeNameDlg::TreeNameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TreeNameDlg)
{
    ui->setupUi(this);
    //
    m_bRenameMode = false;
   /*
    button = new QPushButton( "Ok" );
    connect( button, SIGNAL(clicked()), this, SLOT(okClicked()) );
    */
}

TreeNameDlg::~TreeNameDlg()
{
    delete ui;
}

void TreeNameDlg::setTreeNameLabel   (const QString& str_label)
{
    ui->TreeNameLbl->setText(str_label);
};

void TreeNameDlg::setRootNameLabel   (const QString& str_label)
{
    ui->RootNameLbl->setText(str_label);
};


void TreeNameDlg::accept()
{
    QMessageBox box;
    //
    if ( ui->TreeName->text().length() == 0 ) //if tree name is empty
    {
         box.setText( QObject::trUtf8("You have to define tree name.") );
         box.exec();
         return;
    };
    //
    if ( ( ui->RootNodeName->text().length() == 0 ) && (false == m_bRenameMode) )
    {
        box.setText( QObject::trUtf8("You have to define root node name") );
        box.exec();
        return;
    };
    //
    QDialog::accept();
};

void TreeNameDlg::setTreeName(const QString& str_tree_name)
{
    ui->TreeName->setText(str_tree_name);
};

QString TreeNameDlg::getTreeName     () const
{
    return ui->TreeName->text();
};

QString TreeNameDlg::getRootNodeName () const
{
    return ui->RootNodeName->text();
};

void TreeNameDlg::setRenameMode(bool b_rename_mode_active)
{
    m_bRenameMode = b_rename_mode_active;
    //make root node name label visible or unvisible depends on the income param.
    ui->RootNameLbl->setVisible(!m_bRenameMode);
    ui->RootNodeName->setVisible(!m_bRenameMode);
    //
};
