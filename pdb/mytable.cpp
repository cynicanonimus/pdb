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
#include "mytable.h"
#include "treeleaf.h"
#include "../CommonInclude/pdb/VariantPtr.h"
#include "attachmentclickationdialog.h"
#include "attachimportorreplacedlg.h"
#include "tmpfilecleaner.h"
#include "../CommonInclude/pdb/pdb_style.h"
#include "logger.h"
#include "dlgaskencryptmethod.h"
//
#include <QDesktopServices>
#include  <QHeaderView>
//

MyTable::MyTable(QWidget *parent) :
    QTableWidget(parent)
{
    //
    m_bDeleteAfterAttachReplacement = false;
    m_bProtectAttachReplacement     = false;
    m_bEncryptAttachReplacement     = false;
    //
    m_ptrEncrypter                  = NULL;
    m_ptrProgress                   = NULL;
    //
    this->setColumnCount(ITEM_AMOUNT);
    QStringList str_att_header;
    //              0     1      2          3           4            5
    str_att_header<<"P"<<"E"<< "Name of document" << "Data" << "Size, bytes"<< "Status";
    this->setHorizontalHeaderLabels(str_att_header);
    this->setEditTriggers( QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed );
    //
    QHeaderView *header = this->horizontalHeader();
    //
    this->setColumnWidth(ITEM_LOCKED,   22);
    this->setColumnWidth(ITEM_ENCRYPTED,22);
    this->setColumnWidth(ITEM_NAME,     145);
    this->setColumnWidth(ITEM_DATA,     150);
    this->setColumnWidth(ITEM_SIZE,     100);
    this->setColumnWidth(ITEM_STATUS,   60);
    //
    header->setResizeMode(ITEM_LOCKED,      QHeaderView::Fixed);
    header->setResizeMode(ITEM_ENCRYPTED,   QHeaderView::Fixed);
    header->setResizeMode(ITEM_NAME,        QHeaderView::Stretch);
    header->setResizeMode(ITEM_DATA,        QHeaderView::Fixed);
    header->setResizeMode(ITEM_SIZE,        QHeaderView::Fixed);
    header->setResizeMode(ITEM_STATUS,      QHeaderView::Fixed);
    //
    m_bFillModeOn = false;
    //void 	cellClicked ( int row, int column )
//    QObject::connect( this, SIGNAL (cellClicked(int, int)), this, SLOT( onCellClicked(int, int) ) );
    QObject::connect( this, SIGNAL ( itemChanged            ( QTableWidgetItem* )   ), this, SLOT( onItemChanged (QTableWidgetItem* ) ) );
    QObject::connect( this, SIGNAL ( itemSelectionChanged   ()                      ), this, SLOT( onItemSelectionChanged()           ) );
}

MyTable::~MyTable()
{
    if (m_ptrEncrypter)
        delete m_ptrEncrypter;
    //
    if (m_ptrProgress)
        delete m_ptrProgress;
}

void MyTable::onSelectedNodeChanged(TreeLeaf* ptr_to_current, TreeLeaf* ptr_to_previous)
{
    Q_UNUSED(ptr_to_previous);
    //
    m_ptrActualLeaf = ptr_to_current;
    //
    refreshCurrentAttachmentsList();
};

void MyTable::refreshCurrentAttachmentsList()
{
    clearTable();
    //
    if (NULL != m_ptrActualLeaf)
    {
        showAttachments( m_ptrActualLeaf->getAttachments() );
    };
};

void MyTable::clearTable()
{
    while (this->rowCount() > 0)
    {
        this->removeRow(0);
    }
}

