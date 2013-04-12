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
#include "mytree.h"
//
#include<QMessageBox>
#include<QLabel>
#include<QGridLayout>
#include <QtSql>
#include <QtDebug>
#include <QMutexLocker>
#include <QListView>
#include <qlistview.h>
//
#include <vector>
//
#include "../CommonInclude/pdb/VariantPtr.h"
//
#include "../CommonInclude/pdb/pdb_style.h"
#include "logger.h"
#include "dlgexportparams.h"
#include "connectionmanager.h"
#include "dbacccesssafe.h"
#include "rootoftree.h"
#include "attachimportorreplacedlg.h"
#include "askchangesdetailsdlg.h"
#include "dlgaskencryptmethod.h"
#include "dlgmanageicons.h"
//
using namespace std;
//
MyTree::MyTree(QWidget* parent) : QTreeWidget (parent)
{
    //
    m_ptrEncrypter  = NULL;
    m_ptrProgress   = NULL;
    //
    this->setDropIndicatorShown(false);
    //this->setDragDropMode(QAbstractItemView::DropOnly);
    this->setDragEnabled(false);
    //this->setDragDropOverwriteMode(false);
    this->setAcceptDrops(false);
    //
    this->setEditTriggers(QAbstractItemView::DoubleClicked|EditKeyPressed );
    //
    m_bMoveLeafModeIsOn = false;
    //

    QString str_style = "QTreeView::branch:has-siblings:!adjoins-item { border-image: url(:/images/images/vline.png) 0;    }";
    str_style        += "QTreeView::branch:has-siblings:adjoins-item { border-image: url(:/images/images/branch-more.png) 0; }";
    str_style        += "QTreeView::branch:!has-children:!has-siblings:adjoins-item { border-image: url(:/images/images/branch-end.png) 0; }";
    str_style        += "QTreeView::branch:has-children:!has-siblings:closed, QTreeView::branch:closed:has-children:has-siblings {border-image: none; image: url(:/images/images/branch-closed.png); }";
    str_style        += "QTreeView::branch:open:has-children:!has-siblings, QTreeView::branch:open:has-children:has-siblings  { border-image: none; image: url(:/images/images/branch-open.png);}";
    //
    this->setStyleSheet(str_style);
    //
    this->setColumnCount(2);
    //
    //
    m_ptrMovedItem  = NULL;
    m_bBeginDrag = false;
    //
    // Signal->Slots
    //
    //QObject::connect(this, SIGNAL( delCurrentItem()     ),  this, SLOT( onDelCurrentItem()      ) );
    //QObject::connect(this, SIGNAL( restoreDeletedItem() ),  this, SLOT( onRestoreCurrentItem()  ) );
    //QObject::connect(this, SIGNAL( insertNewItem()      ),  this, SLOT( onInsertNewItem()       ) );
    //
    QObject::connect(this, SIGNAL( currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(onCurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)) );
    //
    QObject::connect(this, SIGNAL( itemExpanded (QTreeWidgetItem*)),        this, SLOT(onTreeItemExpanded   (QTreeWidgetItem*) ) );
    QObject::connect(this, SIGNAL( itemCollapsed(QTreeWidgetItem*)),        this, SLOT(onTreeItemCollapsed  (QTreeWidgetItem*) ) );
    QObject::connect(this, SIGNAL( itemChanged  (QTreeWidgetItem*, int)),   this, SLOT(onItemChanged        (QTreeWidgetItem*, int) ) );
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    m_bDeleteFilesAfterAttachment   = settings.value(g_str_DELETE_AFTER_UPLOAD).value<bool>();
    m_bProtectAttachment            = settings.value(g_str_SEC_PROTECT).value<bool>();
    m_bEncryptAttachment            = settings.value(g_str_SEC_MARK_CRYPT).value<bool>();
    //
    int i_tree_icon_size    = settings.value(g_str_TREE_ICONS_SIZE).value<int>();
    if (0 == i_tree_icon_size)
        i_tree_icon_size = 16;
    //
    this->setIconSize(QSize(i_tree_icon_size, i_tree_icon_size));
 }

MyTree::~MyTree()
{
    if (m_ptrEncrypter)
        delete m_ptrEncrypter;
    //
    if (m_ptrProgress)
        delete m_ptrProgress;
}

void MyTree::initOnDemand ()
{
    if (NULL != m_ptrEncrypter)
        return;
    //
    m_ptrEncrypter  = new CryptoThread;
    m_ptrProgress   = new QProgressDialog;

    //
    QObject::connect( m_ptrEncrypter,        SIGNAL(setTotalAttachmentsNumber(unsigned int)),   m_ptrProgress,  SLOT( setMaximum(int)           ) );
    QObject::connect( m_ptrEncrypter,        SIGNAL(setCurrentLabel(QString)),                  m_ptrProgress,  SLOT( setLabelText(QString)     ) );
    QObject::connect( m_ptrEncrypter,        SIGNAL(setValue(int)),                             m_ptrProgress,  SLOT( setValue(int)             ) );
    QObject::connect( m_ptrEncrypter,        SIGNAL(stopThis()),                                this,           SLOT( onCancelEncryptDecrypt()  ) );
    QObject::connect( m_ptrProgress,         SIGNAL(canceled()),                                m_ptrEncrypter, SLOT( OnTerminate()             ) );
    //
    m_ptrProgress->setAutoClose(true);
    m_ptrProgress->setAutoReset(true);
    m_ptrProgress->setMinimumWidth(350);
}

