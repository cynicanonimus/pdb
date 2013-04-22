#ifndef DBICON_H
#define DBICON_H
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
// this class represents icon object in the table icons_tbl (database)
//
#include <QObject>
#include <QImage>
//
//
//
class DBIcon : public QObject
{
    Q_OBJECT
public:
    explicit DBIcon( const QString& str_icon_name, const QString& str_icon_hash, QImage* ptr_img );
             DBIcon (DBIcon const&);
    void     operator=(DBIcon const&);

    ~DBIcon();
    //
    //void        setHash (const QString& str_hash);
    bool            isHashMatched   (const QString& str_hash) const;
    void            destroy         ();
    QImage*         getImage        (bool b_increase_counter);
    const QString&  getName         ()      {return m_strIconName;}
    //
    bool            isInUse         ()      { return (m_uiInUseCounter > 0);}
    void            minusInUse      ()      { --m_uiInUseCounter; }
    //
signals:
    
public slots:

private:

    void            plusInUse       ()      { ++m_uiInUseCounter; }

private:
    unsigned int    m_uiInUseCounter;
    QString         m_strIconName;
    QString         m_strHash;
    QImage*         m_ptrImage;
    
};

#endif // DBICON_H
