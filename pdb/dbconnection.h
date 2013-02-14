#ifndef DBCONNECTION_H
#define DBCONNECTION_H
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
//dbconnection.h
//
/*
Класс инкапсулирует в себе объект доступа к базе данных и блокирующий его мьютекс
*/
//
#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QMutex>
#include <QString>
#include <QReadWriteLock>
//
class DBAcccessSafe;
class ConnectionManager;
//

class DBConnection : public QObject
{
    Q_OBJECT
public:
    enum ConnectionStatus{CONN_UNKNOWN, CONN_FAIL, CONN_SUCCESS, CONNECTING};

    explicit DBConnection(const QString& str_conn_name, QObject *parent = 0);
    ~DBConnection(){}
    //
    ConnectionStatus    getStatus() const; //   {return m_enStatus;}
    //
    //not a const& because of orginal declaration
    //
    QString     getDBName       () const;
    QString     getHostName     () const;
signals:
    
public slots:

private:
    //do not allow to copy it
    DBConnection (DBConnection const&);
    void operator=(DBConnection const&);
    //---------------------------------------
    QSqlDatabase*       get();
    //---------------------------------------
    //initial connection to the database makes here
    ConnectionStatus    init(const QString &str_db_type,
                             const QString &str_host,
                             const QString &str_db_name,
                             const QString &str_db_user,
                             const QString &str_db_pwd,
                             int   i_port);
    //lock access to the class instance
    bool                lock()          {  return m_DBMutex.tryLock();  }
    //unlock access to the class instance
    void                relase()        {  m_DBMutex.unlock();}

    void                setStatus(ConnectionStatus e_status);
private: //members
    //
    QSqlDatabase            m_DB;
    QMutex                  m_DBMutex;
    QMutex                  m_DBInitMutex;
    mutable QReadWriteLock  m_DBStatuLocker;
    //
    QString             m_strConnName;
    ConnectionStatus    m_enStatus;
    //
    friend class DBAcccessSafe;
    friend class ConnectionManager;
};

#endif // DBCONNECTION_H
