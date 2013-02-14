#ifndef ROOTOFTREE_H
#define ROOTOFTREE_H
//rootoftree.h
/*
Class is representation of the row from root_tbl table
*/
#include <QObject>
#include <QDateTime>
#include <QSemaphore>
//
#include "abstractdatabaseobject.h"
//
class TreeLeaf;
//
class RootOfTree : public AbstractDatabaseObject //root_tbl
{
    Q_OBJECT
public:
             RootOfTree(const QString&      str_root_name,
                        int                 ui_id,
                        const QDateTime&    last_change,
                        bool                b_is_active,
                        QObject*            parent = 0
                       );
             //
             ~RootOfTree();
    //
    const QString&      getName         ()   const                      { return m_strName; }
    bool                rename_it       (const QString& str_name);
    bool                delete_it       (bool b_silence = true);
    //void            setRootIndex(unsigned int);
    bool                restoreObject   ();
    //depends of the status of the node, this function update changes of the node to database or
    //insert the new created node into DB
    //
    void                addChildLeaf    (TreeLeaf* ptr_top_leaf);
    TreeLeaf*           getChildLeaf    ()                              {return m_ptrTopLeaf;}
    //

signals:
    
public slots:

protected:
    int             insertTo_DB         (); //must be defined in the every rarent class
    bool            updateName_DB       ();
    void            setActiveStatus_DB  (bool b_active);

private:
    QString         m_strName;          //name of the node or root in the root_tbl
    //
    TreeLeaf*       m_ptrTopLeaf;
};

#endif // ROOTOFTREE_H
