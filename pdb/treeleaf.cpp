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
#include "treeleaf.h"
#include "mytree.h"
#include "dbacccesssafe.h"
#include "mytree.h"
#include "logger.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QtSql>
#include <QSettings>
#include <QColor>
#include <QThreadPool>
#include <QMessageBox>
//
// create from user interface
//
TreeLeaf::TreeLeaf(TreeLeaf*        prtParentLeaf,
                   int              i_id,
                   int              i_tree_id,
                   const QString&   str_name,           //node name
                   QObject*         parent_tree) :
    AbstractDatabaseObject(i_id,
                           true,                            //node active
                           QDateTime::currentDateTime(),
                           parent_tree
                          )
  ,QTreeWidgetItem(prtParentLeaf)
  ,m_bIsExpandedDB (true)           //
  ,m_bIsExpandedActually (false)    //
{
    if (prtParentLeaf)
        setInitialSettings(prtParentLeaf->getID(), str_name, i_tree_id);
    else
        setInitialSettings(0, str_name, i_tree_id);
    //
    if (i_id < 0)
    {
        if (insertTo_DB() < 0)
        {
            Q_ASSERT( FALSE);
            return;
        }else
        {
            QString str_message = QString("(id = %1) name='%2'").arg(m_iID).arg(this->text(0));
            Logger::getInstance().logIt(en_LOG_CREATE_NODE, str_message );
        };
    };
    //
    m_ptrParentTree = (MyTree *) parent_tree;
    Q_ASSERT (m_ptrParentTree);
};

//
// create from database, non-root leaf
//
TreeLeaf::TreeLeaf(TreeLeaf*        prtParentLeaf,
                  int               i_id,                  // node index in the node_tbl
                  int               i_parent_node_id,      // parent node ID
                  int               i_tree_id,
                  const QString&    str_name,              // node name
                  const QString&    str_html_descriptor,   // node text descriptor
                  bool              b_is_active,           // is node "deleted" or not
                  bool              b_is_expanded,         // index of the parent element in the root_tbl
                  const QDateTime&  dt_last_change,
                  QObject*          parent_tree
               ):
    AbstractDatabaseObject(i_id,
                           b_is_active,
                           dt_last_change,
                           parent_tree)
  ,QTreeWidgetItem(prtParentLeaf)
  ,m_bIsExpandedDB (b_is_expanded)
  ,m_bIsExpandedActually (b_is_expanded)//
{
    setInitialSettings(i_parent_node_id, str_name, i_tree_id);
    m_docDescriptor.setHtml(str_html_descriptor);
    //
    m_ptrParentTree = (MyTree *) parent_tree;
    Q_ASSERT (m_ptrParentTree);
};

TreeLeaf::~TreeLeaf()
{
    //
    //delete attachments list
    //
    for (unsigned int i = 0; i < m_vAttachments.size();  ++i)
    {
        delete m_vAttachments[i];
    };
};

void TreeLeaf::saveToDBExpandStateChange ()
{
    if ( m_bIsExpandedActually != m_bIsExpandedDB)
    {
        updateExpandState_DB();
        //
        m_bIsExpandedDB = m_bIsExpandedActually;
    };
}

void TreeLeaf::placeStatusBarMsg (const QString& str_message)
{
    if (m_ptrParentTree)
        m_ptrParentTree->getStatusBar()->showMessage( str_message );
};

bool  TreeLeaf::exportNode ( const QString& str_directory_path, bool b_recursive_export)
{
    placeStatusBarMsg(tr("Exporting node ") + m_strDatabaseNodeName + "..." );
    //create export directory
    QString str_new_dir = str_directory_path + tr("//") + m_strDatabaseNodeName;
    QDir d_manager;
    bool b_create_success = d_manager.mkdir(str_new_dir);
    if (false == b_create_success)
    {
        placeStatusBarMsg (tr("Unable create ") + str_new_dir);
        return false;
    };
    //if created - create file with the same name and ".txt" on the same level
    QFile file( str_new_dir + tr(".txt") );
    bool b_create_descriptor = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if ( b_create_descriptor == false)
    {
        placeStatusBarMsg( str_new_dir + tr(".txt") );
        return false;
    };
    QTextStream out(&file);
    out << m_docDescriptor.toHtml();
    file.close();
    //
    QString str_message = QString(" (id = %1) name='%2' to '%3'").arg(this->m_iID).arg(this->text(0)).arg(str_directory_path);
    Logger::getInstance().logIt(en_LOG_EXPORT_NODE, str_message );
    //
    //request list of attachments from the server
    requestAttachments ();
    //save all attachments into created directory
    Attachment::AttachmentsList::const_iterator itr_attach;
    //
    for (itr_attach = m_vAttachments.begin(); itr_attach != m_vAttachments.end(); ++itr_attach)
    {
        Attachment* ptr_attachment = (*itr_attach);
        //
        ptr_attachment->export_it(str_new_dir);
        placeStatusBarMsg( tr("Attachment ") + ptr_attachment->getName() + tr(" exported.") );
    };
    //
    placeStatusBarMsg( tr("Node ") + m_strDatabaseNodeName + tr(" exported.") );
    //if export is recursive:
    if (b_recursive_export == false)
        return true;
    //call export for all childs
    for (int i = 0; i < this->childCount(); i++)
    {
        TreeLeaf* ptr_actual = (TreeLeaf*) this->child(i);
        Q_ASSERT ( ptr_actual );
        //
        ptr_actual->exportNode(str_new_dir, b_recursive_export);
    };
    //
    return true;
};

