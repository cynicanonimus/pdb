#ifndef ABSTRACTDATABASEOBJECT_H
#define ABSTRACTDATABASEOBJECT_H
//
// abstractdatabaseobject.h
//
/*
parent class for treeleaf, RootOfTree and attachment
*/
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QSemaphore>
#include <QMutexLocker>
#include <QReadWriteLock>
//
class DbNode;
//
class AbstractDatabaseObject : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDatabaseObject(int                 i_id,
                                    bool                b_is_active,
                                    const QDateTime&    dt_last_change,
                                    QObject*            parent = 0);
public:
    enum DB_OBJECT_STATUS{OBJECT_OK                 = 0,
                          OBJECT_DELETED            = 1,
                          OBJECT_DELETING           = 2,
                          OBJECT_UPDATING           = 3,
                          OBJECT_TREE_ID_UPDATING   = OBJECT_UPDATING,
                          OBJECT_NAME_UPDATING      = 4,
                          OBJECT_ATTACHMENT_INSERT  = 5,
                          OBJECT_DESCRIPTOR_UPDATING= 6,
                          OBJECT_UPDATING_STOP      = OBJECT_DESCRIPTOR_UPDATING,
                          OBJECT_NOT_DEFINED        = 7
                         };
public:
    int                 getID() const                                   { return m_iID; }
    //
    DB_OBJECT_STATUS    getObjectStatus () const;
    void                setObjectStatus (DB_OBJECT_STATUS en_status);
    //
    const QDateTime&    getDateTime     () const                        { return m_dtLastChange;   }
    //
    virtual bool       rename_it        (const QString& str_name)   = 0;
    virtual bool       delete_it        ( bool b_silence=false )    = 0;
    virtual bool       restoreObject    ()                          = 0;


signals:
    
public slots:

protected:
        int                     m_iID;              //object index in the database. if it is "-1" - it is created by user interface, if it is > 0 - by database

protected:
        virtual void            setActiveStatus_DB  (bool b_active) = 0;
        QString                 getSQLAdaptedString (const QString& t_str);      //mask ' and " in the string
        //
        virtual int             insertTo_DB         () = 0;         //must be defined in the every parent class. put new content to DB
        virtual bool            updateName_DB       () = 0;
private:
        DB_OBJECT_STATUS        m_enObjectStatus;   // actual node status
        QDateTime               m_dtLastChange;     // timestamp of the last change
        mutable QReadWriteLock  m_StatusLock;
};

#endif // ABSTRACTDATABASEOBJECT_H
