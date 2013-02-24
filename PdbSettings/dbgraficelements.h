#ifndef DBGRAFICELEMENTS_H
#define DBGRAFICELEMENTS_H
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
#include <QObject>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QProcess>
//
#include "abstractgraficeelements.h"
#include "../CommonInclude/pdb/DBSettings.h"
#include "dbcreator.h"

class QSqlDatabase;

class DBGraficElements : public AbstractGraficeElements
{
    Q_OBJECT
public:

    explicit DBGraficElements(QGroupBox* ptr_parent_frame, QGridLayout* ptr_layout, QObject *parent = 0);
    //
    virtual ~DBGraficElements();
    //
    void            writeData();
signals:
    
public slots:

private slots:
    void            onTestBtnClick      ();
    void            onDefaultBtnClick   ();
    void            onPrevClick         ();
    void            onNextClick         ();
    void            onDeleteClick       ();
    void            onCreateNewClick    ();
    void            onTunnelCheckboxClick();
    void            onTunnelError       ( QProcess::ProcessError );
protected:

private:
    void            readData            ();
    void            updateData          ( bool b_from_dialog ); //exchange between dialog and variables
    void            createLayout        ();
    void            createLinks         ();
    void            setNavButtonsState  ();
    bool            isDBTablesOK        ( QSqlDatabase* ptr_db, QStringList &already_existing_tables );
    bool            createDBTables      ( QSqlDatabase* ptr_db );

public:
    //database configuration controls
    QPushButton*    m_pPrevButton;
    QPushButton*    m_pNextButton;
    QPushButton*    m_pAddButton;
    QPushButton*    m_pDeleteButton;
    //
    QLabel*         m_pLabelPageName;
    QLineEdit*      m_pLineEditPageName;
    //
    QLabel*         m_pLabelDatabaseType;
    QComboBox*      m_pDatabaseType;
    //
    QLabel*         m_pLabelHost;
    QLineEdit*      m_pLineEditHost;
    //
    QLabel*         m_pLabelDBName;
    QLineEdit*      m_pLineEditDBName;
    //
    QLabel*         m_pLabelUserName;
    QLineEdit*      m_pLineEditUserName;
    //
    QLabel*         m_pLabelPwd;
    QLineEdit*      m_pLineEditPwd;
    //
    QLabel*         m_pLabelPort;
    QLineEdit*      m_pLineEditPort;
    //
    QLabel*         m_pLabelConnAmount;
    QLineEdit*      m_pLineEditConnAmount;
    //
    QLabel*         m_ptrBackupLabel;
    QLineEdit*      m_ptrEditBackup;
    //
    QCheckBox*      m_pcheckMakeTunnel;
    QLabel*         m_pLabelMakeTunnel;
    QLineEdit*      m_pLineMakeTunnel;
    //
    QPushButton*    m_pTestDbConnButton;
    //
    QPushButton*    m_pSetCurrentCgfDefault;

private:
    void            makeTunnel      ();
    void            destroyTunnel   ();
private:
    //copy of settings storage
    DBCreator       m_dbCreator;
    bool            m_bDBCreatorIsOK;
    DBSettings      m_stDBSettings;   
    QProcess*       m_ptrTunnelProcess;
    bool            m_bTunnelIsActiveNow;
};

#endif // DBGRAFICELEMENTS_H
