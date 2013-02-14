#include "notificationdialog.h"
#include "ui_notificationdialog.h"

NotificationDialog::NotificationDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NotificationDialog)
{
    ui->setupUi(this);
}

NotificationDialog::~NotificationDialog()
{
    delete ui;
}
