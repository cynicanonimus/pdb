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
QT_FORWARD_DECLARE_CLASS(QActionGroup)

//
class MyEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyEditor(QWidget *parent = 0);
    ~MyEditor();

public:
    void    passUndoRedoAction     ( QAction* ptr_undo, QAction* ptr_redo );
    //Bold,     Underline,     TextItalic
    void    passBUIActions         ( QAction* ptr_bold, QAction* ptr_underline, QAction* ptr_italic );
    void    passAlignActions       ( QAction* ptr_text_align_left, QAction* ptr_text_align_right, QAction* ptr_text_align_center, QAction* ptr_text_align_justify);
    void    addEditorToolBar       ( QToolBar* ptr_tool_bar);

signals:
    void    textExist              (bool);

public slots:
    void    OnPrint                ();
    void    onPrintPreview         ();
    void    onExportToFile         ();
    void    onLoadFromFile         ();
    void    OnExportToPDF          ();

private slots:
    void    onTextFamily           (const QString &f);
    void    onPrintPreview         (QPrinter *);
    void    onTextChanged          ();
    void    onTextSize             (const QString&);
    void    onCurrentCharFormatChanged (const QTextCharFormat &format);
    void    OnCursorPositionChanged();
    void    onTextBold             ();
    void    onTextUnderline        ();
    void    onTextItalic           ();
    void    OnTextAlign            (QAction* a);
    void    onSelectionChanged     ();


private:
    void    mergeFormatOnWordOrSelection (const QTextCharFormat &format);
    void    fileSave              (const QString& f_name);
    void    loadFile              (const QString &f);
    void    fontChanged           (const QFont &f);
    void    colorChanged          (const QColor &c);
    void    alignmentChanged      (Qt::Alignment a);

private:
    QString     m_strLastExportDir;
    QString     m_strLastLoadDir;
    //
    QAction*    m_ptrUndo;
    QAction*    m_ptrRedo;
    //
    QAction*    m_ptrBold;
    QAction*    m_ptrUnderline;
    QAction*    m_ptrItalic;
    //
    QActionGroup* m_grpAction;
    QAction*    m_ptrTextAlignLeft;
    QAction*    m_ptrTextAlignRight;
    QAction*    m_ptrTextAlignCenter;
    QAction*    m_ptrTextAlignJustify;
    //
    QToolBar*   m_ptrToolBar;
    //
    QFontComboBox*  m_ptrFontType;
    QComboBox*  m_ptrFontSize;
};

#endif // MYEDITOR_H
