#ifndef MYTABLE_H
#define MYTABLE_H
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
#include <QtGui>
#include <QTableWidget>
#include <QProgressDialog>
//
#include "treeleaf.h"
#include "cryptothread.h"

//
class MyTable : public QTableWidget
{
public:
    Q_OBJECT

public:
    explicit MyTable(QWidget *parent = 0);
    ~MyTable();
    //
    void    setStatusBar (QStatusBar* ptr_statusbar);
    //
    //void mousePressEvent            (QMouseEvent *event);
    //void mouseMoveEvent             (QMouseEvent *event);
    void   mouseReleaseEvent          (QMouseEvent *event);
    void   mouseDoubleClickEvent      (QMouseEvent *event);
    void   focusInEvent               (QFocusEvent *event);
    void   keyPressEvent              (QKeyEvent *event);
    //
signals:
    void    selectionChanged        ( Attachment::AttachmentsList );
    void    attachmentForCutSelected( Attachment::AttachmentsList );
    void    showPopupMenu           ();



public slots:
    void    onSelectedNodeChanged   (TreeLeaf* ptr_to_current, TreeLeaf* ptr_to_previous);
    void    onAttachmentUpdated     ();
    void    onItemChanged           (QTableWidgetItem * item);
    void    onItemSelectionChanged  ();
    void    onDeleteAttachment      ();
    void    onRestoreAttachment     ();
    void    onExportAttachment      ();
    void    onViewAttachment        ();
    void    onReplaceAttachment     ();
    void    onCutAttachment         ();
    void    refreshCurrentAttachmentsList   ();
    //
    void    onProtectAllAttachments ();
    void    onDropProtectAllAttachments ();
    void    onProtectSelected       ();
    void    onDropProtectSelected   ();
    //
    void    onEncryptAllAttachments ();
    void    onDecryptAllAttachments ();
    void    onEncryptSelected       ();
    void    onDecryptSelected       ();

private slots:
    void    onCancelAsyncOperations ();

private:
    //
    // it is numbers of the columns in the attachment list
    //
    enum ATTACHMENT_TABLE_COLUMNS
    {
        ITEM_LOCKED     = 0,
        ITEM_ENCRYPTED  = 1,
        ITEM_NAME       = 2,
        ITEM_DATA       = 3,
        ITEM_SIZE       = 4,
        ITEM_STATUS     = 5,
        ITEM_AMOUNT     = 6
    };
    //
private:
    /*
        b_encrypt = true    - encrypt it
        b_encrypt = false   - decrypt it
    */
    //void        encrypt_decrypt_selected(bool b_encrypt);
    void        encrypt_decrypt_array   ( bool b_encrypt, const Attachment::AttachmentsList& list );
    void        protect_unprotect_array ( bool b_protect, const Attachment::AttachmentsList &list );
    //
    void        clearTable              ();
    void        showAttachments         ( const Attachment::AttachmentsList& t_attachment_list );
    void        getSelectedAttaches     ( Attachment::AttachmentsList& v_attachments );
    void        setItemColor            ( QTableWidgetItem* ptr_item, const Attachment* ptr_tree_attachment );
    Attachment* getLinkedAttachment     ( QTableWidgetItem* ptr_item );
    //
    void        makeAttachmentsExport   (const Attachment::AttachmentsList&  v_attachments);
    //
    void        makeViewExport          ( const QString&                    str_export_path,
                                          const Attachment::AttachmentsList &v_attachments);
    //
    void        saveLastAttachName      ();
    bool        isStrValidAsFileName    (const QString& str_in);
    /*
    function returns true if we need to call standart event handler (mouse or keyboard)
    or false if we don't
    */
    bool        processAttachmentList   ();

    //create cells
    QTableWidgetItem*       makeCellLocked      ( Attachment* ptr_attachment );
    QTableWidgetItem*       makeCellEncrypted   ( Attachment* ptr_attachment );
    QTableWidgetItem*       makeCellData        ( Attachment* ptr_attachment );
    QTableWidgetItem*       makeCellName        ( Attachment* ptr_attachment );
    QTableWidgetItem*       makeCellSize        ( Attachment* ptr_attachment );
    QTableWidgetItem*       makeCellStatus      ( Attachment* ptr_attachment );
    //
    //create thread and progress dialog on demand, also mak a link between it
    //
    void                    initOnDemand        ();
private:
    //prevent
    bool                m_bFillModeOn;
    TreeLeaf*           m_ptrActualLeaf;
    QStatusBar*         m_ptrStatusBar;
    QString             m_strLastExportDir;
    QPoint              m_pointDearStartPosition;
    QString             m_strAttachmentNameBeforeChange;
    //this variables are using for save last user decision in the replacement dialog. Initially both - false.
    bool                m_bDeleteAfterAttachReplacement;
    bool                m_bProtectAttachReplacement;
    bool                m_bEncryptAttachReplacement;
    //last path for replace attachment. initially - path for import
    QString             m_strLastPathOfReplacement;
    //
    CryptoThread*       m_ptrEncrypter;
    QProgressDialog*    m_ptrProgress;
};

#endif // MYTABLE_H
