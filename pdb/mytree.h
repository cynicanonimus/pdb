#ifndef MYTREE_H
#define MYTREE_H
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
//mytree.h
//
#include <QObject>
#include <QtGui>
#include <QKeyEvent>
#include <QComboBox>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMutex>
#include <QProgressDialog>
//
#include "cryptothread.h"
#include "GDefinitions.h"
#include "treeleaf.h"
//
#include <vector>

class RootOfTree;

class MyTree : public QTreeWidget
{
public:
    Q_OBJECT
public:
    MyTree                          (QWidget* parent = 0);
    ~MyTree();
    //
    void                            setTreeComboBox     (QComboBox* ptr_comobobox);
    QStatusBar*                     getStatusBar        ()                              {return m_ptrStatusBar;}
    void                            setStatusBar        (QStatusBar* ptr_statusbar);
    void                            init                ();
    bool                            setLastUsedRootID_DB() const;
    //
    void keyPressEvent              (QKeyEvent *event);
    void mousePressEvent            (QMouseEvent *event);
    void mouseReleaseEvent          (QMouseEvent *event);
    void mouseMoveEvent             (QMouseEvent *event);
    //
    //void dropEvent                  (QDropEvent *event);
    //void dragEnterEvent             (QDragEnterEvent *event);
    //bool dropMimeData               (QTreeWidgetItem *parent, int index, const QMimeData *data, Qt::DropAction action);
    //void dragMoveEvent              (QDragMoveEvent *event);
    //
    //
    void expandActualTree           ();
    void saveExpandStateOfAllTrees  ();
    bool isEdited                   (); //not const: we reset the values
    //
    // this function called by leaf when attachment of the node is updated.
    //
    void leafAttachmentUpdated      (TreeLeaf* ptr_to_leaf, bool b_result, QString str_attachment_name);
    //
protected:
    //
     void addNewRoot                (RootOfTree* ptr_new_root);
    void DropWhenSourceIsTree       (QDropEvent *event);
    //void drawRow(QPainter* p, const QStyleOptionViewItem &opt, const QModelIndex &idx) const;
    //
public slots:
    //
    void onDelCurrentItem       ();
    void onRestoreCurrentItem   ();
    void onRestoreSubtree       ();
    void onInsertNewItem        ();

    void onCurrentDBChanged     (int index);
    void onImportAttach         ();
    //
    void onCutNode              ();
    void onPasteNode            ();
    //
    void onImportNode           ();
    void onExportNode           ();
    //
    void onAcceptCutAttachment  (Attachment::AttachmentsList);
    void onPasteAttachments     ();
    void onTreeItemExpanded     (QTreeWidgetItem*);
    void onTreeItemCollapsed    (QTreeWidgetItem*);
    //
    void OnProtectAttachmentsGlobal   ();
    void OnUnProtectAttachmentsGlobal ();
    //
    void OnEncryptAttachmentsGlobal   ();
    void OnDecryptAttachmentsGlobal   ();
    //
    void onDropAttachments      ();


private slots:
    void onCurrentItemChanged   (QTreeWidgetItem* current, QTreeWidgetItem* previous);
    void onItemChanged          (QTreeWidgetItem*, int);
    void onCancelEncryptDecrypt ();
    //void onSaveNodeDescriptor   ();

signals:
    //actual leaf
    void endOfInit              (int);
    void showPopupMenu          ();
    void treeSelectionChanged   (TreeLeaf*, bool, bool);
    void treeSelectionChanged   (TreeLeaf*, TreeLeaf*);
    //void delCurrentItem         ();
    //void restoreDeletedItem     ();
    //void insertNewItem          ();
    //void beginMoveNode          ();
    //void endMoveNode            ();
    void updateAttachmentList   ();


    //
    //void itemDragAndDrop      (QTreeWidgetItem* source, QTreeWidgetItem* target);
    //void clickRightButton     ();
    //
private:
    //
    //
    //
    bool isPossibleMoveNodeHere             (TreeLeaf* ptr_moving_node, TreeLeaf* ptr_target_node, bool b_silence = true);
    //
    // This function return id of last used root of tree, or -1
    //
    int         getLastUsedRootID_DB        () const;
    //
    // This functions adds remove all roots from the tree exclude N 0. Uses when we finish move the node
    // or attachment between trees.
    //
    void        removeAddedRootsFromTheTree ();
    //
    //root_tbl table
    //
    bool        getRootsFromDatabase_DB     ();
    //
    // nodetbl
    //
    bool        getLeafsFromDatabase_DB     ();
    //
    //
    void        showMoveNodeMessage         ( const QString &str_name_of_moved, const QString &str_name_of_target);
    //
    //
    void        expandActualTree            ( TreeLeaf*  ptr_root_leaf);
    //
    // get RootOfTree object based on it number in the database
    //
    RootOfTree* getRootByTreeId             ( int i_tree_id);
    //
    //save expand state for the subtree started from this node
    //
    void        saveExpandStateOfTree       ( TreeLeaf* ptr_node );
    //
    //          protect (when b_protect = true) or unprotect (when b_protect = false) node's (and childs) attachments
    //
    void        execGlobalProtectionOperation ( bool b_protect );
    //
    //          encrypt (when b_encrypt = true) or decrypt (when b_encrypt = false) node's (and childs) attachments
    //
    void        execGlobalEncryptionOperation ( bool b_encrypt );
    //
    // create encrypt-decrypt thread and link it by request
    //
    void        initOnDemand                ();


private:
    bool                m_bMoveLeafModeIsOn;
    /*
      this is "fake" changes of node: color, creating new node, ets.
    */
    //bool            m_bFakeChangeOfNode;
    //
    TreeLeaf*           m_ptrMovedItem;
    //
    // new defnitions
    //
    //RootsLeafContainer  m_RootsList;
    QComboBox*          m_ptrTreeComboBox;
    //
    QStatusBar*         m_ptrStatusBar;
    //
    //KIND_OF_MOVEMENT    m_enDragType;
    //
    std::vector <TreeLeaf*>  m_vOrpheline;
    //
    QMutex              m_UpdateAttachmentInfoLocker; //lock access to leafAttachmentUpdated
    //
    bool                m_bBeginDrag;
    QPoint              m_pointDearStartPosition;
    //
    QString             m_strLastAttachmentFromPath;
    Attachment::AttachmentsList   m_vActualCutAtttachments;
    //
    bool                m_bDeleteFilesAfterAttachment;
    bool                m_bProtectAttachment;
    bool                m_bEncryptAttachment;
    //
    CryptoThread*       m_ptrEncrypter;
    QProgressDialog*    m_ptrProgress;
};

#endif // MYTREE_H
