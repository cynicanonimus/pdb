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
#include "databasecleaner.h"
#include <QSettings>
#include <QtSql>

#include "dbacccesssafe.h"
#include "../CommonInclude/pdb/pdb_style.h"
#include "logger.h"


DatabaseCleaner::DatabaseCleaner(QObject *parent) :
    QObject(parent)
{
}

void DatabaseCleaner::run()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    clearTrees();
    //
    if ( settings.value( g_str_ATTACH_NODE_DELETE ).toBool() )
    {
        clearAttachments();
        clearNodes();
    }else if( settings.value( g_str_ATTACH_DELETE ).toBool() )
    {
        clearAttachments();
    }

}

bool DatabaseCleaner::clearTrees()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    const QString str_clear_string = "delete FROM root_tbl WHERE active = 0;";
    //
    if( !qry.prepare( str_clear_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    return true;
}

bool DatabaseCleaner::clearAttachments()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    const QString str_clear_string = "delete FROM attachments WHERE active = 0;";
    //
    if (! qry.prepare( str_clear_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    return true;
}

bool DatabaseCleaner::clearNodes()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    const QString str_clear_string = "delete FROM node_tbl WHERE active = 0;";
    //
    if( !qry.prepare( str_clear_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    return true;
}
