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
    //m_strNames<<""<<""<<""<<""<<"";
    //m_strScanConfigs<<""<<""<<""<<""<<"";
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
    updateData(false, false);
    /*
    scanimage --help --device-name brother4
    scanimage --device-name brother4 -p  --resolution 150 --mode gray > img_grayscale_150.pgm
    ssh server scanimage --device-name brother4 -p  --resolution 600  > /home/alex/WindowsShare/img_color_600.pnm
    */
    //
    onEnableClick(m_bEnableScanner);
}

ScannerSettings::~ScannerSettings()
{

}

void  ScannerSettings::onRecordUp   ()
{
    const int i_row  = m_ptrScannerSettings->currentRow();
    //
    if ( -1 == i_row )
        return;
    //
    if ( 0 == i_row)
        return;
    //
    const QString str_config_name       = m_ptrScannerSettings->item( i_row-1, 0 )->text();
    const QString str_config_command    = m_ptrScannerSettings->item( i_row-1, 1 )->text();
    //
    m_ptrScannerSettings->item(i_row-1, 0)->setText(m_ptrScannerSettings->item( i_row, 0 )->text());
    m_ptrScannerSettings->item(i_row-1, 1)->setText(m_ptrScannerSettings->item( i_row, 1 )->text());
    //
    m_ptrScannerSettings->item(i_row, 0)->setText(str_config_name);
    m_ptrScannerSettings->item(i_row, 1)->setText(str_config_command);
    //
    m_ptrScannerSettings->setCurrentCell(i_row-1,m_ptrScannerSettings->currentColumn());
    //
}

void  ScannerSettings::onRecordDown ()
{
    const int i_row  = m_ptrScannerSettings->currentRow();
    //
    if ( -1 == i_row )
        return;
    //
    if ( (i_row + 1) > (m_ptrScannerSettings->rowCount() -1) )
        return;
    //
    const QString str_config_name       = m_ptrScannerSettings->item( i_row, 0 )->text();
    const QString str_config_command    = m_ptrScannerSettings->item( i_row, 1 )->text();
    //
    m_ptrScannerSettings->item(i_row, 0)->setText(m_ptrScannerSettings->item( i_row+1, 0 )->text());
    m_ptrScannerSettings->item(i_row, 1)->setText(m_ptrScannerSettings->item( i_row+1, 1 )->text());
    //
    m_ptrScannerSettings->item(i_row+1, 0)->setText(str_config_name);
    m_ptrScannerSettings->item(i_row+1, 1)->setText(str_config_command);
    //
    m_ptrScannerSettings->setCurrentCell(i_row+1,m_ptrScannerSettings->currentColumn());
    //
}

void ScannerSettings::onRemoveRecord   ()
{
    const int i_row  = m_ptrScannerSettings->currentRow();
    if ( -1 == i_row )
        return;
    //
    m_ptrScannerSettings->removeRow(i_row);
    //
    onItemSelectionChanged ();
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
    }else
    {
        m_ptrButtonUp->setEnabled(false);
        m_ptrButtonDown->setEnabled(false);
    };
}

void ScannerSettings::onAddNewRecord()
{
    m_ptrScannerSettings->setRowCount(m_ptrScannerSettings->rowCount()+1);
    onItemSelectionChanged ();
    return;
}

void ScannerSettings::onEnableClick(bool state)
{
    m_ptrButtonAdd      ->setEnabled(state);
    m_ptrButtonRemove   ->setEnabled(state);

    if (false == state)
    {
        m_ptrButtonUp       ->setEnabled(state);
        m_ptrButtonDown     ->setEnabled(state);
    }else
    {
        onItemSelectionChanged ();
    };
    //
    m_ptrScannerSettings->setEnabled(state);
    //may be something else later
}

bool ScannerSettings::isTableCorrect()
{
    bool b_res = true;


    return b_res;
}

void ScannerSettings::writeData()
{
    if ( isTableCorrect() == false )
        return;
    //
    updateData(true);
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    settings.setValue(g_str_SCANNER_ENABLE,    m_bEnableScanner);
    //
    settings.beginWriteArray(g_str_SCANNER_SETTINGS);
    for (int i = 0; i < m_strNames.size(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue(g_str_SCANNER_COFIG_NAME, m_strNames[i]);
        settings.setValue(g_str_SCANNER_COFIG_STR, m_strScanConfigs[i]);
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
    m_strNames.erase        ( m_strNames.begin(), m_strNames.end() );
    m_strScanConfigs.erase  ( m_strScanConfigs.begin(), m_strScanConfigs.end() );
    //
    int size = settings.beginReadArray(g_str_SCANNER_SETTINGS);
    for (int i = 0; i < size; ++i)
    {
        settings.setArrayIndex(i);
        m_strNames.push_back        ( settings.value(g_str_SCANNER_COFIG_NAME).toString() );
        m_strScanConfigs.push_back  ( settings.value(g_str_SCANNER_COFIG_STR).toString()  );
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
    //
    m_ptrScannerSettings->setColumnCount(2);
    m_ptrScannerSettings->setColumnWidth(0,150);
    m_ptrScannerSettings->setColumnWidth(1,297);
    //
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

    if(b_from_dialog)
    {
        m_strNames.erase        ( m_strNames.begin(), m_strNames.end() );
        m_strScanConfigs.erase  ( m_strScanConfigs.begin(), m_strScanConfigs.end() );
        //
        for ( int i_row = 0; i_row < m_ptrScannerSettings->rowCount(); ++i_row )
        {
            m_strNames.push_back        ( m_ptrScannerSettings->item(i_row, 0)->text() );
            m_strScanConfigs.push_back  ( m_ptrScannerSettings->item(i_row, 1)->text() );
        };
    }else
    {
        //clear existing records
        while (m_ptrScannerSettings->rowCount() > 0)
            m_ptrScannerSettings->removeRow(0);
        //set we amount of records
        m_ptrScannerSettings->setRowCount( m_strNames.size() );
        //
        for ( int i_row = 0; i_row < m_strNames.size(); ++i_row )
        {
            QTableWidgetItem* ptr_item_name = new QTableWidgetItem (m_strNames[i_row]);
            m_ptrScannerSettings->setItem( i_row, 0, ptr_item_name );
            //
            QTableWidgetItem* ptr_item_config =  new QTableWidgetItem (m_strScanConfigs[i_row]);
            m_ptrScannerSettings->setItem( i_row, 1, ptr_item_config );

            continue;
        };
    };
    //
    m_bChanged = b_data_changed;

}
