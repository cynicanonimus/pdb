#ifndef DBSETTINGS_H
#define DBSETTINGS_H
//
#include <QDataStream>
#include <QString>
#include <QStringList>
#include <QMetaType>
#include <vector>

struct DBSettings
{
    DBSettings();
    //
    QStringList m_NameofSet;
    //
    QStringList m_DbTypes;
    QStringList m_DbHosts;
    QStringList m_DbNames;
    QStringList m_UserNames;
    QStringList m_Passwords;
    //
    std::vector<int>  m_Ports;
    std::vector<int>  m_ConnNumbers;
    //
    std::vector<bool> m_vMakeTunnel;
    QStringList       m_TunnelList;
    //
    void setActivePage      (int i_active_page);
    int  getActivePage      () const;
    bool isCurrentPageActive() const;
    //
    int  getCurrentPage     () const;
    void setCurrentPage     (int i_current_page);
    void setCurrentPageAsActive();
    int  getPagesAmount     () const;
    bool isCurrentPageTheLast() const;
    bool isCurrentPageTheFirst() const;
    void delCurrentPage     ();
    //
    void setPreviousAsCurrent();
    void setNextAsCurrent   ();
    //
    void addDefaultPage     (const QString& str_default_driver_name);
private:
    int m_iActivePage;
    int m_iCurrentPage;
};

Q_DECLARE_METATYPE(DBSettings);

#endif // DBSETTINGS_H