void MyTable::showAttachments (const Attachment::AttachmentsList& t_attachment_list)
{
    if ( t_attachment_list.size() > 0 )
    {
        this->setRowCount ( t_attachment_list.size() );
        //
    }else
        return;    
    //
    //m_ptrStatusBar->showMessage(tr(""));
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    m_bFillModeOn = true;
    //
    for (unsigned int i = 0; i < t_attachment_list.size(); i++)
    {
        Attachment* ptr_attachment  = t_attachment_list[i];
        //
        // -------------------- get lock (protected) icon for of attachment --------------------
        //
        QTableWidgetItem*   ptr_item_locked = makeCellLocked( ptr_attachment );
        //
        // -------------------- get encrypt icon for of attachment --------------------
        //
        QTableWidgetItem*   ptr_item_encrypted = makeCellEncrypted ( ptr_attachment );
        // -------------------- get name of attachment --------------------
        //
        QTableWidgetItem*   ptr_item_name   = makeCellName( ptr_attachment );
        //
        // -------------------- get date of attachment --------------------
        //
        QTableWidgetItem* ptr_item_data = makeCellData( ptr_attachment );
        //
        // -------------------- get size of attachment --------------------
        //
        QTableWidgetItem* ptr_item_size = makeCellSize( ptr_attachment );
        //
        // -------------------- get status of attachment and set name cell flags--------------------
        //
        QTableWidgetItem* ptr_item_status = makeCellStatus( ptr_attachment );
        //
        this->setItem( i, ITEM_LOCKED,      ptr_item_locked     );
        this->setItem( i, ITEM_ENCRYPTED,   ptr_item_encrypted  );
        this->setItem( i, ITEM_NAME,        ptr_item_name       );
        this->setItem( i, ITEM_DATA,        ptr_item_data       );
        this->setItem( i, ITEM_SIZE,        ptr_item_size       );
        this->setItem( i, ITEM_STATUS,      ptr_item_status     );
    };
    //
    m_bFillModeOn = false;
};

QTableWidgetItem* MyTable::makeCellSize (Attachment* ptr_attachment)
{
    const QLocale& cLocale = QLocale::c();
    QString str_size = cLocale.toString( ptr_attachment->getSize() );

    QTableWidgetItem* ptr_item_size = new  QTableWidgetItem( str_size );
    ptr_item_size->setFlags( Qt::ItemIsEnabled );
    ptr_item_size->setTextAlignment(Qt::AlignRight|Qt::AlignCenter);
    setItemColor( ptr_item_size,    ptr_attachment );
    //
    return ptr_item_size;
};

QTableWidgetItem* MyTable::makeCellStatus  (Attachment* ptr_attachment)
{
    QString str_status;
    //
    if ( ptr_attachment->getObjectStatus() == AbstractDatabaseObject::OBJECT_DELETED)
    {
        str_status = tr("Deleted");
    }else if ( ptr_attachment->getObjectStatus() == AbstractDatabaseObject::OBJECT_OK )
    {
        str_status = tr("OK");
    }else if ( ptr_attachment->getObjectStatus() == AbstractDatabaseObject::OBJECT_NOT_DEFINED)
    {
        str_status = tr("FAIL");
    } else  //update status
    {
        str_status = tr("Updating");
    };
    //
    QTableWidgetItem* ptr_item_status = new  QTableWidgetItem (str_status);
    ptr_item_status->setFlags( Qt::ItemIsEnabled );
    setItemColor(ptr_item_status,   ptr_attachment );
    //
    return ptr_item_status;
};

QTableWidgetItem* MyTable::makeCellName (Attachment* ptr_attachment)
{
    QString str_attachment_name = ptr_attachment->getName();
    //
    QTableWidgetItem*   ptr_item_name   = new  QTableWidgetItem( str_attachment_name );
    //
    if ( ptr_attachment->getObjectStatus() == AbstractDatabaseObject::OBJECT_DELETED)
    {
        ptr_item_name->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
        ptr_item_name->setToolTip( tr("Attachment selected as 'deleted'. Impossible rename or  move it") );
    }else if ( ptr_attachment->getObjectStatus() == AbstractDatabaseObject::OBJECT_OK )
    {
        if ( ptr_attachment->is_protected() )
        {
            ptr_item_name->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable); //not allow to edit protected item
        }else
        {
            ptr_item_name->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
        };
    }else if ( ptr_attachment->getObjectStatus() == AbstractDatabaseObject::OBJECT_NOT_DEFINED)
    {
        ptr_item_name->setFlags( Qt::ItemIsEnabled);
    } else  //update status
    {
        ptr_item_name->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
    };
    //
    //add attachment as a data to the name
    //
    QVariant attachment_object = VariantPtr<Attachment>::asQVariant( ptr_attachment );
    ptr_item_name->setData(1,attachment_object);
    //
    setItemColor( ptr_item_name,    ptr_attachment );
    //
    return ptr_item_name;
}

