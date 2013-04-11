#include "iconmanager.h"
#include "dbacccesssafe.h"
#include "logger.h"
//
#include <QtSql>
//----------------------------------------------------------------------------------------
//
IconManager::IconManager(QObject *parent) :
    QObject(parent)
{

}

IconManager::~IconManager()
{
    IconStorage::const_iterator itr;
    //
    for ( itr = m_lIconList.begin(); itr != m_lIconList.end(); ++itr)
    {
        (*itr)->clear();
        delete (*itr);
    };
}

IconManager& IconManager::getInstance()
{
    static IconManager instance;
    //check is open, if not - open it.
    //instance.init();
    //
    return instance;
}

int IconManager::addIcon  (const QString& str_icon_name, QImage* ptr_icon, const QString& str_icon_hash, QByteArray& icon_byte_array)
{
    int i_res = -1;
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    //
    if (NULL == ptr_db)
        return i_res;
    //
    QSqlQuery qry(*ptr_db);
    //
    //
    const QString str_insert_string = "INSERT INTO icons_tbl (icon_name, icon_hash, icon_body) VALUES (:NAME, :HASH, :BODY);";
    //
    if ( !qry.prepare( str_insert_string  ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_insert_string );
        return -1;
    };
    //
    qry.bindValue( ":NAME",     str_icon_name       );
    qry.bindValue( ":HASH",     str_icon_hash       );
    qry.bindValue( ":BODY",     icon_byte_array     );
    //
    if ( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_insert_string );
        return i_res;
    };
    //
    i_res = qry.lastInsertId().toInt();
    //
    DBIcon* ptr_db_icon = new DBIcon(str_icon_name, str_icon_hash, ptr_icon);
    //
    m_IconStorageAccess.lockForWrite();
    m_lIconList[i_res] = ptr_db_icon;
    m_IconStorageAccess.unlock();
    //
    return i_res;
}

bool IconManager::contains (const QString& str_icon_hash) const
{
    bool b_contains = false;
    //
    m_IconStorageAccess.lockForRead();
    //check all hashes
    IconStorage::const_iterator itr;
    //
    for ( itr = m_lIconList.begin(); itr != m_lIconList.end(); ++itr)
    {
        if ( itr.value()->isHashMatched (str_icon_hash) == true)
        {
            b_contains = true;
            break;
        };
    };
    //
    m_IconStorageAccess.unlock();
    return b_contains;
}

QImage* IconManager::getIcon (int i_icon_id, bool b_increase_countrer)
{
    QImage* ptr_icon = NULL;
    m_IconStorageAccess.lockForRead();
    //
    if ( m_lIconList.contains(i_icon_id) == true )
    {
        ptr_icon = m_lIconList[i_icon_id]->getImage(b_increase_countrer);
    }


    m_IconStorageAccess.unlock();
    return ptr_icon;
}

void IconManager::deleteIcon  (int i_icon_id)
{
    if ( m_lIconList.contains(i_icon_id) == true )
    {
        m_IconStorageAccess.lockForWrite();
        //
        m_lIconList[i_icon_id]->clear();
        delete m_lIconList[i_icon_id];
        m_lIconList.remove(i_icon_id);
        //
        m_IconStorageAccess.unlock();
        //
        DBAcccessSafe   db_safe;
        //
        QSqlDatabase* ptr_db = db_safe.getDB();
        //
        if (NULL == ptr_db)
            return;
        //
        QSqlQuery qry(*ptr_db);
        //
        const QString str_del_str = "delete from icons_tbl WHERE id_icon = :ID;";
        //
        if ( false == qry.prepare( str_del_str ) )
        {
            const QString str_message = QString("Unable to delete icon from database, error - '%1'").arg( qry.lastError().text() );
            Logger::getInstance().logIt( en_LOG_ERRORS, str_message, &str_del_str );
            return;
        };
        //
        qry.bindValue( ":ID",     i_icon_id       );
        //
        if ( !qry.exec() )
        {
            Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_del_str );
            return;
        };
    };
}

void IconManager::init()
{
    if ( m_lIconList.size() > 0 )
        return;
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    //
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //
    const QString str_select_str = "SELECT id_icon, icon_name, icon_hash, icon_body FROM icons_tbl;";
    //
    if ( false == qry.prepare( str_select_str ) )
    {
        const QString str_message = QString("Unable to get icons from database, error - '%1'").arg( qry.lastError().text() );
        Logger::getInstance().logIt( en_LOG_ERRORS, str_message, &str_select_str );
        return;
    };
    //
    if( false == qry.exec() )
    {
        const QString str_message = QString("Unable to get icons from database, error - '%1'").arg( qry.lastError().text() );
        Logger::getInstance().logIt(en_LOG_ERRORS, str_message, &str_select_str  );
        return;
    };
    //
    m_IconStorageAccess.lockForWrite();
    //
    while( qry.next() )
    {
        int             id_icon     = qry.value(0).toInt();
        QString         icon_name   = qry.value(1).toString();
        QString         icon_hash   = qry.value(2).toString();
        QByteArray      icon_array  = qry.value(3).toByteArray();
        //
        QImage* ptr_image = new QImage();
        ptr_image->loadFromData(icon_array, "PNG");
        //
        DBIcon* ptr_db_icon = new DBIcon(icon_name, icon_hash, ptr_image);
        m_lIconList[id_icon] = ptr_db_icon;
        //
        //m_vAttachments.push_back(ptr_new_attachment);
    }; // while( qry.next() )
    m_IconStorageAccess.unlock();
    //
    emit InitDone();
}


bool IconManager::isInUse         (int i_icon_id)
{
    if ( m_lIconList.contains(i_icon_id) == true )
    {
        return m_lIconList[i_icon_id]->isInUse();
    };
    //
    return false;
}

void IconManager::minusInUse      (int i_icon_id)
{
    if ( m_lIconList.contains(i_icon_id) == true )
    {
        m_lIconList[i_icon_id]->minusInUse();
    };
}
