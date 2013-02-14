#ifndef GLOBALTREEOPERATIONSHANDLER_H
#define GLOBALTREEOPERATIONSHANDLER_H
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
// class is responsible for global operations with the trees: create new, rename, delete
//
#include <QObject>
#include <QComboBox>
#include <QStatusBar>
#include "mytree.h"
//
class RootOfTree;
//
class GlobalTreeOperationsHandler : public QObject
{
    Q_OBJECT
public:
    explicit GlobalTreeOperationsHandler(QObject *parent = 0);

public:
    void        setPtrToComboBox    (QComboBox*     ptr_combo);
    void        setPtrToTree        (MyTree*        ptr_to_tree);
    void        setPtrToStatusBar   (QStatusBar*    ptr_to_statusbar);

signals:
    
public slots:
    void    onCreateNewTree     ();
    void    onRenameCurrentTree ();
    void    onInfoAboutTree     ();
    void    onDeleteTree        ();

private:
    //
    // incoming parameters: name of the tree, name of the root node
    // outgoing parameters: ** to the root node
    //
    RootOfTree* createNewTree_DB( const QString& str_tree_name, const QString& str_root_node_name );
    //
    void        requestTreeInfo_DB (unsigned int& node_amount,
                                    unsigned int& attachments_amount,
                                    unsigned int& attachments_size,
                                    unsigned int  tree_id);

private:
    QComboBox*       m_ptrCombo;
    MyTree*          m_ptrTree;
    QStatusBar*      m_ptrStatusBar;
};

#endif // GLOBALTREEOPERATIONSHANDLER_H
