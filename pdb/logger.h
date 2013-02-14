#ifndef LOGGER_H
#define LOGGER_H

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
    void                    logIt       (unsigned int ui_err_code, const QString& str_message);
    void                    stopLog();

private:
    explicit    Logger(QObject *parent = 0);
                Logger  (Logger const&);
    void        operator=(Logger const&);

signals:
    
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