bool TreeLeaf::isPossibleToExportNode() const
{
    //check node status
    if(
            (this->getObjectStatus() != OBJECT_OK) &&
            (this->getObjectStatus() != OBJECT_DELETED)
      )
    {
        return false;
    };
    //
    //check attachments status. If attachment was not requested - it is OK, nothing is
    // updated now
    //
    Attachment::AttachmentsList::const_iterator itr_attach;
    //
    for (itr_attach = m_vAttachments.begin(); itr_attach != m_vAttachments.end(); ++itr_attach)
    {
        Attachment* ptr_attachment = (*itr_attach);
        //
        if(
                (ptr_attachment->getObjectStatus() != OBJECT_OK) &&
                (ptr_attachment->getObjectStatus() != OBJECT_DELETED)
          )
        {
            return false;
        };
    };
    //
    for (int i = 0; i < this->childCount(); i++)
    {
        TreeLeaf* ptr_actual = (TreeLeaf*) this->child(i);
        Q_ASSERT ( ptr_actual );
        //
        bool b_res = ptr_actual->isPossibleToExportNode();
        //
        if (false == b_res)
            return b_res;
    };
    //
    return true;
};

bool TreeLeaf::importNode ( const QString& str_directory_path, bool b_recursive_import )
{
    //get name of created node
    QDir income_dir (str_directory_path);
    //QString str_new_node_name       = income_dir.dirName();
    QStringList str_list_of_dirs    = income_dir.entryList(QDir::AllDirs|QDir::NoDotAndDotDot|QDir::Readable|QDir::NoSymLinks);
    QStringList str_list_of_files   = income_dir.entryList(QDir::Files|QDir::Readable|QDir::NoSymLinks);

    int i_rem_index = str_list_of_files.indexOf("index.txt",0);
    if (-1 != i_rem_index)
        str_list_of_files.removeAt(i_rem_index);
    //check all list of dirs, extract "dir name" add ".txt"
    QStringList::const_iterator itr = str_list_of_dirs.begin();
    for (; itr != str_list_of_dirs.end(); ++itr)
    {
        QDir current_dir (*itr);
        QString str_new_node_name = current_dir.dirName();
        QString str_descriptor_file_name = str_new_node_name + tr(".txt");
        //call search in file list here
        int i_file_index = str_list_of_files.indexOf(str_descriptor_file_name);
        //
        QString str_new_node_descriptor;
        //
        if (-1 != i_file_index)
        {
            QFile f (str_directory_path + g_strPATH_DELIMETER + str_descriptor_file_name);
            if ( f.open(QIODevice::ReadOnly) )
            {
                QTextStream stream( &f );
                //
                //int i = 1;
                while ( !stream.atEnd() )
                {
                    str_new_node_descriptor += stream.readLine(); // line of text excluding '\n'
                    str_new_node_descriptor += tr("\n");
                };
                //
                f.close();
                //
                str_list_of_files.removeAt(i_file_index);
            };
        };
        // create child node here using str_new_node_name , str_new_node_descriptor
        TreeLeaf* ptr_new_node = new TreeLeaf(this,
                                              -1,
                                              this->getTreeID(),
                                              str_new_node_name,
                                              m_ptrParentTree );
        ptr_new_node->setDescriptor(str_new_node_descriptor);
        // if b_recursive_import
        if (true == b_recursive_import)
        {
            ptr_new_node->importNode(str_directory_path + g_strPATH_DELIMETER + str_new_node_name, b_recursive_import);
        };
        // call import for this node, pass str_directory_path + "/" + str_new_node_name
        continue;
    };
    //all nodes created, the rest in the str_list_of_files is attachments.
    QStringList str_attachments;
    //
    QStringList::const_iterator itr_files = str_list_of_files.begin();
    //
    for (; itr_files != str_list_of_files.end(); ++itr_files)
    {
        QString str_new_attach = str_directory_path + g_strPATH_DELIMETER + (*itr_files);
        str_attachments.append(str_new_attach);
    };
    //
    // TODO node attachments imported as non-protected.
    //
    if (str_attachments.size() > 0)
        this->addAttachments(str_attachments,false, false, false);
    //
    return true;
};

