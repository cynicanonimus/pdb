#include "myeditor.h"
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
#include <QColor>
#include <QPrintDialog>
#include <QActionGroup>
//
#include "../CommonInclude/pdb/pdb_style.h"

MyEditor::MyEditor(QWidget *parent) :
    QTextEdit(parent)
{
    m_strLastLoadDir = tr("");
    //
    m_ptrUndo       = NULL;
    m_ptrRedo       = NULL;
    //
    m_ptrBold       = NULL;
    m_ptrUnderline  = NULL;
    m_ptrItalic     = NULL;
    m_ptrToolBar    = NULL;
    //
    m_grpAction             = NULL;
    m_ptrTextAlignLeft      = NULL;
    m_ptrTextAlignRight     = NULL;
    m_ptrTextAlignCenter    = NULL;
    m_ptrTextAlignJustify   = NULL;
    //
    m_ptrFontSize   = NULL;
    m_ptrFontType   = NULL;
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
    colorChanged(format.foreground().color());
}

void MyEditor::OnCursorPositionChanged()
{
    alignmentChanged(this->alignment());
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

void MyEditor::passUndoRedoAction (QAction* ptr_undo, QAction* ptr_redo)
{
    if ( ptr_undo && (NULL == m_ptrUndo))
    {
        m_ptrUndo = ptr_undo;
        connect(this->document(), SIGNAL(undoAvailable(bool)), m_ptrUndo, SLOT(setEnabled(bool)));
    };
    //
    if (ptr_redo && (NULL == m_ptrRedo))
    {
        m_ptrRedo  = ptr_redo;
        connect(this->document(), SIGNAL(redoAvailable(bool)), m_ptrRedo, SLOT(setEnabled(bool)));
    };
}

void MyEditor::addEditorToolBar (QToolBar* ptr_tool_bar)
{
    if ( (NULL == m_ptrToolBar) && (NULL != ptr_tool_bar))
    {
        m_ptrToolBar =  ptr_tool_bar;
        //
        m_ptrFontSize = new QComboBox(m_ptrToolBar);
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
        m_ptrFontType = new QFontComboBox(m_ptrToolBar);
        QObject::connect(m_ptrFontType, SIGNAL(activated(QString)), this, SLOT(onTextFamily(QString) ));
        //
        m_ptrToolBar->addWidget(m_ptrFontType);
        m_ptrToolBar->addWidget(m_ptrFontSize);
    };
}

void MyEditor::passBUIActions ( QAction* ptr_bold, QAction* ptr_underline, QAction* ptr_italic )
{
    if ( (NULL == m_ptrBold) && (NULL != ptr_bold) )
    {
        m_ptrBold = ptr_bold;
        //
        //m_ptrBold->setShortcut(Qt::CTRL + Qt::Key_B);
        m_ptrBold->setPriority(QAction::LowPriority);
        //
        QFont bold;
        bold.setBold(true);
        m_ptrBold->setFont(bold);
        QObject::connect(m_ptrBold, SIGNAL(triggered()), this, SLOT(onTextBold()));
        m_ptrBold->setCheckable(true);
    };
    //
    if ( (NULL == m_ptrUnderline) && (NULL != ptr_underline) )
    {
        m_ptrUnderline = ptr_underline;
        //
        //m_ptrUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
        m_ptrUnderline->setPriority(QAction::LowPriority);
        QFont underline;
        underline.setUnderline(true);
        m_ptrUnderline->setFont(underline);
        QObject::connect(m_ptrUnderline, SIGNAL(triggered()), this, SLOT(onTextUnderline()));
        m_ptrUnderline->setCheckable(true);
    };
    //
    if ( (NULL == m_ptrItalic) && (NULL != ptr_italic) )
    {
        m_ptrItalic = ptr_italic;
        //
        m_ptrItalic->setPriority(QAction::LowPriority);
     //m_ptrItalic->setShortcut(Qt::CTRL + Qt::Key_I);
        QFont italic;
        italic.setItalic(true);
        m_ptrItalic->setFont(italic);
        QObject::connect(m_ptrItalic, SIGNAL(triggered()), this, SLOT(onTextItalic()));
        m_ptrItalic->setCheckable(true);
    };
}

void MyEditor::onTextBold             ()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(m_ptrBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void MyEditor::onTextUnderline        ()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(m_ptrUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void MyEditor::onTextItalic           ()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(m_ptrItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
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
    bool b_has = this->textCursor().hasSelection();
    QString str_selected_text;
    //
    if (b_has)
        str_selected_text = this->textCursor().selectedText();
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
    if (m_ptrBold)
        m_ptrBold->setChecked(f.bold());
    //
    if (m_ptrUnderline)
        m_ptrUnderline->setChecked(f.underline());
    //
    if (m_ptrItalic)
        m_ptrItalic->setChecked(f.italic());
    //
}

void MyEditor::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    //actionTextColor->setIcon(pix);
}

void MyEditor::passAlignActions ( QAction* ptr_text_align_left, QAction* ptr_text_align_right, QAction* ptr_text_align_center, QAction* ptr_text_align_justify)
{
    if (NULL == m_grpAction )
    {
        m_grpAction = new QActionGroup(this);
         QObject::connect(m_grpAction, SIGNAL(triggered(QAction*)), this, SLOT(OnTextAlign(QAction*)));
    };
    //
    if ( (NULL == m_ptrTextAlignLeft) && (NULL != ptr_text_align_left) )
    {
        m_ptrTextAlignLeft = ptr_text_align_left;
        m_ptrTextAlignLeft->setCheckable(true);
        m_ptrTextAlignLeft->setPriority(QAction::LowPriority);
        m_grpAction->addAction(m_ptrTextAlignLeft);
    };
    //
    if ( (NULL == m_ptrTextAlignRight) && (NULL != ptr_text_align_right) )
    {
        m_ptrTextAlignRight = ptr_text_align_right;
        m_ptrTextAlignRight->setCheckable(true);
        m_ptrTextAlignRight->setPriority(QAction::LowPriority);
        m_grpAction->addAction(m_ptrTextAlignRight);
    };
    //
    if ( (NULL == m_ptrTextAlignCenter) && (NULL != ptr_text_align_center) )
    {
        m_ptrTextAlignCenter = ptr_text_align_center;
        m_ptrTextAlignCenter->setCheckable(true);
        m_ptrTextAlignCenter->setPriority(QAction::LowPriority);
        m_grpAction->addAction(m_ptrTextAlignCenter);
    };
    //
    if ( (NULL == m_ptrTextAlignJustify) && (NULL != ptr_text_align_justify) )
    {
        m_ptrTextAlignJustify = ptr_text_align_justify;
        m_ptrTextAlignJustify->setCheckable(true);
        m_ptrTextAlignJustify->setPriority(QAction::LowPriority);
        m_grpAction->addAction(m_ptrTextAlignJustify);
    };
}

void MyEditor::OnTextAlign(QAction* a)
{
    if (a == m_ptrTextAlignLeft)
        this->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == m_ptrTextAlignCenter)
        this->setAlignment(Qt::AlignHCenter);
    else if (a == m_ptrTextAlignRight)
        this->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == m_ptrTextAlignJustify)
        this->setAlignment(Qt::AlignJustify);
}

void MyEditor::alignmentChanged (Qt::Alignment a)
{
    if (a & Qt::AlignLeft)
    {
        m_ptrTextAlignLeft->setChecked(true);
    } else if (a & Qt::AlignHCenter)
    {
        m_ptrTextAlignCenter->setChecked(true);
    } else if (a & Qt::AlignRight)
    {
        m_ptrTextAlignRight->setChecked(true);
    } else if (a & Qt::AlignJustify)
    {
        m_ptrTextAlignJustify->setChecked(true);
    };
}
