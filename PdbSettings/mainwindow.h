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
#include <QMainWindow>
//#include <QEvent>
#include <QTreeWidgetItem>
//
#include "dbgraficelements.h"
#include "colorgraficelements.h"
#include "nodesandattachmentsgraficeelements.h"
#include "loggetelements.h"
#include "securityelements.h"
//
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
    void closeEvent(QCloseEvent* e);
    //
signals:
    void saveDBSettings();

public slots:

    void onTreeCurrentItemChanged(QTreeWidgetItem* p_new_item, QTreeWidgetItem* p_previous_item);
    void saveSettings();

private:
    Ui::MainWindow* ui;
    //
    QGridLayout*                        m_pGridLayout;
    //----
    DBGraficElements*                   m_pDBGrafic;
    ColorGraficElements*                m_pColorGrafic;
    NodesAndAttachmentsGraficeElements* m_pNodesAndAttachmentsGrafic;
    LoggetElements*                     m_ptrLogger;
    SecurityElements*                   m_ptrSecurity;
    //
    //TmpStorageSettings      m_Settings;
    //
private:
    void            createSettingsTree  ();
    void            linkSignalsAndSlots ();
    void            saveLeafSettings    (QTreeWidgetItem* ptr_item);
    /*
      save data from grafic elements to storage when we leave tree item
    */
    //void            saveItemData        ( const QString& str_item_name );
    /*
      restore data from storage to grafic elements when we choose new tree item
    */
    //void            restoreItemData     ( const QString& str_item_name );
    /*
    show elements on the right side, belong to the tree leaf
    */
    //void            showItem            ( const QString& str_item_name );
    //
    void            createGraficeElements    ();
    //

};

#endif // MAINWINDOW_H
