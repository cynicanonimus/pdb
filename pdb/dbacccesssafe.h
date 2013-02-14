#ifndef DBACCCESSSAFE_H
#define DBACCCESSSAFE_H
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
