#ifndef TREESEARCHDLG_H
#define TREESEARCHDLG_H

#include <QDialog>
#include <QComboBox>
#include <QGridLayout>
#include <QDateTime>

//
#include "searcher.h"
#include "searchrequest.h"
//
class MyTree;
class TreeLeaf;
class QTableWidgetItem;

namespace Ui {
class TreeSearchDlg;
}

class TreeSearchDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit TreeSearchDlg(QWidget *parent = 0);
    ~TreeSearchDlg();
    //
    void        keyPressEvent       (QKeyEvent * event);
    void        showEvent           (QShowEvent *);
    //
    void        setPtrToComboBox    (QComboBox*     ptr_combo);
    void        setPtrToTree        (MyTree*        ptr_to_tree);

public slots:
    void    onBeginSearch       ();
    void    onEndSearch         ();
    void    onGetTotalNodesAmount(unsigned int);
    void    onJumpNextNode      ();
    void    onfoundNode         (TreeLeaf*,int);

private slots:

    void    onHideButton();
    void    onClearButton();
    void    onSearchButton();
    void    onChangeSearchString(QString);
    void    onClickSearchArray();
    void    onSearchTableClick();
    void    onChangeRegExpState (int);
    void    onFromSearchClick();
    void    onToSearchClick();

private:
    //
    enum ACTUAL_DLG_MODE
    {
        enSEARCH_MODE = 1,
        enCANCEL_MODE = 2
    };
    //
    void    enableSearchButton      (const QString& str_search_string);
    void    swapDlgMode             ();
    //
    void    startSearch             ();
    void    cancelSearch            ();
    //
    void    lockInterface           (bool b_lock = true);
    void    clearSearchResults      ();
    //
    TreeLeaf* getLinkedLeaf         (QTableWidgetItem* ptr_cell);
    QString getTreeNameByTreeID     (int i_tree_id);
    void    addIndexToLocation      (QString& str_location, int i_location);
    void    switchToTheTree         (int i_tree_id);
    void    setSignalSocketLinks    ();
    void    setFormLayout           ();
    void    createSearchRequest     (SearchRequest& request);
    bool    isSearchArrayDefined    ();
    void    fillComboPatternSyntax  ();
    bool    getEarliestDateFromDB   ();

private:
    Ui::TreeSearchDlg *ui;
    //
    QComboBox*              m_ptrCombo;
    MyTree*                 m_ptrTree;
    Searcher                m_Searcher;
    //
    const unsigned int      m_uiTreeColNum;
    const unsigned int      m_uiNodeColNum;
    const unsigned int      m_uiLocationColNum;
    //
    QDateTime               m_dtEarliestDateTime;
    //
    ACTUAL_DLG_MODE m_enMode;
    //
    QGridLayout*     m_ptrFormLayout;

};

#endif // TREESEARCHDLG_H
