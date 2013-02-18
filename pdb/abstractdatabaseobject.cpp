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
