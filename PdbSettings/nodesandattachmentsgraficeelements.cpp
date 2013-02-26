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
#include "nodesandattachmentsgraficeelements.h"
//
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QSettings>
#include <QFileDialog>
#include <QApplication>

NodesAndAttachmentsGraficeElements::NodesAndAttachmentsGraficeElements(QGroupBox*   ptr_parent_frame,
                                                                       QGridLayout* ptr_layout,
                                                                       QObject*     parent) :
    AbstractGraficeElements(ptr_parent_frame, ptr_layout, parent)
{

    m_pLabelAttachMaxSize   = NULL;
    m_pLineAttachSMaxize    = NULL;

    m_pLabelExportPath      = NULL;
    m_pLineExportPath       = NULL;
    m_pExportPathChoose     = NULL;

    m_pLabelImportPath      = NULL;
    m_pLineImportPath       = NULL;
    m_pImportPathChoose     = NULL;
    //
    m_pLabelImportPath      = NULL;
    m_pLineImportPath       = NULL;
    m_pTmpPathChoose        = NULL;
    //
    m_ptrDeleteTmpOnExit                    = NULL;
    m_ptrDeleteAttachmentsOnExit            = NULL;
    m_ptrDeleteAttachmentsAndNodesOnExit    = NULL;
    //
    m_ptrDeleteAfterAttach  = NULL;
    //
    m_strFrameName = tr("Setting of nodes and attachments");
    //
    deleteElementsOnExit(true);
    //
    readData();
    //
    createLayout();
    //
    createLinks();
    //
    updateData(false);
}

NodesAndAttachmentsGraficeElements::~NodesAndAttachmentsGraficeElements()
{

};

void NodesAndAttachmentsGraficeElements::createLinks()
{
    QObject::connect(m_pExportPathChoose, SIGNAL(clicked()),  this, SLOT ( onChooseExportPath() ) );
    QObject::connect(m_pImportPathChoose, SIGNAL (clicked()), this, SLOT ( onChooseImportPath() ) );
    QObject::connect(m_pTmpPathChoose,    SIGNAL(clicked()),  this, SLOT ( onChooseTmpPath   () ) );
    //
    QObject::connect(m_ptrDeleteTmpOnExit, SIGNAL(stateChanged(int)), this, SLOT ( onClickDeleteTmpOnExit(int) ));
    QObject::connect(m_ptrDeleteAttachmentsAndNodesOnExit, SIGNAL(stateChanged(int)), this, SLOT ( onClickDeleteAll(int) ));
    QObject::connect(m_ptrDeleteAfterAttach, SIGNAL(stateChanged(int)), this, SLOT ( onClickDeleteAfterAttach(int) ));
};

void NodesAndAttachmentsGraficeElements::onChooseTmpPath ()
{
    QString str_dir_name = QFileDialog::getExistingDirectory(NULL,
                                                             tr("Choose temporary directory"),
                                                             m_strTmpPath,
                                                             QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks
                                                             );



    if (str_dir_name.length() > 0 )
        m_strTmpPath = str_dir_name;
    //
    updateData(false);
    return;
}

void NodesAndAttachmentsGraficeElements::onChooseExportPath()
{
    QString str_dir_name = QFileDialog::getExistingDirectory(NULL,
                                                             tr("Choose default export directory"),
                                                             m_strDefaultExportPath,
                                                             QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks
                                                             );



    if (str_dir_name.length() > 0 )
        m_strDefaultExportPath = str_dir_name;
    //
    updateData(false);
    return;
};

void NodesAndAttachmentsGraficeElements::onChooseImportPath()
{
    QString str_dir_name = QFileDialog::getExistingDirectory(NULL,
                                                             tr("Choose default Import directory"),
                                                             m_strDefaultImportPath,
                                                             QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks
                                                             );



    if (str_dir_name.length() > 0 )
        m_strDefaultImportPath = str_dir_name;
    //
    updateData(false);
    return;
};

void NodesAndAttachmentsGraficeElements::onClickDeleteAll (int i_state)
{
    if (Qt::Checked == i_state)
    {
        m_bDelAttachAndNodesOnExit   = true;
        m_bDelAttachOnExit           = true;
        m_ptrDeleteAttachmentsOnExit->setEnabled(false);
    }else
    {
        m_bDelAttachAndNodesOnExit  = false;
        m_ptrDeleteAttachmentsOnExit->setEnabled(true);
    };
    //
    updateData(false);
};

void NodesAndAttachmentsGraficeElements::onClickDeleteAfterAttach (int i_state)
{
    if (Qt::Checked == i_state)
    {
        m_bDeleteAfterAttach = true;
    }else
    {
        m_bDeleteAfterAttach   = false;
    };
    //
    updateData(true);
};

