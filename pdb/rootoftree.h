#ifndef ROOTOFTREE_H
#define ROOTOFTREE_H
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
