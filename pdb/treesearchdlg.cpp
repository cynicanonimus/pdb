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
#include "treesearchdlg.h"
#include "ui_treesearchdlg.h"
#include "mytree.h"
#include "GDefinitions.h"
#include "../CommonInclude/pdb/VariantPtr.h"
#include "rootoftree.h"
#include "dbacccesssafe.h"
#include "logger.h"
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QKeyEvent>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QtSql>
//
//
//
TreeSearchDlg::TreeSearchDlg(QWidget *parent) :
    QDialog(parent)
    ,ui(new Ui::TreeSearchDlg)
    ,m_uiTreeColNum(0)
    ,m_uiNodeColNum (1)
    ,m_uiLocationColNum(2)
{
    ui->setupUi(this);
    //
    setSignalSocketLinks();
    //
    setFormLayout();
    //
    this->ui->m_checkSearchInAttach->setToolTip(tr("Search is possible only in non-binary files and can take long time"));
    //
    this->ui->m_SearchResults->setColumnCount(3);
    QStringList str_att_header;
    str_att_header<<"Tree" << "Node" << "Location";
    this->ui->m_SearchResults->setHorizontalHeaderLabels(str_att_header);
    //
    this->ui->m_SearchResults->setColumnWidth(m_uiTreeColNum,100);
    this->ui->m_SearchResults->setColumnWidth(m_uiNodeColNum,245);
    this->ui->m_SearchResults->setColumnWidth(m_uiLocationColNum,70);
    //
    QHeaderView *header = this->ui->m_SearchResults->horizontalHeader();
    header->setDefaultAlignment(Qt::AlignCenter);
    //
    header->setResizeMode(m_uiTreeColNum,       QHeaderView::Fixed);
    header->setResizeMode(m_uiNodeColNum,       QHeaderView::Stretch);
    header->setResizeMode(m_uiLocationColNum,   QHeaderView::Fixed);
    //disable search button
    this->ui->m_bSearch->setEnabled(false);
    //
    m_enMode = enSEARCH_MODE;
    //
    this->ui->m_progressBar->setVisible(false);
    //
    this->ui->m_lblPatternSyntax->setVisible(false);
    this->ui->m_cmbPatternSyntax->setVisible(false);
    //
    fillComboPatternSyntax();
    //
    onFromSearchClick();
    onToSearchClick();
    //
}

TreeSearchDlg::~TreeSearchDlg()
{
    m_Searcher.terminate();
    m_Searcher.wait();
    //
    while( !m_Searcher.isFinished() )
    {
        QTime dieTime= QTime::currentTime().addSecs(1);
        while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    };
    //
    delete ui;
}

bool TreeSearchDlg::getEarliestDateFromDB ()
{
    DBAcccessSafe   db_safe;
    //
    QSqlDatabase* ptr_db = db_safe.getDB();
    Q_ASSERT(ptr_db);
    if (NULL == ptr_db)
        return false;
    //
    QDateTime   attach_change;
    //
    QSqlQuery qry(*ptr_db);
    //
    // select from both tables because it is modification, but not creation date.
    //
    QString str_select_str = QString ("select min(node_tbl.last_change) as node_change, min(attachments.last_change) as attach_change from node_tbl, attachments;");
    //
    if (!qry.prepare( str_select_str ))
    {
        const QString str_err = qry.lastError().text();
        Logger::getInstance().logIt( en_LOG_ERRORS, str_err );
        return false;

    } else if( !qry.exec() )
    {
        const QString str_err = qry.lastError().text();
        Logger::getInstance().logIt( en_LOG_ERRORS, str_err );
        return false;
    }else
    {
        while( qry.next() )
        {
            m_dtEarliestDateTime = qry.value(0).toDateTime();
            attach_change = qry.value(1).toDateTime();
            break;
        }; // while( qry.next() )
    }; //if (!qry.prepare( str_select_str ))
    //
    if ( attach_change < m_dtEarliestDateTime )
        m_dtEarliestDateTime = attach_change;
    //
    return true;
}

