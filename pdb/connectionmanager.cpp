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
#include "connectionmanager.h"
//
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QSettings>
//
#include "GDefinitions.h"
#include "dbacccesssafe.h"
#include "../CommonInclude/pdb/pdb_style.h"

ConnectionManager::ConnectionManager(QObject *parent) :
    QObject(parent)
{
    m_eStatus = STATE_UNKNOWN;
    m_bDBParamsRequested = false;
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    QVariant var = settings.value(g_str_DB_SETTINGS);
    m_stDBSettings = var.value<DBSettings>();
    //
    return;
}

ConnectionManager::~ConnectionManager()
{

}

void ConnectionManager::shutdown()
{
    for (int i = 0; i < m_ConnectionPool.size(); i++)
    {
        DBConnection* ptr_conn = m_ConnectionPool[i];
        ptr_conn->get()->close();
        //
       // delete ptr_conn;
    };
}

ConnectionManager& ConnectionManager::getInstance()
{
    static ConnectionManager instance;
    //check is open, if not - open it.
    instance.init();
    //
    return instance;
}

DBConnection*   ConnectionManager::getDB (int& id)
{
        int counter = 0;
        //
        while (counter < 50)
        {
            for (int i = 0; i < m_ConnectionPool.size(); i++)
            {
                DBConnection* ptr_conn = m_ConnectionPool[i];
                if ( ptr_conn->getStatus() == DBConnection::CONN_UNKNOWN)
                {
                    ptr_conn->init(m_stDBSettings.m_DbTypes   [m_stDBSettings.getCurrentPage()],
                                   m_stDBSettings.m_DbHosts   [m_stDBSettings.getCurrentPage()],
                                   m_stDBSettings.m_DbNames   [m_stDBSettings.getCurrentPage()],
                                   m_stDBSettings.m_UserNames [m_stDBSettings.getCurrentPage()],
                                   m_stDBSettings.m_Passwords [m_stDBSettings.getCurrentPage()],
                                   m_stDBSettings.m_Ports     [m_stDBSettings.getCurrentPage()]
                                   );
                } else if (ptr_conn->getStatus() == DBConnection::CONNECTING)
                    continue;
                //
                const bool b_lock = ptr_conn->lock();
                if (b_lock)
                {
                    id = i;
                    emit takeConn();
                    return ptr_conn;
                };
            };
        };
        return NULL;
}

void ConnectionManager::releaseDB (int id)
{
    DBConnection* ptr_conn = m_ConnectionPool[id];
    ptr_conn->relase();
    emit releaseConn();
}

void ConnectionManager::init()
{
    switch (m_eStatus)
    {
    case STATE_NOT_OK:
    case STATE_OK:
        return;
    case STATE_UNKNOWN:
        {
            const int i_amount_db_connnection = m_stDBSettings.m_ConnNumbers [ m_stDBSettings.getCurrentPage() ];
            //
            for (int i = 0; i < i_amount_db_connnection; i++)
            {
                DBConnection* ptr_conn = new DBConnection(QString::number(i));
                //
                m_ConnectionPool.append(ptr_conn);
                //
                if (0 == i) //open only once, just check the connection
                {
                    if (
                            ptr_conn->init(m_stDBSettings.m_DbTypes   [m_stDBSettings.getCurrentPage()],
                                           m_stDBSettings.m_DbHosts   [m_stDBSettings.getCurrentPage()],
                                           m_stDBSettings.m_DbNames   [m_stDBSettings.getCurrentPage()],
                                           m_stDBSettings.m_UserNames [m_stDBSettings.getCurrentPage()],
                                           m_stDBSettings.m_Passwords [m_stDBSettings.getCurrentPage()],
                                           m_stDBSettings.m_Ports     [m_stDBSettings.getCurrentPage()]
                                           )
                                !=
                            DBConnection::CONN_SUCCESS
                       )
                    {
                        m_eStatus  = STATE_NOT_OK;
                        //
                        break; //no reason to connect another
                    }else
                    {
                        m_eStatus  = STATE_OK;
                        //
                        if ( false == m_bDBParamsRequested )
                        {
                            m_bDBParamsRequested= true;
                        };
                        //
                    };
                };
            };
        }
        break;
    }
}

const QString& ConnectionManager::getHostName () const
{
    return m_stDBSettings.m_DbHosts[m_stDBSettings.getActivePage()];
}

const QString& ConnectionManager::getDBName   () const
{
    return m_stDBSettings.m_DbNames[m_stDBSettings.getActivePage()];
}
