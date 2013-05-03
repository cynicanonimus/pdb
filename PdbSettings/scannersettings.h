#ifndef SCANNERSETTINGS_H
#define SCANNERSETTINGS_H

#include <QObject>
#include <QGroupBox>
//#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QList>
#include <QLineEdit>
//#include <QSpacerItem>
#include <QTableWidget>
#include <QPushButton>
#include <QVector>
//
#include "abstractgraficeelements.h"
//
class ScannerSettings : public AbstractGraficeElements
{
    Q_OBJECT
public:
    explicit ScannerSettings(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout, QObject *parent = 0);
    ~ScannerSettings();

public:
    void        writeData       ();

signals:

public slots:

private slots:
    void        onEnableClick       (bool);
    void        onAddNewRecord      ();
    void        onRemoveRecord      ();
    void        onRecordUp          ();
    void        onRecordDown        ();
    //
    void        onItemSelectionChanged ();
private:
//
    void        readData        ();
    void        updateData      (bool b_from_dialog, bool b_data_changed = true); //exchange between dialog and variables
    void        createLayout    ();
    bool        isTableCorrect  ();


private:
    QCheckBox*      m_ptrEnableDirectScanning;
    bool            m_bEnableScanner;
    //
    QLabel*         m_ptrLHeader;
    QLabel*         m_ptrRHeader;
    //
    QTableWidget*   m_ptrScannerSettings;
    QPushButton*    m_ptrButtonUp;
    QPushButton*    m_ptrButtonDown;
    QPushButton*    m_ptrButtonAdd;
    QPushButton*    m_ptrButtonRemove;
    //
    QVector<QString> m_strNames;
    QVector<QString> m_strScanConfigs;
};

#endif // SCANNERSETTINGS_H
