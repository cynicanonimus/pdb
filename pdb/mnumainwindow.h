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
#include <QToolBar>
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
    
public:
    void syncToolbarsVisibilityAndMenu      ();

signals:
    
public slots:
    void onTreeComboBoxChanged              (int index);
    void onSelectedNodeChanged              (TreeLeaf* ptr_to_current, bool has_cutting_node, bool has_cutting_attach);
    void onSelectedAttachmentChanged        (Attachment::AttachmentsList);
    //
    void showRightClickPopupNodeMenu        ();
    void showRightClickPopupAttachMenu      ();
    //
    void onSaveDescription                  (); //candidates to remove
    void onChangeDescription                (); //candidates to remove
    //
    void onChangeEditText                   ( bool );
    //
    void onCheckPassword                    ( bool );
    //
private slots:
    void onTriggerTreeToolBox               ();
    void onTriggerNodeToolBox               ();
    void onTriggerAttachmentToolBox         ();
    void onTriggerEditToolBox               ();
    void onTriggerTableToolBox              ();
    void onTriggerSecurityToolBox           ();



private:
    void        analyseAttachmentsStatus        ( const Attachment::AttachmentsList&    v_attachment, AttachStatusChecker& checker );
    void        setAccessExportCutViewAttach    ( const AttachStatusChecker& checker );
    void        setAccessDelReplRestoreAttach   ( const AttachStatusChecker& checker );
    //
    void        createTreeControlMenu       ();
    void        createNodeControlMenu       ();
    void        createAttachmentControlMenu ();
    void        createEditorMenu            ();
    void        createTableSubMenu          ();
    void        createProtectionMenu        ();
    void        createSecurityMenu          ();
    void        createToolBarSubMenu        ();
    void        createCryptograficMenu      ();
    void        createHelpMenu              ();
    //
    void        assemblyAllMenus            ();
    //
    void        assemblyTreeMenu                ( QMenu* ptr_node_menu );
    //
    void        assemblyNodeMenu                ( QMenu* ptr_node_menu );
    void        assemblyNodeCryptoSubMenu       ( QMenu* ptr_menu      );
    void        assemblyNodeProtectSubMenu      ( QMenu* ptr_menu      );
    //
    void        assemblyEditorMenu              ( QMenu* ptr_node_menu );
    void        assemblyTextColorSubMenu        ( QMenu* ptr_set_text_color_menu );
    void        assemblyTableSubMenu            ( QMenu* ptr_table_menu );
    void        assemblyInsertInTextSubMenu     ( QMenu* ptr_insert_in_text_menu );
    void        assemblyAlignSubMenu            ( QMenu* ptr_align_menu );
    void        assemblyTextAttribSubMenu       ( QMenu* ptr_attrib_menu);
    void        assemblyChangeCaseSubMenu       ( QMenu* ptr_case_menu );
    //
    void        assemblyAttachMenu              ( QMenu* ptr_node_menu );
    void        assemblyAttachProtectionSubMenu ( QMenu* ptr_node_menu );
    void        assemblyAttachCryptographySubMenu( QMenu* ptr_node_menu );
    //
    void        assemblyViewMenu                ( QMenu* ptr_node_menu );
    //
    void        assemblySecurityMenu            ( QMenu* ptr_node_menu );
    //
    void        assemblyHelpMenu                ();
    //
    void        adjustProtectionMenuForSelectedAttacments   ( const AttachStatusChecker& checker ); //
    void        adjustProtectionMenuForAllAttacmentsOfNode  ( TreeLeaf *ptr_parent );               //
    //
    void        adjustCryptoMenuForSelectedAttacments       ( const AttachStatusChecker& checker );//
    void        adjustCryptographyMenuForAllAttacmentsOfNode( TreeLeaf* ptr_parent );               //
    //
    void        adjustGlobalProtectionMenu                  ( TreeLeaf* ptr_to_current, AttachStatusChecker& checker);
    void        adjustGlobalCryptographyMenu                ( TreeLeaf* ptr_to_current, AttachStatusChecker& checker);
    //
    void        adjustExpandCollapseMenus                   ( TreeLeaf* ptr_to_current, bool& b_allow_expand, bool& b_allow_collapse  );