void NodesAndAttachmentsGraficeElements::onClickDeleteTmpOnExit (int i_state)
{
    if (Qt::Checked == i_state)
    {
        m_bDelTmpOnExit = true;
    }else
    {
        m_bDelTmpOnExit   = false;
    };
    //
    updateData(false);
};

QString NodesAndAttachmentsGraficeElements::getActualAppPath() const
{
    QString str_path =  QApplication::applicationDirPath();
    return str_path;
}

void NodesAndAttachmentsGraficeElements::normalizePath   (QString& str_path) const
{
    if (
            ( str_path[str_path.length() - 1] == '\\' ) ||
            ( str_path[str_path.length() - 1] == '/' )
        )
    {
        str_path.remove((str_path.length() - 1), 1);
    };
}

void NodesAndAttachmentsGraficeElements::writeData()
{
    updateData(true);
    //
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    if (0 == m_uiMaxAttachmentSizeMB)
        m_uiMaxAttachmentSizeMB = 1;
    //
    settings.setValue( g_str_ATTACH_MAX_SIZE_MB, m_uiMaxAttachmentSizeMB );
    //if any path is empty - write default path to the app
    //
    if (m_strDefaultExportPath.length() == 0)
        m_strDefaultExportPath = getActualAppPath();
    //
    if (m_strDefaultImportPath.length() == 0)
        m_strDefaultImportPath = getActualAppPath();
    //
    if (m_strTmpPath.length() == 0)
        m_strTmpPath = getActualAppPath();
    //
    normalizePath(m_strDefaultExportPath);
    normalizePath(m_strDefaultImportPath);
    normalizePath(m_strTmpPath);
    //
    settings.setValue( g_str_ATTACH_EXPORT_PATH, m_strDefaultExportPath  );
    settings.setValue( g_str_ATTACH_IMPORT_PATH, m_strDefaultImportPath  );
    settings.setValue( g_str_ATTACH_TMP_PATH,    m_strTmpPath            );
    //
    settings.setValue( g_str_TMP_DELETE,         m_bDelTmpOnExit         );
    settings.setValue( g_str_ATTACH_DELETE,      m_bDelAttachOnExit );
    settings.setValue( g_str_ATTACH_NODE_DELETE, m_bDelAttachAndNodesOnExit );
    settings.setValue( g_str_DELETE_AFTER_UPLOAD, m_bDeleteAfterAttach );
    //
};

void NodesAndAttachmentsGraficeElements::readData()
{
    QSettings settings( g_strCOMPANY, g_str_CNF_APP_NAME );
    //
    m_uiMaxAttachmentSizeMB     = settings.value(g_str_ATTACH_MAX_SIZE_MB).toInt();
    //
    if (0 == m_uiMaxAttachmentSizeMB)
        m_uiMaxAttachmentSizeMB = 1;
    //
    m_strDefaultExportPath      = settings.value(g_str_ATTACH_EXPORT_PATH).toString();
    m_strDefaultImportPath      = settings.value(g_str_ATTACH_IMPORT_PATH).toString();
    m_strTmpPath                = settings.value(g_str_ATTACH_TMP_PATH).toString();
    //
    m_bDelTmpOnExit             = settings.value( g_str_TMP_DELETE ).toBool();
    m_bDelAttachOnExit          = settings.value( g_str_ATTACH_DELETE ).toBool();
    m_bDelAttachAndNodesOnExit  = settings.value( g_str_ATTACH_NODE_DELETE ).toBool();
    //
    m_bDeleteAfterAttach        = settings.value( g_str_DELETE_AFTER_UPLOAD ).toBool();
};

void NodesAndAttachmentsGraficeElements::updateData (bool b_from_dialog)
{
    if(b_from_dialog)
    {
        m_uiMaxAttachmentSizeMB = m_pLineAttachSMaxize->text().toInt();
        //
        m_strDefaultExportPath  = m_pLineExportPath->text();
        m_strDefaultImportPath  = m_pLineImportPath->text();
        m_strTmpPath            = m_pLineTmpPath->text();
        //
        m_bDelTmpOnExit             = m_ptrDeleteTmpOnExit->checkState();
        m_bDelAttachOnExit          = m_ptrDeleteAttachmentsOnExit->checkState();
        m_bDelAttachAndNodesOnExit  = m_ptrDeleteAttachmentsAndNodesOnExit->checkState();
        //
        m_bDeleteAfterAttach        = m_ptrDeleteAfterAttach->checkState();
    }else
    {
        m_pLineAttachSMaxize->setText( QString::number( m_uiMaxAttachmentSizeMB ) );
        //
        m_pLineExportPath->setText(m_strDefaultExportPath);
        m_pLineImportPath->setText(m_strDefaultImportPath);
        m_pLineTmpPath->setText(m_strTmpPath);
        //
        if (m_bDelTmpOnExit)
            m_ptrDeleteTmpOnExit->setCheckState(Qt::Checked);
        else
            m_ptrDeleteTmpOnExit->setCheckState(Qt::Unchecked);
        //
        if (m_bDelAttachOnExit)
            m_ptrDeleteAttachmentsOnExit->setCheckState(Qt::Checked);
        else
            m_ptrDeleteAttachmentsOnExit->setCheckState(Qt::Unchecked);
        //
        if(m_bDelAttachAndNodesOnExit)
            m_ptrDeleteAttachmentsAndNodesOnExit->setCheckState(Qt::Checked);
        else
            m_ptrDeleteAttachmentsAndNodesOnExit->setCheckState(Qt::Unchecked);
        //
        if(m_bDeleteAfterAttach)
            m_ptrDeleteAfterAttach->setCheckState(Qt::Checked);
        else
            m_ptrDeleteAfterAttach->setCheckState(Qt::Unchecked);
    };
};

