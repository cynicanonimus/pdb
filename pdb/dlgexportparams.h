#ifndef DLGEXPORTPARAMS_H
#define DLGEXPORTPARAMS_H
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

namespace Ui {
class DlgExportParams;
}

class DlgExportParams : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgExportParams(QWidget *parent = 0);
    ~DlgExportParams();
    //
    void            reset          ();
    bool            isExportAll    () const;
    bool            isExportCurrent() const;
    const QString&  getExportPath  () const;
    //
    void    keyPressEvent(QKeyEvent * event);

private slots:
    void    onClickExportAll     ();
    void    onClickExportCurrent ();
    void    onClickCancel        ();
    void    onChangeExportPath   (QString);
    void    onClickChangeExportPath();
    
private:
    Ui::DlgExportParams *ui;


private:

    enum en_EXPORT_TYPE
    {
        en_EXPORT_ALL,
        en_EXPORT_CURRENT,
        en_DO_NOTHING
    };
    //
    en_EXPORT_TYPE  m_enUserChoice;
    QString         m_strExportPath;
};

#endif // DLGEXPORTPARAMS_H
