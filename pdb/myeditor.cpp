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
#include "myeditor.h"
#include "dlginserttable.h"
#include "dlginserthtml.h"
//
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QSettings>
#include <QMessageBox>
#include <QTextCodec>
#include <QAction>
#include <QToolBar>
#include <QComboBox>
#include <QApplication>
#include <QFontDatabase>
#include <QTextCharFormat>
#include <QFont>
#include <QFontComboBox>
#include <QColorDialog>
#include <QColor>
#include <QPrintDialog>
#include <QActionGroup>
//
#include <QTextTable>
#include <QTextTableFormat>
#include <QTextFrameFormat>
#include <QInputDialog>

//

#include "mnumainwindow.h"
#include "../CommonInclude/pdb/pdb_style.h"

MyEditor::MyEditor(QWidget *parent) :
    QTextEdit(parent)
{
    m_strLastLoadDir = tr("");
    //
    m_ptrMnuObject  = NULL;
    m_grpAction     = NULL;
    //
    //QObject::connect( ui->m_textEditor,                 SIGNAL(textChanged() ), m_pMainMenu,SLOT (onEditorTextChanged() ));
    QObject::connect( this, SIGNAL(textChanged()),                              this, SLOT(onTextChanged()                              ));
    //
    QObject::connect( this, SIGNAL(currentCharFormatChanged(QTextCharFormat)),  this, SLOT(onCurrentCharFormatChanged(QTextCharFormat)  ));
    QObject::connect( this, SIGNAL(cursorPositionChanged()),                    this, SLOT(OnCursorPositionChanged()                    ));
    QObject::connect( this, SIGNAL(selectionChanged()),                         this, SLOT(onSelectionChanged()                         ));
}

MyEditor::~MyEditor()
{
    if ( m_grpAction )
        delete m_grpAction;
}

