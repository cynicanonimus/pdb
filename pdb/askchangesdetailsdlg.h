#ifndef ASKCHANGESDETAILSDLG_H
#define ASKCHANGESDETAILSDLG_H
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
#include <QKeyEvent>
/*
askchangesdetailsdlg.h

Dialog is using for define the encrypt/decrypt and protect/unprotect operations with node.
User can apply this operations only for node or for node and it childs or cancel operation
*/


namespace Ui {
class AskChangesDetailsDlg;
}

class AskChangesDetailsDlg : public QDialog
{
    Q_OBJECT

public:
    enum USAGE_MODE
    {
        ONLY_ONE_NODE,
        CHILDS_ALSO,
        DO_NOTHING
    };

public:
    explicit AskChangesDetailsDlg(QWidget *parent = 0);
    ~AskChangesDetailsDlg();
    
public:
    void    keyPressEvent(QKeyEvent * event);
    //
    USAGE_MODE getMode () const {return m_enMode;}

private slots:
    void onClickOnlyOne     ();
    void onClickChildsAlso  ();
    void onClickCancel      ();

private:
    Ui::AskChangesDetailsDlg *ui;

private:
    USAGE_MODE  m_enMode;
};

#endif // ASKCHANGESDETAILSDLG_H