void TreeLeaf::setInitialSettings(int i_parent_id, const QString &str_name, int i_tree_id)
{
    setAutoDelete(false); //do not delete instance of the object after the "run" execution
    setItemColor();
    //
    m_iParentID = i_parent_id;  // id of the parent node (id_parent_node). For the trop-levels node it is 0
    //
    m_iDBTreeID = i_tree_id;
    //
    this->setText(0, str_name);
    //
    m_strDatabaseNodeName = str_name;
    //
    //if (0 == m_iParentID)
        this->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable);
    //else
    //    this->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable/*|Qt::ItemIsDropEnabled|Qt::ItemIsDragEnabled*/);
    //
    //no attachments requested yet.
    //
    m_bIsAttachmentRequestedAlready = false;
    //
    //force delete is disabled
    //
    m_bForceDelete = false;
}

void TreeLeaf::notifyAboutAttachmentUpdate(bool b_result, QString str_attachment_name)
{
    if (m_ptrParentTree)
        m_ptrParentTree->leafAttachmentUpdated(this,b_result,str_attachment_name);
};

void TreeLeaf::addAttachments  (const QStringList& file_name_list, bool b_delete_files_after, bool b_protect_attachments, unsigned int ui_encrypt_type)
{
    //for preventing second request after click on node
    m_bIsAttachmentRequestedAlready = true;
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );

    const unsigned int ui_max_allow_file_size = settings.value(g_str_ATTACH_MAX_SIZE_MB).toInt();
    //
    QStringList::const_iterator constIterator;
    //
    for (constIterator = file_name_list.constBegin(); constIterator != file_name_list.constEnd(); ++constIterator)
    {
        //check the size of attachment, compare with the max. allow size
        QFileInfo file_info ((*constIterator));
        if ( file_info.exists() )
        {
            const unsigned int ui_current_file_size = file_info.size();
            if ( ui_current_file_size/1000000 <= ui_max_allow_file_size)
            {
                //create attachment and add it into database
                Attachment* ptr_new_attachment = new Attachment((*constIterator), this, b_delete_files_after, b_protect_attachments, ui_encrypt_type);
                m_vAttachments.push_back(ptr_new_attachment); //create new attachments
                //
                //QString str_message =
                placeStatusBarMsg( tr("Importing ") + (*constIterator) + tr("...") );
            }else
            {
                //TODO: put in into log
                placeStatusBarMsg( tr("Attach ") + (*constIterator) + tr(" rejected. Too big.") );
            };
        };
    };
    //push all attachments object into threadpool, make loading into the dababase
  {
    QMutexLocker locker(&m_RequestAttachLocker);

    Attachment::AttachmentsList::const_iterator itr_attach;
    //
    for (itr_attach = m_vAttachments.begin(); itr_attach != m_vAttachments.end(); ++itr_attach)
    {
        //add created attachment into database
        Attachment* ptr_attachment = (*itr_attach);
        //
        QThreadPool::globalInstance()->start(ptr_attachment);
    };
  };
    //inform parent tree (for updating the status of attachments through message)
    //
    placeStatusBarMsg( tr("Import done") );
    //
    notifyAboutAttachmentUpdate(true, tr(""));
};

AbstractDatabaseObject::DB_OBJECT_STATUS TreeLeaf::getObjectStatus () const
{
    Attachment::AttachmentsList::const_iterator itr;
    //
    for (itr = m_vAttachments.begin(); itr != m_vAttachments.end(); ++itr)
    {
        if ( (*itr)-> getObjectStatus() == OBJECT_ATTACHMENT_INSERT )
            return OBJECT_ATTACHMENT_INSERT;
        //
        if ( (*itr)-> getObjectStatus() == OBJECT_UPDATING )
            return OBJECT_UPDATING;
    };
    //
    return AbstractDatabaseObject::getObjectStatus();
};

bool TreeLeaf::rename_it (const QString &str_name)
{
    Q_UNUSED(str_name);
    //
    QString str_new_name = this->text(0);
    //
    if (str_new_name.length() == 0)
        return false;
    //
    if (m_strDatabaseNodeName == str_new_name)
        return false;
    //
    setObjectStatus(OBJECT_NAME_UPDATING);
    //
    QThreadPool::globalInstance()->start(this);
    //
    return true;
}

