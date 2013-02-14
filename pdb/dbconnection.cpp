#include "dbconnection.h"
#include "GDefinitions.h"
#include "logger.h"
//
#include <QtSql>
#include <QtDebug>
#include <QMessageBox>
#include <QMutexLocker>
//
#include "../CommonInclude/pdb/pdb_style.h"
//
DBConnection::DBConnection(const QString& str_conn_name, QObject *parent) :
    QObject(parent)
{
    m_strConnName = str_conn_name;
    setStatus(CONN_UNKNOWN);
}

DBConnection::ConnectionStatus DBConnection::init(const QString& str_db_type,
                                                  const QString& str_host,
                                                  const QString& str_db_name,
                                                  const QString& str_db_user,
                                                  const QString& str_db_pwd,
                                                  int   i_port)
{
    switch ( getStatus() )
    {
    case CONNECTING:
    case CONN_SUCCESS:
    case CONN_FAIL:
        break;
    case CONN_UNKNOWN:
      {
        QMutexLocker initlocker (&m_DBInitMutex);
        setStatus(DBConnection::CONNECTING);
        //
        m_DB = QSqlDatabase::addDatabase ( str_db_type, m_strConnName );//( "QOCI", "main");  // my ORACLE or MySQL Server
        m_DB.setHostName    ( str_host      ); //localhost   "192.168.178.3"
        m_DB.setDatabaseName( str_db_name   ); //the name of my database
        m_DB.setUserName    ( str_db_user   ); // the name of my database user
        m_DB.setPassword    ( str_db_pwd    ); //the password of my database user. DO NOT use ONLY from notebook
        m_DB.setPort        ( i_port        );
        //
        if ( !m_DB.open() )
        {
            QSqlError err = m_DB.lastError();
            QString str_msg = err.text();
            //
            Logger::getInstance().logIt(en_LOG_ERRORS, str_msg);
            setStatus (CONN_FAIL);
        }else
        {
            setStatus (CONN_SUCCESS);
        };
      };
    };
    //
    return getStatus();
}

QString DBConnection::getDBName   () const
{
    return m_DB.databaseName();
}

QString DBConnection::getHostName () const
{
    return m_DB.hostName();
}

QSqlDatabase* DBConnection::get()
{
    return &m_DB;
}

void DBConnection::setStatus(ConnectionStatus e_status)
{
    m_DBStatuLocker.lockForWrite();
    m_enStatus = e_status;
    m_DBStatuLocker.unlock();
}

DBConnection::ConnectionStatus DBConnection::getStatus() const
{
    m_DBStatuLocker.lockForRead();
    ConnectionStatus status = m_enStatus;
    m_DBStatuLocker.unlock();
    return status;
}
