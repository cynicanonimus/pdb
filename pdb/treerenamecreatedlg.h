#ifndef TREERENAMECREATEDLG_H
#define TREERENAMECREATEDLG_H
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
#include <QDialog>

namespace Ui {
class TreeRenameCreateDlg;
}

class TreeRenameCreateDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit TreeRenameCreateDlg(QWidget *parent = 0);
    ~TreeRenameCreateDlg();

public slots:
    void accept();

public:
    enum DLG_MODE
    {
        RENAME_DLG,
        CREATE_DLG
    };
public:

    void            setDlgMode          (DLG_MODE en_dlg_mode);
    //
    const QString&  getTreeName         ();
    void            setTreeName         (const QString&  str_tree_name);
    //
    const QString&  getRootNodeName     ();
private:
    DLG_MODE        m_enActualDlgMode;
    //
    QString         m_strTreeName;
    QString         m_strRootNodeName;
    //
    Ui::TreeRenameCreateDlg *ui;
};

#endif // TREERENAMECREATEDLG_H