void  TreeLeaf::setDescriptor  (const QString& str_html_descriptor)
{
    if (str_html_descriptor == m_docDescriptor.toHtml())
        return;
    //
    {
        //bug in Qt, this code is NOT useless.
        m_docDescriptor.setHtml(str_html_descriptor);
        QString str_original_html = m_docDescriptor.toHtml();
    }
    //
    setObjectStatus(OBJECT_DESCRIPTOR_UPDATING);
    //
    QThreadPool::globalInstance()->start(this);
}

void TreeLeaf::dropAttachments             (bool b_recursive)
{
    if (m_vAttachments.size() > 0)
    {
        Attachment::AttachmentsList::const_iterator itr = m_vAttachments.begin();
        //
        for (; itr != m_vAttachments.end(); ++itr)
        {
            delete (*itr);
        };
        //
        m_vAttachments.erase( m_vAttachments.begin(), m_vAttachments.end() );
    };
    //
    m_bIsAttachmentRequestedAlready = false;
    //
    if (b_recursive)
    {
        for (int i = 0; i < this->childCount(); i++)
        {
            TreeLeaf* ptr_actual = (TreeLeaf*) this->child(i);
            Q_ASSERT ( ptr_actual );
            //
            ptr_actual->dropAttachments(b_recursive);
        };
    };
}

void  TreeLeaf::getAttachments ( Attachment::AttachmentsList& v_attachments, bool b_recursive )
{
    const Attachment::AttachmentsList& v_current_attach = getAttachments();
    //
    Attachment::AttachmentsList::const_iterator itr = v_current_attach.begin();
    //
    for (; itr != v_current_attach.end(); ++itr)
    {
        v_attachments.push_back(*itr);
    };
    //
    if (b_recursive)
    {
        for (int i = 0; i < this->childCount(); i++)
        {
            TreeLeaf* ptr_actual = (TreeLeaf*) this->child(i);
            Q_ASSERT ( ptr_actual );
            //
            ptr_actual->getAttachments(v_attachments, b_recursive);
        };
    };
}
/*
void TreeLeaf::encr_decr_all_attachments (bool b_ecnrypt, bool b_recursive)
{
    const TreeLeaf::AttachmentsList& v_attachments  = getAttachments();
    TreeLeaf::AttachmentsList::const_iterator itr = v_attachments.begin();
    //
    for (; itr != v_attachments.end(); ++itr)
    {
        if (b_ecnrypt)
            (*itr)->encrypt_it();
        else
            (*itr)->decrypt_it();
    };
    //
    if (b_recursive)
    {
        for (int i = 0; i < this->childCount(); i++)
        {
            TreeLeaf* ptr_actual = (TreeLeaf*) this->child(i);
            Q_ASSERT ( ptr_actual );
            //
            ptr_actual->encr_decr_all_attachments(b_ecnrypt, b_recursive);
        };
    };
};
*/
const Attachment::AttachmentsList&  TreeLeaf::getAttachments()
{
    QMutexLocker locker (&m_RequestAttachLocker);
    //
    requestAttachments ();

    return m_vAttachments;
}

//TreeLeaf::AttachmentsList&  TreeLeaf::getAttachments  ();


void  TreeLeaf::requestAttachments ()
{
    //
    if (m_bIsAttachmentRequestedAlready)
        return;
    //
    getAttachments_DB();
    //
    m_bIsAttachmentRequestedAlready = true;
}

void TreeLeaf::deleteAllAttachments_DB()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_update_string = QString("update attachments set active = 0 where id_parent = %1;").arg(m_iID);
    //
    if (! qry.prepare( str_update_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
    };
};

void TreeLeaf::getAttachments_DB()
{
    QMessageBox box;
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    Q_ASSERT(ptr_db);
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_select_str = QString("select id_attach, attach_name, attach_size, active, crypted, locked, is_binary, last_change from attachments WHERE ID_PARENT = %1;").arg(m_iID);
    //
    if (!qry.prepare( str_select_str ))
    {
        QString str_err_txt = qry.lastError().text();
        box.setText( "Unable to get exec the query. "+ str_err_txt );
        box.exec();
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return;

    } else if( !qry.exec() )
    {
        QString str_err_txt = qry.lastError().text();

        box.setText( "Unable to get exec the query. "+ str_err_txt );
        box.exec();
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return;
    }
    //
    while( qry.next() )
    {
        int         id_attach         = qry.value(0).toInt();
        QString     str_attach_name   = qry.value(1).toString();
        int         i_attach_size     = qry.value(2).toInt();
        bool        b_attach_active   = qry.value(3).toBool();
        int         i_attach_crypted  = qry.value(4).toInt();
        bool        b_attach_locked   = qry.value(5).toBool();
        bool        b_attach_binary   = qry.value(6).toBool();
        QDateTime   dt_time_change    = qry.value(7).toDateTime();
        //
        Attachment* ptr_new_attachment = new Attachment(id_attach,
                                                        str_attach_name,
                                                        i_attach_size,
                                                        b_attach_active,
                                                        i_attach_crypted,
                                                        b_attach_locked,
                                                        b_attach_binary,
                                                        dt_time_change,
                                                        this);
        //
        m_vAttachments.push_back(ptr_new_attachment);
    }; // while( qry.next() )
}