void MyEditor::passMnuItems ( MnuMainWindow* ptr_mnu_window )
{
    if ( NULL != m_ptrMnuObject )
        return;
    //
    m_ptrMnuObject = ptr_mnu_window;
    //---------------------------------------------------------------------------------------------------------------------
    QObject::connect(this->document(), SIGNAL(undoAvailable(bool)), m_ptrMnuObject->m_ptrUndo, SLOT(setEnabled(bool)));
    QObject::connect(this->document(), SIGNAL(redoAvailable(bool)), m_ptrMnuObject->m_ptrRedo, SLOT(setEnabled(bool)));
    //---------------------------------------------------------------------------------------------------------------------
    QPixmap pix_black(16, 16);
    pix_black.fill(Qt::black);
    m_ptrMnuObject->m_ptrChangeTextColor->setIcon(pix_black);
    QObject::connect(m_ptrMnuObject->m_ptrChangeTextColor, SIGNAL(triggered()), this, SLOT(onTextColor()));
    //
    QPixmap pix_white(16, 16);
    pix_white.fill(Qt::white);
    m_ptrMnuObject->m_ptrChangeBackgroundColor->setIcon(pix_white);
    QObject::connect(m_ptrMnuObject->m_ptrChangeBackgroundColor, SIGNAL(triggered()), this, SLOT(onBackColor()));
    //
    m_ptrFontSize = new QComboBox(m_ptrMnuObject->m_ptrEditorToolBar);
    m_ptrFontSize->setEditable(true);
    QFontDatabase db;
    foreach(int size, db.standardSizes())
    {
        m_ptrFontSize->addItem(QString::number(size));
    };
    //
    QObject::connect(m_ptrFontSize, SIGNAL(activated(QString)),this, SLOT(onTextSize(QString)));
    m_ptrFontSize->setCurrentIndex(m_ptrFontSize->findText(QString::number(QApplication::font().pointSize())));
    //
    m_ptrFontType = new QFontComboBox(m_ptrMnuObject->m_ptrEditorToolBar);
    QObject::connect(m_ptrFontType, SIGNAL(activated(QString)), this, SLOT(onTextFamily(QString) ));
    //
    m_ptrMnuObject->m_ptrEditorToolBar->addWidget(m_ptrFontType);
    m_ptrMnuObject->m_ptrEditorToolBar->addSeparator();
    m_ptrMnuObject->m_ptrEditorToolBar->addWidget(m_ptrFontSize);
    //
    m_ptrMnuObject->m_ptrEditorToolBar->addSeparator();
    m_ptrMnuObject->m_ptrEditorToolBar->addAction(m_ptrMnuObject->m_ptrChangeTextColor);
    m_ptrMnuObject->m_ptrEditorToolBar->addSeparator();
    m_ptrMnuObject->m_ptrEditorToolBar->addAction(m_ptrMnuObject->m_ptrChangeBackgroundColor);
    //---------------------------------------------------------------------------------------------------------------------
    m_ptrMnuObject->m_ptrBold->setPriority(QAction::LowPriority);
    //
    QFont bold;
    bold.setBold(true);
    m_ptrMnuObject->m_ptrBold->setFont(bold);
    QObject::connect(m_ptrMnuObject->m_ptrBold, SIGNAL(triggered()), this, SLOT(onTextBold()));
    m_ptrMnuObject->m_ptrBold->setCheckable(true);
    //m_ptrUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    m_ptrMnuObject->m_ptrUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    m_ptrMnuObject->m_ptrUnderline->setFont(underline);
    QObject::connect(m_ptrMnuObject->m_ptrUnderline, SIGNAL(triggered()), this, SLOT(onTextUnderline()));
    m_ptrMnuObject->m_ptrUnderline->setCheckable(true);
    //
    m_ptrMnuObject->m_ptrItalic->setPriority(QAction::LowPriority);
    //m_ptrItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    m_ptrMnuObject->m_ptrItalic->setFont(italic);
    QObject::connect(m_ptrMnuObject->m_ptrItalic, SIGNAL(triggered()), this, SLOT(onTextItalic()));
    m_ptrMnuObject->m_ptrItalic->setCheckable(true);
    //---------------------------------------------------------------------------------------------------------------------
    if (NULL == m_grpAction )
    {
        m_grpAction = new QActionGroup(this);
         QObject::connect(m_grpAction, SIGNAL(triggered(QAction*)), this, SLOT(OnTextAlign(QAction*)));
    };
    //
    m_ptrMnuObject->m_ptrTextAlignLeft->setCheckable(true);
    m_ptrMnuObject->m_ptrTextAlignLeft->setPriority(QAction::LowPriority);
    m_grpAction->addAction(m_ptrMnuObject->m_ptrTextAlignLeft);
    //
    m_ptrMnuObject->m_ptrTextAlignRight->setCheckable(true);
    m_ptrMnuObject->m_ptrTextAlignRight->setPriority(QAction::LowPriority);
    m_grpAction->addAction(    m_ptrMnuObject->m_ptrTextAlignRight);
    //
    m_ptrMnuObject->m_ptrTextAlignCenter->setCheckable(true);
    m_ptrMnuObject->m_ptrTextAlignCenter->setPriority(QAction::LowPriority);
    m_grpAction->addAction(m_ptrMnuObject->m_ptrTextAlignCenter);
    //
    m_ptrMnuObject->m_ptrTextAlignJustify->setCheckable(true);
    m_ptrMnuObject->m_ptrTextAlignJustify->setPriority(QAction::LowPriority);
    m_grpAction->addAction(m_ptrMnuObject->m_ptrTextAlignJustify);
    //
    QObject::connect(m_ptrMnuObject->m_ptrSentenceCase, SIGNAL(triggered()), this, SLOT(onTextSentenceCase()    ));
    QObject::connect(m_ptrMnuObject->m_ptrUpperCase,    SIGNAL(triggered()), this, SLOT(onTextUpperCase()       ));
    QObject::connect(m_ptrMnuObject->m_ptrLowerCase,    SIGNAL(triggered()), this, SLOT(onTextLowerCase()       ));
    QObject::connect(m_ptrMnuObject->m_ptrToggleCase,   SIGNAL(triggered()), this, SLOT(onTextToggleCase()      ));
    //
    QObject::connect(m_ptrMnuObject->m_ptrInsertTable,  SIGNAL(triggered()), this, SLOT(onInsertTable()         ));
    //
    QObject::connect(m_ptrMnuObject->m_ptrInsertImageLink,  SIGNAL(triggered()), this, SLOT(onInsertImageLink() ));
    QObject::connect(m_ptrMnuObject->m_ptrInsertHTML,   SIGNAL(triggered()), this, SLOT(onInsertHTML    ()      ));
    //
    QObject::connect(m_ptrMnuObject->m_ptrInsertRow,    SIGNAL(triggered()), this, SLOT(onAddRow        ()      ));
    QObject::connect(m_ptrMnuObject->m_ptrInsertColumn, SIGNAL(triggered()), this, SLOT(onAddColumn     ()      ));
    QObject::connect(m_ptrMnuObject->m_ptrRemoveRow,    SIGNAL(triggered()), this, SLOT(onRemoveRow     ()      ));
    QObject::connect(m_ptrMnuObject->m_ptrRemoveColumn, SIGNAL(triggered()), this, SLOT(onRemoveColumn  ()      ));
    QObject::connect(m_ptrMnuObject->m_ptrCellsMerge,   SIGNAL(triggered()), this, SLOT(onMergeCells    ()      ));
    QObject::connect(m_ptrMnuObject->m_ptrCellSplit,    SIGNAL(triggered()), this, SLOT(onCellSplit     ()      ));
    QObject::connect(m_ptrMnuObject->m_ptrTableSettings, SIGNAL(triggered()), this, SLOT(onTableSettings()      ));
}
/*
void MyEditor::onZoomIn ()
{
    //this->setReadOnly(true);
    //this->is
}
*/

