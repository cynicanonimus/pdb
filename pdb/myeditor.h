#ifndef MYEDITOR_H
#define MYEDITOR_H
//
#include <QTextEdit>

QT_FORWARD_DECLARE_CLASS(QAction)


//
class MyEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyEditor(QWidget *parent = 0);

public:
    void    passUndoRedoAction     (QAction* ptr_undo, QAction* ptr_redo);
    
signals:
    void    textExist              (bool);

public slots:
    void    onFilePrintPreview     ();
    void    onExportToFile         ();
    void    onLoadFromFile         ();

private slots:
    void    onPrintPreview         (QPrinter *);
    void    onTextChanged          ();

private:
    void    fileSave              (const QString& f_name);
    void    loadFile              (const QString &f);

private:
    QString     m_strLastExportDir;
    QString     m_strLastLoadDir;
    QAction*    m_ptrUndo;
    QAction*    m_ptrRedo;
};

#endif // MYEDITOR_H
