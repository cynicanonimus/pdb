#ifndef TREESTATISTICDLG_H
#define TREESTATISTICDLG_H
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
class TreeStatisticDlg;
}

class TreeStatisticDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit TreeStatisticDlg(QWidget *parent = 0);
    ~TreeStatisticDlg();

public:
    void keyPressEvent          (QKeyEvent * event);
    void setNodeAmount          (unsigned int ui_node_amount);
    void setAttachmentsAmount   (unsigned int ui_attachment_amount);
    void setAttachmentsSize     (unsigned int ui_attachment_size);

private slots:
    void onOK();


private:
    Ui::TreeStatisticDlg *ui;
};

#endif // TREESTATISTICDLG_H
