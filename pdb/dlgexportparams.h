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
#include "../CommonInclude/pdb/pdb_style.h"
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
    const QString&  getExportPath           () const        { return m_strExportPath;           }
    ExportFormat    getExportFormat         () const        { return m_enExportFormat;          }
    bool            isExportOnlyCurrentNode () const        { return m_bExportOnlyCurrentNode;  }
    bool            isExportIncludeAttach   () const        { return m_bExportAttachments;      }
    bool            exportEncrypted         () const        { return m_bExportEncrypted;        }
    //
    void    keyPressEvent(QKeyEvent * event);

private slots:
    //
    void    onClickChangeExportPath ();
    void    onOK                    ();
    
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
    QString         m_strExportPath;
    bool            m_bExportOnlyCurrentNode;
    ExportFormat    m_enExportFormat;
    bool            m_bExportAttachments;
    bool            m_bExportEncrypted;
};

#endif // DLGEXPORTPARAMS_H