void NodesAndAttachmentsGraficeElements::createLayout()
{
    //const int i_edit_width  = 155;
    //const int i_edit_height = 20;
    //-------------------------------
    int i_row               = 0;

    m_pLabelAttachMaxSize = new QLabel ( "Max. size of attacment, Mb:" );
    m_pLabelAttachMaxSize->setText("Max. size of attacment, Mb:");
    //m_pLabelAttachMaxSize->setFixedSize(i_edit_width,i_edit_height);
    addAndRegisterElement(m_pLabelAttachMaxSize,i_row,0,1,1);
    //
    m_pLineAttachSMaxize = new QLineEdit( m_pParentFrame );
    m_pLineAttachSMaxize->setAlignment(Qt::AlignRight);
    m_pLineAttachSMaxize->setToolTip("Be sure that client and server sides supports this size of attachment");
    //m_pLineAttachSMaxize->toolTip();
    addAndRegisterElement(m_pLineAttachSMaxize, i_row,2,1,1);
    //
    //-----------------
    i_row++;
    m_pLabelExportPath = new QLabel ("Default path for attachment export:");
    addAndRegisterElement(m_pLabelExportPath, i_row,0,1,1);
    //
    m_pLineExportPath = new QLineEdit( m_pParentFrame );
    addAndRegisterElement(m_pLineExportPath, i_row,1,1,2);
    //
    m_pExportPathChoose = new QPushButton("Change...",  m_pParentFrame);
    addAndRegisterElement(m_pExportPathChoose, i_row,3,1,1);
    //-----------------
    i_row++;
    m_pLabelImportPath = new QLabel ("Default path for attachment import:");
    addAndRegisterElement(m_pLabelImportPath, i_row,0,1,1);
    //
    m_pLineImportPath = new QLineEdit( m_pParentFrame );
    addAndRegisterElement(m_pLineImportPath, i_row,1,1,2);
    //
    m_pImportPathChoose = new QPushButton("Change...",  m_pParentFrame);
    addAndRegisterElement(m_pImportPathChoose, i_row,3,1,1);
    //-----------------
    i_row++;
    m_pLabelTmpPath = new QLabel ("Path for temporary operations:");
    addAndRegisterElement(m_pLabelTmpPath, i_row,0,1,1);
    //
    m_pLineTmpPath = new QLineEdit( m_pParentFrame );
    addAndRegisterElement(m_pLineTmpPath, i_row,1,1,2);
    //
    m_pTmpPathChoose = new QPushButton("Change...",  m_pParentFrame);
    addAndRegisterElement(m_pTmpPathChoose, i_row,3,1,1);
    //-----------------
    i_row++;
    m_ptrDeleteTmpOnExit = new QCheckBox(tr("delete temporary files on exit"));
    addAndRegisterElement(m_ptrDeleteTmpOnExit, i_row,0,1,4);
    //-----------------
    i_row++;
    m_ptrDeleteAttachmentsOnExit = new QCheckBox(tr("remove attachment marked as 'deleted' from database on exit"));
    addAndRegisterElement(m_ptrDeleteAttachmentsOnExit, i_row,0,1,4);
    //-----------------
    i_row++;
    m_ptrDeleteAttachmentsAndNodesOnExit = new QCheckBox(tr("remove attachment and nodes marked as 'deleted' from database on exit"));
    addAndRegisterElement(m_ptrDeleteAttachmentsAndNodesOnExit, i_row,0,1,4);
    //-----------------
    i_row++;
    m_ptrDeleteAfterAttach = new QCheckBox(tr("Always select \"Delete file(s)\" in upload and replace dialogs"));
    addAndRegisterElement(m_ptrDeleteAfterAttach, i_row,0,1,4);
}