void TreeSearchDlg::fillComboPatternSyntax()
{
    this->ui->m_cmbPatternSyntax->addItem(tr("Wildcard"), QRegExp::Wildcard);
    this->ui->m_cmbPatternSyntax->addItem(tr("Regular expression v1"), QRegExp::RegExp);
    this->ui->m_cmbPatternSyntax->addItem(tr("Regular expression v2"), QRegExp::RegExp2);
    //this->ui->m_cmbPatternSyntax->addItem(tr("Fixed string"), QRegExp::FixedString);
    this->ui->m_cmbPatternSyntax->addItem(tr("W3C Xml Schema 1.1"), QRegExp::W3CXmlSchema11);
}

void TreeSearchDlg::setFormLayout()
{
    m_ptrFormLayout = new QGridLayout(this);
    int i_row = 0;
    //->addWidget(ptr_widget, row,column,rowSpan,columnSpan, aligment);
    m_ptrFormLayout->addWidget(this->ui->SearchLabel, i_row,0,1,1, Qt::AlignLeft);
    m_ptrFormLayout->addWidget(this->ui->m_eSearchString, i_row, 1, 1, 5);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_lblPatternSyntax, i_row,0,1,1, Qt::AlignLeft);
    m_ptrFormLayout->addWidget(this->ui->m_cmbPatternSyntax, i_row, 1, 1, 5);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->LblSearchOptions, i_row,0,1,6);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_checkCaseSensitive, i_row,0,1,3);
    m_ptrFormLayout->addWidget(this->ui->m_checkSearchInNode, i_row,3,1,3);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_checkWholeWords, i_row,0,1,3);
    m_ptrFormLayout->addWidget(this->ui->m_checkSearchInDescriptor, i_row,3,1,3);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_checkRegExp, i_row,0,1,3);
    m_ptrFormLayout->addWidget(this->ui->m_checkSearchInANames, i_row,3,1,3);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_checkCurrentTree, i_row,0,1,3);
    m_ptrFormLayout->addWidget(this->ui->m_checkSearchInAttach, i_row,3,1,3);
    //
    // earlieast search day is the 1-st day from db (attachment or node)
    // latest day is always today
    getEarliestDateFromDB ();
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_checkBoxFrom, i_row,0,1,1);
    m_ptrFormLayout->addWidget(this->ui->m_dateTimeEdit_from, i_row,1,1,2);
    this->ui->m_dateTimeEdit_from->setToolTip(tr("earliest creation or modification date"));
    this->ui->m_dateTimeEdit_from->setDateTimeRange(m_dtEarliestDateTime, QDateTime::currentDateTime());
    this->ui->m_dateTimeEdit_from->setCalendarPopup(true);
    //..
    m_ptrFormLayout->addWidget(this->ui->m_checkBoxTo, i_row,3,1,1);
    m_ptrFormLayout->addWidget(this->ui->m_dateTimeEdit_to, i_row,4,1,2);
    this->ui->m_dateTimeEdit_to->setToolTip(tr("latest creation or modification date"));
    this->ui->m_dateTimeEdit_to->setDateTimeRange(m_dtEarliestDateTime, QDateTime::currentDateTime());
    this->ui->m_dateTimeEdit_to->setDateTime(QDateTime::currentDateTime());
    this->ui->m_dateTimeEdit_to->setCalendarPopup(true);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->LblSearchResults, i_row,0,1,6);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_SearchResults, i_row,0,1,6);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->LblFake2, i_row,0,1,6);
    this->ui->LblFake2->setVisible(false);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_progressBar, i_row,0,1,6);
    //
    //this is fake element, just for space
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->LblFake, i_row,0,1,6);
    this->ui->LblFake->setVisible(false);
    //
    i_row++;
    m_ptrFormLayout->addWidget(this->ui->m_bSearch, i_row,0,1,2);
    m_ptrFormLayout->addWidget(this->ui->m_bClearResult, i_row,2,1,2);
    m_ptrFormLayout->addWidget(this->ui->m_bHide, i_row,4,1,2);
    //
    this->setLayout(m_ptrFormLayout);
}