QTableWidgetItem* MyTable::makeCellData (Attachment* ptr_attachment)
{
    QString str_data = QString::number( ptr_attachment->getDateTime().date().day() );
    str_data += "-";
    str_data += QString::number( ptr_attachment->getDateTime().date().month() );
    str_data += "-";
    str_data += QString::number( ptr_attachment->getDateTime().date().year() );
    str_data += " ";
    str_data += QString::number( ptr_attachment->getDateTime().time().hour() );
    str_data += ":";
    str_data += QString::number( ptr_attachment->getDateTime().time().minute() );
    //
    QTableWidgetItem* ptr_item_data = new  QTableWidgetItem(str_data);
    setItemColor( ptr_item_data, ptr_attachment );
    ptr_item_data->setFlags( Qt::ItemIsEnabled );
    ptr_item_data->setTextAlignment(Qt::AlignCenter);
    //
    return ptr_item_data;
}

QTableWidgetItem* MyTable::makeCellEncrypted(Attachment* ptr_attachment)
{
    QTableWidgetItem*   ptr_item_encrypted   = new  QTableWidgetItem();
    //
    if ( ptr_attachment->is_crypted() )
    {
        ptr_item_encrypted->setIcon(QIcon(":/images/images/lock.png"));
        ptr_item_encrypted->setToolTip(tr("Attachment is encrypted."));
    }else
    {
        ptr_item_encrypted->setToolTip(tr("Attachment is not encrypted."));
    };
    //
    return ptr_item_encrypted;
}

QTableWidgetItem* MyTable::makeCellLocked(Attachment* ptr_attachment)
{
    QTableWidgetItem*   ptr_item_locked   = new  QTableWidgetItem();
    //
    if ( ptr_attachment->is_protected() )
    {
        ptr_item_locked->setIcon(QIcon(":/images/images/shield_green.png"));
        ptr_item_locked->setToolTip(tr("Attachment is protected. Impossible move, update, rename or delete it."));
    }else
    {
        //ptr_item_locked->setIcon(QIcon(":/images/images/shield.png"));
        ptr_item_locked->setToolTip(tr("Attachment is not protected. You can move, rename, update or delete it."));
    };
    //
    ptr_item_locked->setFlags( Qt::ItemIsEnabled );
    //
    return ptr_item_locked;
}

void MyTable::setItemColor (QTableWidgetItem* ptr_item, const Attachment *ptr_tree_attachment)
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    QColor fg_color;
    QColor bg_color;
    //
    if ( ptr_tree_attachment->getObjectStatus() == AbstractDatabaseObject::OBJECT_DELETED)
    {
        fg_color = settings.value(g_str_CLR_ATTACH_DELETED_FG).value<QColor>();
        bg_color = settings.value(g_str_CLR_ATTACH_DELETED_BG).value<QColor>();
    }else if  ( ptr_tree_attachment->getObjectStatus() == AbstractDatabaseObject::OBJECT_OK)
    {
        fg_color = settings.value(g_str_CLR_ATTACH_NORNAL_FG).value<QColor>();
        bg_color = settings.value(g_str_CLR_ATTACH_NORNAL_BG).value<QColor>();
    }else if  ( ptr_tree_attachment->getObjectStatus() == AbstractDatabaseObject::OBJECT_NOT_DEFINED)
    {
        fg_color = settings.value(g_str_CLR_ATTACH_FAIL_FG).value<QColor>();
        bg_color = settings.value(g_str_CLR_ATTACH_FAIL_BG).value<QColor>();
    }else //updating
    {
        fg_color = settings.value(g_str_CLR_ATTACH_UPDATING_FG).value<QColor>();
        bg_color = settings.value(g_str_CLR_ATTACH_UPDATING_BG).value<QColor>();

    };
    //
    ptr_item->setForeground( fg_color );
    ptr_item->setBackground( bg_color );
};
//
void MyTable::onAttachmentUpdated()
{
    m_bFillModeOn = true;
    clearTable();
    showAttachments( m_ptrActualLeaf->getAttachments() );
    m_bFillModeOn = false;
};

