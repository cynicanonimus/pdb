#ifndef SCANNERSETTINGS_H
#define SCANNERSETTINGS_H

#include <QObject>
#include <QGroupBox>
#include <QGridLayout>
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

private:
//
    void        readData        ();
    void        updateData      (bool b_from_dialog, bool b_data_changed = true); //exchange between dialog and variables
    void        createLayout    ();

signals:
    
public slots:
    
};

#endif // SCANNERSETTINGS_H