void MyEditor::onInsertHTML ()
{
    DlgInsertHTML dlg;
    //
    if (dlg.exec() == QDialog::Accepted)
    {
        QString str_html = dlg.getHTMLCode();
        QTextCursor cursor = this->textCursor();
        cursor.insertHtml(str_html);
    };
}

void MyEditor::onInsertImageLink()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              m_strLastLoadDir,
                                              "JPG (*.jpg *.jpeg);;BMP (*.bmp);;PNG (*.png);;All Files (*)");
    if (fn.isEmpty())
        return;
    //
    //<img src="/home/alex/1.png">
    //
    QString str_img = "<img src=\"";
    str_img += fn;
    str_img += "\">";
    //
    QTextCursor cursor = this->textCursor();
    cursor.insertHtml(str_img);
}

void MyEditor::onTableSettings ()
{
    QTextTable* ptr_tbl = this->textCursor().currentTable();
    if (NULL == ptr_tbl)
        return;
    //
    QTextTableFormat table_fmt =  ptr_tbl->format();
    //
    DlgInsertTable dlg;
    //
    dlg.setTableFormat(table_fmt);
    //
    QDialog::DialogCode en_code = (QDialog::DialogCode) dlg.exec();
    if ( QDialog::Rejected == en_code )
        return;
    //
    QTextTableFormat table_fmt_changed = dlg.getTableFormat();
    //
    ptr_tbl->setFormat(table_fmt_changed);
    //
    return;
}

void MyEditor::onCellSplit ()
{
    QTextCursor cursor = this->textCursor();
    QTextTable* ptr_tbl = cursor.currentTable();
    //
    if (ptr_tbl)
    {
        QTextTableCell cell = ptr_tbl->cellAt(cursor);
        ptr_tbl->splitCell( cell.row(), cell.column(), 1,1);//mergeCells(cursor);
    };
}

void MyEditor::onMergeCells ()
{
    QTextCursor cursor = this->textCursor();
    QTextTable* ptr_tbl = cursor.currentTable();
    //
    if (ptr_tbl)
    {
        ptr_tbl->mergeCells(cursor);
    };
}

