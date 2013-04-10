#ifndef ICONMANAGER_H
#define ICONMANAGER_H
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
#include <QReadWriteLock>
#include <QImage>
#include <QMap>
//
#include "dbicon.h"
//
typedef QMap<int, DBIcon*> IconStorage;
//
class IconManager : public QObject
{
    Q_OBJECT
public:
                            ~IconManager    ();
    static IconManager&     getInstance     ();
    //
    // adds new icon into database, returns id of the icon ( -1 means "database error")
    //
    void                init();
    int                 addIcon     (const QString& str_icon_name, QImage* ptr_icon, const QString &str_icon_hash, QByteArray &icon_byte_array);
    void                deleteIcon  (int i_icon_id);
    bool                contains    (const QString& str_icon_hash) const;

    const IconStorage&  getIconList ()                                                      {return m_lIconList;}
    QImage*             getIcon     (int i_icon_id, bool b_increase_countrer);

    bool                isInUse     (int i_icon_id);
    //void                plusInUse   (int i_icon_id);
    void                minusInUse  (int i_icon_id);

signals:
    void                InitDone    (); //end of icon reading

public slots:
    //
private:
    explicit                IconManager(QObject *parent = 0);
                            IconManager (IconManager const&);
    void                    operator=(IconManager const&);
    //

private:
    mutable QReadWriteLock  m_IconStorageAccess;
    IconStorage             m_lIconList;
};

#endif // ICONMANAGER_H
