#include "abstractdatabaseobject.h"
#include "GDefinitions.h"
//
AbstractDatabaseObject::AbstractDatabaseObject(int                  i_id,
                                               bool                 b_is_active,
                                               const QDateTime&     dt_last_change,
                                               QObject*             parent) :
    QObject(parent)
{
    m_iID               = i_id;
    //
    if ( true == b_is_active )
    {
        m_enObjectStatus = OBJECT_OK;
    }else
    {
        m_enObjectStatus = OBJECT_DELETED;
    };
    //
    m_dtLastChange      = dt_last_change;
}

QString AbstractDatabaseObject::getSQLAdaptedString(const QString& t_str)
{
    QString str_adapted_name = t_str;
    //
    str_adapted_name.replace("'", "\\'");
    str_adapted_name.replace("\"","\"");
    //
    return str_adapted_name;
}

AbstractDatabaseObject::DB_OBJECT_STATUS AbstractDatabaseObject::getObjectStatus () const
{
    m_StatusLock.lockForRead();
    DB_OBJECT_STATUS status = m_enObjectStatus;
    m_StatusLock.unlock();
    return status;
}

void  AbstractDatabaseObject::setObjectStatus(DB_OBJECT_STATUS en_status)
{
    m_StatusLock.lockForWrite();
    m_enObjectStatus = en_status;
    m_StatusLock.unlock();
}
