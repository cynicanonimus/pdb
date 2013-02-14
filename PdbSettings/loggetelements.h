#ifndef LOGGETELEMENTS_H
#define LOGGETELEMENTS_H

#include "abstractgraficeelements.h"

#include <QObject>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBitArray>

//

class LoggetElements : public AbstractGraficeElements
{
    Q_OBJECT
public:
    explicit LoggetElements(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout);
    virtual ~LoggetElements();
    //
    void writeData();
signals:
    
public slots:

private slots:
    void onClickEnableLog(int);
    void onChooseLogPath();
    void onClickAllCheckboxes();

private:
    void    createLinks ();
    void    createLayout();
    void    readData    ();
    void    updateData  (bool b_from_dialog);   //exchange between dialog and variables
    void    updateFlags (bool b_from_dialog);   //exchange between dialog and variables
    void    enableInterface(bool b_enable = true);
private:

    QCheckBox*      m_ptrEnableLog;
    bool            m_bEnableLog;
    //
    QLabel*         m_pLabelLogPath;
    QLineEdit*      m_pLineLogPath;
    QPushButton*    m_pLogPathChoose;
    QString         m_strLogPath;
    //
    QCheckBox*      m_ptrLogErrors;
    bool            m_bLogErrors;
    //--------------------------------------------
    QGroupBox*      m_ptrNodeGroupBox;
    QGridLayout*    m_ptrNodeGridbox;
    //
    QCheckBox*      m_ptrLogCreateNode;
    QCheckBox*      m_ptrLogUpdateNode;
    QCheckBox*      m_ptrLogDeleteNode;
    QCheckBox*      m_ptrLogMoveNode;
    QCheckBox*      m_ptrLogNodeExport;
    QCheckBox*      m_ptrLogNodeDescriptor;
    //--------------------------------------------
    QGroupBox*      m_ptrAttGroupBox;
    QGridLayout*    m_ptrAttGridbox;
    //
    QCheckBox*      m_ptrLogAttachDel;
    QCheckBox*      m_ptrLogAttachCreate;
    QCheckBox*      m_ptrLogAttachChange;
    QCheckBox*      m_ptrLogAttachRename;
    QCheckBox*      m_ptrLogAttachMove;
    QCheckBox*      m_ptrLogAttachView;
    QCheckBox*      m_ptrLogAttachExport;
    QCheckBox*      m_ptrLogAtachCrypt;
    //--------------------------------------------
    QGroupBox*      m_ptrTreeGroupBox;
    QGridLayout*    m_ptrTreeGridbox;
    //
    QCheckBox*      m_ptrLogTreeCreate;
    QCheckBox*      m_ptrLogTreeDel;
    QCheckBox*      m_ptrLogTreeRename;
    //
    QBitArray       m_bitFlags;
    //just to avoid trigger when "setChecked" for checkboxes.
    bool            m_bInitialLoadFlag;
};

#endif // LOGGETELEMENTS_H
