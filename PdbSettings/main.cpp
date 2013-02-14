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