void TreeSearchDlg::setSignalSocketLinks()
{
    //
    //process signals from buttons
    //
    QObject::connect(this->ui->m_bHide,         SIGNAL(clicked()),              this, SLOT( onHideButton  () ));
    QObject::connect(this->ui->m_bClearResult,  SIGNAL(clicked()),              this, SLOT( onClearButton () ));
    QObject::connect(this->ui->m_bSearch,       SIGNAL(clicked()),              this, SLOT( onSearchButton() ));
    //when search string changed
    QObject::connect(this->ui->m_eSearchString, SIGNAL(textChanged(QString)),   this, SLOT (onChangeSearchString(QString) ));
    //
    QObject::connect( this->ui->m_checkRegExp,             SIGNAL(stateChanged(int)),              this, SLOT( onChangeRegExpState (int)  ));
    //
    //when search conditions are changed (in combination with search line status enable or disable search button)
    QObject::connect(this->ui->m_checkSearchInNode,         SIGNAL(stateChanged(int)),  this, SLOT (onClickSearchArray() ));
    QObject::connect(this->ui->m_checkSearchInDescriptor,   SIGNAL(stateChanged(int)),  this, SLOT (onClickSearchArray() ));
    QObject::connect(this->ui->m_checkSearchInANames,       SIGNAL(stateChanged(int)),  this, SLOT (onClickSearchArray() ));
    QObject::connect(this->ui->m_checkSearchInAttach,       SIGNAL(stateChanged(int)),  this, SLOT (onClickSearchArray() ));
    //
    //process signal from/to search thread
    //
    QObject::connect(&m_Searcher, SIGNAL(beginOfSearch()),      this, SLOT  (onBeginSearch() ));
    QObject::connect(&m_Searcher, SIGNAL(getTotalNodesAmount(uint)), this, SLOT (onGetTotalNodesAmount(unsigned int) ));
    QObject::connect(&m_Searcher, SIGNAL(endOfSearch()),             this, SLOT (onEndSearch          ()    ));
    QObject::connect(&m_Searcher, SIGNAL(jumpToNextNode()),          this, SLOT (onJumpNextNode       ()    ));
    QObject::connect(&m_Searcher, SIGNAL(foundNode(TreeLeaf*,int)),  this, SLOT (onfoundNode(TreeLeaf*,int) ));
    //
    //process click to the selected item
    QObject::connect(this->ui->m_SearchResults, SIGNAL ( itemSelectionChanged () ), this, SLOT( onSearchTableClick() ) );
    //
    QObject::connect( this->ui->m_checkBoxFrom, SIGNAL(clicked()), this, SLOT(onFromSearchClick()) );
    QObject::connect( this->ui->m_checkBoxTo, SIGNAL(clicked()),    this, SLOT(onToSearchClick())  );
}

void TreeSearchDlg::keyPressEvent (QKeyEvent * event)
{
    int key_code = event->key();
    //
    switch( key_code )
    {
    case Qt::Key_Escape:
        onHideButton();
        return;
    default:
        break;
    };
    //
    QDialog::keyPressEvent(event);
};

void TreeSearchDlg::setPtrToComboBox (QComboBox*     ptr_combo)
{
    m_ptrCombo = ptr_combo;
};

void TreeSearchDlg::setPtrToTree (MyTree*        ptr_to_tree)
{
    m_ptrTree = ptr_to_tree;
}
//-----------------------------------------------------------------------------------------
void    TreeSearchDlg::onFromSearchClick()
{
    if (this->ui->m_checkBoxFrom->checkState() == Qt::Checked)
    {
        this->ui->m_dateTimeEdit_from->setEnabled(true);
    }
    else
    {
        this->ui->m_dateTimeEdit_from->setEnabled(false);
    };
}

