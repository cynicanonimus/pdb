#include "dlginserthtml.h"
#include "ui_dlginserthtml.h"
#include "htmlsyntaxhighlighter.h"

DlgInsertHTML::DlgInsertHTML(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgInsertHTML)
{
    ui->setupUi(this);
    //
    m_ptrHighLighter = new HTMLSyntaxHighlighter(ui->HTMLEdit->document());
    //
    QObject::connect(ui->btnHREF,       SIGNAL(clicked()), this, SLOT(onInsertHREF    ()  ));
    QObject::connect(ui->btnTABLE,      SIGNAL(clicked()), this, SLOT(onInsertTABLE   ()  ));
    QObject::connect(ui->btnLIST,       SIGNAL(clicked()), this, SLOT(onInsertLIST    ()  ));
    QObject::connect(ui->btnNUMLIST,    SIGNAL(clicked()), this, SLOT(onInsertNumLIST ()  ));
}

DlgInsertHTML::~DlgInsertHTML()
{
    delete ui;
    if (m_ptrHighLighter)
        delete m_ptrHighLighter;

}

void DlgInsertHTML::onInsertHREF()
{
//  <a href="http://url">Link text</a>
    QString str_href = "<a href=\"http://";
    str_href += "\"> </a>";
    //
    ui->HTMLEdit->textCursor().insertText(str_href);
}

void DlgInsertHTML::onInsertTABLE()
{
/*
<table border="1">
<tr>
    <td></td>
</tr>
</table>
*/
    QString str_table = "<table border=\"1\">\n";
    str_table += "<tr>\n";
    str_table += "   <td></td>\n";
    str_table += "</tr>\n";
    str_table += "</table>\n";
    //
    ui->HTMLEdit->textCursor().insertText(str_table);
}

void DlgInsertHTML::onInsertLIST()
{
/*
    <ul>
        <li> </li>
        <li> </li>
    </ul>

*/
    QString str_list = "<ul>\n";
    str_list += "<li> </li>\n";
    str_list += "<li> </li>\n";
    str_list += "</ul>\n\n";
    //
    //
    ui->HTMLEdit->textCursor().insertText(str_list);
}

void DlgInsertHTML::onInsertNumLIST    ()
{
/*
    <ol>
       <li> </li>
       <li> </li>
    </ol>
*/
    QString str_list = "<ol>\n";
    str_list += "<li> </li>\n";
    str_list += "<li> </li>\n";
    str_list += "</ol>\n\n";
    //
    ui->HTMLEdit->textCursor().insertText(str_list);
}


const QString&  DlgInsertHTML::getHTMLCode()
{
    m_strHTMLCode = ui->HTMLEdit->document()->toPlainText();
    return m_strHTMLCode;
}

void DlgInsertHTML::showEvent (QShowEvent* e)
{
    QDialog::showEvent(e);
    //
    ui->HTMLEdit->setFocus();
}
