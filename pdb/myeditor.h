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
    void onExportToFile         ();

private slots:
    void onPrintPreview         (QPrinter *);
    void onTextChanged          ();

private:
    void fileSave(const QString& f_name);
};

#endif // MYEDITOR_H
