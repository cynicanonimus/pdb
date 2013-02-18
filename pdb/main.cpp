//
/*
 This file is part of project pdb.

    pdb is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    pdb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pdb.  If not, see <http://www.gnu.org/licenses/>.
*/
//
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
    qRegisterMetaTypeStreamOperators<DBSettings>("DBSettings");
    //
    const QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    //check config
    //
    bool b_config_correct = true;
    QVariant var  = settings.value(g_str_DB_SETTINGS);
    DBSettings db_settings = var.value<DBSettings>();
    //
    if (
            ( var.isNull()== false  ) &&
            ( true == var.isValid() )
       )
    {
        //DBSettings db_settings = var.value<DBSettings>();
        const int i_active = db_settings.getActivePage();
        const int i_current = db_settings.getCurrentPage();
        if ( (-1 == i_active) || (-1 == i_current) )
            b_config_correct = false;

    }else
    {
        QMessageBox::critical(NULL, "Error", "Database is not defined. Launch PdbSetings and define database first.", QMessageBox::Ok);
        b_config_correct = false;
    };
    //add another conditions later.
    //
    if (false == b_config_correct)
        return -1;
    //
    if (settings.value(g_str_CLR_APP_STYLE).toString().length() > 0)
        QApplication::setStyle(settings.value(g_str_CLR_APP_STYLE).toString());
    else
        QApplication::setStyle(g_str_ar_APP_STYLES[0]);
    //
    //qRegisterMetaTypeStreamOperators<DBSettings>("DBSettings");
    //
    MainWindow w;
    //
    w.show();
    //
    exit(a.exec());
    //return a.exec();
}