void MyEditor::onInsertTable ()
{
    DlgInsertTable dlg;

    QDialog::DialogCode en_code = (QDialog::DialogCode) dlg.exec();
    if ( QDialog::Rejected == en_code )
        return;
    //
    const QTextTableFormat& table_fmt = dlg.getTableFormat();
    //
    this->textCursor().insertTable(dlg.rows(), dlg.columns(), table_fmt);
    //
}

void MyEditor::onAddRow ()
{
    QTextTable* ptr_tbl = this->textCursor().currentTable();
    if (NULL == ptr_tbl)
        return;
    //

    //
    bool ok = false;
    QTextCursor cursor = this->textCursor();
    QTextTableCell cell = ptr_tbl->cellAt(cursor);
    int cell_rowcursor_is = cell.row(); /* int value start from zero */
    //
    int approwtot = QInputDialog::getInteger(0, tr("Append NR. line row"),tr("Row:"), 1, 1, 100, 1, &ok);
    //
    ptr_tbl->insertRows(cell_rowcursor_is+1, approwtot);
}

void MyEditor::onAddColumn ()
{
    QTextTable* ptr_tbl = this->textCursor().currentTable();
    if (NULL == ptr_tbl)
        return;
    //
    bool ok = false;
    QTextCursor cursor = this->textCursor();
    QTextTableCell cell = ptr_tbl->cellAt(cursor);
    int cell_colcursor_is = cell.column(); /* int value start from zero */
    //
    int approwtot = QInputDialog::getInteger(0, tr("Append NR. col row"),tr("Col:"), 1, 1, 100, 1, &ok);
    //
    ptr_tbl->insertColumns(cell_colcursor_is+1, approwtot);
}

void MyEditor::onRemoveRow ()
{
    QTextTable* ptr_tbl = this->textCursor().currentTable();
    if (NULL == ptr_tbl)
        return;
    //
    QTextCursor cursor = this->textCursor();
    QTextTableCell cell = ptr_tbl->cellAt(cursor);
    int cell_rowcursor_is = cell.row(); /* int value start from zero */
    //
    ptr_tbl->removeRows(cell_rowcursor_is, 1);
}

void MyEditor::onRemoveColumn ()
{
    QTextTable* ptr_tbl = this->textCursor().currentTable();
    if (NULL == ptr_tbl)
        return;
    //
    QTextCursor cursor = this->textCursor();
    QTextTableCell cell = ptr_tbl->cellAt(cursor);
    int cell_colcursor_is = cell.column(); /* int value start from zero */
    //
    ptr_tbl->removeColumns(cell_colcursor_is, 1);
}

void MyEditor::OnPrint ()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    //
    if (this->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    //
    dlg->setWindowTitle(tr("Print Document"));
    //
    if (dlg->exec() == QDialog::Accepted)
    {
        this->print(&printer);
    }
    delete dlg;
#endif
}

void MyEditor::OnExportToPDF ()
{
#ifndef QT_NO_PRINTER
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    if( m_strLastExportDir.length() == 0 )
    {
        m_strLastExportDir = settings.value(g_str_ATTACH_EXPORT_PATH).toString();
    };
    //
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",
                                                    m_strLastExportDir,
                                                    "*.pdf");
    if (!fileName.isEmpty())
    {
        if (QFileInfo(fileName).suffix().isEmpty())
            fileName.append(".pdf");
        //
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        this->document()->print(&printer);
    }
#endif
}

void MyEditor::onCurrentCharFormatChanged (const QTextCharFormat &format)
{
    fontChanged(format.font());
    onTextColorChanged(format.foreground().color());
    onBackColorChanged(format.background().color());
}