void MyTree::onCancelEncryptDecrypt ()
{
    m_ptrProgress->close();
}
//
/*
void MyTree::dropEvent (QDropEvent *event)
{
    //QMessageBox box;
    //box.setText(tr("Drop here?"));
    //box.exec();
    //
    MyTree* source = qobject_cast<MyTree*>(event->source());
    if (source && (source == this) )
    {
        const QMimeData* ptrdata = event->mimeData();
        //
        bool b1 = ptrdata->hasColor();
        QString str_format;
        bool b2 = ptrdata->hasFormat(str_format);
        bool b3 = ptrdata->hasHtml();
        bool b4 = ptrdata->hasImage();
        bool b5 = ptrdata->hasText();
        bool b6 = ptrdata->hasUrls();
        //
        QString str_text = ptrdata->text();
        //
        m_ptrStatusBar->showMessage( "text is: " + ptrdata->text() );
        //
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }else
    {
        m_ptrStatusBar->showMessage( tr("Attachment?: ") );
        //
        const QMimeData* ptrdata = event->mimeData();
        //
        bool b1 = ptrdata->hasColor();
        QString str_format;
        bool b2 = ptrdata->hasFormat(str_format);
        bool b3 = ptrdata->hasHtml();
        bool b4 = ptrdata->hasImage();
        bool b5 = ptrdata->hasText();
        bool b6 = ptrdata->hasUrls();
        //
        QString str_text = ptrdata->text();
        //
        m_ptrStatusBar->showMessage( "text is: " + ptrdata->text() );
        //
        event->setDropAction(Qt::MoveAction);
        event->ignore();
    };
};
*/
/*
void  MyTree::dragEnterEvent(QDragEnterEvent *event)
{
        event->accept();
};
*/
//
void MyTree::setStatusBar (QStatusBar* ptr_statusbar)
{
    Q_ASSERT( ptr_statusbar );
    //
    m_ptrStatusBar = ptr_statusbar;
}

void MyTree::removeAddedRootsFromTheTree()
{
    while (1)
    {
        TreeLeaf* ptr_remove_element = (TreeLeaf*) this-> takeTopLevelItem(1);
        if (NULL == ptr_remove_element)
            break;
    };
}

void MyTree::onSetIconNodes ()
{
    m_IconLoadLocker.lock();
    //
    for (int i = 0; i < m_ptrTreeComboBox->count(); i++)
    {
        QVariant root_back_data = m_ptrTreeComboBox->itemData(i);
        RootOfTree* ptr_root    = VariantPtr<RootOfTree>::asPtr( root_back_data );
        //
        ptr_root->getChildLeaf()->setIconByID();
    };
    //
    m_IconLoadLocker.unlock();
}

void MyTree::onDropAttachments ()
{
    for (int i = 0; i < m_ptrTreeComboBox->count(); i++)
    {
        QVariant root_back_data = m_ptrTreeComboBox->itemData(i);
        //
        RootOfTree* ptr_root    = VariantPtr<RootOfTree>::asPtr( root_back_data );
        if (ptr_root)
        {
            TreeLeaf* ptr_root_leaf = ptr_root->getChildLeaf();
            {
                ptr_root_leaf->dropAttachments(true);
            };
        };
    };
}

void  MyTree::onExportNode ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();

    if (NULL == ptr_actual_item)
        return;
    //
    DlgExportParams dlg;

    if ( QDialog::Accepted == dlg.exec() )
    {
        ptr_actual_item->exportNode(dlg.getExportPath(),
                                    dlg.getExportFormat(),
                                    !dlg.isExportOnlyCurrentNode(),
                                    dlg.isExportIncludeAttach(),
                                    dlg.exportEncrypted() );
    };
    //
/*
    if ( dlg.isExportAll() == true )
    {
        ptr_actual_item->exportNode(dlg.getExportPath(), true);
    }else if( dlg.isExportCurrent() == true )
    {
        ptr_actual_item->exportNode(dlg.getExportPath(), false);
    };
*/
}

//-------------------------------------------------------------------------

void MyTree::onChangeColor ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if (NULL == ptr_actual_item)
        return;
    //
    QColor color = QColorDialog::getColor(Qt::black, this);
    //
    if (color.isValid())
    {
        ptr_actual_item->setColor(color.name(), true);
    };
}

void MyTree::onChangeIcon()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if (NULL == ptr_actual_item)
        return;
    //
    DlgManageIcons dlg;
    if ( dlg.exec() == QDialog::Rejected )
        return;
    //
    int i_icon_index = dlg.getSelectedIconID();
    //
    ptr_actual_item->setIconByID(i_icon_index);
    //
}

void MyTree::onImportNode ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();

    if (NULL == ptr_actual_item)
        return;
    //
    QString str_msg;
    QMessageBox box;
    //
    //import under deleted node is not allowed
    //
    if (AbstractDatabaseObject::OBJECT_DELETED == ptr_actual_item->getObjectStatus())
    {
        str_msg = "Current node deleted. Can not create new child for this node";
        box.setText(str_msg);
        box.exec();
        return;
    };
    //
    QString str_dir_name = QFileDialog::getExistingDirectory(this,
                                                             tr("Choose directory for import"),
                                                             "/home/alex/WindowsShare",
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (str_dir_name.length() == 0 )
    {
        return;
    };
    //
    str_msg = "Do you want to import subdirectories also?";
    box.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    box.setDefaultButton(QMessageBox::Yes);
    box.setText(str_msg);
    //
    bool b_recursive_import = false;
    //
    int i_res = box.exec();
    if (QMessageBox::Cancel == i_res)
    {
        return;
    };
    //
    if (QMessageBox::Yes == i_res)
    {
        b_recursive_import = true;
    };
    //
    //m_bFakeChangeOfNode = true;
    //
    ptr_actual_item->importNode( str_dir_name, b_recursive_import );
    //
    //m_bFakeChangeOfNode = false;
}

