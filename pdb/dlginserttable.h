#ifndef DLGINSERTTABLE_H
#define DLGINSERTTABLE_H

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
