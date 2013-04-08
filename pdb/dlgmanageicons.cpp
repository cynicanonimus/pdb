//
#include    <QGridLayout>
#include    <QFileDialog>
#include    <QFile>
#include    <QCryptographicHash>
#include    <QBuffer>
//
#include    "iconmanager.h"
#include    "logger.h"
#include    "dlgmanageicons.h"
#include    "ui_dlgmanageicons.h"
//

DlgManageIcons::DlgManageIcons(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlgManageIcons)
{
    ui->setupUi(this);
    //
    makeLayout();
    //
    ui->buttonDelete->setEnabled(false);
    ui->buttonSave->setEnabled(false);
    //ui->buttonLoad->setEnabled(false);
    ui->buttonSelected->setEnabled(false);
    //
    QObject::connect(ui->buttonDelete,      SIGNAL(clicked()), this, SLOT(onDelete  ()     ));
    QObject::connect(ui->buttonSave,        SIGNAL(clicked()), this, SLOT(onSave    ()     ));
    QObject::connect(ui->buttonLoad,        SIGNAL(clicked()), this, SLOT(onLoad    ()     ));
    QObject::connect(ui->buttonSelected,    SIGNAL(clicked()), this, SLOT(onSelect  ()     ));
    QObject::connect(ui->buttonSetNothing,  SIGNAL(clicked()), this, SLOT(onSetNothing  () ));
    //
    QObject::connect(ui->buttonCancel,      SIGNAL(clicked()), this, SLOT(reject    ()     ));
    //
    QSize s(48,48);
    //
    ui->listIcons->setIconSize(s);
    //
    fillList();
}

DlgManageIcons::~DlgManageIcons()
{
    delete ui;
}

void DlgManageIcons::makeLayout()
{
    m_ptrLayout = new QGridLayout(this);
    this->setLayout(m_ptrLayout);
    //
    int i_row               = 0;
    //
    m_ptrLayout->addWidget(ui->labelQuickSearch, i_row,0,1,2);
    m_ptrLayout->addWidget(ui->lineEditQuickSearch, i_row,2,1,3);
    //
    i_row++;
    m_ptrLayout->addWidget(ui->listIcons,i_row,0,6,5);
    //
    m_ptrLayout->addWidget(ui->buttonDelete,i_row,6,1,2);
    i_row++;
    m_ptrLayout->addWidget(ui->buttonSave,i_row,6,1,2);
    i_row++;
    m_ptrLayout->addWidget(ui->buttonLoad,i_row,6,1,2);
    i_row++;
    i_row++;
    m_ptrLayout->addWidget(ui->buttonSelected,i_row,6,1,2);
    i_row++;
    m_ptrLayout->addWidget(ui->buttonSetNothing,i_row,6,1,2);
    //
    i_row++;
    m_ptrLayout->addWidget(ui->buttonCancel,i_row,0,1,7);

    //ui->labelQuickSearch

}

void DlgManageIcons::onDelete       ()
{

}

void DlgManageIcons::onSave         ()
{

}

void DlgManageIcons::onLoad         ()
{
    QString str_ext_list = "PNG (*.png);;JPG (*.jpg *.jpeg);;BMP (*.bmp);;GIF (*.gif);;TIFF (*.tif *.tif);;PBM (*.pbm);;PGM (*.pgm);;PPM (*.ppm);;XBM (*.xbm);;XPM (*.xpm);;All Files (*)";
    //
    QFileDialog dlg(NULL, "Select icons for load in system", "/home/alex/MyProjects/Icon_storage/png/48x48/plain/", str_ext_list);
    dlg.setOption(QFileDialog::DontUseNativeDialog);
    dlg.setFileMode(QFileDialog::ExistingFiles);
    dlg.setLabelText(QFileDialog::Accept, "Add");
    //
    if (!dlg.exec()) // if user click "cancel"
    {
        return;
    };
    //
    QStringList f_list =  dlg.selectedFiles();
    //
    foreach (const QString &f_name, f_list)
    {
        QImage* ptr_img = new QImage (f_name);
        //
        //
        //
        QByteArray bytearray;
        QBuffer buffer(&bytearray); //( (const char *) ptr_img->bits(), ptr_img->numBytes() );
        buffer.open(QIODevice::WriteOnly);
        ptr_img->save(&buffer, "PNG");
        //
        QString str_icon_hash  = QCryptographicHash::hash(bytearray, QCryptographicHash::Sha1).toBase64();
        //
        if ( IconManager::getInstance().contains(str_icon_hash) == true )
        {
            delete ptr_img;
            continue; //this icon is already in the storage
        };
        //
        QFileInfo finfo(f_name);
        int icon_id = IconManager::getInstance().addIcon( finfo.baseName(), ptr_img, str_icon_hash, bytearray );
        if (-1 != icon_id)
        {
            QListWidgetItem* ptr_item = new QListWidgetItem (QPixmap::fromImage(*ptr_img)/*(*ptr_new_icon)*/, finfo.baseName());
            ptr_item->setData(Qt::UserRole, icon_id);
            ui->listIcons->addItem(ptr_item);
        };
    };
}

void DlgManageIcons::fillList       ()
{
    const IconStorage& i_list = IconManager::getInstance().getIconList();
    //
    IconStorage::const_iterator itr;
    //
    for ( itr = i_list.begin(); itr != i_list.end(); ++itr)
    {
        QImage* ptr_img = itr.value()->getImage();
        //
        QListWidgetItem* ptr_item = new QListWidgetItem (QPixmap::fromImage(*ptr_img)/*(*ptr_new_icon)*/, itr.value()->getName());
        ptr_item->setData(Qt::UserRole, itr.key());
        ui->listIcons->addItem(ptr_item);
    };
}

void DlgManageIcons::onSelect       ()
{
    this->accept();
}
void DlgManageIcons::onSetNothing   ()
{

}