void  MyTree::onItemChanged (QTreeWidgetItem* ptr_item, int column)
{
    Q_UNUSED(column);
    //
    if (NULL == ptr_item)
        return;
    //
    //QString str_name = ptr_item->text(column);
    //
    TreeLeaf* ptr_current_item = (TreeLeaf*) ptr_item;
    //ptr_item
    ptr_current_item->rename_it(NULL);
    return;
}

void  MyTree::onCurrentDBChanged     (int index)
{
    //
    if (-1 == index)
        return;
    //
    QVariant root_back_data = m_ptrTreeComboBox->itemData(index);
    RootOfTree* ptr_new_root= VariantPtr<RootOfTree>::asPtr( root_back_data );
    //
    // check - if the new top-level element matched with thwe old one
    //
    TreeLeaf* ptr_actual_top_leaf = (TreeLeaf*) this->topLevelItem(0);
    //
    if ( ptr_new_root->getChildLeaf() == ptr_actual_top_leaf )
    {
        return; //it is the same combobox index, nothing to do
    };
    //
    // if not matched - remove old top-level element
    //
    this->takeTopLevelItem(0);
    if (ptr_new_root->getChildLeaf())
        this->addTopLevelItem(ptr_new_root->getChildLeaf());
    //
    expandActualTree();
    this->setCurrentItem(this->topLevelItem(0),0);
    //this->repaint();
    //
    //emit treeSelectionChanged(ptr_actual_top_leaf, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
    //emit treeSelectionChanged(ptr_actual_top_leaf, NULL);
    //
    return;
}

void MyTree::onTreeItemExpanded     (QTreeWidgetItem* ptr_item)
{
    if (ptr_item)
    {
        TreeLeaf* ptr_leaf = (TreeLeaf*) ptr_item;
        ptr_leaf->setActualExpandState(true);
    };
    //
    //it is for expand-collapse
    //
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
}

void MyTree::onTreeItemCollapsed    (QTreeWidgetItem* ptr_item)
{
    if (ptr_item)
    {
        TreeLeaf* ptr_leaf = (TreeLeaf*) ptr_item;
        ptr_leaf->setActualExpandState(false);
    };
    //
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
}

void MyTree::onCascadeExpand ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    cascadeExpand (ptr_actual_item, true);
    //
    emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
}

void MyTree::onCascadeCollapse ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    cascadeExpand (ptr_actual_item, false);
    //
    emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
}

void MyTree::cascadeExpand ( TreeLeaf* ptr_node, bool b_expand )
{
    if (NULL == ptr_node)
        return;
    //
    ptr_node->setExpanded(b_expand);
    //
    TreeLeaf::ChildList childs = ptr_node->getChildList();
    //
    for (unsigned int i = 0; i < childs.size(); i++)
    {
        cascadeExpand( childs[i], b_expand );
    };
}

void MyTree::onImportAttach ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if (NULL == ptr_actual_item)
        return;
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    QString str_msg = tr("Select one or more files for attaching to the node '");
    str_msg += ptr_actual_item->text(0);
    str_msg += tr("'");
    //
    if ( m_strLastAttachmentFromPath.length() == 0 )
    {
        m_strLastAttachmentFromPath = settings.value(g_str_ATTACH_IMPORT_PATH).toString();
    };
    //
    AttachImportOrReplaceDlg dlg(tr("Import file(s)"), true);
    //
    bool b_res = dlg.exec(m_strLastAttachmentFromPath, "Import", m_bDeleteFilesAfterAttachment, m_bProtectAttachment, m_bEncryptAttachment);
    if (b_res)
    {
        QStringList file_list           = dlg.getFileList();
        m_bDeleteFilesAfterAttachment   = dlg.getDeleteFilesAfterAttachment();
        m_bProtectAttachment            = dlg.getProtectAttachment();
        m_bEncryptAttachment            = dlg.getEncryptAttachment();
        m_strLastAttachmentFromPath     = dlg.getLastAttachmentFromPath();
        unsigned int ui_encrypt_type    = dlg.getTypeOfEncrypt();
        //
        setCursor(Qt::BusyCursor);
        ptr_actual_item->addAttachments(file_list, m_bDeleteFilesAfterAttachment, m_bProtectAttachment, ui_encrypt_type);
        setCursor(Qt::ArrowCursor);
    };
    //
    emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
}

void MyTree::onCurrentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) current; //this->currentItem();
    //
    if (ptr_actual_item)
    {
        //request attachments list
        ptr_actual_item->getAttachments();
        //used for menu
        //
        emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
        //
        //used for attachments and description
        //
        TreeLeaf* ptr_previous_item =  (TreeLeaf*) previous;
        emit treeSelectionChanged (ptr_actual_item, ptr_previous_item);
        emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
    };
}

void MyTree::onDelCurrentItem ()
{
    //m_bFakeChangeOfNode = true;
    //
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if (ptr_actual_item)
    {
        ptr_actual_item->delete_it();
        emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
        emit updateAttachmentList();
    };
    //
    //m_bFakeChangeOfNode = false;

};

