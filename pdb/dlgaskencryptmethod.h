#ifndef DLGASKENCRYPTMETHOD_H
#define DLGASKENCRYPTMETHOD_H

#include <QDialog>

namespace Ui {
class DlgAskEncryptMethod;
}

class DlgAskEncryptMethod : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgAskEncryptMethod(QWidget *parent = 0);
    ~DlgAskEncryptMethod();
public:

private slots:
    void            onOK                    ();
    void            onEncryptModeChanged    (int);
    
private:

    void fillEncryptMethodList              ();

private:
    Ui::DlgAskEncryptMethod *ui;
    //
    unsigned int m_uiEncryptMode;
    unsigned int m_uiInitialEncryptMode;
};

#endif // DLGASKENCRYPTMETHOD_H