void    TreeSearchDlg::onToSearchClick()
{
    if (this->ui->m_checkBoxTo->checkState() == Qt::Checked)
    {
        this->ui->m_dateTimeEdit_to->setEnabled(true);
    }
    else
    {
        this->ui->m_dateTimeEdit_to->setEnabled(false);
    };
}

void TreeSearchDlg::onChangeRegExpState (int value)
{
    switch (value)
    {
        case Qt::Checked:
            this->ui->SearchLabel->setText(tr("Search pattern"));
            //
            this->ui->m_lblPatternSyntax->setVisible(true);
            this->ui->m_cmbPatternSyntax->setVisible(true);
            //
            this->ui->m_checkCaseSensitive->setChecked(false);
            this->ui->m_checkCaseSensitive->setText(tr("Enable minimal match"));
            //
            this->ui->m_checkWholeWords->setChecked(false);
            this->ui->m_checkWholeWords->setEnabled(false);
        break;
        case Qt::Unchecked:
            this->ui->SearchLabel->setText(tr("Search"));
            //
            this->ui->m_lblPatternSyntax->setVisible(false);
            this->ui->m_cmbPatternSyntax->setVisible(false);
            //
            this->ui->m_checkCaseSensitive->setText(tr("Case sensitive"));
            this->ui->m_checkCaseSensitive->setChecked(false);
            //
            this->ui->m_checkWholeWords->setEnabled(true);
        break;
    };
}

//-----------------------------------------------------------------------------------------
void TreeSearchDlg::onClickSearchArray()
{
    QString str_search = this->ui->m_eSearchString->text();
    enableSearchButton(str_search);
}

void TreeSearchDlg::createSearchRequest (SearchRequest& request)
{
    request.setSearchPhrase(this->ui->m_eSearchString->text());
    //------------------------------------------------------------------
    //define timing
    if (this->ui->m_checkBoxFrom->checkState() == Qt::Checked)
    {

        request.setBeginSearchDateTime(this->ui->m_dateTimeEdit_from->dateTime());
    }else
    {
        request.dropBeginSearchDateTime();
    };
    //
    if (this->ui->m_checkBoxTo->checkState() == Qt::Checked)
    {
         request.setEndSearchDateTime(this->ui->m_dateTimeEdit_to->dateTime());
    }
    else
    {
        request.dropEndSearchDateTime();
    };
    //
    //define regexp as search condition
    //
    if (this->ui->m_checkRegExp->checkState() == Qt::Checked)
    {
        //
        if (this->ui->m_checkCaseSensitive->checkState() == Qt::Checked)
            request.setEnableMinMatching();
        else
            request.setEnableMinMatching(false);
        //

        QVariant qv_pattern = this->ui->m_cmbPatternSyntax->itemData ( this->ui->m_cmbPatternSyntax->currentIndex() );
        QRegExp::PatternSyntax reg_pattern = (QRegExp::PatternSyntax)qv_pattern.toInt();
        request.setRegExpSyntax(reg_pattern );
    }else
    {
        request.dropRegularExp();
        //
        if (this->ui->m_checkCaseSensitive->checkState() == Qt::Checked)
            request.setCaseSensitive();
        else
            request.setCaseSensitive(false);
    };
    //
    if (this->ui->m_checkWholeWords->checkState() == Qt::Checked)
        request.setWholeWord();
    //
    if (this->ui->m_checkCurrentTree->checkState() == Qt::Checked)
        request.setCurrentTreeOnly();
    //------------------------------------------------------------------
    if (this->ui->m_checkSearchInNode->checkState() == Qt::Checked)
        request.setSearchInNodeNames();
    //
    if (this->ui->m_checkSearchInDescriptor->checkState() == Qt::Checked)
        request.setSearchInNodeDescriptor();
    //
    if (this->ui->m_checkSearchInANames->checkState() == Qt::Checked)
        request.setSearchInAttachmentNames();
    //
    if (this->ui->m_checkSearchInAttach->checkState() == Qt::Checked)
        request.setSearchInAtttachmentBody();
    //------------------------------------------------------------------

}