void  MyTree::onRestoreSubtree ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if (ptr_actual_item)
    {
        ptr_actual_item->restoreSubtree();
        emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
        emit updateAttachmentList();
    };
    //
    this->setCurrentItem(ptr_actual_item,0);
};

void MyTree::onRestoreCurrentItem()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if (ptr_actual_item)
    {
        ptr_actual_item->restoreObject();
        emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
        emit updateAttachmentList();
    };
    //
    this->setCurrentItem(ptr_actual_item,0);
    //
    //m_bFakeChangeOfNode = false;
}

void  MyTree::OnEncryptAttachmentsGlobal   ()
{
    execGlobalEncryptionOperation(true);   
}

void  MyTree::OnDecryptAttachmentsGlobal   ()
{
    execGlobalEncryptionOperation(false);
}

void MyTree::execGlobalEncryptionOperation ( bool b_encrypt )
{
    if ( b_encrypt )
    {
        DlgAskEncryptMethod dlg;
        //
        if ( dlg.exec() == QDialog::Rejected)
            return;
    };
    //
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    AskChangesDetailsDlg dlg;
    dlg.exec();
    AskChangesDetailsDlg::USAGE_MODE mode =  dlg.getMode();
    //
    bool b_recursive = false;
    //
    switch (mode)
    {
    case AskChangesDetailsDlg::DO_NOTHING:
        return;
    case AskChangesDetailsDlg::ONLY_ONE_NODE:
        b_recursive = false;
        break;
    case AskChangesDetailsDlg::CHILDS_ALSO:
        b_recursive = true;
        break;
    default:
        Q_ASSERT(FALSE);
    };
    //
    if (ptr_actual_item)
    {
        Attachment::AttachmentsList v_attachments;
        ptr_actual_item->getAttachments (v_attachments, b_recursive);
        //ptr_actual_item->encr_decr_all_attachments(b_encrypt, b_recursive);
        initOnDemand();
        //
        if (b_encrypt)
        {
            m_ptrEncrypter->setAction(CryptoThread::ENCRYPT_IT);
            m_ptrProgress->setWindowTitle("Encrypt list of attachments");
        }
        else
        {
            m_ptrEncrypter->setAction(CryptoThread::DECRYPT_IT);
            m_ptrProgress->setWindowTitle("Decrypt list of attachments");
        };
        //
        m_ptrEncrypter->addAttacmentsToProcessList(v_attachments);
        //
        m_ptrProgress->setMinimum(0);
        m_ptrProgress->setMaximum(v_attachments.size());
        //
        m_ptrEncrypter->start(QThread::IdlePriority);
        m_ptrProgress->exec();
        m_ptrEncrypter->wait();
        //
        emit updateAttachmentList ();
    };
    //
    emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
}

void MyTree::OnProtectAttachmentsGlobal   ()
{
    execGlobalProtectionOperation(true);
}

void MyTree::OnUnProtectAttachmentsGlobal ()
{
    execGlobalProtectionOperation(false);
}

void MyTree::execGlobalProtectionOperation ( bool b_protect )
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    AskChangesDetailsDlg dlg;
    dlg.exec();
    AskChangesDetailsDlg::USAGE_MODE mode =  dlg.getMode();
    //
    bool b_recursive = false;
    //
    switch (mode)
    {
    case AskChangesDetailsDlg::DO_NOTHING:
        return;
    case AskChangesDetailsDlg::ONLY_ONE_NODE:
        b_recursive = false;
        break;
    case AskChangesDetailsDlg::CHILDS_ALSO:
        b_recursive = true;
        break;
    default:
        Q_ASSERT(FALSE);
    }
    //
    if (ptr_actual_item)
    {
        Attachment::AttachmentsList v_attachments;
        ptr_actual_item->getAttachments (v_attachments, b_recursive);
        //ptr_actual_item->encr_decr_all_attachments(b_encrypt, b_recursive);
        initOnDemand();
        //
        if (b_protect)
        {
            m_ptrEncrypter->setAction(CryptoThread::PROTECT_IT);
            m_ptrProgress->setWindowTitle("Protect list of attachments");
        }
        else
        {
            m_ptrEncrypter->setAction(CryptoThread::UNPROTECT_IT);
            m_ptrProgress->setWindowTitle("Encrypt list of attachments");
        };
        //
        m_ptrEncrypter->addAttacmentsToProcessList(v_attachments);
        //
        m_ptrProgress->setMinimum(0);
        m_ptrProgress->setMaximum(v_attachments.size());
        //
        m_ptrEncrypter->start(QThread::IdlePriority);
        m_ptrProgress->exec();
        m_ptrEncrypter->wait();
        //
        emit updateAttachmentList ();
    };
    //
    emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
}

void MyTree::onInsertNewItem ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();

    if (NULL == ptr_actual_item)
        return;
    //
    QString str_msg;
    QMessageBox box;
    //
    if (AbstractDatabaseObject::OBJECT_DELETED == ptr_actual_item->getObjectStatus())
    {
        str_msg = "Current node deleted. Can not create new child for this node";
        box.setText(str_msg);
        box.exec();
        return;
    };
    //
    //m_bFakeChangeOfNode = true;
    //
    TreeLeaf* ptr_new_leaf = new TreeLeaf(ptr_actual_item, -1, ptr_actual_item->getTreeID(), tr("New node"), this );
    if (ptr_new_leaf->getID() == -1)
    {
        str_msg = "Can not create new node. Database error.";
        box.setText(str_msg);
        box.exec();
        //m_bFakeChangeOfNode = false;
        return;
    }
    //
    ptr_actual_item->addChild(ptr_new_leaf);
    ptr_actual_item->setExpanded(true);
    //
    this->setCurrentItem(ptr_new_leaf, 0);
    //
    //m_bFakeChangeOfNode = false;
};

