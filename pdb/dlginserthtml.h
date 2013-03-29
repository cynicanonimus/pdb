#ifndef DLGINSERTHTML_H
#define DLGINSERTHTML_H

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
