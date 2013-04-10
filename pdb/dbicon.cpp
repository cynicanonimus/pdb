#include "dbicon.h"
//

DBIcon::DBIcon( const QString& str_icon_name, const QString& str_icon_hash, QImage* ptr_img ) :
    QObject(NULL)
{
    m_strIconName       = str_icon_name;
    m_strHash           = str_icon_hash;
    m_ptrImage          = ptr_img;
    m_uiInUseCounter    = 0;
}

DBIcon::DBIcon (DBIcon const& obj)
    : QObject(obj.parent())
{
    m_strIconName   = obj.m_strIconName;
    m_strHash       = obj.m_strHash;
    m_ptrImage      = obj.m_ptrImage;
    m_uiInUseCounter= obj.m_uiInUseCounter;
}

void DBIcon::operator=(DBIcon const& obj)
{
    m_strIconName   = obj.m_strIconName;
    m_strHash       = obj.m_strHash;
    m_ptrImage      = obj.m_ptrImage;
    m_uiInUseCounter= obj.m_uiInUseCounter;
}

DBIcon::~DBIcon()
{

}

void DBIcon::clear()
{
    if (m_ptrImage)
        delete m_ptrImage;
}

QImage* DBIcon::getImage (bool b_increase_counter)
{
    if (b_increase_counter)
        plusInUse();

    return m_ptrImage;
}

bool DBIcon::isHashMatched(const QString& str_hash) const
{
    if ( m_strHash.compare(str_hash, Qt::CaseInsensitive) == 0 )
        return true;
    //
    return false;
}
