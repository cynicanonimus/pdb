#ifndef TMPFILECLEANER_H
#define TMPFILECLEANER_H
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
