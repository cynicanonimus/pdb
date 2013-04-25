#include "scannersettings.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QSettings>

ScannerSettings::ScannerSettings(QGroupBox *ptr_parent_frame, QGridLayout *ptr_layout, QObject *parent) :
    AbstractGraficeElements(ptr_parent_frame, ptr_layout, parent)
{
    m_ptrEnableDirectScanning   = NULL;
    m_ptrLHeader                = NULL;
    m_ptrRHeader                = NULL;

    //
    readData();
    //
    createLayout();
    //
    QObject::connect(m_ptrEnableDirectScanning, SIGNAL(toggled(bool)), this, SLOT (onEnableClick(bool) ));
    //
    updateData(false, false);
    //
    onEnableClick(m_bEnableScanner);
}

ScannerSettings::~ScannerSettings()
{

}

void ScannerSettings::onEnableClick(bool state)
{
    mm_ptrConfigName0->setEnabled(state);
    mm_ptrConfigLine0->setEnabled(state);
    //
    mm_ptrConfigName1->setEnabled(state);
    mm_ptrConfigLine1->setEnabled(state);
    //
    mm_ptrConfigName2->setEnabled(state);
    mm_ptrConfigLine2->setEnabled(state);
    //
    mm_ptrConfigName3->setEnabled(state);
    mm_ptrConfigLine3->setEnabled(state);
    //
    mm_ptrConfigName4->setEnabled(state);
    mm_ptrConfigLine4->setEnabled(state);
    //
    updateData(true, true);
}

void ScannerSettings::writeData()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    settings.setValue(g_str_SCANNER_ENABLE,    m_bEnableScanner);
}

void ScannerSettings::readData()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    m_bEnableScanner  = settings.value(g_str_SCANNER_ENABLE).value<bool>();
}

void ScannerSettings::createLayout()
{
    int i_row = 0;
    const QString str_tooltip = "Assing name to the scanner configuration. For example - \"400 DPI, Full color\" ";
    //
    m_ptrEnableDirectScanning = new QCheckBox(tr("Enable direct scan of documents"));
    m_ptrEnableDirectScanning->setToolTip("This option allows you to take image from scanner directly into database");
    addAndRegisterElement(m_ptrEnableDirectScanning, i_row,0,1,5, Qt::AlignTop);
    //-------------------
    i_row++;
    m_ptrLHeader = new QLabel("Configuration name");
    addAndRegisterElement(m_ptrLHeader, i_row,0,1,2, Qt::AlignTop);
    //...
    m_ptrRHeader = new QLabel("Scanner command");
    addAndRegisterElement(m_ptrRHeader, i_row,2,1,3, Qt::AlignTop);
    //-------------------
    i_row++;
    mm_ptrConfigName0   = new QLineEdit("Configuration N 1");
    mm_ptrConfigName0->setToolTip(str_tooltip);
    addAndRegisterElement(mm_ptrConfigName0, i_row,0,1,2, Qt::AlignTop);
    //...
    mm_ptrConfigLine0   = new QLineEdit("");
    mm_ptrConfigLine0->setToolTip("define command line here");
    addAndRegisterElement(mm_ptrConfigLine0, i_row,2,1,3, Qt::AlignTop);
    //-------------------
    i_row++;
    mm_ptrConfigName1   = new QLineEdit("Configuration N 2");
    mm_ptrConfigName1->setToolTip(str_tooltip);
    addAndRegisterElement(mm_ptrConfigName1, i_row,0,1,2, Qt::AlignTop);
    //...
    mm_ptrConfigLine1   = new QLineEdit("");
    mm_ptrConfigLine1->setToolTip("define command line here");
    addAndRegisterElement(mm_ptrConfigLine1, i_row,2,1,3, Qt::AlignTop);
    //-------------------
    i_row++;
    mm_ptrConfigName2   = new QLineEdit("Configuration N 3");
    mm_ptrConfigName2->setToolTip(str_tooltip);
    addAndRegisterElement(mm_ptrConfigName2, i_row,0,1,2, Qt::AlignTop);
    //...
    mm_ptrConfigLine2   = new QLineEdit("");
    mm_ptrConfigLine2->setToolTip("define command line here");
    addAndRegisterElement(mm_ptrConfigLine2, i_row,2,1,3, Qt::AlignTop);
    //-------------------
    i_row++;
    mm_ptrConfigName3   = new QLineEdit("Configuration N 4");
    mm_ptrConfigName3->setToolTip(str_tooltip);
    addAndRegisterElement(mm_ptrConfigName3, i_row,0,1,2, Qt::AlignTop);
    //...
    mm_ptrConfigLine3   = new QLineEdit("");
    mm_ptrConfigLine3->setToolTip("define command line here");
    addAndRegisterElement(mm_ptrConfigLine3, i_row,2,1,3, Qt::AlignTop);
    //-------------------
    i_row++;
    mm_ptrConfigName4   = new QLineEdit("Configuration N 5");
    mm_ptrConfigName4->setToolTip(str_tooltip);
    addAndRegisterElement(mm_ptrConfigName4, i_row,0,1,2, Qt::AlignTop);
    //...
    mm_ptrConfigLine4   = new QLineEdit("");
    mm_ptrConfigLine4->setToolTip("define command line here");
    addAndRegisterElement(mm_ptrConfigLine4, i_row,2,1,3, Qt::AlignTop);


}

void ScannerSettings::updateData (bool b_from_dialog, bool b_data_changed)
{
    if(b_from_dialog)
    {
        if (m_ptrEnableDirectScanning->checkState() == Qt::Checked)
            m_bEnableScanner = true;
        else
            m_bEnableScanner  = false;
    }else
    {
        if (m_bEnableScanner)
            m_ptrEnableDirectScanning->setCheckState(Qt::Checked);
        else
            m_ptrEnableDirectScanning->setCheckState(Qt::Unchecked);
    };
}
