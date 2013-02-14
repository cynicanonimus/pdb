#ifndef MNUMAINWINDOW_H
#define MNUMAINWINDOW_H
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
// mnumainwindow.h
//
// class supports menu for the main window of application and
// mouse right-click pop-up menu
//
#include <QObject>
#include <QMenu>
//
#include "treeleaf.h"
#include "abstractdatabaseobject.h"
#include "GDefinitions.h"
#include "attachstatuschecker.h"
//
class MainWindow;


class MnuMainWindow : public QObject
{
    Q_OBJECT
public:
    explicit MnuMainWindow                  (MainWindow* parent);
    virtual ~MnuMainWindow                  ();
    
signals:
    
public slots:
    void onTreeComboBoxChanged              (int index);
    void onSelectedNodeChanged              (TreeLeaf* ptr_to_current, bool has_cutting_node, bool has_cutting_attach);
    void onSelectedAttachmentChanged        (Attachment::AttachmentsList);
    //void enableSaveNodeDescriptor           (bool);
    //
    void showRightClickPopupNodeMenu        ();
    void showRightClickPopupAttachMenu      ();
    //
    void onSaveDescription                  ();
    void onChangeDescription                ();
    //
    void onCheckPassword                    ( bool);

private:
    void        analyseAttachmentsStatus        ( const Attachment::AttachmentsList&    v_attachment, AttachStatusChecker& checker );
    void        setAccessExportCutViewAttach    ( const AttachStatusChecker& checker );
    void        setAccessDelReplRestoreAttach   ( const AttachStatusChecker& checker );
    //
    void        createTreeControlMenu       ();
    void        createNodeControlMenu       ();
    void        createAttachmentControlMenu ();
    void        createProtectionMenu        ();
    void        createSecurityMenu          ();
    void        createCryptograficMenu      ();
    //
    void        assemblyAllMenus            ();
    //
    void        assemblyTreeMenu                ( QMenu* ptr_node_menu );
    //
    void        assemblyNodeMenu                ( QMenu* ptr_node_menu );
    void        assemblyNodeCryptoSubMenu       ( QMenu* ptr_menu      );
    void        assemblyNodeProtectSubMenu      ( QMenu* ptr_menu      );
    //
    void        assemblyAttachMenu              ( QMenu* ptr_node_menu );
    void        assemblyAttachProtectionSubMenu ( QMenu* ptr_node_menu );
    void        assemblyAttachCryptographySubMenu( QMenu* ptr_node_menu );
    //
    void        assemblySecurityMenu            ( QMenu* ptr_node_menu );
    //
    void        adjustProtectionMenuForSelectedAttacments   ( const AttachStatusChecker& checker ); //
    void        adjustProtectionMenuForAllAttacmentsOfNode  ( TreeLeaf *ptr_parent );               //
    //
    void        adjustCryptoMenuForSelectedAttacments       ( const AttachStatusChecker& checker );//
    void        adjustCryptographyMenuForAllAttacmentsOfNode( TreeLeaf* ptr_parent );               //
    //
    void        adjustGlobalProtectionMenu  ( TreeLeaf* ptr_to_current, AttachStatusChecker& checker);
    void        adjustGlobalCryptographyMenu( TreeLeaf* ptr_to_current, AttachStatusChecker& checker);
public:
    //----------------------------- tree menu -----------------------------
    QMenu*      m_ptrTreeControlMenu;
    //
    QAction*    m_ptrDelTree;
    QAction*    m_ptrCreateNewTree;
    QAction*    m_ptrRenameTree;
    QAction*    m_ptrInfoTree;
    QAction*    m_ptrSearchInTree;
    //
    QAction*    m_ptrExitApp;
    //-----------------------------  node menu -----------------------------
    QMenu*      m_ptrNodeControlMenu;
    //
    QAction*    m_ptrDelNode;
    QAction*    m_ptrRestoreNode;
    QAction*    m_ptrRestoreSubtree;
    QAction*    m_ptrInsertNewNode;
    //
    QAction*    m_ptrCutNode;
    QAction*    m_ptrPasteNode;
    //
    QAction*    m_ptrExportNode;
    QAction*    m_ptrImportNode;
    //
    QMenu*      m_ptrNodeCryptoSubMenu;
    QAction*    m_ptrEncryptAttachmentsOfAllNodes;        //global
    QAction*    m_ptrDecryptAttachmentsOfAllNodes;        //global
    //
    QMenu*      m_ptrNodeProtectSubMenu;
    //
    QAction*    m_ptrProtectAttachmentsOfAllNodes;       //global
    QAction*    m_ptrUn_ProtectAttachmentsOfAllNodes;    //global
    //
    //
    QAction*    m_ptrSaveNodeDescriptor;
    //----------------------------- attachment menu -----------------------------
    QMenu*      m_ptrAttachmentMenu;
    //
    QAction*    m_ptrRestoreAttach;
    QAction*    m_ptrReplaceAttach;
    QAction*    m_ptrDeleteAttach;
    QAction*    m_ptrExportAttach;
    QAction*    m_ptrImportAttach;
    //
    QAction*    m_ptrCutAttach;
    QAction*    m_ptrPasteAttach;
    //
    QAction*    m_ptrViewAttach;
    //----------------------------- protection - submenu of the Attach menu  -----------------------------
    QMenu*      m_ptrAttachProtectionSubMenu;
    //
    QAction*    m_ptrProtectAllAttachmentsOfNode;   //for all attachments of the currently selected node
    QAction*    m_ptrUn_ProtectAllAttachmentsOfNode;//for all attachments of the currently selected node
    //
    QAction*    m_ptrProtectSelectedAttachments;    //for all currently selected attachments
    QAction*    m_ptrUn_ProtectSelectedAttachments; //for all currently selected attachments
    //----------------------------- security - submenu of the Attach menu  -----------------------------
    QMenu*      m_ptrSecurity;
    QAction*    m_ptrCreateChangePassword;
    //----------------------------- protection - submenu of the Attach menu  -----------------------------
    QMenu*      m_ptrCryptography;
    //--
    QAction*    m_ptrEncryptAllAttachmentsOfNode;        //for all attachments of the currently selected node
    QAction*    m_ptrDecryptAllAttachmentsOfNode;        //for all attachments of the currently selected node
    //
    QAction*    m_ptrEncryptSelectedAttachments;         //for all currently selected attachments
    QAction*    m_ptrDecryptSelectedAttachments;         //for all currently selected attachments
    //
private:
    MainWindow*     m_ptrParent;
    bool            m_bPasswordExist;
    
};

#endif // MNUMAINWINDOW_H
