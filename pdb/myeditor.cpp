#include "myeditor.h"
#include <QPrinter>
#include <QPrintPreviewDialog>

MyEditor::MyEditor(QWidget *parent) :
    QTextEdit(parent)
{
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
