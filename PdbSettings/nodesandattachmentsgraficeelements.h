#ifndef NODESANDATTACHMENTSGRAFICEELEMENTS_H
#define NODESANDATTACHMENTSGRAFICEELEMENTS_H

#include <QObject>
//#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QGridLayout>
//#include <QComboBox>
#include "abstractgraficeelements.h"

class NodesAndAttachmentsGraficeElements : public AbstractGraficeElements
{
    Q_OBJECT
public:
    explicit NodesAndAttachmentsGraficeElements(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout, QObject *parent = 0);
    virtual ~NodesAndAttachmentsGraficeElements();
public:

void            writeData();

signals:
    
public slots:

private slots:

void onChooseExportPath         ();
void onChooseImportPath         ();
void onChooseTmpPath            ();
void onClickDeleteAll           (int i_state);
void onClickDeleteAfterAttach   (int i_state);
void onClickDeleteTmpOnExit     (int i_state);

private:
    void            createLayout    ();
    void            readData        ();
    void            updateData      (bool b_from_dialog);   //exchange between dialog and variables
    void            createLinks     ();
    QString         getActualAppPath() const;
    void            normalizePath   (QString& str_path) const;

private:
    //
    QLabel*         m_pLabelAttachMaxSize;
    QLineEdit*      m_pLineAttachSMaxize;
    //
    QLabel*         m_pLabelExportPath;
    QLineEdit*      m_pLineExportPath;
    QPushButton*    m_pExportPathChoose;
    //
    QLabel*         m_pLabelImportPath;
    QLineEdit*      m_pLineImportPath;
    QPushButton*    m_pImportPathChoose;
    //
    QLabel*         m_pLabelTmpPath;
    QLineEdit*      m_pLineTmpPath;
    QPushButton*    m_pTmpPathChoose;
    //
    QCheckBox*      m_ptrDeleteTmpOnExit;
    // add: delete marked attachments on exit
    QCheckBox*      m_ptrDeleteAttachmentsOnExit;
    // add: delete market attachments and nodes on exit
    QCheckBox*      m_ptrDeleteAttachmentsAndNodesOnExit;
    //
    QCheckBox*      m_ptrDeleteAfterAttach;
    //--------------------------------------------

    unsigned int    m_uiMaxAttachmentSizeMB;
    //
    QString         m_strDefaultExportPath;
    QString         m_strDefaultImportPath;
    QString         m_strTmpPath;
    //
    bool            m_bDelTmpOnExit;
    bool            m_bDelAttachOnExit;
    bool            m_bDelAttachAndNodesOnExit;
    //
    bool            m_bDeleteAfterAttach;

};

#endif // NODESANDATTACHMENTSGRAFICEELEMENTS_H
