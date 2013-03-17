#ifndef MYEDITOR_H
#define MYEDITOR_H

#include <QTextEdit>

class MyEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit MyEditor(QWidget *parent = 0);
    
signals:
    void textExist            (bool);
public slots:
    void onFilePrintPreview     ();

private slots:
    void onPrintPreview         (QPrinter *);
    void onTextChanged          ();
};

#endif // MYEDITOR_H