void MyTree::leafAttachmentUpdated (TreeLeaf* ptr_to_leaf, bool b_result, QString str_attachment_name)
{
    QMutexLocker locker (&m_UpdateAttachmentInfoLocker);
    //
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if (ptr_to_leaf != ptr_actual_item)
        return;
    //
    //emit signal for updating the list of attachments
    //
    if (b_result)
    {
        emit updateAttachmentList ();
        emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
    }
    else
    {
        QMessageBox box;
        box.setText("Can not update attachment " + str_attachment_name);
        box.exec();
    };
}

bool MyTree::isPossibleMoveNodeHere(TreeLeaf* ptr_moving_node, TreeLeaf* ptr_target_node, bool b_silence)
{
    //
    QString         str_message;
    QMessageBox     box;
    //do not move empty
    if  (
            (NULL == ptr_moving_node) ||
            (NULL == ptr_target_node)
        )
    {
        return false;
    };
    //do not move under deleted
    if ( (ptr_moving_node->getObjectStatus() != AbstractDatabaseObject::OBJECT_OK) ||
         (ptr_target_node->getObjectStatus() == AbstractDatabaseObject::OBJECT_DELETED) //impossible to drop on deleted node
       )
    {
        if (false == b_silence)
        {
            str_message = tr("Impossible to move node here. Target node is deleted or moving node has wrong status.");
            box.setText(str_message);
            box.exec();
        };
        //
        return false;
    };
    //do not move under same place
    if( ptr_moving_node->getID() == ptr_target_node->getID() )
    {
        if (false == b_silence)
        {
            str_message = tr("Node is already here. Nothing to do.");
            box.setText(str_message);
            box.exec();
        };
        return false;
    };
    //
    //do not move under same parent
    if( ptr_moving_node->getParentID() == ptr_target_node->getID() )
    {
        if (false == b_silence)
        {
            str_message = tr("Node is already here. Nothing to do.");
            box.setText(str_message);
            box.exec();
        };
        return false;
    };
    //do not move parent under child
    TreeLeaf* ptr_parent_of_parent_node = ptr_target_node->getParent();
    //
    while (ptr_parent_of_parent_node)
    {
        if (ptr_parent_of_parent_node->getID() == ptr_moving_node->getID())
        {
            if (false == b_silence)
            {
                str_message = tr("Can not move node under it child.");
                box.setText(str_message);
                box.exec();
            };
            return false;
        };
        ptr_parent_of_parent_node = ptr_parent_of_parent_node->getParent();
    };
    //
    return true;
};

void MyTree::onCutNode  ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if (ptr_actual_item)
    {
        m_ptrMovedItem = ptr_actual_item;
        QString str_status_msg = tr("Choose new parent node and click Alt+V");
        m_ptrStatusBar->showMessage(str_status_msg);
    };
};

void MyTree::onPasteNode()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if (ptr_actual_item && m_ptrMovedItem)
    {
        if ( isPossibleMoveNodeHere(m_ptrMovedItem, ptr_actual_item, false) == true )
        {
            m_ptrMovedItem->setNewParent( ptr_actual_item );
            //
            showMoveNodeMessage( m_ptrMovedItem->text(0), ptr_actual_item->text(0) );
/*
            QString str_status_msg = tr("node '");
            str_status_msg +=  m_ptrMovedItem->text(0);
            str_status_msg +=  tr("' moved under node '");
            str_status_msg +=  ptr_actual_item->text(0);
            str_status_msg +=  tr("'.");
            //
            m_ptrStatusBar->showMessage(str_status_msg);
*/
            ptr_actual_item->setExpanded(true);
            m_ptrMovedItem->setExpanded(true);
            m_ptrMovedItem = NULL;
            //
            emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
        };
    };
};

void MyTree::showMoveNodeMessage(const QString& str_name_of_moved, const QString& str_name_of_target)
{
    QString str_status_msg = tr("node '");
    str_status_msg +=  str_name_of_moved;
    str_status_msg +=  tr("' moved under node '");
    str_status_msg +=  str_name_of_target;
    str_status_msg +=  tr("'.");
    //
    m_ptrStatusBar->showMessage(str_status_msg);
};

void MyTree::mouseReleaseEvent (QMouseEvent *event)
{
    Qt::MouseButton btn = event->button();
    // Qt::LeftButton, Qt::RightButton
    if (btn == Qt::LeftButton)
    {
        if (m_bBeginDrag)
        {
            TreeLeaf* ptr_target_leaf = (TreeLeaf*) this->itemAt ( event->pos().x(), event->pos().y() );
            //try to move the node here
            if (m_ptrMovedItem && ptr_target_leaf)
            {
                if(isPossibleMoveNodeHere(m_ptrMovedItem, ptr_target_leaf))
                {
                    m_ptrMovedItem->setNewParent( ptr_target_leaf );
                    showMoveNodeMessage( m_ptrMovedItem->text(0), ptr_target_leaf->text(0) );
                    m_ptrMovedItem = NULL;
                };
            };
            m_bBeginDrag = false;
            setCursor(Qt::ArrowCursor);
        };
    }else if (btn == Qt::RightButton )
    {
        emit showPopupMenu();
    }
    //
    QTreeWidget::mouseReleaseEvent(event);
};

