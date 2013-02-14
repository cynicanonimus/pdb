#ifndef SECURITYELEMENTS_H
#define SECURITYELEMENTS_H
//
//class is responsible for representing security-related user interface
//
#include "abstractgraficeelements.h"
//
#include <QObject>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
//

class SecurityElements : public AbstractGraficeElements
{
    Q_OBJECT
public:
    explicit SecurityElements(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout);
    virtual ~SecurityElements();

    void writeData();
signals:
    
public slots:

private slots:
    void    onClickShred        ();
    void    onEncryptModeChanged(int);
    //

private:
    void    createLayout        ();
    void    readData            ();
    void    updateData          ( bool b_from_dialog );   //exchange between dialog and variables
    void    createLinks         ();
    //

private:
    //--------------------------------------------------
    QGroupBox*      m_ptrAttachmentGroupBox;
    QGridLayout*    m_ptrAttachmentGrid;
    //
    QLabel*         m_ptrEncryptModeLabel;
    QComboBox*      m_ptrEncryptMode;
    unsigned int    ui_encrypt_mode;
    //
    QCheckBox*      m_ptrMarkCryptByDefault;
    bool            m_bMarkCryptByDefault;
    //--------------------------------------------------
    QGroupBox*      m_ptrProtectionGroupBox;
    QGridLayout*    m_ptrProtectionGrid;
    QCheckBox*      m_ptrProtectionByDefault;
    //..
    bool            m_bProtectByDefault;
    //--------------------------------------------------
    QGroupBox*      m_ptrTmpFilesGroupBox;
    QGridLayout*    m_ptrTmpFilesGrid;
    //
    QCheckBox*      m_ptrCheckboxUseShredder;
    QLabel*         m_ptrShredderLabel;
    QLineEdit*      m_ptrEditShredderCmdLine;
    //..
    bool            m_bShred;
    QString         m_strSchredCommandLine;
    //
    bool            m_bSuccessEncryptDecryptIndicator;
};

#endif // SECURITYELEMENTS_H
