#ifndef DLGINSERTHTML_H
#define DLGINSERTHTML_H
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

QT_FORWARD_DECLARE_CLASS(HTMLSyntaxHighlighter)

namespace Ui {
class DlgInsertHTML;
}

class DlgInsertHTML : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgInsertHTML(QWidget *parent = 0);
    ~DlgInsertHTML();
    //
    void showEvent (QShowEvent *);
    //
    const QString&  getHTMLCode();
    //
private slots:
    void onInsertHREF       ();
    void onInsertTABLE      ();
    void onInsertLIST       ();
    void onInsertNumLIST    ();

private:
    Ui::DlgInsertHTML*      ui;
    QString                 m_strHTMLCode;
    HTMLSyntaxHighlighter*  m_ptrHighLighter;
};

#endif // DLGINSERTHTML_H