void MyTable::setStatusBar (QStatusBar* ptr_statusbar)
{
    Q_ASSERT( ptr_statusbar );
    //
    m_ptrStatusBar = ptr_statusbar;
}

Attachment* MyTable::getLinkedAttachment (QTableWidgetItem* ptr_item )
{
    QVariant v_attachment_object  = ptr_item->data(1);
    if ( v_attachment_object.isValid() )
    {
        Attachment* ptr_attach = VariantPtr<Attachment>::asPtr( v_attachment_object );
        //Q_ASSERT ( ptr_attach );
        return ptr_attach;
    };
    //
    return NULL;
}

void  MyTable::onItemChanged (QTableWidgetItem* item)
{
    if (m_bFillModeOn)
        return;
    //
    QString  str = item->text();
    //
    if (str.compare(m_strAttachmentNameBeforeChange) == 0)
        return; //nothing to do
    //
    if ( false == isStrValidAsFileName(str) )
    {
        item->setText(m_strAttachmentNameBeforeChange);
        return;
    };
    //call rename process but make setData first!
    //QVariant v_attachment_object  = item->data(1);
    //VariantPtr<Attachment>::asPtr( v_attachment_object );
    //
    Attachment* ptr_attach = getLinkedAttachment (item);
    //
    if (ptr_attach)
    {
        ptr_attach->rename_it(str);
        //
        if (m_ptrStatusBar)
            m_ptrStatusBar->showMessage(tr("Attachment renamed"));
    };
}

void  MyTable::onItemSelectionChanged ()
{
    //QList<QTableWidgetItem* > selection_list = this->selectedItems();
    //
    Attachment::AttachmentsList v_attachments;
    //
    getSelectedAttaches(v_attachments);
    //
    emit selectionChanged(v_attachments);
};

void MyTable::getSelectedAttaches ( Attachment::AttachmentsList& v_attachments )
{
    QList<QTableWidgetItem* > selection_list = this->selectedItems();
    //
    const unsigned int ui_list_size = selection_list.size();
    //
    for (unsigned int i = 0; i < ui_list_size; ++i)
    {
        QTableWidgetItem* ptr_current_table_cell = selection_list.at(i);
        //
        Attachment* ptr_attach = getLinkedAttachment(ptr_current_table_cell);
        //
        if (ptr_attach)
            v_attachments.push_back(ptr_attach);
     };
}

void MyTable::onProtectAllAttachments ()
{
    if (!m_ptrActualLeaf)
        return;
    //
    initOnDemand();
    m_bFillModeOn = true;
    //
    //m_ptrActualLeaf->protect_all_attachments(false, false);
    const Attachment::AttachmentsList& v_attachments  = m_ptrActualLeaf->getAttachments();
    //
    protect_unprotect_array(true,v_attachments);
    //
    emit selectionChanged(v_attachments);
}

void MyTable::onDropProtectAllAttachments ()
{
    if (!m_ptrActualLeaf)
        return;
    //
    initOnDemand();
    m_bFillModeOn = true;
    //
    const Attachment::AttachmentsList& v_attachments  = m_ptrActualLeaf->getAttachments();
    //
    protect_unprotect_array(false,v_attachments);
    //
    emit selectionChanged(v_attachments);
}