public:
    //----------------------------- tree menu -----------------------------
    QMenu*      m_ptrTreeControlMenu;
    //
    QAction*    m_ptrBackupDatabase;
    //
    QAction*    m_ptrDelTree;
    QAction*    m_ptrCreateNewTree;
    QAction*    m_ptrRenameTree;
    QAction*    m_ptrInfoTree;
    QAction*    m_ptrSearchInTree;
    //
    QAction*    m_ptrExitApp;
    //
    QToolBar*    m_ptrTreeToolBar;
    //-----------------------------  node menu -----------------------------
    QMenu*      m_ptrNodeControlMenu;
    //
    QAction*    m_ptrDelNode;
    QAction*    m_ptrRestoreNode;
    QAction*    m_ptrRestoreSubtree;
    QAction*    m_ptrInsertNewNode;
    //
    QAction*    m_ptrChangeNodeColor;
    //
    QAction*    m_ptrSetNodeIcon;
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
    QAction*    m_ptrExpandSubtree;
    QAction*    m_ptrCollapseSubtree;
    //
    QToolBar*    m_ptrNodeToolBar;
    //------------------------------- editor menu -------------------------------
    QMenu*      m_ptrEditorMenu;
    //
    QAction*    m_ptrLoadFromFile;
    QAction*    m_ptrSaveToFile;
    //
    QAction*    m_ptrSaveNodeDescriptor;
    //
    QAction*    m_ptrUndo;
    QAction*    m_ptrRedo;
    //
    QAction*    m_ptrPrint;
    QAction*    m_ptrPrintPreview;
    QAction*    m_ptrExportToPdf;
    //--------------------------------text color ---------------------------------
    QMenu*      m_ptrTextColorMenu;
    //
    QAction*    m_ptrChangeTextColor;
    QAction*    m_ptrChangeBackgroundColor;
    //------------------------------- table --------------------------------------
    QMenu*      m_ptrTableMenu;
    //
    QAction*    m_ptrInsertTable;
    QAction*    m_ptrInsertRow;
    QAction*    m_ptrInsertColumn;
    QAction*    m_ptrRemoveRow;
    QAction*    m_ptrRemoveColumn;
    QAction*    m_ptrCellsMerge;
    QAction*    m_ptrCellSplit;

    QAction*    m_ptrTableSettings;

    QToolBar*   m_ptrTableToolBar;
    //---------------------------------------------------------------------------
    QMenu*      m_ptrInsertInTextMenu;

    QAction*    m_ptrInsertHTML;
    QAction*    m_ptrInsertImageLink;
    //---------------------------------------------------------------------------
    QMenu*      m_ptrCaseMenu;
    //
    QAction*    m_ptrSentenceCase;
    QAction*    m_ptrUpperCase;
    QAction*    m_ptrLowerCase;
    QAction*    m_ptrToggleCase;
    //---------------------------------------------------------------------------
    QMenu*      m_ptrMakeTextAttribSubMenu;
    //
    QAction*    m_ptrBold;
    QAction*    m_ptrUnderline;
    QAction*    m_ptrItalic;
    //---------------------------------------------------------------------------
    QMenu*      m_ptrAlignSubMenu;
    //
    QAction*    m_ptrTextAlignLeft;
    QAction*    m_ptrTextAlignRight;
    QAction*    m_ptrTextAlignCenter;
    QAction*    m_ptrTextAlignJustify;
    //---------------------------------------------------------------------------
    //QAction*    m_ptrFamily;
    QAction*    m_ptrTextSize;
    QAction*    m_ptrTextStyle;
    QAction*    m_ptrTextColor;
    //
    //QAction*    m_ptrZoomIn;
    //
    QToolBar*    m_ptrEditorToolBar;
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
    //
    QAction*    m_ptrDirectScan;
    //
    QToolBar*    m_ptrAttachmentToolBar;
    //----------------------------- protection - submenu of the Attach menu  -----------------------------
    QMenu*      m_ptrAttachProtectionSubMenu;
    //
    QAction*    m_ptrProtectAllAttachmentsOfNode;   //for all attachments of the currently selected node
    QAction*    m_ptrUn_ProtectAllAttachmentsOfNode;//for all attachments of the currently selected node
    //
    QAction*    m_ptrProtectSelectedAttachments;    //for all currently selected attachments
    QAction*    m_ptrUn_ProtectSelectedAttachments; //for all currently selected attachments
    //----------------------------- View - main menu  -----------------------------
    QMenu*      m_ptrViewMenu;
    QMenu*      m_ptrToolbarsMenu;
    QAction*    m_ptrLogWindow;
    //
    QAction*    m_ptrMnuNodeToolBar;
    QAction*    m_ptrMnuAttachmentToolBar;
    QAction*    m_ptrMnuTreeToolBar;
    QAction*    m_ptrMnuEditorToolBar;
    QAction*    m_ptrMnuTableToolBar;
    QAction*    m_ptrMnuSecurityToolBar;
    //----------------------------- security - main menu  -----------------------------
    QMenu*      m_ptrSecurity;
    //
    QAction*    m_ptrCreateChangePassword;
    QAction*    m_ptrLockAppScreen;
    //
    QToolBar*    m_ptrSecurityToolBar;
    //----------------------------- protection - submenu of the Attach menu  -----------------------------
    QMenu*      m_ptrCryptography;
    //--
    QAction*    m_ptrEncryptAllAttachmentsOfNode;        //for all attachments of the currently selected node
    QAction*    m_ptrDecryptAllAttachmentsOfNode;        //for all attachments of the currently selected node
    //
    QAction*    m_ptrEncryptSelectedAttachments;         //for all currently selected attachments
    QAction*    m_ptrDecryptSelectedAttachments;         //for all currently selected attachments
    //----------------------------------------------------------------------------------------------------
    QMenu*      m_ptrHelpMenu;
    QAction*    m_ptrAbout;
    //
private:
    MainWindow*     m_ptrParent;
    bool            m_bPasswordExist;
    
};

#endif // MNUMAINWINDOW_H
