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
#include "attachment.h"
#include "dbacccesssafe.h"
#include "treeleaf.h"
#include "logger.h"
#include "tmpfilecleaner.h"
#include "servicescfg.h"
#include "../CommonInclude/pdb/cryptosupport.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QFile>
#include <QSettings>
#include <QtSql>
#include <QThreadPool>
#include <QMutexLocker>
#include <QMessageBox>
#include <QTime>
//
//#include <QCryptographicHash>
//
// from interface
//
Attachment::Attachment(const QString&      full_path_filename,
                       TreeLeaf*           ptr_parent_leaf,
                       bool                b_delete_files_after,
                       bool                b_protect_attachment,
                       unsigned int        ui_encrypt_type):
    AbstractDatabaseObject(-1,               //attachment has no ID now.
                           true,             // attachment active
                           QDateTime ( QDateTime::currentDateTime() ),
                           NULL)
{
    m_pParentNode            = ptr_parent_leaf;
    m_iSize                  = 0;
    m_strFilePath            = full_path_filename;
    m_iCryptType             = 0;
    m_bIsProtected           = b_protect_attachment;
    m_bDeleteFileAfterUpload = b_delete_files_after;
    m_iCryptType             = ui_encrypt_type;         //attachment must be encrypted in the thread pool using default key
    //
    makeAttachmentName(full_path_filename);
    //
    setAutoDelete(false); //do not delete instance of the object after "run" finished
    //
    if (readFileToMemory(m_strFilePath) == true)
        setObjectStatus(OBJECT_ATTACHMENT_INSERT);
    //I do not put it into threadpool here because I want to minimize locking time
    else
    {
        setObjectStatus(OBJECT_NOT_DEFINED);
        Q_ASSERT (FALSE);
    }
}
//
//from database
//
Attachment::Attachment(int                  i_id,
                       const QString&       t_str_attach_name,
                       int                  i_attach_size,
                       bool                 b_active,
                       int                  i_crypt_type,
                       bool                 b_protected,
                       bool                 b_binary,
                       const QDateTime&     dt_last_change,
                       TreeLeaf*            ptr_parent_leaf):
    AbstractDatabaseObject(i_id,
                           b_active,           // attachment active
                           dt_last_change,
                           NULL)
{
    m_iSize         = i_attach_size;
    m_strAttachName = t_str_attach_name;
    m_pParentNode   = ptr_parent_leaf;
    m_iCryptType    = i_crypt_type;
    m_bIsProtected  = b_protected;
    m_bIsBinary     = b_binary;
    //
    setAutoDelete(false); //do not delete instance of the object after the "run" execution
    //
    if (b_active)
        setObjectStatus(OBJECT_OK);
    else
        setObjectStatus(OBJECT_DELETED);
}

TreeLeaf* Attachment::getParentLeaf()
{
    return m_pParentNode;
};

void Attachment::makeAttachmentName(QString str_full_path_name)
{
    QFileInfo pathInfo( str_full_path_name );
    //
    m_strAttachName = pathInfo.fileName();
}

void Attachment::run()
{
    QMutexLocker locker (&m_RunLocker);
    //
    //
    switch (getObjectStatus())
    {
    case OBJECT_OK:
        return; //do nothing
    case OBJECT_DELETED:
        return; //do nothing
    case OBJECT_NOT_DEFINED:
        return; //do nothing
    case OBJECT_ATTACHMENT_INSERT:
        insert_new_attacment ();
        return;
    case OBJECT_NAME_UPDATING:
        {
            const bool b_rename = updateName_DB();
            if (b_rename)
            {
                const QString str_message = QString("Attach (id = %1) renamed (%2 => %3) ").arg(m_iID).arg(m_strOldAttachName).arg(m_strAttachName);
                Logger::getInstance().logIt(en_LOG_RENAME_ATTACH, str_message );
            }else
            {
                m_strAttachName = m_strOldAttachName;
            };
            setObjectStatus(OBJECT_OK);
        }
        return;
    default:
        Q_ASSERT ( false );
        return;
    };
}

