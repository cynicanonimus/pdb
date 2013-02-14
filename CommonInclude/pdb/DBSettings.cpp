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
#include "DBSettings.h"
//
#include "../CommonInclude/pdb/pdb_style.h"
#include <QObject>

DBSettings::DBSettings()
{
    qRegisterMetaType<DBSettings>("DBSettings");
    //
    m_iActivePage = -1;
    m_iCurrentPage  = -1;
}

int  DBSettings::getActivePage() const
{
    return m_iActivePage;
}

void  DBSettings::setCurrentPage     (int i_current_page)
{
    m_iCurrentPage = i_current_page;
}

void DBSettings::setActivePage(int i_active_page)
{
    m_iActivePage = i_active_page;
}

bool DBSettings::isCurrentPageActive() const
{
    return m_iActivePage == m_iCurrentPage;
}

int  DBSettings::getCurrentPage() const
{
    return m_iCurrentPage;
}

void DBSettings::setCurrentPageAsActive()
{
    m_iActivePage = m_iCurrentPage;
}

int  DBSettings::getPagesAmount() const
{
    return m_NameofSet.size();
}

bool DBSettings::isCurrentPageTheLast() const
{
    return ( (m_NameofSet.size() - m_iCurrentPage) == 1 );
}

bool  DBSettings::isCurrentPageTheFirst() const
{
    return ( 0 == m_iCurrentPage );
}

void DBSettings::setPreviousAsCurrent()
{
    m_iCurrentPage--;
}

void DBSettings::setNextAsCurrent()
{
    m_iCurrentPage++;
}

void DBSettings::delCurrentPage ()
{
    m_NameofSet.removeAt(m_iCurrentPage);
    m_DbTypes.removeAt(m_iCurrentPage);
    m_DbHosts.removeAt(m_iCurrentPage);
    m_DbNames.removeAt(m_iCurrentPage);
    m_UserNames.removeAt(m_iCurrentPage);
    m_Passwords.removeAt(m_iCurrentPage);
    m_Ports.erase( m_Ports.begin() + m_iCurrentPage);
    m_ConnNumbers.erase( m_ConnNumbers.begin() + m_iCurrentPage);
    //
    if ( m_iCurrentPage > 0)
            m_iCurrentPage--;
    //
    //else - do nothing 1 page makes actual and gets the number 0
}

void  DBSettings::addDefaultPage(const QString& str_default_driver_name)
{
    m_NameofSet<<"Define configuration name";
    m_DbTypes<<str_default_driver_name;
    m_DbHosts<<"localhost";
    m_DbNames<<"Define database name";
    m_UserNames<<"Define user of database";
    m_Passwords<<"DB access password";
    m_Ports.push_back(3306);
    m_ConnNumbers.push_back(5);
    //
    m_vMakeTunnel.push_back(false);
    m_TunnelList<<"";
    //
    m_iCurrentPage = m_NameofSet.size() - 1;
}
