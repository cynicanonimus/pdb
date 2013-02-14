#ifndef TMPFILECLEANER_H
#define TMPFILECLEANER_H
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
///tmpfilecleaner.h
//
#include <QObject>
#include <vector>
#include <QMutex>

class TmpFileCleaner : public QObject
{
    Q_OBJECT

public:
    ~TmpFileCleaner();
    static TmpFileCleaner&      getInstance ();
    void                        deleteFile  ( const QString& str_tmp, bool b_immediately = false);
    void                        clearAll    ();

private:
    explicit    TmpFileCleaner  (QObject *parent = 0);
                TmpFileCleaner  (TmpFileCleaner const&);
void            operator=(TmpFileCleaner const&);


signals:
    
public slots:

private:
        void    getShredstatus  ();
        void    shred_it        ( const QString& str_file_name );

private:
    std::vector<QString>    m_vTmpFilesList;
    QMutex                  m_ListAccessBlocker;
    //
    bool                    mbUseShred;
    QString                 m_strShredCommand;
    bool                    m_bDeleteOnExit;  //cashed value

};

#endif // TMPFILECLEANER_H