void Attachment::insert_new_attacment ()
{
    if (CryptoSupport::NO_ENCRYPT != m_iCryptType)
    {
        //QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
        //const unsigned int ui_default_encrypt_method = settings.value(g_str_SEC_TEC_CODE).value<unsigned int>();
        //
        QByteArray encrypted_aray;
        CryptoSupport crypto;
        bool b_encrypt = crypto.encrypt_bytearray( (CryptoSupport::SymmetricMethods) m_iCryptType, ServicesCfg::getInstance().getPassword(), m_ByteArray, encrypted_aray);
        //
        if (false == b_encrypt)
        {
            m_iCryptType = CryptoSupport::NO_ENCRYPT;
            m_ByteArray.clear();
            return;
        };
        //
        m_ByteArray     = encrypted_aray;
        m_bIsBinary     = true;
    }else
    {
        //m_bIsBinary - TODO (calc it)
    };
    //
    m_iID = insertTo_DB ();
    //
    if (-1 == m_iID)
    {
        setObjectStatus(OBJECT_NOT_DEFINED);
    }else
    {
        delOriginalFiles();
        setObjectStatus(OBJECT_OK);
        //
        const QString str_message = QString("(id = %1) name='%2'").arg(m_iID).arg(m_strAttachName);
        Logger::getInstance().logIt(en_LOG_CREATE_ATTACH, str_message );
    };
    //
    if(m_pParentNode)
        m_pParentNode->notifyAboutAttachmentUpdate(true,tr(""));
}


bool Attachment::decrypt_it ()
{
    if (false == is_crypted() )
        return true;            //nothing to do, already decrypted
    //
 //   if (m_pParentNode)
 //       m_pParentNode->placeStatusBarMsg("Decrypting " + m_strAttachName);
    //
    bool b_extract = extractBlobToByteArray_DB();
    //
    if (false == b_extract)
        return false;
    //
    unsigned int ui_old_encrypt_type = m_iCryptType;
    m_iCryptType = 0;
    //
    bool b_replace = replaceBlob_DB();
    //
    if (false == b_replace)
    {
        m_iCryptType = ui_old_encrypt_type;
        m_ByteArray.clear();
        return false;
    };
    //
    m_ByteArray.clear();
    //
//    if (m_pParentNode)
//        m_pParentNode->placeStatusBarMsg(m_strAttachName + " decrypted");
    //
    return true;
}

bool Attachment::encrypt_it () //
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //always crypt with the default tec of crypting
    //const unsigned int ui_default_encrypt_method = settings.value(g_str_SEC_TEC_CODE).value<unsigned int>();
    const CryptoSupport::SymmetricMethods encrypt_method = (CryptoSupport::SymmetricMethods) settings.value(g_str_SEC_TEC_CODE).value<unsigned int>();
    if (encrypt_method == CryptoSupport::NO_ENCRYPT)
    {
        const QString str_msg = "Unable to encrypt: default encrypt method not defined. Check your settings";
        Logger::getInstance().logIt(en_LOG_ERRORS,str_msg);
        return false;
    };
    //
    if (true == is_crypted() )
        return true;            //nothing to do, already encrypted
    //
 //   if (m_pParentNode)
 //       m_pParentNode->placeStatusBarMsg("Encrypting " + m_strAttachName);
    //
    //get non-crypted_attachment
    //
    bool b_extract = extractBlobToByteArray_DB();
    //
    if (false == b_extract)
        return false;
    //
    QByteArray encrypted_aray;
    CryptoSupport crypto;
    bool b_encrypt = crypto.encrypt_bytearray( encrypt_method, ServicesCfg::getInstance().getPassword(), m_ByteArray, encrypted_aray);
    //
    if (false == b_encrypt)
        return false;
    //
    m_ByteArray     = encrypted_aray;
    m_iCryptType    = encrypt_method;
    //
    bool b_replace = replaceBlob_DB();
    //
    if (false == b_replace)
    {
        m_iCryptType = 0;
        m_ByteArray.clear();
        return false;
    };
    //
    m_ByteArray.clear();
    //
//    if (m_pParentNode)
//        m_pParentNode->placeStatusBarMsg(m_strAttachName + " encrypted");
    //
    return true;
}

bool Attachment::readFileToMemory (QString str_full_path_name)
{
    QFile f(str_full_path_name);
    //
    if ( !f.open(QIODevice::ReadOnly ) )
    {
        const QString str_msg = QString("Unable to open file %1").arg(str_full_path_name);
        Logger::getInstance().logIt(en_LOG_ERRORS,str_msg);
        return false;
    };
    //
    //QByteArray bytearray  = f.readAll();
    m_ByteArray = f.readAll();
    //
    //check here - contains or does not contains "0"
    const char c_zero  = 0;
    const int i_zero_index   = m_ByteArray.indexOf(c_zero, 0);
    //
    if (i_zero_index != -1 )
        m_bIsBinary = true;
    else
        m_bIsBinary = false;
    //
    m_iSize = m_ByteArray.size();
    return true;
}