bool TreeSearchDlg::isSearchArrayDefined()
{
    if (
            (this->ui->m_checkSearchInNode->checkState() == Qt::Checked)        ||
            (this->ui->m_checkSearchInDescriptor->checkState() == Qt::Checked)  ||
            (this->ui->m_checkSearchInANames->checkState() == Qt::Checked)      ||
            (this->ui->m_checkSearchInAttach->checkState() == Qt::Checked)
        )
    {
        return true;
    };
    //
    return false;
}

void TreeSearchDlg::enableSearchButton  (const QString& str_search_string)
{
    if ( (str_search_string.length() != 0) && isSearchArrayDefined () )
    {
        this->ui->m_bSearch->setEnabled(true);
    }
    else
    {
        this->ui->m_bSearch->setEnabled(false);
    };
};
//
// click "search" button here
//
void TreeSearchDlg::onSearchButton()
{
    if ( enSEARCH_MODE == m_enMode )
    {
        startSearch ();
    }else
    {
        cancelSearch();
    };
    //lock all comboboxes here
};

void TreeSearchDlg::startSearch  ()
{
    /*
    int i_search_conditions = getSearchConditions ();
    int i_search_area       = getSearchArray();
    QString str_search      = this->ui->m_eSearchString->text();
    */
    //
    SearchRequest request;
    createSearchRequest(request);
    //
    if (request.isRegExpValid() == false)
    {
        QMessageBox box;
        box.setText(tr("Regular expression: ") + request.getSearchPhrase() + tr(" contains errors. Unable to search"));
        box.exec();
        return;
    };
    //
    m_Searcher.setPtrToCombo(m_ptrCombo);
    m_Searcher.setPtrToTree(m_ptrTree);
    //
    m_Searcher.setSearchConditions(request);
    //
    this->ui->m_bSearch->setEnabled(false);
    //
    clearSearchResults();
    //
    m_Searcher.start(QThread::IdlePriority);
    //
    return;
};

void TreeSearchDlg::cancelSearch ()
{
    this->ui->m_bSearch->setEnabled(false);
    m_Searcher.cancelSearch();
};

//
//need to switch it on the signal from thread!!!
//

void TreeSearchDlg::swapDlgMode ()
{
    if (enSEARCH_MODE == m_enMode)
    {
        m_enMode = enCANCEL_MODE;
        this->ui->m_bSearch->setText(tr("Stop search"));
    }else
    {
        m_enMode = enSEARCH_MODE;
        this->ui->m_bSearch->setText(tr("Begin search"));
    };
};


void TreeSearchDlg::onChangeSearchString(QString str_search)
{
    enableSearchButton(str_search);
};

void TreeSearchDlg::onHideButton()
{
    hide();
}

void TreeSearchDlg::onClearButton()
{
    this->ui->m_eSearchString->setText(tr(""));

    while (this->ui->m_SearchResults->rowCount() > 0)
    {
        this->ui->m_SearchResults->removeRow(0);
    };
 }
//---------------------- thread feedback

void TreeSearchDlg::onBeginSearch ()
{
    this->ui->m_bSearch->setEnabled(true);
    lockInterface(true);
    swapDlgMode();
}

void TreeSearchDlg::onJumpNextNode()
{
    this->ui->m_progressBar->setValue( this->ui->m_progressBar->value() + 1);
}

