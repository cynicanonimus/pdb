#include "dbacccesssafe.h"
#include "connectionmanager.h"


DBAcccessSafe::DBAcccessSafe()
{
    m_iDBID = 0;
};

DBAcccessSafe::~DBAcccessSafe()
{
    ConnectionManager& manager = ConnectionManager::getInstance();
    manager.releaseDB(m_iDBID);
};
//
QSqlDatabase* DBAcccessSafe::getDB()
{
    ConnectionManager& manager = ConnectionManager::getInstance();
    if ( manager.getManagerStatus() == ConnectionManager::STATE_OK )
    {
        DBConnection* conn = manager.getDB(m_iDBID);
        return conn->get();
    };
    //
    return NULL;
};
