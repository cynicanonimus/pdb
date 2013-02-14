#ifndef CRYPTOTHREAD_H
#define CRYPTOTHREAD_H
//
#include <QThread>
//
#include "treeleaf.h"
/*
class supports async ecnrypt and decrypt data same as protect and unprotect attachments
*/


class CryptoThread : public QThread
{
    Q_OBJECT
public:
    explicit CryptoThread(QObject *parent = 0);

public:

    enum REQESTED_ACTION
    {
        ENCRYPT_IT,
        DECRYPT_IT,
        //RE_ENCRYPT_IT,
        PROTECT_IT,
        UNPROTECT_IT,
        UNKNOWN_ACTION
    };

public:
    void setAction                      ( REQESTED_ACTION                      en_action )    { m_enWhatToDo = en_action;}
    //for non-global operations
    void addAttacmentsToProcessList     ( const Attachment::AttachmentsList&     attach_list );
    //
    bool isFinishedCorrectly            () const                                              {return m_bFinishedCorrectly;}
    //
    void run();
    
signals:
    void setTotalAttachmentsNumber  (unsigned int);
    void setValue                   (int);
    void setCurrentLabel            (QString);
    void stopThis                   ();

public slots:
    void OnTerminate                ();

private:
    //clear internal attachments buffer
    void reset();
    //
    REQESTED_ACTION                 m_enWhatToDo;
    Attachment::AttachmentsList     m_vProcess_list;
    bool                            m_bContinueWork;
    bool                            m_bFinishedCorrectly;
    
};

#endif // CRYPTOTHREAD_H