void MyTree::mousePressEvent (QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    //
    m_pointDearStartPosition = event->pos();
};

void MyTree::mouseMoveEvent (QMouseEvent *event)
{
    if ( !(event->buttons() & Qt::LeftButton) )
    {
        QTreeWidget::mouseMoveEvent(event);
        return;
    };
    //
    if ( (event->pos() - m_pointDearStartPosition).manhattanLength()  < QApplication::startDragDistance() )
    {
        if (false == m_bBeginDrag)
        {
            //get candidate for drag
            m_ptrMovedItem = (TreeLeaf*) this->itemAt ( event->pos().x(), event->pos().y() );

            if (
                    (NULL == m_ptrMovedItem)              ||
                    (m_ptrMovedItem->getParentID() == 0)  ||
                    (m_ptrMovedItem->getObjectStatus() != AbstractDatabaseObject::OBJECT_OK)
               )
            {
                //bad candidate...
                m_ptrMovedItem = NULL;
            }else
            {
                m_bBeginDrag = true;
            };
        };
    };
    //
    if (true == m_bBeginDrag)
    {
        //is it possible to drop here?
        QTreeWidgetItem* ptr_to_highlighted_item = this->itemAt ( event->pos().x(), event->pos().y() );
        if (ptr_to_highlighted_item)
        {
            TreeLeaf* ptr_to_target_node = (TreeLeaf*) ptr_to_highlighted_item;
            //
            if (isPossibleMoveNodeHere(m_ptrMovedItem, ptr_to_target_node) == true)
            {
                QCursor drag_enable (QPixmap(tr("images/drag_node.png")));
                setCursor(drag_enable);
            }
            else
            {
                QCursor drag_disable (QPixmap(tr("images/no_drag_node.png")));
                setCursor(drag_disable);
            };
        }else
        {
            QCursor drag_disable (QPixmap(tr("images/no_drag_node.png")));
            setCursor(drag_disable);
        };
        //ForbiddenCursor

    };
    //
    QTreeWidget::mouseMoveEvent(event);
};

void MyTree::keyPressEvent(QKeyEvent *event)
{
    //QMessageBox box;
    //
    //
    int key_code = event->key();
    //
    switch( key_code )
    {
//    case Qt::Key_Escape:
//        break;
//
//    case Qt::Key_Delete:
//            emit delCurrentItem();        //already done in menu
//        break;
//
//    case Qt::Key_Insert:
//            emit insertNewItem();
//        break;
//    case Qt::Key_R:                     //restore
//            emit restoreDeletedItem();
//        break;
//        case Qt::Key_F2:
//        m_F2 = true;
//        break;
    default:
        break;
    };
    //
    QTreeWidget::keyPressEvent(event);
};


void MyTree::setTreeComboBox(QComboBox* ptr_comobobox)
{
    Q_ASSERT ( ptr_comobobox );
    //
    m_ptrTreeComboBox = ptr_comobobox;
};

void MyTree::init()
{
    getRootsFromDatabase_DB();
    //
    m_IconLoadLocker.lock();
    //
    getLeafsFromDatabase_DB();
    //
    m_IconLoadLocker.unlock();
    //
    this->setSortingEnabled(true);
    //
    int i_last_used_tree_id = getLastUsedRootID_DB();
    //
    if (-1 != i_last_used_tree_id)
    {
        //
        for (int i = 0; i < m_ptrTreeComboBox->count(); i++)
        {
            QVariant root_back_data = m_ptrTreeComboBox->itemData(i);
            RootOfTree* ptr_root    = VariantPtr<RootOfTree>::asPtr( root_back_data );
            if (ptr_root)
            {
                if ( ptr_root->getID() == i_last_used_tree_id)
                {
                    m_ptrTreeComboBox->setCurrentIndex(i);
                    onCurrentDBChanged(i);
                    emit endOfInit(i);
                    break;
                };
            };
        };
    }else
    {
        m_ptrTreeComboBox->setCurrentIndex(-1);
    };
}

int MyTree::getLastUsedRootID_DB    () const
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
    QString str_query = "SELECT id_tree FROM root_tbl where active = 1 AND current_active = 1;";
    //
    if ( !qry.prepare( str_query ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
        return -1;
    };
    //
    if( !qry.exec() )
    {
        QMessageBox box;
        box.setText("Unable to get exec the query. Stop. ");
        box.exec();
        //
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
        return -1;
    };
    //
    while( qry.next() )
    {
        int         i_tree_id       = qry.value(0).toInt();
        return      i_tree_id;
    };
    //
    return -1;
}

bool MyTree::setLastUsedRootID_DB   () const
{
    if (m_ptrTreeComboBox->currentIndex() == -1)
        return false;
    //
    QVariant root_back_data = m_ptrTreeComboBox->itemData(m_ptrTreeComboBox->currentIndex());
    RootOfTree* ptr_root    = VariantPtr<RootOfTree>::asPtr( root_back_data );
    //
    if (NULL == ptr_root)
        return false;
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_query_1 = QString ("UPDATE root_tbl  SET current_active = 0 WHERE id_tree <> %1;").arg( ptr_root->getID() );
    QString str_query_2 = QString ("UPDATE root_tbl  SET current_active = 1 WHERE id_tree = %1;").arg( ptr_root->getID() );
    //
    if ( ptr_db->driver()->hasFeature(QSqlDriver::Transactions) )
        ptr_db->driver()->beginTransaction();
    //
    bool b_exec =qry.exec(str_query_1);
    //
    if (b_exec)
        b_exec =qry.exec(str_query_2);
    //
    if ( ptr_db->driver()->hasFeature(QSqlDriver::Transactions) )
    {
        if (b_exec)
            ptr_db->driver()->commitTransaction();
        else
            ptr_db->driver()->rollbackTransaction();
    };
    //
    return b_exec;
}

