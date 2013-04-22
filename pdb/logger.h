#ifndef LOGGER_H
#define LOGGER_H
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
#include "../CommonInclude/pdb/pdb_style.h"
//
#include <QObject>
#include <QBitArray>
#include <QMutex>
#include <QTextStream>
#include <QFile>

class Logger : public QObject
{
    Q_OBJECT

public:

                            ~Logger     ();
    static      Logger&    getInstance  ();
    //init function
    void                    logIt       (unsigned int ui_err_code,  const QString& str_message, const QString *ptr_str_query = NULL);
    void                    stopLog();

private:
    explicit    Logger(QObject *parent = 0);
                Logger  (Logger const&);
    void        operator=(Logger const&);

signals:
    void        newLogEvent(QString);
public slots:

private:
    QBitArray       m_bitFlags;     //logging settings
    QString         m_strLogPath;   //path to the log-file
    //
    QMutex          m_FileLocker;
    QFile           m_File;
    QTextStream*    m_ptrOutStream;
    //
private:
    void            readSettings        ();
    void            writeBeginLog       ();
    bool            isLogCodeActive     (unsigned int ui_err_code = en_ENABLE_LOG);
    QString         getLogString        (unsigned int ui_err_code) const;
};


#endif // LOGGER_H
