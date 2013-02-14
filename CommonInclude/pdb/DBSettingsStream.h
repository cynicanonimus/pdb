#ifndef DBSETTINGSSTREAM_H
#define DBSETTINGSSTREAM_H
//
//
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
