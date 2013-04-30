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
    m_strNames<<""<<""<<""<<""<<"";
    m_strScanConfigs<<""<<""<<""<<""<<"";
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
    for (unsigned int i = 0; i < 5; i++ )
    {
        m_ConfigName[i]->setEnabled(state);
        m_ConfigLine[i]->setEnabled(state);
    };
}

void ScannerSettings::writeData()
{
    updateData(true);
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    settings.setValue(g_str_SCANNER_ENABLE,    m_bEnableScanner);
    //
    settings.beginWriteArray(g_str_SCANNER_SETTINGS);
    for (int i = 0; i < m_strNames.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue(g_str_SCANNER_COFIG_NAME, m_strNames.at(i));
        settings.setValue(g_str_SCANNER_COFIG_STR, m_strScanConfigs.at(i));
    }
    settings.endArray();
    //
    m_bChanged = false;

}

void ScannerSettings::readData()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    m_bEnableScanner  = settings.value(g_str_SCANNER_ENABLE).value<bool>();
    //
    int size = settings.beginReadArray(g_str_SCANNER_SETTINGS);
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        m_strNames[i]       = settings.value(g_str_SCANNER_COFIG_NAME).toString();
        m_strScanConfigs[i] = settings.value(g_str_SCANNER_COFIG_STR).toString();
    }
    settings.endArray();
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
    addAndRegisterElement(m_ptrRHeader, i_row,2,1,3, Qt::AlignTop|Qt::AlignCenter);
    //-------------------
    //const int height = 25;
    const int width = 385;
    //
    i_row++;
    //
    for (int i = 0; i < 5; ++i)
    {
        m_ConfigName.append(new QLineEdit("Configuration N 1"));
        m_ConfigName[i]->setToolTip(str_tooltip);
        addAndRegisterElement(m_ConfigName[i], i_row,0,1,2, Qt::AlignTop);
        //...
        m_ConfigLine.append( new QLineEdit(""));
        m_ConfigLine[i] ->setToolTip("define command line here");
        m_ConfigLine[i] ->setFixedWidth(width);
        addAndRegisterElement(m_ConfigLine[i] , i_row,2,1,3, Qt::AlignTop);
        i_row++;
    };
}

void ScannerSettings::updateData (bool b_from_dialog, bool b_data_changed)
{
    if(b_from_dialog)
    {
        if (m_ptrEnableDirectScanning->checkState() == Qt::Checked)
            m_bEnableScanner = true;
        else
            m_bEnableScanner  = false;
        //
        for (int i = 0; i < m_ConfigName.size(); ++i)
        {
            m_strNames[i]       = m_ConfigName[i]->text();
            m_strScanConfigs[i] = m_ConfigLine[i]->text();
        };
    }else
    {
        if (m_bEnableScanner)
            m_ptrEnableDirectScanning->setCheckState(Qt::Checked);
        else
            m_ptrEnableDirectScanning->setCheckState(Qt::Unchecked);
        //
        for (int i = 0; i < m_ConfigName.size(); ++i)
        {
            m_ConfigName[i]->setText(m_strNames[i]);
            m_ConfigLine[i]->setText(m_strScanConfigs[i]);
        };

    };
    //
    m_bChanged = b_data_changed;
}