void MyTable::onProtectSelected       ()
{
    initOnDemand();
    m_bFillModeOn = true;
    //
    Attachment::AttachmentsList v_attachments;
    //
    getSelectedAttaches(v_attachments);
    //
    protect_unprotect_array(true,v_attachments);
    //
    refreshCurrentAttachmentsList();
    //
    m_bFillModeOn = false;
    emit selectionChanged(v_attachments);
}

void MyTable::onDropProtectSelected   ()
{
    initOnDemand();
    m_bFillModeOn = true;
    //
    Attachment::AttachmentsList v_attachments;
    //
    getSelectedAttaches(v_attachments);
    //
    protect_unprotect_array(false,v_attachments);
    //
    refreshCurrentAttachmentsList();
    //
    m_bFillModeOn = false;
    emit selectionChanged(v_attachments);
}

void MyTable::onEncryptAllAttachments ()
{
    m_bFillModeOn = true;
    const Attachment::AttachmentsList& v_attachments  = m_ptrActualLeaf->getAttachments();
    //
    //getSelectedAttaches(v_attachments);
    //
    encrypt_decrypt_array(true, v_attachments);
    //
    return;
}

void MyTable::onDecryptAllAttachments ()
{
    m_bFillModeOn = true;
    const Attachment::AttachmentsList& v_attachments  = m_ptrActualLeaf->getAttachments();
    //
    //getSelectedAttaches(v_attachments);
    //
    encrypt_decrypt_array(false, v_attachments);
    //
    return;
}

void MyTable::onEncryptSelected       ()
{
    m_bFillModeOn = true;
    Attachment::AttachmentsList v_attachments;
    //
    getSelectedAttaches(v_attachments);
    //
    encrypt_decrypt_array(true, v_attachments);
    //
    return;
}

void MyTable::onDecryptSelected       ()
{
    m_bFillModeOn = true;
    Attachment::AttachmentsList v_attachments;
    //
    getSelectedAttaches(v_attachments);
    //
    encrypt_decrypt_array(false, v_attachments);
    //
    return;
}

void MyTable::initOnDemand        ()
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
    QObject::connect( m_ptrEncrypter,        SIGNAL(stopThis()),                                this,           SLOT( onCancelAsyncOperations() ) );
    QObject::connect( m_ptrProgress,         SIGNAL(canceled()),                                m_ptrEncrypter, SLOT( OnTerminate()             ) );
    //
    m_ptrProgress->setAutoClose(true);
    m_ptrProgress->setAutoReset(true);
    m_ptrProgress->setMinimumWidth(380);
}

void MyTable::MyTable::onCancelAsyncOperations ()
{
    m_ptrProgress->close();
}

void MyTable::encrypt_decrypt_array(bool b_encrypt, const Attachment::AttachmentsList &list)
{
    if ( b_encrypt )
    {
        DlgAskEncryptMethod dlg;
        //
        if ( dlg.exec() == QDialog::Rejected)
            return;
    };
    //
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
    m_ptrEncrypter->addAttacmentsToProcessList(list);
    //
    m_ptrProgress->setMinimum(0);
    m_ptrProgress->setMaximum(list.size());
    //
    m_ptrEncrypter->start(QThread::IdlePriority);
    m_ptrProgress->exec();
    m_ptrEncrypter->wait();
    //
    refreshCurrentAttachmentsList();
    //
    m_bFillModeOn = false;
    //
    emit selectionChanged(list);
}

void MyTable::protect_unprotect_array(bool b_protect, const Attachment::AttachmentsList &list)
{
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
    m_ptrEncrypter->addAttacmentsToProcessList(list);
    //
    m_ptrProgress->setMinimum(0);
    m_ptrProgress->setMaximum(list.size());
    //
    m_ptrEncrypter->start(QThread::IdlePriority);
    m_ptrProgress->exec();
    m_ptrEncrypter->wait();
    //
    refreshCurrentAttachmentsList();
    //
    m_bFillModeOn = false;
}

