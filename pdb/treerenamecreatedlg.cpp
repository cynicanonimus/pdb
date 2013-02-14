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
#include "treerenamecreatedlg.h"
#include "ui_treerenamecreatedlg.h"

#include <QMessageBox>

TreeRenameCreateDlg::TreeRenameCreateDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TreeRenameCreateDlg)
{
    ui->setupUi(this);
    //
    ui->TreeNameFld->setFocus();
}

TreeRenameCreateDlg::~TreeRenameCreateDlg()
{
    delete ui;
}

void        TreeRenameCreateDlg::setDlgMode  (DLG_MODE en_dlg_mode)
{
    m_enActualDlgMode = en_dlg_mode;
    //
    if (RENAME_DLG == m_enActualDlgMode)
    {
        this->setWindowTitle(tr("Rename tree"));
        ui->TreeNameLbl->setText(tr("Actual tree name"));
        ui->RootNodeLbl->setVisible(false);
        ui->RootNodeFld->setVisible(false);
        //ui->RootNodeLbl->setText(tr("Actual root node name"));
    }else
    {
        this->setWindowTitle(tr("Create new tree"));
        ui->TreeNameLbl->setText(tr("Enter new tree name"));
        ui->RootNodeLbl->setText(tr("Enter new root node name"));
        ui->RootNodeLbl->setVisible(true);
        ui->RootNodeFld->setVisible(true);
    };
};

void TreeRenameCreateDlg::setTreeName (const QString&  str_tree_name)
{
    ui->TreeNameFld->setText(str_tree_name);
};

const QString&  TreeRenameCreateDlg::getTreeName     ()
{
    return m_strTreeName;
};

const QString&  TreeRenameCreateDlg::getRootNodeName ()
{
    return m_strRootNodeName;
};

void TreeRenameCreateDlg::accept()
{
    QMessageBox box;
    //
    if ( ui->TreeNameFld->text().length() == 0 ) //if tree name is empty
    {
         box.setText( QObject::trUtf8("You have to define tree name.") );
         box.exec();
         return;
    };
    //
    if ( ( ui->RootNodeFld->text().length() == 0 ) && (RENAME_DLG != m_enActualDlgMode) )
    {
        box.setText( QObject::trUtf8("You have to define root node name") );
        box.exec();
        return;
    };
    //
    m_strTreeName       =   ui->TreeNameFld->text();
    m_strRootNodeName   =   ui->RootNodeFld->text();
    //
    QDialog::accept();
};
