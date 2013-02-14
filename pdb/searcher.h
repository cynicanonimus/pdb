#ifndef SEARCHER_H
#define SEARCHER_H

#include <QObject>
#include <QComboBox>
#include <QThread>
#include <QMutex>
#include <QRegExp>

//
#include "searchrequest.h"
#include "abstractdatabaseobject.h"
//
class MyTree;
class TreeLeaf;
//
class Searcher : public QThread
{
    Q_OBJECT
public:
    explicit Searcher();

public:
    void setPtrToCombo      ( QComboBox* ptr_combo );
    void setPtrToTree       ( MyTree* ptr_tree );
    void setSearchConditions(const SearchRequest& request);
    void cancelSearch       ();
    void run                ();

signals:
    void getTotalNodesAmount(unsigned int);
    void beginOfSearch      ();
    void endOfSearch        ();
    void jumpToNextNode     ();
    void foundNode          (TreeLeaf*,  int); //tree ID, pointer to the leaf, int SEARCH_LOCATION code (where was found)

public slots:
    
private:
    bool            isOKToStartSearch      ();
    bool            isOKToContinueSearch   ();
    bool            resetContinueFlag      ();
    unsigned int    getTotalNodesAmount    ();
    void            getNodesAmountPerTree  ( TreeLeaf* ptr_root, unsigned int& ui_already_calc );
    void            search                 ();
    void            recursiveSearchInTree  (TreeLeaf* ptr_root, QRegExp& rx);
    //
    void            searchInNodeName       (TreeLeaf* ptr_root, QRegExp& rx);
    void            searchInNodeDescriptor (TreeLeaf* ptr_root, QRegExp& rx);
    void            searchInAttachNames    (TreeLeaf* ptr_root, QRegExp& rx);
    void            searchInAttachBody     (TreeLeaf* ptr_root, QRegExp& rx);
    //
    bool            searchInAnyText                         (const QString& str_target_pfrase, QRegExp& rx);
    bool            isTimingOKToSearchInThisObject    (const AbstractDatabaseObject *ptr_object) const;

private:
    //
    QComboBox*      m_ptrCombo;
    MyTree*         m_ptrTree;
    //
    SearchRequest   m_SearchRequest;
    //
    bool            m_bContinueSearch;
    QMutex          m_mContinueLocker;
};

#endif // SEARCHER_H