void TreeSearchDlg::onGetTotalNodesAmount(unsigned int ui_amount)
{
    //set progressbar params
    this->ui->m_progressBar->setVisible(true);
    //
    this->ui->m_progressBar->setMinimum(0);
    this->ui->m_progressBar->setMaximum(ui_amount);
    this->ui->m_progressBar->setValue(0);
    this->ui->m_progressBar->setFormat("%v");
};

void TreeSearchDlg::onEndSearch()
{
    this->ui->m_bSearch->setEnabled(true);
    lockInterface(false);
    swapDlgMode();
    this->ui->m_progressBar->setVisible(false);
};

void TreeSearchDlg::lockInterface (bool b_lock )
{
    this->ui->m_eSearchString->setEnabled(!b_lock);
    //
    this->ui->m_checkSearchInNode->setEnabled(!b_lock);
    this->ui->m_checkSearchInDescriptor->setEnabled(!b_lock);
    this->ui->m_checkSearchInANames->setEnabled(!b_lock);
    this->ui->m_checkSearchInAttach->setEnabled(!b_lock);
    //
    this->ui->m_checkCaseSensitive->setEnabled(!b_lock);
    //
    this->ui->m_checkRegExp->setEnabled(!b_lock);
    if (this->ui->m_checkRegExp->checkState() != Qt::Checked)
    {
        this->ui->m_checkWholeWords->setEnabled(!b_lock);
    };
    //
    this->ui->m_checkCurrentTree->setEnabled(!b_lock);
    //
    this->ui->m_bClearResult->setEnabled(!b_lock);
    this->ui->m_bHide->setEnabled(!b_lock);
    //
    this->ui->m_progressBar->setVisible(b_lock);

};

void TreeSearchDlg::onfoundNode (TreeLeaf* ptr_node,int i_location)
{
    //analyse the search result table
    TreeLeaf* ptr_tbl_node = NULL;
    int i_target_row = -1;
    //
    for  (int i = 0; i < this->ui->m_SearchResults->rowCount(); ++i)
    {
        QTableWidgetItem* ptr_cell = this->ui->m_SearchResults->item(i, m_uiNodeColNum);
        //
        ptr_tbl_node = getLinkedLeaf(ptr_cell);
        //
        Q_ASSERT (ptr_tbl_node);
        //
        if ( ptr_tbl_node->getID() == ptr_node->getID() )
        {
            i_target_row = i;
            break;
        };
    };
    //
    QTableWidgetItem* ptr_location_cell = NULL;
    //
    if (-1 == i_target_row)
    {
        //create the table entries
        //get the tree name
        QString str_tree_name = getTreeNameByTreeID( ptr_node->getTreeID() );
        //
        QString str_node_name = ptr_node->text(0);
        //create tree cell
        QTableWidgetItem*   ptr_tree_cell   = new  QTableWidgetItem( str_tree_name );
        ptr_tree_cell->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        //create node_ cell
        QTableWidgetItem*   ptr_node_cell   = new  QTableWidgetItem( str_node_name );
        ptr_node_cell->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
        //
        QVariant v_node = VariantPtr<TreeLeaf>::asQVariant( ptr_node );
        ptr_node_cell->setData(1,v_node);
        //create location cell
        ptr_location_cell = new  QTableWidgetItem(" ");
        ptr_location_cell->setFlags( Qt::ItemIsEnabled | Qt::ItemIsSelectable );
        //ptr_location_cell->setFlags( Qt::ItemIsEnabled |Qt::ItemIsSelectable );
        //this->ui->m_SearchResults->rowCount()
        this->ui->m_SearchResults->setRowCount(this->ui->m_SearchResults->rowCount() + 1);
        //
        this->ui->m_SearchResults->setItem(this->ui->m_SearchResults->rowCount()-1, m_uiTreeColNum, ptr_tree_cell);
        this->ui->m_SearchResults->setItem(this->ui->m_SearchResults->rowCount()-1, m_uiNodeColNum, ptr_node_cell);
        this->ui->m_SearchResults->setItem(this->ui->m_SearchResults->rowCount()-1, m_uiLocationColNum, ptr_location_cell);
    }else
    {
        ptr_location_cell = this->ui->m_SearchResults->item(i_target_row, m_uiLocationColNum);
    };
    //Add new index to the location cell
    QString str_location_text = ptr_location_cell->text();
    //
    addIndexToLocation(str_location_text, i_location);
    ptr_location_cell->setText(str_location_text);
    ptr_location_cell->setToolTip(tr("N - node name, D - node descriptor, A - attachment name, B - body of attachment"));
    //
    return;
};