bool Attachment::replace_it (const QString str_name, bool b_delete_after_replacement, bool b_protect_attach, unsigned int ui_encrypt_type)
{
    const QString   str_old_path_name       = m_strFilePath;
    const QString   str_old_attachment_name = m_strAttachName;
    bool            b_result                = true;
    //
    if (readFileToMemory(str_name) == true)
    {
        m_strFilePath = str_name;
        makeAttachmentName(m_strFilePath);
        //
        if (CryptoSupport::NO_ENCRYPT !=  ui_encrypt_type)
        {
            //QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
            //const unsigned int ui_default_encrypt_method = settings.value(g_str_SEC_TEC_CODE).value<unsigned int>();
            //
            QByteArray encrypted_aray;
            CryptoSupport crypto;
            bool b_encrypt = crypto.encrypt_bytearray( (CryptoSupport::SymmetricMethods) ui_encrypt_type, ServicesCfg::getInstance().getPassword(), m_ByteArray, encrypted_aray);
            //
            if (false == b_encrypt)
                return false;
            //
            m_ByteArray     = encrypted_aray;
            m_iCryptType    = ui_encrypt_type;
        };
        //
        if ( false == replaceBlob_DB() )
        {
            const QString str_msg = QString("Unable to replace attach %1 to %2 from file %3").arg(str_old_path_name).arg(m_strAttachName).arg(m_strFilePath);
            Logger::getInstance().logIt(en_LOG_ERRORS,str_msg);
            //
            m_strFilePath   = str_old_path_name;
            m_strAttachName = str_old_attachment_name;
            m_iCryptType    = 0;
            //
            b_result = false;
        }else //true == replace_DB()
        {
            if(m_pParentNode)
            {
                //if we have to delete original file - do it
                if (b_delete_after_replacement)
                {
                    m_bDeleteFileAfterUpload = true;
                    delOriginalFiles();
                };
                //
                m_bIsProtected = b_protect_attach;
                //
                m_pParentNode->notifyAboutAttachmentUpdate(b_result, m_strAttachName);
                const QString str_message = QString("Attach (id = %1) updated (%2 => %3) ").arg(m_iID).arg(str_old_attachment_name).arg(m_strAttachName);
                Logger::getInstance().logIt(en_LOG_UPDATE_ATTACH, str_message );                  
                //
                b_result = true;
            }else
            {
                b_result = false;
            };
        };
    }else // readFileToMemory(str_name) == false
    {
        b_result = false;
    }
    //
    return b_result;
};

bool Attachment::replaceBlob_DB ()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    //
    if (NULL == ptr_db)
        return -1;
    //
    QSqlQuery qry(*ptr_db);
    //
    unsigned int  ui_is_binary = 0;
    if (m_bIsBinary)
        ui_is_binary = 1;
    //
    const QString str_update_string = QString("UPDATE attachments set attach = :BLB, attach_name= '%1', is_binary = %2, crypted = %3 WHERE id_attach = %4;").
                                        arg(m_strAttachName).
                                        arg(ui_is_binary).
                                        arg(m_iCryptType).
                                        arg(m_iID);
    //
    if ( !qry.prepare( str_update_string  ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        m_ByteArray.clear();
        return false;
    };
    //

    //
    qry.bindValue(":BLB",   m_ByteArray );
    //
    if ( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        m_ByteArray.clear();
        return -1;
    };
    //
    m_ByteArray.clear();
    //
    return true;
}

int Attachment::insertTo_DB()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    //
    if (NULL == ptr_db)
        return -1;
    //
    QSqlQuery qry(*ptr_db);
    //
    //
    const QString str_insert_string = "INSERT INTO attachments (id_parent, attach_name, attach_size, is_binary, locked, crypted, attach) VALUES (:ID, :NAME, :ASIZE, :ISBINARY, :LC, :ENCRYPT, :BLB);";
    //
    if ( !qry.prepare( str_insert_string  ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        m_iSize = 0;
        m_ByteArray.clear();
        return -1;
    };
    //
    qry.bindValue(":ID",    m_pParentNode->getID());
    qry.bindValue(":NAME",  m_strAttachName);
    qry.bindValue(":ASIZE", m_iSize );
    if (m_bIsBinary)
        qry.bindValue(":ISBINARY", 1);
    else
        qry.bindValue(":ISBINARY", 0);
    //:LC
    if (m_bIsProtected)
        qry.bindValue(":LC", 1);
    else
        qry.bindValue(":LC", 0);
    //
    qry.bindValue(":ENCRYPT", m_iCryptType);
    //
    qry.bindValue(":BLB", m_ByteArray);
    //
    if ( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        m_iSize = 0;
        m_ByteArray.clear();
        return -1;
    };
    //
    m_ByteArray.clear();
    return qry.lastInsertId().toInt();
}

