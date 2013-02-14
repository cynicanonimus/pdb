#include <QtGui/QApplication>
#include "mainwindow.h"
#include "../CommonInclude/pdb/DBSettings.h"
#include "../CommonInclude/pdb/DBSettingsStream.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //Q_IMPORT_PLUGIN(qsqlpsql)
    //"windows", "motif", "cde", "plastique", "windowsxp", or "macintosh"
    QApplication::setStyle("plastique");
    //
    qRegisterMetaTypeStreamOperators<DBSettings>("DBSettings");
    //
    MainWindow w;
    w.show();
    //
    return a.exec();
}
