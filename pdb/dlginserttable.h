#ifndef DLGINSERTTABLE_H
#define DLGINSERTTABLE_H
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
#include <QTextTableFormat>
#include <QCloseEvent>
//
namespace Ui {
class DlgInsertTable;
}

class DlgInsertTable : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgInsertTable(QWidget *parent = 0);
    ~DlgInsertTable();
    //
    const QTextTableFormat&         getTableFormat  () const { return m_TableFormat;}
    unsigned int                    columns         () const { return m_uiColumnsNumber;}
    unsigned int                    rows            () const { return m_uiRowsNumber;}
    //
    void                            setTableFormat  (const QTextTableFormat& tbl_format);

private:
    QTextFrameFormat::BorderStyle   getBorderStyle() const;

private:
    Ui::DlgInsertTable* ui;
    //
    QTextTableFormat    m_TableFormat;
    unsigned int        m_uiColumnsNumber;
    unsigned int        m_uiRowsNumber;
    QColor              m_cFrameColor;

private slots:

    void onOK                   ();
    void onChangeFrameColor     ();
};

#endif // DLGINSERTTABLE_H
