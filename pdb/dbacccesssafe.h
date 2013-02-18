#ifndef DBACCCESSSAFE_H
#define DBACCCESSSAFE_H
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
// dbacccesssafe.h
//
#include <QObject>
#include <QtSql/QSqlDatabase>
//
#include "dbconnection.h"
//
class DBAcccessSafe
{
public:
    DBAcccessSafe();
    //
    ~DBAcccessSafe();
    //
    QSqlDatabase* getDB();
private:
    //do not allow to copy it
    DBAcccessSafe (DBAcccessSafe const&);
    void operator=(DBAcccessSafe const&);
    //
    int m_iDBID;

};

#endif // DBACCCESSSAFE_H