void MyEditor::OnCursorPositionChanged()
{
    alignmentChanged(this->alignment());
    //
    QTextCursor cursor = this->textCursor();
    QTextTable* ptr_tbl = cursor.currentTable();
    if (NULL != ptr_tbl)
    {
        m_ptrMnuObject->m_ptrInsertRow->setEnabled(true);
        m_ptrMnuObject->m_ptrInsertColumn->setEnabled(true);
        m_ptrMnuObject->m_ptrTableSettings->setEnabled(true);
        //
        int i_firstRow     = 0;
        int i_numRows      = 0;
        int i_firstColumn  = 0;
        int i_numColumns   = 0;
        //
        this->textCursor().selectedTableCells(&i_firstRow,&i_numRows,&i_firstColumn,&i_numColumns);
        //
        if ( (i_numRows > 0) || (i_numColumns > 0) )
        {
            m_ptrMnuObject->m_ptrCellsMerge->setEnabled(true);
            m_ptrMnuObject->m_ptrCellSplit->setEnabled(false);
        }
        else
        {
            m_ptrMnuObject->m_ptrCellsMerge->setEnabled(false);
            QTextTableCell cell = ptr_tbl->cellAt(cursor);
            if ( (cell.columnSpan() > 1) || (cell.rowSpan() > 1) )
                m_ptrMnuObject->m_ptrCellSplit->setEnabled(true);
            else
                m_ptrMnuObject->m_ptrCellSplit->setEnabled(false);
        };
        //
        if ( ptr_tbl->rows() > 1 )
            m_ptrMnuObject->m_ptrRemoveRow->setEnabled(true);
        else
            m_ptrMnuObject->m_ptrRemoveRow->setEnabled(false);
        //
        if ( ptr_tbl->columns() > 1 )
            m_ptrMnuObject->m_ptrRemoveColumn->setEnabled(true);
        else
            m_ptrMnuObject->m_ptrRemoveColumn->setEnabled(false);
    }else
    {
        m_ptrMnuObject->m_ptrInsertRow->setEnabled(false);
        m_ptrMnuObject->m_ptrInsertColumn->setEnabled(false);
        m_ptrMnuObject->m_ptrTableSettings->setEnabled(false);
        //
        m_ptrMnuObject->m_ptrRemoveRow->setEnabled(false);
        m_ptrMnuObject->m_ptrRemoveColumn->setEnabled(false);
        //
        m_ptrMnuObject->m_ptrCellsMerge->setEnabled(false);
        //
        m_ptrMnuObject->m_ptrCellSplit->setEnabled(false);
    }
    return;
}

void MyEditor::onTextChanged ()
{
    if ( this->document()->toPlainText().length() > 0 )
        emit textExist(true);
    else
        emit textExist(false);
}

void MyEditor::onLoadFromFile ()
{
    bool b_continue = true;
    //
    if ( this->document()->toPlainText().length() > 0 )
    {
        const int i_res = QMessageBox::warning(NULL,
                                         trUtf8("WARNING!") ,
                                         "Do you want to overwrite existing document?",
                                         QMessageBox::Yes|QMessageBox::No,
                                         QMessageBox::No
                                        );
        if (QMessageBox::No == i_res)
            b_continue = false;
    };
    //
    if (false == b_continue)
        return;
    //
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              m_strLastLoadDir,
                                              tr("Txt-Files (*.txt);;HTML-Files (*.htm *.html);;All Files (*)"));
    if (fn.isEmpty())
        return;
    //
    QFileInfo pathInfo( fn );
    m_strLastLoadDir = pathInfo.absolutePath();
    //
    loadFile(fn);
}

void MyEditor::onPrintPreview()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(onPrintPreview(QPrinter*)));
    preview.exec();
}

void MyEditor::onPrintPreview(QPrinter* printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    this->print(printer);
#endif
}

void MyEditor::onTextFamily (const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
}

void MyEditor::onExportToFile ()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    if( m_strLastExportDir.length() == 0 )
    {
        m_strLastExportDir = settings.value(g_str_ATTACH_EXPORT_PATH).toString();
    };
    //
    QString fn = QFileDialog::getSaveFileName(this,
                                              tr("Save as..."),
                                              m_strLastExportDir,
                                              tr("ODF files (*.odt);;Txt files (*.txt);;HTML-Files (*.htm *.html);;All Files (*)"));
    if (fn.isEmpty())
        return;
    //
    QFileInfo pathInfo( fn );
    m_strLastExportDir = pathInfo.absolutePath();
    //
    if (! (fn.endsWith(".odt", Qt::CaseInsensitive) ||
           fn.endsWith(".htm", Qt::CaseInsensitive) ||
           fn.endsWith(".html",Qt::CaseInsensitive) ||
           fn.endsWith(".txt", Qt::CaseInsensitive) )
       )
        fn += ".odt"; // default
    //
    fileSave(fn);
}

