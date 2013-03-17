#include "myeditor.h"
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QFileDialog>
#include <QTextDocumentWriter>
#include <QSettings>
#include <QMessageBox>
#include <QTextCodec>
#include <QAction>
//
#include "../CommonInclude/pdb/pdb_style.h"

MyEditor::MyEditor(QWidget *parent) :
    QTextEdit(parent)
{
    m_strLastLoadDir = tr("");
    //
    m_ptrUndo   = NULL;
    m_ptrRedo   = NULL;
    //
    //QObject::connect( ui->m_textEditor,                 SIGNAL(textChanged() ), m_pMainMenu,SLOT (onEditorTextChanged() ));
    QObject::connect( this,                 SIGNAL(textChanged()), this, SLOT(onTextChanged() ));
}

void  MyEditor::onTextChanged ()
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
