#ifndef CRYPTOTHREAD_H
#define CRYPTOTHREAD_H
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