bool  TreeLeaf::updateDescriptor_DB()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    const QString str_html_string = m_docDescriptor.toHtml();
    QString str_update_string = QString("UPDATE node_tbl SET node_descriptor = :DESCRIPTOR WHERE id_node = :ID;");
    //
    if ( !qry.prepare( str_update_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    qry.bindValue(":DESCRIPTOR",   str_html_string );
    qry.bindValue(":ID",   m_iID );
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    return true;
}

void  TreeLeaf::updateExpandState_DB()
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
    unsigned int ui_expanded_actually = 0;
    if (m_bIsExpandedActually)
    {
        ui_expanded_actually = 1;
    };
    //
    QString str_update_string = QString ("UPDATE node_tbl SET expanded = %1  WHERE id_node = %2;").arg(ui_expanded_actually).arg(m_iID);
    //
    if (! qry.prepare( str_update_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
    };
}

TreeLeaf::ChildList TreeLeaf::getChildList ()
{
    ChildList childs;
    //
    for (int i = 0; i < this->childCount(); i++)
    {
        TreeLeaf* ptr_actual = (TreeLeaf*) this->child(i);
        Q_ASSERT ( ptr_actual );
        //
        childs.push_back(ptr_actual);
    };
    //
    return childs;
}

TreeLeaf* TreeLeaf::getChildNode    (int i_node_id)
{
    if (this->getID() == i_node_id)
        return this;
    //
    for (int i = 0; i < this->childCount(); i++)
    {
        TreeLeaf* ptr_actual = (TreeLeaf*) this->child(i);
        Q_ASSERT ( ptr_actual );
        //
        TreeLeaf* ptr_next = ptr_actual->getChildNode(i_node_id);
        //
        if (ptr_next != NULL)
            return ptr_next;
    };
    //
    return NULL;
};

TreeLeaf* TreeLeaf::getParent()
{
    return (TreeLeaf*) QTreeWidgetItem::parent();
}

void TreeLeaf::setNewParent(TreeLeaf* ptr_new_parent)
{
    if (NULL == ptr_new_parent)
        return;
    //get old parent
    TreeLeaf* ptr_old_parent = getParent();
    if (NULL == ptr_old_parent)
        return; //can not move top-node to somewere else
    //
    ptr_old_parent->removeChild(this);
    ptr_new_parent->addChild(this);
    //
    if ( ptr_new_parent->getID() != (int) m_iParentID )
    {
        m_iParentID = ptr_new_parent->getID();
        //
        updateParentInfoIn_DB();
        //
        setNewTreeID( ptr_new_parent->getTreeID() );
        //
        QString str_message = QString("(id = %1) name='%2' from old parent (id=%3, name ='%4' tree_id = %5) to new parent (id=%6, name ='%7' tree_id = %8)").
                arg(m_iID).arg(this->text(0)).arg( ptr_old_parent->getID() ).arg( ptr_old_parent->text(0) ).arg(ptr_old_parent->getTreeID()).
                arg( ptr_new_parent->getID() ).arg( ptr_new_parent->text(0) ).arg(ptr_new_parent->getTreeID());
        //
        Logger::getInstance().logIt(en_LOG_MOVE_NODE, str_message );
    };
};

void TreeLeaf::setNewTreeID(unsigned int ui_tree_id)
{
    if (m_iDBTreeID == ui_tree_id)
        return;  //do nothing, it is the same tree
    //
    m_iDBTreeID = ui_tree_id;
    //
    setObjectStatus(OBJECT_TREE_ID_UPDATING);
    //
    QThreadPool::globalInstance()->start(this);
    //
    for (int i = 0; i < this->childCount(); i++)
    {
        TreeLeaf* ptr_child_leaf =  (TreeLeaf*) this->child(i);
        if (ptr_child_leaf)
            ptr_child_leaf->setNewTreeID(ui_tree_id);
    };
};

void TreeLeaf::updateTreeID_DB()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_update_string = QString( "UPDATE node_tbl SET id_tree = %1 WHERE id_node = %2;").arg(m_iDBTreeID).arg(m_iID);
    //
    if (! qry.prepare( str_update_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
    };
};

void TreeLeaf::updateParentInfoIn_DB()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_update_string = QString("UPDATE node_tbl SET id_parent = %1 WHERE id_node = %2;").arg(m_iParentID).arg(m_iID);
    //
    if( !qry.prepare( str_update_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
    };
};
//
// Thread pool processes
//
void TreeLeaf::run()
{
    QMutexLocker locker(&m_RunLocker);
    //
    switch(getObjectStatus())
    {
    case AbstractDatabaseObject::OBJECT_OK:
        return;
    case AbstractDatabaseObject::OBJECT_NAME_UPDATING:
        {
            bool b_update_name = updateName_DB();
            if (b_update_name)
            {
                QString str_message = QString("(id = %1) new name='%2'").arg(m_iID).arg(this->text(0));
                Logger::getInstance().logIt(en_LOG_UPDATE_NODE, str_message );
            };
        };
        break;
    case AbstractDatabaseObject::OBJECT_TREE_ID_UPDATING:
        updateTreeID_DB();
        break;
    case AbstractDatabaseObject::OBJECT_DESCRIPTOR_UPDATING:
        {
            bool b_update = updateDescriptor_DB();
            if (b_update)
            {
                QString str_message = QString("(id = %1) name='%2' updated").arg(m_iID).arg(this->text(0));
                Logger::getInstance().logIt(en_LOG_DESCRIPTOR_NODE, str_message );
            };
        };
        break;
    case AbstractDatabaseObject::OBJECT_DELETING:
        setObjectStatus(OBJECT_DELETED);
        deleteAllAttachments();
        setActiveStatus_DB(false);
        setItemColor();
        notifyAboutAttachmentUpdate(true, tr(""));
        {
            QString str_message = QString("(id = %1) name='%2'").arg(m_iID).arg(this->text(0));
            Logger::getInstance().logIt(en_LOG_DELETE_NODE, str_message );
        }
        return;
    case AbstractDatabaseObject::OBJECT_ATTACHMENT_INSERT: //do nothing
        return;
    default:
/*
#ifdef QT_DEBUG
        DB_OBJECT_STATUS status = getObjectStatus(); //just for check under debugger
#endif
*/
        Q_ASSERT( FALSE );
    };
    //
    setObjectStatus(AbstractDatabaseObject::OBJECT_OK);
    //
//    m_RunLocker.unlock();
};

void TreeLeaf::setItemColor()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    QColor fg_color;
    QColor bg_color;
    //
    switch(getObjectStatus())
    {
    case OBJECT_DELETED:
        {
            fg_color = settings.value(g_str_CLR_DELETED_FG).value<QColor>();
            bg_color = settings.value(g_str_CLR_DELETED_BG).value<QColor>();
        };
        break;
    default:
        {
            fg_color = settings.value(g_str_CLR_NORNAL_FG).value<QColor>();
            bg_color = settings.value(g_str_CLR_NORNAL_BG).value<QColor>();
        }
        break;
    //TODO: add colors for updating items
    };
    //
    QBrush fg_brash   ( fg_color );
    QBrush bg_brash   ( bg_color );
    //
    this->setForeground(0,fg_brash);
    this->setBackground(0,bg_brash);
};