void MyEditor::fileSave(const QString& f_name)
{
    QTextDocumentWriter writer(f_name);
    writer.write(this->document());
}

void MyEditor::loadFile(const QString &f)
{
    if (!QFile::exists(f))
        return;
    //
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return;
    //
    QByteArray data = file.readAll();
    //
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str))
    {
        this->setHtml(str);
    } else
    {
        str = QString::fromLocal8Bit(data);
        this->setPlainText(str);
    }
    //
    return;
}

void MyEditor::onTextSize (const QString &p)
{
    qreal pointSize = p.toFloat();
    //
    if (p.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    };
}

void MyEditor::onBackColor ()
{
    QColor col = QColorDialog::getColor(this-> textBackgroundColor(), this);
    if (!col.isValid())
        return;
    //
    QTextCharFormat fmt;
    fmt.setBackground(col);
    mergeFormatOnWordOrSelection(fmt);
    onBackColorChanged(col);
}

void MyEditor::onTextColor()
{
    QColor col = QColorDialog::getColor(this->textColor(), this);
    if (!col.isValid())
        return;
    //
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    onTextColorChanged(col);
}

void MyEditor::onTextBold             ()
{
    if (m_ptrMnuObject)
    {
        QTextCharFormat fmt;
        fmt.setFontWeight(m_ptrMnuObject->m_ptrBold->isChecked() ? QFont::Bold : QFont::Normal);
        mergeFormatOnWordOrSelection(fmt);
    };
}

void MyEditor::onTextUnderline        ()
{
    if (m_ptrMnuObject)
    {
        QTextCharFormat fmt;
        fmt.setFontUnderline(m_ptrMnuObject->m_ptrUnderline->isChecked());
        mergeFormatOnWordOrSelection(fmt);
    };
}

void MyEditor::onTextItalic           ()
{
    if (m_ptrMnuObject)
    {
        QTextCharFormat fmt;
        fmt.setFontItalic(m_ptrMnuObject->m_ptrItalic->isChecked());
        mergeFormatOnWordOrSelection(fmt);
    };
}

void MyEditor::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor();
    //
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    //
    cursor.mergeCharFormat(format);
    this->mergeCurrentCharFormat(format);
    //
}

void MyEditor::onSelectionChanged()
{
    if (NULL == m_ptrMnuObject)
        return;
    //
    bool b_has = this->textCursor().hasSelection();
    //
    m_ptrMnuObject->m_ptrSentenceCase->setEnabled(b_has);
    //
    m_ptrMnuObject->m_ptrUpperCase->setEnabled(b_has);
    //
    m_ptrMnuObject->m_ptrLowerCase->setEnabled(b_has);
    //
    m_ptrMnuObject->m_ptrToggleCase->setEnabled(b_has);
    //
    return;
}

void MyEditor::fontChanged(const QFont &f)
{
    if (m_ptrFontType)
        m_ptrFontType->setCurrentIndex(m_ptrFontType->findText(QFontInfo(f).family()));
    //
    if (m_ptrFontSize)
        m_ptrFontSize->setCurrentIndex(m_ptrFontSize->findText(QString::number(f.pointSize())));
    //
    if (NULL == m_ptrMnuObject)
        return;
    //
    m_ptrMnuObject->m_ptrBold->setChecked(f.bold());
    //
    m_ptrMnuObject->m_ptrUnderline->setChecked(f.underline());
    //
    m_ptrMnuObject->m_ptrItalic->setChecked(f.italic());
    //
}

void MyEditor::onBackColorChanged (const QColor &c)
{
    if (NULL == m_ptrMnuObject)
        return;
    //
    QPixmap pix(48, 48);
    pix.fill(c);
    m_ptrMnuObject->m_ptrChangeBackgroundColor->setIcon(pix);
}

