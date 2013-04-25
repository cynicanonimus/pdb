#ifndef SCANNERSETTINGS_H
#define SCANNERSETTINGS_H

#include <QObject>
#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
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
    void        onEnableClick(bool);
private:
//
    void        readData        ();
    void        updateData      (bool b_from_dialog, bool b_data_changed = true); //exchange between dialog and variables
    void        createLayout    ();




private:
    QCheckBox*      m_ptrEnableDirectScanning;
    bool            m_bEnableScanner;
    //
    QLabel*         m_ptrLHeader;
    QLabel*         m_ptrRHeader;
    //
    QLineEdit*      mm_ptrConfigName0;
    QLineEdit*      mm_ptrConfigLine0;
    //
    QLineEdit*      mm_ptrConfigName1;
    QLineEdit*      mm_ptrConfigLine1;
    //
    QLineEdit*      mm_ptrConfigName2;
    QLineEdit*      mm_ptrConfigLine2;
    //
    QLineEdit*      mm_ptrConfigName3;
    QLineEdit*      mm_ptrConfigLine3;
    //
    QLineEdit*      mm_ptrConfigName4;
    QLineEdit*      mm_ptrConfigLine4;
};

#endif // SCANNERSETTINGS_H
