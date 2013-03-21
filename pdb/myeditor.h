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
    void    passBUIActions         ( QAction* ptr_bold,             QAction* ptr_underline,         QAction* ptr_italic );
    void    passAlignActions       ( QAction* ptr_text_align_left,  QAction* ptr_text_align_right,  QAction* ptr_text_align_center, QAction* ptr_text_align_justify);
    void    passTextCaseActions    ( QAction* ptr_sentence_case,    QAction* ptr_upper_case,        QAction* ptr_lower_case,        QAction* ptr_toggle_case );
    void    passTextColorActions   ( );
    //
    void    addEditorToolBarAndColorActions ( QToolBar* ptr_tool_bar, QAction* ptr_set_text_color, QAction* ptr_set_background_color );

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
    //
    void    onTextUpperCase        ();
    void    onTextLowerCase        ();
    void    onTextToggleCase       ();
    void    onTextSentenceCase     ();
    void    onTextColor            ();
    void    onBackColor            ();

private:
    void    mergeFormatOnWordOrSelection (const QTextCharFormat &format);
    void    fileSave              (const QString& f_name);
    void    loadFile              (const QString &f);
    void    fontChanged           (const QFont &f);
    void    onTextColorChanged    (const QColor &c);
    void    onBackColorChanged    (const QColor &c);
    void    alignmentChanged      (Qt::Alignment a);
    void    prepareTextFormatting (QTextCharFormat& fmt);

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
    QAction*    m_ptrSentenceCase;
    QAction*    m_ptrUpperCase;
    QAction*    m_ptrLowerCase;
    QAction*    m_ptrToggleCase;
    //
    QAction*    m_ptrChangeTextColor;
    QAction*    m_ptrChangeBackgroundColor;
    //
    QToolBar*   m_ptrToolBar;
    //
    QFontComboBox*  m_ptrFontType;
    QComboBox*  m_ptrFontSize;
};

#endif // MYEDITOR_H