bool TreeLeaf::isPossibleToDeleteNode(bool b_silence, bool b_force)
{
    QMessageBox box;
    QString str_msg;
    bool b_possible_to_delete = true;
    //
    if ( ( 0 == m_iParentID ) && (false == b_force) )
    {
        str_msg = "Can not delete root node.";
        b_possible_to_delete = false;
    }
    //
    if (
            ( getObjectStatus() >= OBJECT_UPDATING )        &&
            ( getObjectStatus() <= OBJECT_UPDATING_STOP )   &&
            b_possible_to_delete
       )
    {
        str_msg = QString("Can not delete node '%1', it is updating now.").arg(this->text(0));
        b_possible_to_delete = false;
    };
    //
    if( b_possible_to_delete )
        b_possible_to_delete = isPossibleToDeleteAttachments(str_msg);
    //
    if (false == b_possible_to_delete)
    {
        if (false == b_silence)
        {
            box.setText(str_msg);
            box.exec();
        };
    };
    //
    if( b_possible_to_delete )
    {
        //ask all childs and all childs attachments.
        for (int i = 0; i < this->childCount(); i++)
        {
            TreeLeaf* ptr_child_leaf =  (TreeLeaf*) this->child(i);
            if (ptr_child_leaf->isPossibleToDeleteNode(b_silence) == false)
            {
                b_possible_to_delete = false;
                break;
            };
        };
    };
    //
    return b_possible_to_delete;
}

