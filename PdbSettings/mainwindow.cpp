#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "SectionNames.h"
#include "VariantPtr.h"

#include <QSqlDatabase>
//
#include <QMessageBox>
//
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //
    m_pGridLayout   = NULL;
    m_pDBGrafic     = NULL;
    m_pNodesAndAttachmentsGrafic = NULL;
    m_ptrLogger     = NULL;
    m_ptrSecurity   = NULL;
    //
#define QT_DEBUG_PLUGINS = 1;
    QStringList str_list = QCoreApplication::libraryPaths ();
    QStringList str_drv_list = QSqlDatabase::drivers ();
    //
    createGraficeElements();
    //
    createSettingsTree();
    //
    linkSignalsAndSlots();
    //
}

MainWindow::~MainWindow()
{
    if (m_pGridLayout) delete m_pGridLayout;
    //
    const unsigned short ush_top_level_items = ui->m_TreeOfSettings->topLevelItemCount();
    //
    for (unsigned short ush_item = 0; ush_item < ush_top_level_items; ush_item++)
    {
        QTreeWidgetItem* ptr_item = ui->m_TreeOfSettings->topLevelItem(ush_item);
        //
        QVariant v_page  = ptr_item->data(1,0);
        AbstractGraficeElements* ptr_page = VariantPtr<AbstractGraficeElements>::asPtr( v_page );
        //
        if (ptr_page)
            delete ptr_page;
    };
    //
    delete ui;
}

void MainWindow::linkSignalsAndSlots()
{
    QObject::connect(ui->m_TreeOfSettings, SIGNAL( currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this,        SLOT(onTreeCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)) );
    QObject::connect(ui->m_AcceptButton,   SIGNAL( clicked() ),                                            this,        SLOT( saveSettings() ) );
//    QObject::connect(this,                 SIGNAL( saveDBSettings() ),                                     &m_Settings, SLOT( saveSettings() ) );
};

void  MainWindow::saveSettings()
{
    const unsigned short ush_top_level_items = ui->m_TreeOfSettings->topLevelItemCount();

    for (unsigned short ush_item = 0; ush_item < ush_top_level_items; ush_item++)
    {
        QTreeWidgetItem* ptr_item = ui->m_TreeOfSettings->topLevelItem(ush_item);
        saveLeafSettings (ptr_item);
    };
    //
    this->statusBar()->showMessage(tr("Settings saved."));

};

void MainWindow::saveLeafSettings (QTreeWidgetItem* ptr_item)
{
    if (NULL == ptr_item)
        return;
    //
    QVariant v_page  = ptr_item->data(1,0);
    AbstractGraficeElements* ptr_page = VariantPtr<AbstractGraficeElements>::asPtr( v_page );
    //
    if(ptr_page)
        ptr_page->writeData();
    //
    for (unsigned short ui_child_num = 0; ui_child_num < ptr_item->childCount(); ui_child_num++)
    {
        QTreeWidgetItem* ptr_child = ptr_item->child(ui_child_num);
        saveLeafSettings(ptr_child);
    };
};


void MainWindow::onTreeCurrentItemChanged(QTreeWidgetItem* p_new_item, QTreeWidgetItem* p_previous_item)
{
    if (p_previous_item)
    {
        QVariant v_page  = p_previous_item->data(1,0);
        AbstractGraficeElements* ptr_page = VariantPtr<AbstractGraficeElements>::asPtr( v_page );
        //
        if (ptr_page)
            ptr_page->show(false);
    };
    //
    if (p_new_item)
    {
        QVariant v_page  = p_new_item->data(1,0);
        AbstractGraficeElements* ptr_page = VariantPtr<AbstractGraficeElements>::asPtr( v_page );
        //
        if (ptr_page)
            ptr_page->show(true);
    };
};

void MainWindow::createGraficeElements ()
{
    m_pGridLayout   = new QGridLayout(ui->m_groupBoxDB);
    //
    m_pColorGrafic = new ColorGraficElements(ui->m_groupBoxDB, m_pGridLayout);
    m_pColorGrafic->show(false);
    //
    m_pDBGrafic     = new DBGraficElements(ui->m_groupBoxDB, m_pGridLayout);
    m_pDBGrafic->show(false);
    //
    m_pNodesAndAttachmentsGrafic = new NodesAndAttachmentsGraficeElements(ui->m_groupBoxDB, m_pGridLayout);
    m_pNodesAndAttachmentsGrafic->show(false);
    //
    m_ptrLogger = new LoggetElements(ui->m_groupBoxDB, m_pGridLayout);
    m_ptrLogger->show(false);
    //
    m_ptrSecurity = new SecurityElements(ui->m_groupBoxDB, m_pGridLayout);
    m_ptrSecurity->show(false);
    //
    ui->m_groupBoxDB->setLayout(m_pGridLayout);
    //

};

void MainWindow::createSettingsTree()
{
    //Add database node
    QTreeWidgetItem* ptr_database_root = new QTreeWidgetItem(ui->m_TreeOfSettings);
    ptr_database_root->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    ptr_database_root->setText(0,QObject::trUtf8("Database"));
    //attach settings page to the node
    QVariant db_page = VariantPtr<AbstractGraficeElements>::asQVariant( m_pDBGrafic );
    ptr_database_root->setData(1,0,db_page);
    //
    ui->m_TreeOfSettings->addTopLevelItem(ptr_database_root);
    //Add color node
    QTreeWidgetItem* ptr_color_root = new QTreeWidgetItem(ui->m_TreeOfSettings);
    ptr_color_root->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    ptr_color_root->setText(0,QObject::trUtf8("Colors and application style"));
    //attach settings page to the node
    QVariant color_page = VariantPtr<AbstractGraficeElements>::asQVariant( m_pColorGrafic );
    ptr_color_root->setData(1,0,color_page);
    //
    ui->m_TreeOfSettings->addTopLevelItem(ptr_color_root);
    //Add Nodes and Attachments
    QTreeWidgetItem* ptr_att_root = new QTreeWidgetItem(ui->m_TreeOfSettings);
    ptr_att_root->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    ptr_att_root->setText(0,QObject::trUtf8("Nodes and attachments"));
    //attach settings page to the node
    QVariant att_page = VariantPtr<AbstractGraficeElements>::asQVariant( m_pNodesAndAttachmentsGrafic );
    ptr_att_root->setData(1,0,att_page);
    //
    QTreeWidgetItem* ptr_logger = new QTreeWidgetItem(ui->m_TreeOfSettings);
    ptr_logger->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    ptr_logger->setText(0,QObject::trUtf8("Logging"));
    //
    QVariant logger_page = VariantPtr<AbstractGraficeElements>::asQVariant( m_ptrLogger );
    ptr_logger->setData(1,0,logger_page);
    //
    ui->m_TreeOfSettings->addTopLevelItem(ptr_logger);
    //
    QTreeWidgetItem* ptr_att_security = new QTreeWidgetItem(ui->m_TreeOfSettings);
    ptr_att_security->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);
    ptr_att_security->setText(0,QObject::trUtf8("Security"));
    QVariant security_page =  VariantPtr<AbstractGraficeElements>::asQVariant( m_ptrSecurity );
    ptr_att_security->setData(1,0,security_page);
    //
    ui->m_TreeOfSettings->addTopLevelItem(ptr_att_security);
};
