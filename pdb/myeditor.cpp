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
//
#include "../CommonInclude/pdb/pdb_style.h"

MyEditor::MyEditor(QWidget *parent) :
    QTextEdit(parent)
{
    m_strLastLoadDir = tr("");
    //
    m_ptrUndo   = NULL;
    m_ptrRedo   = NULL;
    m_ptrToolBar= NULL;
    //
    m_ptrFontSize= NULL;
    m_ptrFontType = NULL;
    //
    //QObject::connect( ui->m_textEditor,                 SIGNAL(textChanged() ), m_pMainMenu,SLOT (onEditorTextChanged() ));
    QObject::connect( this, SIGNAL(textChanged()),                              this, SLOT(onTextChanged()                              ));
    //
    QObject::connect( this, SIGNAL(currentCharFormatChanged(QTextCharFormat)),  this, SLOT(onCurrentCharFormatChanged(QTextCharFormat)  ));
    QObject::connect( this, SIGNAL(cursorPositionChanged()),                    this, SLOT(OnCursorPositionChanged()                    ));
}

void MyEditor::onCurrentCharFormatChanged (const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void MyEditor::OnCursorPositionChanged()
{
    //alignmentChanged(this->alignment());
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

void MyEditor::onFilePrintPreview()
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

void MyEditor::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor();
    //
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    //
    cursor.mergeCharFormat(format);
    this->mergeCurrentCharFormat(format);
}

void MyEditor::fontChanged(const QFont &f)
{
    if (m_ptrFontType)
        m_ptrFontType->setCurrentIndex(m_ptrFontType->findText(QFontInfo(f).family()));
    //
    if (m_ptrFontSize)
        m_ptrFontSize->setCurrentIndex(m_ptrFontSize->findText(QString::number(f.pointSize())));
    //
    /*
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
    */
}

void MyEditor::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    //actionTextColor->setIcon(pix);
}
