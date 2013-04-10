//
#include    <QGridLayout>
#include    <QFileDialog>
#include    <QFile>
#include    <QCryptographicHash>
#include    <QBuffer>
#include    <QMessageBox>
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
    m_iSelectedIconID = -1;
    //
    makeLayout();
    //
    m_strExtList = "PNG (*.png);;JPG (*.jpg *.jpeg);;BMP (*.bmp);;GIF (*.gif);;TIFF (*.tif *.tif);;PBM (*.pbm);;PGM (*.pgm);;PPM (*.ppm);;XBM (*.xbm);;XPM (*.xpm);;All Files (*)";
    //
    ui->buttonDelete->setEnabled(false);
    ui->buttonSave->setEnabled(false);
    //ui->buttonLoad->setEnabled(false);
    ui->buttonSelected->setEnabled(false);
    //
    QObject::connect(ui->buttonDelete,          SIGNAL(clicked()),              this,    SLOT(onDelete  ()                      ));
    QObject::connect(ui->buttonSave,            SIGNAL(clicked()),              this,    SLOT(onSave    ()                      ));
    QObject::connect(ui->buttonLoad,            SIGNAL(clicked()),              this,    SLOT(onLoad    ()                      ));
    QObject::connect(ui->buttonSelected,        SIGNAL(clicked()),              this,    SLOT(onSelect  ()                      ));
    QObject::connect(ui->buttonSetNothing,      SIGNAL(clicked()),              this,    SLOT(onSetNothing  ()                  ));
    //
    QObject::connect(ui->buttonCancel,          SIGNAL(clicked()),              this,    SLOT(reject    ()                      ));
    QObject::connect(ui->lineEditQuickSearch,   SIGNAL(textChanged(QString)),   this,    SLOT(onSearchTextChanged(QString)      ));
    //
    QObject::connect(ui->listIcons,         SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(onListItemChanged(QListWidgetItem*,QListWidgetItem*) ));
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

void DlgManageIcons::onSearchTextChanged(QString str_text)
{
    for (unsigned int i = 0; i < ui->listIcons->count(); i++)
    {
        const QString str_item_text = ui->listIcons->item(i)->text();
        if ( str_item_text.contains(str_text,Qt::CaseInsensitive) == true )
        {
            ui->listIcons->setCurrentRow(i);
            break;
        };
    };
}

void DlgManageIcons::onListItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    if (current)
    {
        ui->buttonSave->setEnabled(true);
        int i_icon_id = current->data(Qt::UserRole).toInt();
        if ( IconManager::getInstance().isInUse(i_icon_id) == true)
        {
            ui->buttonDelete->setEnabled(false);
        }else
        {
            ui->buttonDelete->setEnabled(false);
            ui->buttonDelete->setEnabled(true);
        };
        ui->buttonSelected->setEnabled(true);
    }else
    {
        ui->buttonSave->setEnabled(false);
        ui->buttonSelected->setEnabled(false);
    };
}

void DlgManageIcons::onDelete       ()
{
    QListWidgetItem* ptr_current_item = ui->listIcons->currentItem();
    if (NULL == ptr_current_item)
        return;
    //
    int i_icon_id = ptr_current_item->data(Qt::UserRole).toInt();
    //
    IconManager::getInstance().deleteIcon(i_icon_id);
    //
    int i_row = ui->listIcons->row(ptr_current_item);
    //
    ptr_current_item = ui->listIcons->takeItem(i_row);
    if (ptr_current_item)
        delete ptr_current_item;

    //TODO remove item from list
    //ui->listIcons->removeItemWidget(ui->listIcons->currentItem());
    //ui->listIcons->


}

void DlgManageIcons::onSave         ()
{
    QListWidgetItem* ptr_current_item = ui->listIcons->currentItem();
    if (NULL == ptr_current_item)
        return;
    //
    QFileDialog dlg(NULL, "Select file to save icon", "/home/alex/", "PNG (*.png)");
    dlg.setOption(QFileDialog::DontUseNativeDialog);
    //dlg.setFileMode(QFileDialog::ExistingFile);
    dlg.setLabelText(QFileDialog::Accept, "Save");
    //
    if (!dlg.exec()) // if user click "cancel"
    {
        return;
    };
    //
    //check if file already exist
    QStringList f_list = dlg.selectedFiles();
    //check file extension
    QString str_f_name = f_list[0];
    //
    if ( str_f_name.endsWith(".png",Qt::CaseInsensitive) == false )
    {
        str_f_name += ".png";
    };
    //
    if (QFile::exists(str_f_name) == true)
    {
        QString str_message = "File ";
        str_message+= str_f_name;
        str_message+= " already exists. Overwrite?";
        int i_resp = QMessageBox::question(NULL, "Overwriting file", str_message, QMessageBox::Yes, QMessageBox::No);
        if (QMessageBox::No == i_resp)
            return;
    };
    //
    int i_icon_id = ptr_current_item->data(Qt::UserRole).toInt();
    QImage* ptr_img = IconManager::getInstance().getIcon(i_icon_id, false);
    if (NULL == ptr_img)
        return;
    //
    QByteArray bytearray;
    QBuffer buffer(&bytearray); //( (const char *) ptr_img->bits(), ptr_img->numBytes() );
    buffer.open(QIODevice::WriteOnly);
    ptr_img->save(&buffer, "PNG");
    //
    QFile f_icon (str_f_name);
    if ( f_icon.open(QIODevice::WriteOnly) == false )
    {
        return;
    };
    //
    if( -1 == f_icon.write(bytearray) )
    {
        QString str_message = "Can not write file ";
        str_message += str_f_name;
        QMessageBox::critical(NULL, "Error", str_message, QMessageBox::Ok);
    }else
    {
        QString str_message = "Icon saved in the file ";
        str_message += str_f_name;
        QMessageBox::information(NULL, "Success", str_message, QMessageBox::Ok);
    };
    //
    f_icon.close();
    //
    return;
}

void DlgManageIcons::onLoad         ()
{
    QFileDialog dlg(NULL, "Select icons for load in system", "/home/alex/MyProjects/Icon_storage/png/48x48/plain/", m_strExtList);
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
        QImage* ptr_img = itr.value()->getImage(false);
        //
        QListWidgetItem* ptr_item = new QListWidgetItem (QPixmap::fromImage(*ptr_img)/*(*ptr_new_icon)*/, itr.value()->getName());
        ptr_item->setData(Qt::UserRole, itr.key());
        ui->listIcons->addItem(ptr_item);
    };
}

void DlgManageIcons::onSelect       ()
{
    QListWidgetItem* ptr_current_item = ui->listIcons->currentItem();
    //
    if( NULL != ptr_current_item)
    {
        m_iSelectedIconID = ptr_current_item->data(Qt::UserRole).toInt();
        this->accept();
    };
    //do nothing.
}
void DlgManageIcons::onSetNothing   ()
{

}
