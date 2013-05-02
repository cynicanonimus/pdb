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
    QObject::connect( m_ptrScannerSettings, SIGNAL ( itemSelectionChanged() ),  this, SLOT( onItemSelectionChanged()    ));
    QObject::connect( m_ptrButtonAdd,       SIGNAL(clicked()),                  this, SLOT( onAddNewRecord()            ));
    QObject::connect( m_ptrButtonRemove,    SIGNAL(clicked()),                  this, SLOT( onRemoveRecord()            ));
    QObject::connect( m_ptrButtonUp,        SIGNAL(clicked()),                  this, SLOT( onRecordUp    ()            ));
    QObject::connect( m_ptrButtonDown,      SIGNAL(clicked()),                  this, SLOT( onRecordDown  ()            ));
    //
    // up
    // down
    //
    updateData(false, false);
    //
    onEnableClick(m_bEnableScanner);
}

ScannerSettings::~ScannerSettings()
{

}

void  ScannerSettings::onRecordUp   ()
{

}

void  ScannerSettings::onRecordDown ()
{

}

void ScannerSettings::onRemoveRecord   ()
{
    const int i_row  = m_ptrScannerSettings->currentRow();
    if ( -1 == i_row )
        return;
    //
    m_ptrScannerSettings->removeRow(i_row);
}

void ScannerSettings::onItemSelectionChanged ()
{
    const int i_row  = m_ptrScannerSettings->currentRow();
    //
    if ( -1 == i_row )
    {
        m_ptrButtonUp->setEnabled(false);
        m_ptrButtonDown->setEnabled(false);
        m_ptrButtonRemove->setEnabled(false);
        return; //nothing is selected or more than 1 i selected. can not move or delete item.
    };
    //
    m_ptrButtonRemove->setEnabled(true);
    //
    if (m_ptrScannerSettings->rowCount() > 1) //only in this case we can move records
    {
        if ( i_row < (m_ptrScannerSettings->rowCount() -1) )
            m_ptrButtonDown->setEnabled(true);
        else
            m_ptrButtonDown->setEnabled(false);
        //
        if ( i_row > 0 )
            m_ptrButtonUp->setEnabled(true);
        else
            m_ptrButtonUp->setEnabled(false);
    };
}

void ScannerSettings::onAddNewRecord()
{
    m_ptrScannerSettings->setRowCount(m_ptrScannerSettings->rowCount()+1);
    //m_ptrScannerSettings->selectedItems()
    return;
}

void ScannerSettings::onEnableClick(bool state)
{
    m_ptrButtonAdd->setEnabled(state);
    //may be something else later
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
    //const QString str_tooltip = "Assing name to the scanner configuration. For example - \"400 DPI, Full color\" ";
    //
    m_ptrEnableDirectScanning = new QCheckBox(tr("Enable direct scan of documents"));
    m_ptrEnableDirectScanning->setToolTip("This option allows you to take image from scanner directly into database");
    addAndRegisterElement(m_ptrEnableDirectScanning, i_row,0,1,5, Qt::AlignTop);
    //-------------------
    i_row++;
    m_ptrScannerSettings = new QTableWidget();
    m_ptrScannerSettings->setColumnCount(2);
    m_ptrScannerSettings->setColumnWidth(0,125);
    m_ptrScannerSettings->setColumnWidth(1,325);
    m_ptrScannerSettings->setFixedHeight(330);
    m_ptrScannerSettings->setFixedWidth(470);
    m_ptrScannerSettings->setSelectionMode(QAbstractItemView::SingleSelection);
    QStringList str_att_header;
    //                      0              1
    str_att_header<<"Configuration name"<<"Scanner command";
    m_ptrScannerSettings->setHorizontalHeaderLabels(str_att_header);
    m_ptrScannerSettings->setEditTriggers( QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed );
    //
    addAndRegisterElement(m_ptrScannerSettings, i_row,0,4,4, Qt::AlignTop);
    //
    i_row++;
    const int i_size = 32;
    m_ptrButtonUp = new QPushButton();
    m_ptrButtonUp->setFixedWidth(i_size);
    m_ptrButtonUp->setFixedHeight(i_size);
    m_ptrButtonUp->setIcon(QIcon(":/images/images/up.png"));
    m_ptrButtonUp->setToolTip("move current record up");
    m_ptrButtonUp->setEnabled(false);
    addAndRegisterElement(m_ptrButtonUp, i_row,4,1,1, Qt::AlignTop|Qt::AlignRight);
    //-------------------
    i_row +=2;
    m_ptrButtonDown = new QPushButton();
    m_ptrButtonDown->setFixedWidth(i_size);
    m_ptrButtonDown->setFixedHeight(i_size);
    m_ptrButtonDown->setIcon(QIcon(":/images/images/down.png"));
    m_ptrButtonDown->setToolTip("move current record down");
    m_ptrButtonDown->setEnabled(false);
    addAndRegisterElement(m_ptrButtonDown, i_row,4,1,1, Qt::AlignTop|Qt::AlignRight);
    //-------------------
    i_row +=1;
    m_ptrButtonAdd = new QPushButton();
    m_ptrButtonAdd->setIcon(QIcon(":/images/images/plus.png"));
    m_ptrButtonAdd->setToolTip("add new record");
    m_ptrButtonAdd->setEnabled(false);
    addAndRegisterElement(m_ptrButtonAdd, i_row,0,1,1, Qt::AlignTop|Qt::AlignCenter);
    //-------------------
    m_ptrButtonRemove = new QPushButton();
    m_ptrButtonRemove->setIcon(QIcon(":/images/images/minus.png"));
    m_ptrButtonRemove->setToolTip("remove current record");
    m_ptrButtonRemove->setEnabled(false);
    addAndRegisterElement(m_ptrButtonRemove, i_row,3,1,1, Qt::AlignTop|Qt::AlignCenter);
}

void ScannerSettings::updateData (bool b_from_dialog, bool b_data_changed)
{
/*
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
*/
}
