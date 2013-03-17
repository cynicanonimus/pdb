#ifndef MYEDITOR_H
#define MYEDITOR_H
//
#include <QTextEdit>

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QToolBar)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QTextCharFormat)
QT_FORWARD_DECLARE_CLASS(QFont)
QT_FORWARD_DECLARE_CLASS(QFontComboBox)
QT_FORWARD_DECLARE_CLASS(QColor)

//
class MyEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyEditor(QWidget *parent = 0);

public:
    void    passUndoRedoAction     (QAction* ptr_undo, QAction* ptr_redo);
    void    addEditorToolBar       (QToolBar* ptr_tool_bar);
    
signals:
    void    textExist              (bool);

public slots:
    void    onFilePrintPreview     ();
    void    onExportToFile         ();
    void    onLoadFromFile         ();

private slots:
    void    onTextFamily           (const QString &f);
    void    onPrintPreview         (QPrinter *);
    void    onTextChanged          ();
    void    onTextSize             (const QString&);
    void    onCurrentCharFormatChanged (const QTextCharFormat &format);
    void    OnCursorPositionChanged();


private:
    void    mergeFormatOnWordOrSelection (const QTextCharFormat &format);
    void    fileSave              (const QString& f_name);
    void    loadFile              (const QString &f);
    void    fontChanged           (const QFont &f);
    void    colorChanged          (const QColor &c);

private:
    QString     m_strLastExportDir;
    QString     m_strLastLoadDir;
    //
    QAction*    m_ptrUndo;
    QAction*    m_ptrRedo;
    QToolBar*   m_ptrToolBar;
    //
    QFontComboBox*  m_ptrFontType;
    QComboBox*  m_ptrFontSize;
};

#endif // MYEDITOR_H