bool Attachment::rename_it(const QString &str_name)
{
    if (str_name.length() == 0)
        return false;
    //
    if (str_name == m_strAttachName)
        return false;
    //
    m_strOldAttachName = m_strAttachName;
    m_strAttachName = str_name;
    //
    setObjectStatus(OBJECT_NAME_UPDATING);
    QThreadPool::globalInstance()->start(this);
    //
    return true;
}

bool Attachment::restoreObject()
{
    if ( getObjectStatus() != OBJECT_DELETED )
        return false;

    setObjectStatus(OBJECT_OK);
    setActiveStatus_DB(true);
    return true;
}

bool Attachment::delete_it(bool b_silence)
{
    Q_UNUSED(b_silence);
    //
    if ( getObjectStatus() != OBJECT_OK )
        return false;
    //
    if (m_bIsProtected) //unable to delete protected attachment
        return false;
    //
    setObjectStatus(OBJECT_DELETED);
    setActiveStatus_DB(false);
    //
    return true;
};

bool Attachment::protect_it      (bool b_protection)
{
    if (m_bIsProtected == b_protection)
        return true; //nothing to do
    //
    if ( getObjectStatus() != OBJECT_OK )
        return false; //can not protect it now
    //
    m_bIsProtected = b_protection;
    return protect_it_DB(b_protection);
}

bool Attachment::protect_it_DB(bool b_protection)
{
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    unsigned int ui_protected = 0;
    if (b_protection)
        ui_protected = 1;
    //
    const QString str_update_string = QString("UPDATE attachments set locked = %1 WHERE id_attach = %2;").arg(ui_protected).arg(m_iID);
    //
    if (!qry.prepare( str_update_string ))
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    return true;
}

void Attachment::setActiveStatus_DB (bool b_active)
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return;
    //
    QSqlQuery qry(*ptr_db);
    //
    unsigned int ui_active = 0;
    if (b_active)
        ui_active = 1;
    //
    const QString str_update_string = QString("UPDATE attachments set active = %1 WHERE id_attach = %2;").arg(ui_active).arg(m_iID);
    //
    if (!qry.prepare( str_update_string ))
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return;
    };
    //
    if( !qry.exec() )
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
};

bool Attachment::updateName_DB()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    const QString str_update_string = QString("UPDATE attachments SET attach_name = :NAME WHERE id_attach = :ID;");
    //
    if (!qry.prepare( str_update_string ))
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    qry.bindValue(":NAME", m_strAttachName);
    qry.bindValue(":ID",   m_iID);
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    return true;
};

bool Attachment::updateParent_DB ()
{
    if (NULL == m_pParentNode)
        return false;
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    const QString str_update_string = QString("UPDATE attachments SET id_parent = '%1' WHERE id_attach = %2;").arg( m_pParentNode->getID() ).arg( m_iID );
    //
    if ( !qry.prepare( str_update_string ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text() );
        return false;
    };
    //
    return true;
};

bool Attachment::isPossibleToExport ()
{
    if (
            ( getObjectStatus() != OBJECT_OK )          &&
            ( getObjectStatus() != OBJECT_DELETED )
       )
    {
        return false; //only OK or deleted attachments possible to export
    };
    //
    return true;
};

bool Attachment::export_for_view  (const QString& str_target_dir, QString& str_temp_name)
{
    if ( false == isPossibleToExport() )
    {
        return false; //only OK or deleted attachments possible to export
    };
    //
    const QTime c_time = QTime::currentTime();
    const QString m_str_timestamp = c_time.toString("hh_mm_ss");
    QString str_full_path =QString("%1/%2_%3").arg( str_target_dir ).arg( m_str_timestamp ).arg( m_strAttachName );
    //str_full_path.replace(" ", "\\ ");
    //
    bool b_ret = export_attach( str_full_path );
    //
    if (b_ret)
    {
        str_temp_name = str_full_path;
    };
    //
    return b_ret;
}

bool Attachment::export_it (const QString& str_target_dir)
{
    if ( false == isPossibleToExport() )
    {
        return false; //only OK or deleted attachments possible to export
    };
    //
    QMessageBox box;
    QString str_full_path = str_target_dir + g_strPATH_DELIMETER + m_strAttachName;
    //str_full_path.replace(" ","\\ ");
    //
    const QFileInfo file_info (str_full_path);
    if ( file_info.exists() )
    {
        QString str_message = tr("file ");
        str_message += m_strAttachName;
        str_message += tr(" already exists into ") + str_target_dir + "\n";
        str_message += tr("Do you want to owerwrite it?");
        box.setStandardButtons( QMessageBox::Yes|QMessageBox::No );
        box.setText(str_message);
        int ret = box.exec();
        if (QMessageBox::Yes != ret)
            return false;
    };
    //
    return export_attach ( str_full_path );
}