void MyEditor::onTextColorChanged(const QColor &c)
{
    if (NULL == m_ptrMnuObject)
        return;
    //
    QPixmap pix(48, 48);
    pix.fill(c);
    m_ptrMnuObject->m_ptrChangeTextColor->setIcon(pix);
}

void MyEditor::onTextSentenceCase ()
{
    if (false == this->textCursor().hasSelection() )
        return;
    //
    QString str_selected_text = this->textCursor().selectedText();
    str_selected_text = str_selected_text.toLower();
    str_selected_text[0] = str_selected_text[0].toUpper();
    //
    QTextCharFormat fmt;
    prepareTextFormatting(fmt);
    //
    this->textCursor().insertText(str_selected_text, fmt);
    //
    mergeFormatOnWordOrSelection(fmt);
}

void MyEditor::onTextToggleCase ()
{
    if (false == this->textCursor().hasSelection() )
        return;
    //
    QString str_selected_text = this->textCursor().selectedText();
    //
    for (int i = 0; i < str_selected_text.length(); ++i)
    {
        if (str_selected_text[i].isUpper())
            str_selected_text[i] = str_selected_text[i].toLower();
        else if(str_selected_text[i].isLower())
            str_selected_text[i] = str_selected_text[i].toUpper();
    };
    //
    QTextCharFormat fmt;
    prepareTextFormatting(fmt);
    //
    this->textCursor().insertText(str_selected_text, fmt);
    //
    mergeFormatOnWordOrSelection(fmt);
}

void MyEditor::onTextLowerCase ()
{
    if (false == this->textCursor().hasSelection() )
        return;
    //
    QString str_selected_text = this->textCursor().selectedText();
    QTextCharFormat fmt;
    prepareTextFormatting(fmt);
    //
    this->textCursor().insertText(str_selected_text.toLower(), fmt);
    //
    mergeFormatOnWordOrSelection(fmt);
}

void MyEditor::onTextUpperCase ()
{
    if (false == this->textCursor().hasSelection() )
        return;
    //
    QString str_selected_text = this->textCursor().selectedText();
    QTextCharFormat fmt;
    prepareTextFormatting(fmt);
    //
    this->textCursor().insertText(str_selected_text.toUpper(), fmt);
    //
    mergeFormatOnWordOrSelection(fmt);
}

void MyEditor::prepareTextFormatting (QTextCharFormat& fmt)
{
    if (NULL == m_ptrMnuObject)
        return;
    //
    fmt = this->textCursor().blockCharFormat();
    //
    fmt.setFontWeight   ( m_ptrMnuObject->m_ptrBold->isChecked() ? QFont::Bold : QFont::Normal);
    fmt.setFontUnderline( m_ptrMnuObject->m_ptrUnderline->isChecked()    );
    fmt.setFontItalic   ( m_ptrMnuObject->m_ptrItalic->isChecked()       );
}

void MyEditor::OnTextAlign(QAction* a)
{
    if (NULL == m_ptrMnuObject)
        return;
    //
    if (a == m_ptrMnuObject->m_ptrTextAlignLeft)
        this->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == m_ptrMnuObject->m_ptrTextAlignCenter)
        this->setAlignment(Qt::AlignHCenter);
    else if (a == m_ptrMnuObject->m_ptrTextAlignRight)
        this->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == m_ptrMnuObject->m_ptrTextAlignJustify)
        this->setAlignment(Qt::AlignJustify);
}

void MyEditor::alignmentChanged (Qt::Alignment a)
{
    if (NULL == m_ptrMnuObject)
        return;
    //
    if (a & Qt::AlignLeft)
    {
        m_ptrMnuObject->m_ptrTextAlignLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter)
    {
       m_ptrMnuObject->m_ptrTextAlignCenter->setChecked(true);
    } else if (a & Qt::AlignRight)
    {
        m_ptrMnuObject->m_ptrTextAlignRight->setChecked(true);
    } else if (a & Qt::AlignJustify)
    {
        m_ptrMnuObject->m_ptrTextAlignJustify->setChecked(true);
    };
}