void MyTable::onDeleteAttachment ()
{
    m_bFillModeOn = true;
    //
    QString str_del_list;
    //
    Attachment::AttachmentsList v_attachments;
    //
    getSelectedAttaches(v_attachments);
    //
    unsigned int ui_deleted_amount = 0;
    //
    Attachment::AttachmentsList::const_iterator itr = v_attachments.begin();
    //
    for (; itr != v_attachments.end(); ++itr)
    {
        if( (*itr)->delete_it() )
        {
            if ( str_del_list.length() > 0 )
                str_del_list += tr(", ");
            //
            str_del_list += tr("'");
            str_del_list += (*itr)->getName();
            str_del_list += tr("'");
            //
            QString str_message = QString("Attach (id = %1) name '%2'").arg( (*itr)->getID() ).arg( (*itr)->getName() );
            Logger::getInstance().logIt(en_LOG_DELETE_ATTACH, str_message );
            //
            ui_deleted_amount++;
        };
    };
    //
    refreshCurrentAttachmentsList();
    //
    m_bFillModeOn = false;
    //
    if (str_del_list.length() > 0)
    {
        QString str_msg;
        if(ui_deleted_amount > 1)
            str_msg += tr("Attachments: ");
        else
            str_msg += tr("Attachment: ");
        //
        str_msg += str_del_list;
        str_msg +=  tr(" deleted. ");
        //
        m_ptrStatusBar->showMessage(str_msg);
    }
    //
    emit selectionChanged(v_attachments);
}

void MyTable::onRestoreAttachment ()
{
    m_bFillModeOn = true;
    //
    QString str_restore_list;
    //
    Attachment::AttachmentsList v_attachments;
    //
    getSelectedAttaches(v_attachments);
    //
    unsigned int ui_restored_amount = 0;
    //
    Attachment::AttachmentsList::const_iterator itr = v_attachments.begin();
    //
    for (; itr != v_attachments.end(); ++itr)
    {
        if( (*itr)->restoreObject() )
        {
            if ( str_restore_list.length() > 0 )
                str_restore_list += tr(", ");
            //
            str_restore_list += tr("'");
            str_restore_list += (*itr)->getName();
            str_restore_list += tr("'");
            //
            ui_restored_amount++;
        };
    };
    //
    refreshCurrentAttachmentsList();
    //
    m_bFillModeOn = false;
    //
    if (str_restore_list.length() > 0)
    {
        QString str_msg;
        if(ui_restored_amount > 1)
            str_msg += tr("Attachments: ");
        else
            str_msg += tr("Attachment: ");
        //
        str_msg += str_restore_list;
        str_msg +=  tr(" restored. ");
        //
        m_ptrStatusBar->showMessage(str_msg);
    };
    //
    emit selectionChanged(v_attachments);
}

void MyTable::onExportAttachment()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    if( m_strLastExportDir.length() == 0 )
    {
        m_strLastExportDir = settings.value(g_str_ATTACH_EXPORT_PATH).toString();
    }
    //
    QString str_dir_name = QFileDialog::getExistingDirectory(this,
                                                             tr("Choose export directory"),
                                                             m_strLastExportDir,
                                                             QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //
    if (str_dir_name.length() == 0)
        return;
    //
    m_strLastExportDir = str_dir_name;
    //
    Attachment::AttachmentsList v_attachments;
    getSelectedAttaches(v_attachments);
    //
    makeAttachmentsExport(v_attachments);
}

void MyTable::onViewAttachment()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    Attachment::AttachmentsList v_attachments;
    getSelectedAttaches(v_attachments);
    //
    makeViewExport(settings.value(g_str_ATTACH_TMP_PATH).toString(), v_attachments);
}

