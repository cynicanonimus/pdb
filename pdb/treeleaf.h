#ifndef TREELEAF_H
#define TREELEAF_H
//
// treeleaf.h
//
#include <QObject>
#include <QTreeWidgetItem>
#include <QTextDocument>
#include <QRunnable>
#include <vector>
#include <QMutex>
//
#include "abstractdatabaseobject.h"
#include "attachment.h"
//
class MyTree;
//
class TreeLeaf : public AbstractDatabaseObject, public QTreeWidgetItem, public QRunnable
{
    Q_OBJECT
public:

typedef  std::vector<TreeLeaf*> ChildList;

public:
    //
    // create from user interface
    //
    explicit TreeLeaf(TreeLeaf*        prtParentLeaf,
                      int              i_id,
                      int              i_tree_id,
                      const QString&   str_name,                  // node name
                      QObject*         parent_tree);
    //
    // create from database, non-root leaf
    //
    explicit TreeLeaf(TreeLeaf*         prtParentLeaf,
                      int               i_id,                     // node index in the node_tbl
                      int               i_parent_node_id,         // parent node ID
                      int               i_tree_id,
                      const QString&    str_name,                 // node name
                      const QString&    str_html_descriptor,      // node text descriptor
                      bool              b_is_active,              // is node "deleted" or not
                      bool              b_is_expanded,            // is node expanded or not
                      const QDateTime&  dt_last_change,           // last date and time change
                      QObject*          parent_tree
                   );
    //
    ~TreeLeaf();

signals:
    
public slots:

public:
    ChildList               getChildList                ();
    TreeLeaf*               getChildNode                (int i_node_id);
    TreeLeaf*               getParent                   ();
    void                    setNewParent                (TreeLeaf* ptr_new_parent);
    //
    bool                    delete_it                   (bool b_silence=false);
    //
    //this one is used only for deleting whole tree
    //
    bool                    force_delete_it             ();
    //void                    protect_all_attachments     (bool b_protect, bool b_recursive = false);
    //
    //b_ecnrypt-true  - encrypt, b_ecnrypt-false  - decrypt
    //
    //void                    encr_decr_all_attachments   (bool b_ecnrypt, bool b_recursive = false);
    bool                    rename_it                   (const QString &str_name);
    bool                    restoreObject               ();
    bool                    restoreSubtree              ();
    //
    const QTextDocument&    getDescriptor               () const            {return m_docDescriptor;}
    void                    setDescriptor               (const QString& str_html_descriptor);
    //
    bool                    isActualStateIsExpand       () const            {return m_bIsExpandedActually;}
    void                    setActualExpandState        (bool b_is_expanded){m_bIsExpandedActually = b_is_expanded;}
    //
    void                    saveToDBExpandStateChange   ();
    //
    int                     getParentID                 ()  const           {return m_iParentID;}
    int                     getTreeID                   ()  const           {return m_iDBTreeID;}
    //
    void                    dropAttachments             (bool b_recursive);
    void                    getAttachments              ( Attachment::AttachmentsList& v_attachments, bool b_recursive );
    //request aqttachments list from database, only once
    const Attachment::AttachmentsList&  getAttachments              ();//                  {return m_vAttachments;}
    //
    void                    addAttachments              (const QStringList& file_name_list,
                                                         bool b_delete_files_after,
                                                         bool b_protect_attachments,
                                                         bool b_encrypt_attachments);
    //
    void                    addAttachment               (Attachment* ptr_attachment);
    void                    removeAttachment            (Attachment* ptr_attachment);
    void                    placeStatusBarMsg           (const QString &str_message);
    //
    //calls after start of attachment operation (adding, for example) end of operation
    //
    void                    notifyAboutAttachmentUpdate (bool b_result, QString str_attachment_name);
    //
    bool                    importNode                  ( const QString& str_directory_path, bool b_recursive_import);
    bool                    exportNode                  ( const QString& str_directory_path, bool b_recursive_export);
    //
    // move to open section because need to ask root node.
    // warning! this is recurcive call
    //
    bool                    isPossibleToDeleteNode  (bool b_silence, bool b_force = false);
    bool                    isPossibleToExportNode  () const;
//
    void                    run();
//
public:
    //
    AbstractDatabaseObject::DB_OBJECT_STATUS      getObjectStatus () const;
//
private:
    //
    // request attachments list from database
    //
    void                    requestAttachments ();
    //
    //  this function is private, because we call only when change the parent of the node and do it for the all child nodes also.
    //
    void                    setNewTreeID        (unsigned int ui_tree_id);
    // function sets initial settings of the node
    void                    setInitialSettings  (int i_parent_id, const QString& str_name, int i_tree_id);
    // function updates information about ID of the parent node for this node
    void                    updateParentInfoIn_DB();
    // function sets new color of the node depends of it status
    void                    setItemColor();
    //
    bool                    isPossibleToRestoreNode();
    bool                    isPossibleToDeleteAttachments(QString &str_msg);
    //
    void                    deleteAllAttachments ();
    void                    restoreAllAttachments();
    //
    // function marks current node as "deleted" or "normal" depends of parameter
    //
    void                    setActiveStatus_DB      (bool b_active);
    int                     insertTo_DB             ();//must be defined in the every parent class. put new content to DB
    bool                    updateName_DB           ();
    bool                    updateDescriptor_DB     ();
    void                    updateTreeID_DB         ();
    void                    updateExpandState_DB    ();
    void                    getAttachments_DB       ();
    //CHANGE IT, make restore-delete
    void                    deleteAllAttachments_DB ();
    //
private:
    //
    unsigned int    m_iParentID;               // id of the parent node (id_parent_node). For the trop-levels node it is 0
    unsigned int    m_iDBTreeID;               // database ID (id_tree_FK) from root_tbl
    QTextDocument  m_docDescriptor;           // description of the node in the node_tbl
    Attachment::AttachmentsList m_vAttachments;            // list of attached files;
    //
    bool            m_bIsExpandedDB;           // is node expanded or not, db value changed once, before update
    bool            m_bIsExpandedActually;     // is node expanded or not, current value
    bool            m_bIsAttachmentRequestedAlready;
    //
    QMutex          m_RunLocker;               // lock "run" execution
    QMutex          m_RequestAttachLocker;     // lock "requestAttachments" execution
    //
    MyTree*         m_ptrParentTree;
    bool            m_bForceDelete;
    QString         m_strDatabaseNodeName;
    //bool          m_bIsInternalNameAssigment;
//

};

#endif // TREELEAF_H
