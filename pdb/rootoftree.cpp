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
#include <QtSql>
//
#include "rootoftree.h"
//#include "dbnode.h"
#include "dbacccesssafe.h"
#include "GDefinitions.h"
#include "treeleaf.h"
#include "logger.h"
#include "waiter.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
RootOfTree::RootOfTree(const QString&      str_root_name,
                       int                 ui_id,
                       const QDateTime&    last_change,
                       bool                b_is_active,
                       QObject*            parent) :
             AbstractDatabaseObject(ui_id,
                                    b_is_active,
                                    last_change,
                                    parent)
{
    m_ptrTopLeaf = NULL;
    //
    m_strName = str_root_name;
    //
    if (-1 == ui_id)
    {
        int i_ret = insertTo_DB();
        if (i_ret)
        {
            QString str_message = QString("(id = %1) name='%2'").arg(m_iID).arg(m_strName);
            Logger::getInstance().logIt(en_LOG_CREATE_TREE, str_message );
        }else
        {
            QString str_message = QString("Can not create tree name='%1'").arg(m_strName);
            Logger::getInstance().logIt(en_LOG_ERRORS, str_message );
        };
    };
}
//
RootOfTree::~RootOfTree()
{

};
//
bool RootOfTree::rename_it(const QString& str_name)
{
    if ( str_name.length() > 0 )
    {
        QString str_old_name = m_strName;
        //
        m_strName = str_name;
        bool b_rename = updateName_DB ();
        if(b_rename)
        {
            QString str_message = QString("(id = %1) old name='%2' new name='%2'").arg(m_iID).arg(str_old_name).arg(m_strName);
            Logger::getInstance().logIt(en_LOG_RENAME_TREE, str_message );
        };
        //
        return b_rename;
    };
    return false;

}
//
bool RootOfTree::delete_it (bool b_silence)
{
    Q_UNUSED(b_silence);
    //
    bool b_res = true;
    //
    if (NULL != m_ptrTopLeaf)
    {
        while (m_ptrTopLeaf->isPossibleToDeleteNode(true, true) == false) //because in the another thread we can make upload right now
        {
            Waiter::wait(1);
/*
            QTime dieTime= QTime::currentTime().addSecs(1);
            while( QTime::currentTime() < dieTime )
            QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
*/
        };
        //make force delete
        b_res = m_ptrTopLeaf->force_delete_it();
    };
    //
    setActiveStatus_DB(false);
    //
    QString str_message = QString("(id = %1) name='%2'").arg(m_iID).arg(m_strName);
    Logger::getInstance().logIt(en_LOG_DELETE_TREE, str_message );
    //
    return b_res;
};
//
void RootOfTree::setActiveStatus_DB  (bool b_active)
{
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //
    int i_status = 0; //not active
    if (b_active)
        i_status = 1;
    //
    QString str_update_string = QString("update root_tbl set current_active = %1 WHERE id_tree = %2;").arg( i_status ).arg(m_iID);
    //
    qry.prepare( str_update_string );
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_update_string );
    };
};
//
bool RootOfTree::updateName_DB ()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_update_string = QString("UPDATE root_tbl SET  tree_name = :NAME WHERE  id_tree = :ID;");
    //
    if( !qry.prepare( str_update_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_update_string );
        return false;
    }
    //
    qry.bindValue(":NAME", m_strName);
    qry.bindValue(":ID",   m_iID);
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_update_string );
        return false;
    };
    return true;
};
//
int RootOfTree::insertTo_DB() //
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return -1;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_insert_string = QString("INSERT INTO root_tbl (tree_name) values (:NAME);");
    //
    if( !qry.prepare( str_insert_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_insert_string );
        return -1;
    };
    //
    qry.bindValue(":NAME", m_strName);
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_insert_string );
        return -1;
    };
    //
    m_iID = qry.lastInsertId().toInt();
    //
    return m_iID;
}

bool RootOfTree::restoreObject()
{
    //restore the tree
    Q_ASSERT(false); //impossible delete (and restore) root node of the tree
    //
    setObjectStatus(OBJECT_OK);
    return false;
}

void RootOfTree::addChildLeaf(TreeLeaf* ptr_leaf)
{
    if ( NULL == ptr_leaf )
        return;
    //
    if (ptr_leaf->getParentID() == 0) //this is root node
    {
        m_ptrTopLeaf = ptr_leaf;
        return;
    };
    //
    Q_ASSERT (FALSE);
};

