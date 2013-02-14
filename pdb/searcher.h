#ifndef SEARCHER_H
#define SEARCHER_H
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