bool Attachment::export_attach ( const QString& str_full_path )
{
    bool b_extract = extractBlobToByteArray_DB();
    //
    if (false == b_extract)
        return false;
    //
    const bool b_ret = writeByteArrayToFile (str_full_path);
    //
    m_ByteArray.clear();
    //
    return b_ret;
}

bool Attachment::writeByteArrayToFile (const QString str_file_name) const
{
    const int i_size = m_ByteArray.size();
    //
    if (i_size > 0)
    {
        QFile export_file ( str_file_name );
        if (export_file.open( QIODevice::WriteOnly) == false)
        {
            QMessageBox box;
            box.setText("Unable to to create file " + str_file_name);
            box.exec();
            const QString str_message = QString("Unable to to create file '%1'").arg(str_file_name);
            Logger::getInstance().logIt(en_LOG_ERRORS, str_message );
            //
            return false;
        };
        //
        export_file.write(m_ByteArray);
        export_file.close();
    };
    //
    return true;
}

QByteArray Attachment::getBlob ()
{
    if (m_ByteArray.size() == 0)
        extractBlobToByteArray_DB();
    //
    QByteArray ret_array = m_ByteArray;
    //
    return ret_array;
}

bool Attachment::extractBlobToByteArray_DB()
{
    QMessageBox     box;
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    Q_ASSERT(ptr_db);
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    const QString str_select_str = QString("select attach from attachments WHERE id_attach = %1;").arg( m_iID );
    //
    bool b_sql_res = qry.prepare( str_select_str );
    //
    if ( false == b_sql_res )
    {
        box.setText( tr("Unable to get file from database. \n ") + qry.lastError().text() );
        box.exec();
        //
        const QString str_message = QString("Unable to get file from database, error - '%1'").arg( qry.lastError().text() );
        Logger::getInstance().logIt(en_LOG_ERRORS, str_message );
        return false;
    };
    //
    b_sql_res = qry.exec();
    //
    if( false == b_sql_res )
    {
        box.setText( tr("Unable to get file from database. \n ") + qry.lastError().text() );
        box.exec();
        const QString str_message = QString("Unable to get file from database, error - '%1'").arg( qry.lastError().text() );
        Logger::getInstance().logIt(en_LOG_ERRORS, str_message );
        return false;
    };
    //
    qry.next();
    //
    if ( is_crypted() == true )
    {
        QByteArray encrypted_array = qry.value(0).toByteArray();
        //
        CryptoSupport crypto;
        bool b_decrypt = crypto.decrypt_bytearray( (CryptoSupport::SymmetricMethods) m_iCryptType, ServicesCfg::getInstance().getPassword(), encrypted_array, m_ByteArray);
        if (false == b_decrypt)
        {
            return false;
        };
    }else
    {
        m_ByteArray = qry.value(0).toByteArray();
    };
    //
    return true;
}

void Attachment::setNewParent  (TreeLeaf*    ptr_parent_leaf)
{
    if (NULL == m_pParentNode)
        return;
    //
    if (NULL == ptr_parent_leaf)
        return;
    //
    const QString str_old_parent_name = m_pParentNode->text(0);
    const unsigned int ui_old_parent_id = m_pParentNode->getID();
    //
    TreeLeaf* ptr_old_parent = m_pParentNode;
    //
    m_pParentNode->removeAttachment(this);
    //
    m_pParentNode = ptr_parent_leaf;
    //
    const QString str_new_parent_name = m_pParentNode->text(0);
    const unsigned int ui_new_parent_id = m_pParentNode->getID();
    //
    const bool b_res = updateParent_DB();
    //
    if (b_res)
    {
        m_pParentNode->addAttachment(this);
        //
        const QString str_message = QString("Attach (id = %1) moved from node '%1' (id = %2) to node '%3' (id = %4)").arg(str_old_parent_name).arg(ui_old_parent_id).arg(str_new_parent_name).arg(ui_new_parent_id);
        Logger::getInstance().logIt(en_LOG_MOVE_ATTACH, str_message );
    }else
    {
        m_pParentNode->removeAttachment(this);
        ptr_old_parent->addAttachment(this);
    };
}

void Attachment::delOriginalFiles () const
{
    //delete file if upload into database was success

    if ( m_bDeleteFileAfterUpload == false )
        return;
    //
    TmpFileCleaner::getInstance().deleteFile(m_strFilePath, true);
    //
    return;
}