bool  TreeLeaf::isPossibleToDeleteAttachments(QString& str_msg)
{
    bool b_possible_to_delete = true;
    //
    for (unsigned int i = 0; i < m_vAttachments.size();  ++i)
    {
        if (
                (m_vAttachments[i]->getObjectStatus() != AbstractDatabaseObject::OBJECT_OK)         &&
                (m_vAttachments[i]->getObjectStatus() != AbstractDatabaseObject::OBJECT_DELETED)    &&
                (m_vAttachments[i]->getObjectStatus() != AbstractDatabaseObject::OBJECT_NOT_DEFINED)
           )
        {
            str_msg = QString("Can not delete '%1', attachment '%2' is updating now.").arg(this->text(0)).arg(m_vAttachments[i]->getName());
            b_possible_to_delete = false;
            break;
        };
        //
        if( m_vAttachments[i]->is_protected() )
        {
            str_msg = QString("Can not delete node '%1', attachment '%2' protected.").arg(this->text(0)).arg(m_vAttachments[i]->getName());
            b_possible_to_delete = false;
            break;
        };
    };
    //
    return b_possible_to_delete;
}

bool TreeLeaf::force_delete_it ()
{
    m_bForceDelete = true;
    delete_it(true);
    return true;
};

bool TreeLeaf::delete_it(bool b_silence)
{
    if ( false == isPossibleToDeleteNode( b_silence, m_bForceDelete) )
        return false;
    //
    //already deleted, nothing to delete
    //
    if (getObjectStatus() == OBJECT_DELETED)
        return true;
    //
    int ret = QMessageBox::Ok;
    //
    if ( this->childCount() > 0 )
    {
        //
        if (false == b_silence)
        {
            QString str_msg = "You will delete all child nodes also.";
            QMessageBox box;
            box.setInformativeText(str_msg);
            box.setStandardButtons(QMessageBox::Ok| QMessageBox::Cancel);
            box.setDefaultButton(QMessageBox::Ok);
            ret = box.exec();
        };
        //
    };
    //
    if (ret == QMessageBox::Ok)
    {
        setObjectStatus( OBJECT_DELETING );
        //
/*
        //delete all attachments also
        deleteAllAttachments();
        //
        //TODO: push it into thread pool?
        //
        setActiveStatus_DB(false);
*/
        QThreadPool::globalInstance()->start(this);
        //
        for (int i = 0; i < this->childCount(); i++)
        {
            TreeLeaf* ptr_child_leaf =  (TreeLeaf*) this->child(i);
            if (ptr_child_leaf)
                ptr_child_leaf->delete_it(true);
        };
    }else
    {
        return false;
    };
    //
    QString str_msg1 = tr("node ");
    str_msg1 += this->text(0);
    str_msg1 += tr(" deleted.");
    //
    placeStatusBarMsg(str_msg1);
    //
    return true;
}

void TreeLeaf::deleteAllAttachments()
{
    deleteAllAttachments_DB();
    //    
    for (unsigned int i = 0; i < m_vAttachments.size();  ++i)
    {
        m_vAttachments[i]->setObjectStatus(AbstractDatabaseObject::OBJECT_DELETED);
        //
        QString str_message = QString("Attach (id = %1) name '%2'").arg( m_vAttachments[i]->getID() ).arg( m_vAttachments[i]->getName() );
        Logger::getInstance().logIt(en_LOG_DELETE_ATTACH, str_message );
    };
};

void  TreeLeaf::restoreAllAttachments()
{
    QMutexLocker locker (&m_RequestAttachLocker);
    //
    Attachment::AttachmentsList::iterator itr_attach;
    //
    for (itr_attach = m_vAttachments.begin(); itr_attach != m_vAttachments.end(); ++itr_attach)
    {
        (*itr_attach)->restoreObject();
        //restore in database by 1 request later
    };
}

bool TreeLeaf::isPossibleToRestoreNode()
{
    if ( OBJECT_OK == getObjectStatus() )
        return false; //already OK
    //
    QString str_msg;
    //TODO: check: are all parents OK?
    TreeLeaf* ptr_parent = this->getParent();
    if (ptr_parent)
    {
        if (OBJECT_DELETED == ptr_parent->getObjectStatus())
        {
            QMessageBox box;
            str_msg ="parent node also deleted. Restore it first";
            box.setText(str_msg);
            box.exec();
            return false;
        };
    };
    //
    return true;
}