bool  MyTree::getRootsFromDatabase_DB()
{
    bool b_res = false;
    //
        DBAcccessSafe   db_safe;
        //
        QSqlDatabase* ptr_db = db_safe.getDB();
        if (NULL == ptr_db)
            return false;
        //
        QSqlQuery qry(*ptr_db);
        //
        QString str_query = "SELECT id_tree, tree_name, last_change, active FROM root_tbl where active = 1;";
        //
        if ( !qry.prepare( str_query ) )
        {
            Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
            return false;
        };
        //
        if( !qry.exec() )
        {
            QMessageBox box;
            box.setText("Unable to get exec the query. Stop. ");
            box.exec();
            //
            Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
            b_res = false;
        }else
        {
            unsigned int ui_item_number = 0;
            //
            while( qry.next() )
            {
                int         i_tree_id       = qry.value(0).toInt();
                QString     str_root_name   = qry.value(1).toString();
                QDateTime   dt_time         = qry.value(2).toDateTime();
                bool        b_active        = qry.value(3).toBool();
                RootOfTree* ptr_new_root    = new RootOfTree(str_root_name, i_tree_id, dt_time, b_active, NULL);
                //
                addNewRoot  (ptr_new_root);
                //
                ui_item_number++;
            };
            //
            b_res = true;
         };
    //
    b_res = true;
    //
    return b_res;
}

void MyTree::addNewRoot (RootOfTree* ptr_new_root)
{
    QVariant new_tree_root = VariantPtr<RootOfTree>::asQVariant(ptr_new_root);
    m_ptrTreeComboBox->addItem(ptr_new_root->getName(), new_tree_root);
}

bool MyTree::getLeafsFromDatabase_DB()
{
    std::vector <TreeLeaf*>  v_orpheline_nodes;
    //
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    if (NULL == ptr_db)
        return false;
    //
    QSqlQuery qry(*ptr_db);
    //
    QString str_query = "select id_node, id_parent, id_tree, id_icon, node_name, node_color, node_descriptor, active, expanded, last_change from node_tbl order by id_tree, id_parent, id_node asc;";
    //
    if ( !qry.prepare( str_query ) )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
        return false;
    };
    //
    if( !qry.exec() )
    {
        Logger::getInstance().logIt( en_LOG_ERRORS, qry.lastError().text(), &str_query );
        QMessageBox box;
        box.setText("Unable to get exec the query. Stop. ");
        box.exec();
        //
        return false;
    }else
    {
        RootOfTree* actual_root_of_tree = NULL;

        while( qry.next() )
        {
            const int     i_node_id           = qry.value(0).toInt();
            const int     i_parent_node_id    = qry.value(1).toInt();
            const int     i_parent_tree_id    = qry.value(2).toInt();
            const int     i_icon_id           = qry.value(3).toInt();
            const QString str_node_name       = qry.value(4).toString();
            const QString str_node_color      = qry.value(5).toString();
            const QString str_node_descriptor = qry.value(6).toString();
            const bool    b_node_active       = qry.value(7).toBool();
            const bool    b_expanded          = qry.value(8).toBool();
/*
            QString str_check = qry.value(5).toString();
            int i_check = qry.value(5).toInt();
*/
            QDateTime dt_time_change    = qry.value(7).toDateTime();
            //
            // if actual tree root is not defined or does not matched with the new node
            //
            if (
                    (NULL == actual_root_of_tree) ||
                    (actual_root_of_tree->getID() != i_parent_tree_id)
               )
            {
                actual_root_of_tree = getRootByTreeId (i_parent_tree_id);
                //Q_ASSERT ( actual_root_of_tree );
                //
                if (NULL == actual_root_of_tree)
                    continue;
            };
            //
            TreeLeaf* ptr_new_leaf = NULL;
            //
            if (0 == i_parent_node_id) //this is root element of the tree
            {
                //MyTree* ptr_null = NULL;
                //
                ptr_new_leaf = new TreeLeaf(NULL,
                                            i_node_id,
                                            i_parent_node_id,
                                            i_parent_tree_id,
                                            i_icon_id,
                                            str_node_name,
                                            str_node_color,
                                            str_node_descriptor,
                                            b_node_active,
                                            b_expanded,
                                            dt_time_change,
                                            this);
                //
                actual_root_of_tree->addChildLeaf(ptr_new_leaf);
                //
//
//                if (i_parent_tree_id == 1)
//                    this->addTopLevelItem(ptr_new_leaf);
//

            }
            else //this is non-root element of the tree
            {
                //root object returns top-level node, and after it - recurse search of the node with the right node ID
                TreeLeaf* ptr_parent_leaf = actual_root_of_tree->getChildLeaf()->getChildNode(i_parent_node_id);
                //
                // the parent of this not we did not download yet. Store it into special place temporary.
                //
                bool b_it_is_orpheline = false;

                if (NULL == ptr_parent_leaf)
                {
                    ptr_parent_leaf = actual_root_of_tree->getChildLeaf(); //temporary put int under the root node
                    b_it_is_orpheline = true;
                };
                //
                Q_ASSERT ( ptr_parent_leaf );
                //
                ptr_new_leaf = new TreeLeaf(ptr_parent_leaf,
                                            i_node_id,
                                            i_parent_node_id, //because can be orpheline
                                            i_parent_tree_id,
                                            i_icon_id,
                                            str_node_name,
                                            str_node_color,
                                            str_node_descriptor,
                                            b_node_active,
                                            b_expanded,
                                            dt_time_change,
                                            this);
                //
                if (b_it_is_orpheline)
                {
                    v_orpheline_nodes.push_back(ptr_new_leaf);
                };
            };
        };//while( qry.next() )
    };//if( !qry.exec() )
