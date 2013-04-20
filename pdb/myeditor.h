#ifndef MYEDITOR_H
#define MYEDITOR_H
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
#include <QTextEdit>

QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QToolBar)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QTextCharFormat)
QT_FORWARD_DECLARE_CLASS(QFont)
QT_FORWARD_DECLARE_CLASS(QFontComboBox)
QT_FORWARD_DECLARE_CLASS(QColor)
QT_FORWARD_DECLARE_CLASS(QActionGroup)
QT_FORWARD_DECLARE_CLASS(MnuMainWindow)

//
class MyEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyEditor(QWidget *parent = 0);
    ~MyEditor();

public:
    void    passMnuItems           ( MnuMainWindow* ptr_mnu_window );
    //
    //void    addEditorToolBarAndColorActions ( QToolBar* ptr_tool_bar, QAction* ptr_set_text_color, QAction* ptr_set_background_color );

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
    //
    //void    onZoomIn               ();
    //
    void    onTextColor            ();
    void    onBackColor            ();
    //
    void    onInsertImageLink      ();
    void    onInsertHTML           ();
    //
    void    onInsertTable          ();
    void    onAddRow               ();
    void    onAddColumn            ();
    void    onRemoveRow            ();
    void    onRemoveColumn         ();
    void    onMergeCells           ();
    void    onCellSplit            ();
    void    onTableSettings        ();
    //
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
    MnuMainWindow*  m_ptrMnuObject;
    QActionGroup*   m_grpAction;
    //
    QFontComboBox*  m_ptrFontType;
    QComboBox*      m_ptrFontSize;
};

#endif // MYEDITOR_H
