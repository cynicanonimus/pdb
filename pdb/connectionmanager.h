#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
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
