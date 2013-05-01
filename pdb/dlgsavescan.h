#ifndef DLGSAVESCAN_H
#define DLGSAVESCAN_H
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
class DlgSaveScan;
}

class DlgSaveScan : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgSaveScan(QWidget *parent = 0);
    ~DlgSaveScan();

public:
    
    int             getImageQuality   () const {return m_iImageQuality;}
    const QString&  getAttachmentName () const {return m_strAttachmentName;}
    const QString&  getAttachmentExt  () const {return m_strExtention;}

private slots:
    void onSave();

private:
    Ui::DlgSaveScan *ui;

private:
    int         m_iImageQuality;
    QString     m_strAttachmentName;
    QString     m_strExtention;
    //
};

#endif // DLGSAVESCAN_H
