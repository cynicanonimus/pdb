#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
//
//connectionmanager.h
//
/*
class manages pool of connections
*/
#include <QObject>
#include <QVector>
//
#include "dbconnection.h"
#include "../CommonInclude/pdb/DBSettings.h"

class DBAcccessSafe; //forward declaration
//
//
class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    ~ConnectionManager();
    //
    static ConnectionManager&   getInstance ();
    void                        shutdown    ();
    const QString&              getHostName () const;
    const QString&              getDBName   () const;
    //
    enum eConnMgrStatus {STATE_UNKNOWN, STATE_OK, STATE_NOT_OK};
    //
    eConnMgrStatus              getManagerStatus() const {return m_eStatus;}
    //
signals:
    void takeConn       ();
    void releaseConn    ();

public slots:



private: //functions
    DBConnection*   getDB       (int& id); //& - because we need to know what to unlock
    void            releaseDB   (int id); //lock makes int
    //
    //initial connection to the database makes here
    //
    void    init();
    //
    explicit ConnectionManager(QObject *parent = 0);
             ConnectionManager (ConnectionManager const&);
    void operator=(ConnectionManager const&);
    //
    QVector<DBConnection*> m_ConnectionPool;
    eConnMgrStatus  m_eStatus;
    //
    bool            m_bDBParamsRequested;
    //
    DBSettings      m_stDBSettings;
    //
    friend class DBAcccessSafe;
};

#endif // CONNECTIONMANAGER_H