void MyTable::makeViewExport( const QString&                        str_export_path,
                              const Attachment::AttachmentsList&    v_attachments)
{
    if (v_attachments.size() == 0)
        return; //nothing to do
    //
    Attachment::AttachmentsList::const_iterator itr = v_attachments.begin();
    //
    for (; itr != v_attachments.end(); ++itr)
    {
        QString str_tmp_name;
        bool b_export_for_view = (*itr)->export_for_view(str_export_path, str_tmp_name);
        if (b_export_for_view)
        {
            QString str_message = QString("Attach (id = %1) viewed '%2'").arg( (*itr)->getID() ).arg( (*itr)->getName() );
            Logger::getInstance().logIt(en_LOG_VIEW_ATTACH, str_message );
        }
        //
        //QDesktopServices::openUrl(QUrl("file:///C:/Documents and Settings/All Users/Desktop", QUrl::TolerantMode));
        //
        QString str_url = tr("file:///");
        str_url += str_tmp_name;
        //
        //add every file to the TmpFileCleaner
        //
        TmpFileCleaner::getInstance().deleteFile(str_tmp_name);
        //
        if ( false == QDesktopServices::openUrl( QUrl::fromLocalFile(str_tmp_name) ) )
        {
            QMessageBox box;
            box.setText( tr("Can not open file ") + str_tmp_name );
            box.exec();
        };
    };
}

void MyTable::makeAttachmentsExport (const Attachment::AttachmentsList&   v_attachments)
{
    if (v_attachments.size() == 0)
        return; //nothing to do
    //
    QString str_export_list;
    //
    unsigned int ui_exported_amount = 0;
    //
    Attachment::AttachmentsList::const_iterator itr = v_attachments.begin();
    //
    for (; itr != v_attachments.end(); ++itr)
    {
         if( (*itr)->export_it(m_strLastExportDir) )
        {
            if ( str_export_list.length() > 0 )
                str_export_list += tr(",\n");
            //
            str_export_list += tr("'");
            str_export_list += (*itr)->getName();
            str_export_list += tr("'");
            //
            QString str_message = QString("Attach (id = %1) exported '%2' ").arg( (*itr)->getID() ).arg( (*itr)->getName() );
            Logger::getInstance().logIt(en_LOG_EXPORT_ATTACH, str_message );
            //
            ui_exported_amount++;
        };
    };
    //
    if (
            (str_export_list.length() > 0)
       )
    {
        QString str_msg;
        if(ui_exported_amount > 1)
            str_msg += tr("Attachments: \n");
        else
            str_msg += tr("Attachment: \n");
        //
        str_msg += str_export_list;
        str_msg +=  tr("\nexported to the ");
        str_msg +=  m_strLastExportDir;
        //
        QMessageBox box;
        box.setText(str_msg);
        box.exec();
    };
    //
    return;
}

void MyTable::onReplaceAttachment()
{
    AttachImportOrReplaceDlg dlg(tr("Choose attachment for replace selected"), false);
    //
    if (m_strLastPathOfReplacement.length() == 0)
    {
        QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
        m_strLastPathOfReplacement = settings.value(g_str_ATTACH_IMPORT_PATH).toString();
    };
    //
    bool b_res = dlg.exec(m_strLastPathOfReplacement, "Replace", m_bDeleteAfterAttachReplacement, m_bProtectAttachReplacement, m_bEncryptAttachReplacement);
    //
    if (b_res)
    {
        m_bDeleteAfterAttachReplacement = dlg.getDeleteFilesAfterAttachment();
        m_bProtectAttachReplacement     = dlg.getProtectAttachment();
        m_bEncryptAttachReplacement     = dlg.getEncryptAttachment();
        m_strLastPathOfReplacement      = dlg.getLastAttachmentFromPath();
        unsigned int ui_encrypt_type    = dlg.getTypeOfEncrypt();
        //
        QStringList file_list           = dlg.getFileList(); // only one element can be here (see constructor of AttachImportOrReplaceDlg )
        //
        //get what to replace
        //
        Attachment::AttachmentsList v_attachments;
        getSelectedAttaches(v_attachments);
        //
        bool b_res_replacement = v_attachments[0]->replace_it( file_list.at(0),
                                                               m_bDeleteAfterAttachReplacement,
                                                               m_bProtectAttachReplacement,
                                                               ui_encrypt_type);
        //
        if (b_res_replacement)
        {
            QMessageBox box;
            box.setText(tr("Attachment replaced."));
            box.exec();
        };
    };
}

