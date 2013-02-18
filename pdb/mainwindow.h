#ifndef MAINWINDOW_H
#define MAINWINDOW_H
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
// mainwindow.h
//
#include <QMainWindow>
#include <QMutex>
#include <QIcon>
#include <QPixmap>
#include <QLabel>
#include <qsplitter.h>
//
#include "abstractdatabaseobject.h"
#include "notificationdialog.h"
#include "passworddlg.h"
#include "globaltreeoperationshandler.h"
#include "tunnelmaker.h"

//
class TreeLeaf;
class TreeSearchDlg;

class MnuMainWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //
    void closeEvent(QCloseEvent *e);


public slots:
    void onSelectedNodeChanged      (TreeLeaf* ptr_to_current, TreeLeaf* ptr_to_previous);
    void onEditorTextChanged        ();
    void onReleaseConn              ();
    void onTakeConn                 ();
    void onCheckPassword            (bool);
    void onCreateChangePassword     ();
    //void onEstablishConnectionToDB(const QString& str_host_name, const QString& str_db_name);

signals:
    void changeDescription          ();
    void saveDescription            ();

private slots:
    void onSearch                   ();
    void onDbAlreadyInUse           () const;
    void saveCurrentNodeDescriptor  ();
    void onClickAbout               ();

private:
    Ui::MainWindow* ui;
    //Сплиттеры главного окна
    QSplitter*          m_pMainVSplitter; //разделитель включает слева m_pComboBoxTreeVSplitter, а справа m_pEditorServiceBarHSplitter
    QSplitter*          m_pComboBoxTreeVSplitter;
    QSplitter*          m_pEditorServiceBarHSplitter;
    //tree mnu and own elements
    MnuMainWindow*      m_pMainMenu;
    //
    bool                m_bEditorTextChanged;
    //
    GlobalTreeOperationsHandler m_G_TreeActions;
    //
    NotificationDialog  m_dlgWaiting;
    PasswordDlg         m_PwdDlg;
    TreeSearchDlg*      m_ptrSearchDlg;
    //ServicesCfg         m_ServiceCfg;
    TunnelMaker         m_Tunnel;
    bool                m_bIsDBUsageIconVisible;
    //
    QMutex              m_ConnCounterProtector;
    unsigned int        m_uiConnectionsInUse;
    QPixmap*            m_ptrDbInUseIcon;
    QLabel*             m_ptrDbInUseLabel;
private:
    //
    // init graphic elements of the main window
    void initMainVSplitter              ();
    void initComboBoxTreeVSplitter      ();
    void initEditorServiceBarHSplitter  ();
    //
    void setTextChangeSignal            (bool b_changed);
    //
    void saveTextOfDescription          (TreeLeaf* ptr_to_node);
    void showTextOfDescription          (TreeLeaf* ptr_to_node);
    void showDBUsageIcon                (bool b_show = true);
    //
    void restoreWindowParams            ();
    void saveWindowParams               ();
    void connectSignalsAndSlots         ();

    //
};

#endif // MAINWINDOW_H
