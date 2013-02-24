#ifndef DBSETTINGSSTREAM_H
#define DBSETTINGSSTREAM_H
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
#include <QDataStream>

QDataStream& operator<<(QDataStream& out, const DBSettings& player)
{
    out << player.getActivePage();
    //
    out << player.m_NameofSet;
    //
    out << player.m_DbTypes;
    out << player.m_DbHosts;
    out << player.m_DbNames;
    out << player.m_UserNames;
    out << player.m_Passwords;
    out << player.m_Backups;
    //
    int  i_port_size = player.m_Ports.size();
    //
    out << i_port_size;
    for (int i = 0; i < i_port_size; i++)
    {
        int i_element =  player.m_Ports[i];
        out << i_element;
    };
    //
    int  i_conn_size = player.m_ConnNumbers.size();
    //
    out << i_conn_size;
    for (int i = 0; i < i_conn_size; i++)
    {
        int i_element = player.m_ConnNumbers[i];
        out << i_element;
    };
    //----------------------------------------------------
    int i_tunnel_size = player.m_vMakeTunnel.size();
    out << i_tunnel_size;
    //
    for (int i = 0; i < i_tunnel_size; i++)
    {
        bool b_element = player.m_vMakeTunnel[i];
        out << b_element;
    };
    //
    out << player.m_TunnelList;
    //
    return out;
};


QDataStream& operator>>(QDataStream& in, DBSettings& player)
{
    int i_active_page = -1;
    in >> i_active_page;
    player.setCurrentPage(i_active_page);
    player.setCurrentPageAsActive();

    //
    in >> player.m_NameofSet;
    //
    in >> player.m_DbTypes;
    in >> player.m_DbHosts;
    in >> player.m_DbNames;
    in >> player.m_UserNames;
    in >> player.m_Passwords;
    in >> player.m_Backups;
    //
    int i_port_size = 0;
    in >> i_port_size;
    for (int i = 0; i < i_port_size; ++i)
    {
        int i_element = 0;
        in >> i_element;
        player.m_Ports.push_back(i_element);
    };
    //
    int i_conn_size = 0;
    in >> i_conn_size;
    for (int i = 0; i < i_conn_size; ++i)
    {
        int i_element = 0;
        in >> i_element;
        player.m_ConnNumbers.push_back(i_element);
    };
    ///---------------------------------------
    //
    int i_tunnel_size = 0;
    in >> i_tunnel_size;
    //
    for (int i = 0; i < i_tunnel_size; i++)
    {
        bool b_element = false;
        in >> b_element;
        player.m_vMakeTunnel.push_back(b_element);
    };
    //
    in >> player.m_TunnelList;
    //
    return in;
};

#endif // DBSETTINGSSTREAM_H