void MyTable::onCutAttachment ()
{
    Attachment::AttachmentsList v_attachments;
    //
    getSelectedAttaches(v_attachments);
    //
    QString str_msg;
    //
    str_msg=tr("Choose another node and click \"Paste attachment\" or just Ctrl+P");
    //
    m_ptrStatusBar->showMessage(str_msg);
    //
    emit attachmentForCutSelected (v_attachments);
};

void MyTable::mouseDoubleClickEvent (QMouseEvent *event)
{
    bool b_execute_standart_event = processAttachmentList();
    //
    if (b_execute_standart_event)
        QTableWidget::mouseDoubleClickEvent(event);
}

bool MyTable::processAttachmentList ()
{
    Attachment::AttachmentsList v_attachments;
    //
    getSelectedAttaches(v_attachments);
    //
    if (v_attachments.size() == 0)
    {
        return true;
    };
    bool b_rename_enabled = true;
    //
    for (unsigned int i = 0; i < v_attachments.size(); i++)
    {
        if (v_attachments[i]->is_protected() == true)
        {
            b_rename_enabled = false;
            break;
        };
    };
    //
    AttachmentClickAtionDialog dlg(b_rename_enabled);
    //
    dlg.exec();
    //
    if ( dlg.isOpen() )
    {
        onViewAttachment();
        return false;
    }else if ( dlg.isDownload() )
    {
        onExportAttachment();
        return false;
    }
    else if( dlg.isRename() )
    {
        saveLastAttachName();
        return true;
    };
    //user press "cancel" he wants nothing
    return false;
}

void MyTable::focusInEvent (QFocusEvent *event)
{

    QString str_msg;
    //
    str_msg=tr("Up,Down,Left,Right-Navigation; F2-rename; Right button - menu; DoubleClick - alternate actions");
    //
    m_ptrStatusBar->showMessage(str_msg);

    QTableWidget::focusInEvent(event);
}

void MyTable::keyPressEvent ( QKeyEvent *event )
{
    int key_code = event->key();
    //
    switch( key_code )
    {
    case Qt::Key_F2:
        saveLastAttachName();
        break;
    default:
        break;
    };
    //
    QTableWidget::keyPressEvent(event);
}

void MyTable::saveLastAttachName ()
{
    QList<QTableWidgetItem* > selection_list = this->selectedItems();
    //
    if ( selection_list.size() > 0 )
    {
        QTableWidgetItem* ptr_current_table_cell = selection_list.at(0);
        m_strAttachmentNameBeforeChange = ptr_current_table_cell->text();
    };
}

bool MyTable::isStrValidAsFileName    (const QString& str_in)
{
    const QString str_vorbidden = "\\/:?\"<>|";
    for (int i = 0; i < str_vorbidden.length(); i++)
    {
        if ( str_in.contains(str_vorbidden[i]) )
        {
            QMessageBox box;
            box.setText(tr("Characters ") + str_vorbidden + tr(" are not allowed."));
            box.exec();
            return false;
        };
    };
    //
    return true;
}

void MyTable::mouseReleaseEvent (QMouseEvent *event)
{
    Qt::MouseButton btn = event->button();
    // Qt::LeftButton, Qt::RightButton
    if (btn == Qt::RightButton )
    {
        emit showPopupMenu();
    }
    //
    QTableWidget::mouseReleaseEvent(event);
};
