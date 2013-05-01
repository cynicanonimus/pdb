#ifndef ATTACHMENT_H
#define ATTACHMENT_H
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
// class represent alone attachment
//
// attachment.h
//
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QMutexLocker>
#include <QProcess>
//
#include <vector>
//
#include "abstractdatabaseobject.h"
#include "advthreadjob.h"

class TreeLeaf;
//
class Attachment : public AbstractDatabaseObject, public AdvThreadJob//QRunnable
{
    Q_OBJECT
public:
    typedef  std::vector<Attachment*> AttachmentsList;
public:
    //
    //-------------------------------------------------------------------
    //this constructor used for creating Attachment from file
    //
                        Attachment(const QString&      full_path_filename,
                                   TreeLeaf*           ptr_parent_leaf,
                                   bool                b_delete_files_after,
                                   bool                b_protect_attachment,
                                   unsigned int        ui_encrypt_type);
    //
    //this constructor for creating Attachment from DB
    //
                        Attachment(int                  i_id,
                                   const QString&       t_str_attach_name,
                                   int                  i_attach_size,
                                   bool                 b_active,
                                   int                  i_crypt_type,
                                   bool                 b_protected,
                                   bool                 b_binary,
                                   const QDateTime&     dt_last_change,
                                   TreeLeaf*            ptr_parent_leaf);
    //-------------------------------------------------------------------
    int                     getSize         () const { return m_iSize;          }
    const QString&          getName         () const { return m_strAttachName;  }
    //
    bool                    rename_it       ( const QString& str_name);
    bool                    delete_it       ( bool b_silence=false);
    bool                    restoreObject   ();
    bool                    export_it       ( const QString& str_target_dir);
    bool                    export_for_view ( const QString& str_target_dir, QString& str_temp_name);
    bool                    replace_it      ( const QString str_name, bool b_delete_after_replacement, bool b_protect_attach, unsigned int ui_encrypt_type);
    bool                    is_binary       () const { return m_bIsBinary;      }
    bool                    is_crypted      () const { return (0 != m_iCryptType);     }
    //
    bool                    is_protected    () const { return m_bIsProtected;   }
    bool                    protect_it      ( bool b_protection);
    bool                    encrypt_it      ();
    bool                    re_encrypt_it   (); //TODO when user change password
    bool                    decrypt_it      ();
    //
    QByteArray              getBlob         ();
    //
    void                    setNewParent    ( TreeLeaf*    ptr_parent_leaf);
    //
    TreeLeaf*               getParentLeaf   ();
    //
    //overloaded function of QRunnable, executed in the thread pool
    //
    bool                    exec            ();
    //
private slots:
    //
protected:
    //
    bool                    extractBlobToByteArray_DB   ();
    int                     insertTo_DB                 ();
    bool                    replaceBlob_DB              ();
    void                    setActiveStatus_DB          ( bool b_active );
    //
    virtual bool            updateName_DB               ();
    bool                    updateParent_DB             ();
    //void                    updateName      ();
    bool                    protect_it_DB               ( bool b_protection );


private:
    void                    insert_new_attacment        ();
    void                    makeAttachmentName          ( QString str_full_path_name );
    bool                    readFileToMemory            ( QString str_full_path_name );
    bool                    isPossibleToExport          ( );
    bool                    writeByteArrayToFile        ( const QString str_file_name ) const;
    void                    delOriginalFiles            ( ) const; //delete file if upload into database was success
    bool                    export_attach               ( const QString& str_full_path );

private:

    int                     m_iSize;
    TreeLeaf*               m_pParentNode;
    QMutex                  m_RunLocker;
    //path to the attachment
    QString                 m_strFilePath;
    //name of the attachment (usially part of the path)
    QString                 m_strAttachName;
    QString                 m_strOldAttachName;
    //attachment BLOB
    QByteArray              m_ByteArray;
    //type of encrypt, see enum SymmetricMethods
    int                     m_iCryptType;
    bool                    m_bIsProtected;
    bool                    m_bIsBinary;
    bool                    m_bDeleteFileAfterUpload;
//    bool                    m_bFinishEncryptDecryptSuccess;
};

#endif // ATTACHMENT_H