bool TreeLeaf::restoreObject()
{
    if ( isPossibleToRestoreNode() == false )
        return false;
    //
    QString str_msg;
    //
    setActiveStatus_DB(true);
    //
    setObjectStatus(OBJECT_OK);
    //
    str_msg = "node ";
    str_msg += this->text(0);
    str_msg += " restored.";
    //
    placeStatusBarMsg(str_msg);
    //
    setItemColor();
    //
    return true;
};

bool  TreeLeaf::restoreSubtree  ()
{
    if ( isPossibleToRestoreNode() == false )
        return false;
    //
    setActiveStatus_DB(true);
    //
    setObjectStatus(OBJECT_OK);
    //
    QString str_msg;
    str_msg = "node ";
    str_msg += this->text(0);
    str_msg += " restored.";
    //
    placeStatusBarMsg(str_msg);
    //
    setItemColor();
    //restore all attachments
    restoreAllAttachments();
    //
    for (int i = 0; i < this->childCount(); i++)
    {
        TreeLeaf* ptr_child_leaf =  (TreeLeaf*) this->child(i);
        if (ptr_child_leaf)
            ptr_child_leaf->restoreSubtree();
    };
    //
    return true;
};

int  TreeLeaf::insertTo_DB()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return -1;
    //
    QSqlQuery qry(*ptr_db);
    //INSERT INTO into node_tbl (id_parent, id_tree, node_name, node_descriptor) values (0, 1, 'Root node Tree 1', 'empty_desciptor');
    QString str_insert_string;
    //
    if (m_docDescriptor.toPlainText().length() > 0) //check plaintext because toHtml always gets at least empty HTML-document
    {
        str_insert_string = QString("INSERT INTO node_tbl (id_parent, id_tree, node_name, node_descriptor) values (:ID_PARENT, :ID_TREE, :NODENAME, :DESCRIPTOR);");
    }else
    {
        str_insert_string = QString("INSERT INTO node_tbl (id_parent, id_tree, node_name) values (:ID_PARENT, :ID_TREE, :NODENAME);");
    };
    //
    if (!qry.prepare( str_insert_string ))
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return -1;
    };
    //
    if (m_docDescriptor.toPlainText().length() > 0) //because use for import (where descriptor can exist) and for new created node, where can not.
    {
        qry.bindValue(":ID_PARENT",   this->getParentID() );
        qry.bindValue(":ID_TREE",     this->getTreeID()   );
        qry.bindValue(":NODENAME",    this->text(0)       );
        qry.bindValue(":DESCRIPTOR",  m_docDescriptor.toHtml() );
    }
    else
    {
        qry.bindValue(":ID_PARENT",   this->getParentID() );
        qry.bindValue(":ID_TREE",     this->getTreeID()   );
        qry.bindValue(":NODENAME",    this->text(0)       );
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return -1;
    };
    //
    m_iID = qry.lastInsertId().toInt();
    //
    return m_iID;
}

bool TreeLeaf::updateName_DB()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_update_string = QString("UPDATE node_tbl SET node_name = :NAME WHERE id_node = :ID;");
    //
    if(! qry.prepare( str_update_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    qry.bindValue(":NAME", this->text(0));
    qry.bindValue(":ID",   m_iID);
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    m_strDatabaseNodeName = this->text(0);
    return true;
};

void TreeLeaf::setActiveStatus_DB(bool b_active)
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
    QString str_update_string;
    //
    unsigned int ui_is_active = 0;
    if (b_active)
        ui_is_active = 1;
    //
    //update node_tbl set active = 0 where id_node = 10;
    //
    str_update_string = QString ("update node_tbl set active = %1  WHERE id_node = %2;").arg(ui_is_active).arg(m_iID);
    //
    if( !qry.prepare( str_update_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return;
    };
    //
    if( !qry.exec() )
      Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
};

void  TreeLeaf::removeAttachment(Attachment* ptr_attachment)
{
    QMutexLocker locker (&m_RequestAttachLocker);
    //
    Attachment::AttachmentsList::iterator itr_attach;
    //
    for (itr_attach = m_vAttachments.begin(); itr_attach != m_vAttachments.end(); ++itr_attach)
    {
        if ( (*itr_attach)->getID() == ptr_attachment->getID() )
        {
            m_vAttachments.erase(itr_attach);
            break;
        };
    };
}

void TreeLeaf::addAttachment (Attachment* ptr_attachment)
{
    if ( NULL == ptr_attachment )
        return;
    //
    QMutexLocker locker (&m_RequestAttachLocker);
    //
    //register changing of parent_id into database
    //
    m_vAttachments.push_back(ptr_attachment);
};
