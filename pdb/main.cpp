#include <QtGui/QApplication>
#include <QSettings>
#include <QSharedMemory>
#include <QMessageBox>
//
#include "mainwindow.h"
#include "../CommonInclude/pdb/pdb_style.h"
#include "../CommonInclude/pdb/DBSettings.h"
#include "../CommonInclude/pdb/DBSettingsStream.h"
//


int main(int argc, char *argv[])
{
    //
    QApplication a(argc, argv);
    //
    const QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    if (settings.value(g_str_CLR_APP_STYLE).toString().length() > 0)
        QApplication::setStyle(settings.value(g_str_CLR_APP_STYLE).toString());
    else
        QApplication::setStyle(g_str_ar_APP_STYLES[0]);
    //
    qRegisterMetaTypeStreamOperators<DBSettings>("DBSettings");
    //
    MainWindow w;
    //
    w.show();
    //
    exit(a.exec());
    //return a.exec();
}