/*
  ------------------------------------------------------------------
*/
    unsigned int i_found_counter = 0;
    //
    for (int i = 0; i < m_ptrTreeComboBox->count(); i++)
    {
        QVariant root_back_data = m_ptrTreeComboBox->itemData(i);
        RootOfTree* ptr_root    = VariantPtr<RootOfTree>::asPtr( root_back_data );
        //
        std::vector <TreeLeaf*>::iterator itr;
        //

        //
        for (itr = v_orpheline_nodes.begin(); itr != v_orpheline_nodes.end(); itr++)
        {
            int i_search_id = (*itr)->getParentID();
            //
            TreeLeaf* ptr_parent_leaf = ptr_root->getChildLeaf()->getChildNode(i_search_id);
            //
            if (NULL ==  ptr_parent_leaf)
                continue;
            //
            (*itr)->setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsDropEnabled);
            (*itr)->setNewParent(ptr_parent_leaf);
            i_found_counter++;
            //
        };
    };
    //
    if (i_found_counter != v_orpheline_nodes.size())
    {
        Q_ASSERT ( FALSE );        //something wrong witth DB!!!
    };
    //
    // v_orpheline_nodes.erase(v_orpheline_nodes.begin(), v_orpheline_nodes.end());
    //
    return true;
};

void MyTree::expandActualTree ()
{
    const int i_index = m_ptrTreeComboBox->currentIndex();
    if (-1 == i_index)
        return;
    //
    QVariant root_back_data = m_ptrTreeComboBox->itemData(i_index);
    RootOfTree* ptr_tree_root = VariantPtr<RootOfTree>::asPtr( root_back_data );
    Q_ASSERT ( ptr_tree_root );
    //
    TreeLeaf*  ptr_root_node = ptr_tree_root->getChildLeaf();
    Q_ASSERT (ptr_root_node);
    //
    if (ptr_root_node)
        expandActualTree(ptr_root_node);
    //
};

void MyTree::expandActualTree (TreeLeaf*  ptr_root_leaf)
{
    if (NULL == ptr_root_leaf)
        return;
    //
    if ( ptr_root_leaf->isActualStateIsExpand() )
            this->expandItem(ptr_root_leaf);
        else
            this->collapseItem(ptr_root_leaf);
    //
    for (int i = 0; i < ptr_root_leaf->childCount(); ++i)
    {
        expandActualTree((TreeLeaf*) ptr_root_leaf->child(i));
    };
};
//
void MyTree::saveExpandStateOfAllTrees ()
{
    for (int i = 0; i < m_ptrTreeComboBox->count(); i++)
    {
        QVariant root_back_data = m_ptrTreeComboBox->itemData(i);
        RootOfTree* ptr_root_source = VariantPtr<RootOfTree>::asPtr( root_back_data );
        //
        saveExpandStateOfTree ( ptr_root_source->getChildLeaf() );
    };
}
//
void MyTree::saveExpandStateOfTree ( TreeLeaf* ptr_node )
{
    ptr_node->saveToDBExpandStateChange();
    //
    for (int i = 0; i < ptr_node->childCount(); ++i)
    {
        saveExpandStateOfTree( (TreeLeaf*) ptr_node->child(i) );
    };
};
//
RootOfTree* MyTree::getRootByTreeId(int i_tree_id)
{
    RootOfTree* ptr_root = NULL;
    //
    for (int i = 0; i < m_ptrTreeComboBox->count(); i++)
    {
        QVariant root_back_data = m_ptrTreeComboBox->itemData(i);
        RootOfTree* ptr_root_source = VariantPtr<RootOfTree>::asPtr( root_back_data );
        //
        if (ptr_root_source->getID() == i_tree_id)
            return ptr_root_source;
    };
    //
    return ptr_root;
};

void MyTree::onAcceptCutAttachment  (Attachment::AttachmentsList att_list)
{

    m_vActualCutAtttachments = att_list;
};

void MyTree::onPasteAttachments ()
{
    TreeLeaf* ptr_actual_item =  (TreeLeaf*) this->currentItem();
    //
    if ( NULL == ptr_actual_item )
        return;
    //make past attachments here
    Attachment::AttachmentsList::iterator itr_attach;
    //
    for (itr_attach = m_vActualCutAtttachments.begin(); itr_attach != m_vActualCutAtttachments.end(); ++itr_attach)
    {
        (*itr_attach)->setNewParent( ptr_actual_item );
    };

    //
    m_vActualCutAtttachments.erase( m_vActualCutAtttachments.begin(), m_vActualCutAtttachments.end() );
    //for menu
    emit treeSelectionChanged(ptr_actual_item, (m_ptrMovedItem != NULL), (m_vActualCutAtttachments.size() > 0) );
    //for table
    emit treeSelectionChanged(ptr_actual_item, NULL);

}