void TreeSearchDlg::addIndexToLocation  (QString& str_location, int i_location)
{
    if (enNODE_NAMES == i_location)
    {
        if (str_location.contains(tr("N")) == false)
            str_location += tr("N");
    }else if(enNODE_DESCRIPTOR == i_location)
    {
        if (str_location.contains(tr("D")) == false)
            str_location += tr("D");
    }else if(enATT_NAMES == i_location)
    {
        if (str_location.contains(tr("A")) == false)
            str_location += tr("A");
    }else if(enATT_BODY == i_location)
    {
        if (str_location.contains(tr("B")) == false)
            str_location += tr("B");
    };
};

QString TreeSearchDlg::getTreeNameByTreeID(int i_tree_id)
{
    for (int i = 0; i < m_ptrCombo->count(); i++ )
    {
        QVariant root_data = m_ptrCombo->itemData(i);
        RootOfTree* ptr_root = VariantPtr<RootOfTree>::asPtr( root_data );
        Q_ASSERT (ptr_root);
        //
        if (ptr_root->getID() == i_tree_id)
            return ptr_root->getName();
    };

    return tr("");

};

TreeLeaf*  TreeSearchDlg::getLinkedLeaf (QTableWidgetItem* ptr_cell)
{
    QVariant ptr_linked_leaf = ptr_cell->data(1);
    TreeLeaf* ptr_node = VariantPtr<TreeLeaf>::asPtr( ptr_linked_leaf );
    Q_ASSERT ( ptr_node );
    //
    return ptr_node;
};

void TreeSearchDlg::clearSearchResults()
{
    while (this->ui->m_SearchResults->rowCount() > 0)
    {
        this->ui->m_SearchResults->removeRow(0);
    };
};

void TreeSearchDlg::onSearchTableClick()
{
    QList<QTableWidgetItem* > selection_list = this->ui->m_SearchResults->selectedItems();
    //
    if (selection_list.size() == 0)
        return;
    //
    QTableWidgetItem* ptr_current_table_cell = selection_list.at(0);
    //
    int i_row = ptr_current_table_cell->row();
    //
    ptr_current_table_cell = this->ui->m_SearchResults->item(i_row, m_uiNodeColNum);
    //
    TreeLeaf* ptr_leaf = getLinkedLeaf(ptr_current_table_cell);
    //
    switchToTheTree( ptr_leaf->getTreeID() );
    //
    m_ptrTree->setCurrentItem(ptr_leaf);
    //
    return;
};

void TreeSearchDlg::switchToTheTree (int i_tree_id)
{
    const int i_actual_index = m_ptrCombo->currentIndex();

    for (int i = 0; i < m_ptrCombo->count(); i++ )
    {
        QVariant root_data = m_ptrCombo->itemData(i);
        RootOfTree* ptr_root = VariantPtr<RootOfTree>::asPtr( root_data );
        Q_ASSERT (ptr_root);
        //
        if (ptr_root->getID() == i_tree_id)
        {
            if (i_actual_index != i )
                m_ptrCombo->setCurrentIndex(i);
            //
            return;
        };
    };
};

void TreeSearchDlg::showEvent (QShowEvent *)
{
    this->ui->m_eSearchString->setFocus();
}
