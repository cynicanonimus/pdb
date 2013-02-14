#include "cryptothread.h"

CryptoThread::CryptoThread(QObject *parent) :
    QThread(parent)
{
    reset();
    m_bFinishedCorrectly = true;
}

void CryptoThread::addAttacmentsToProcessList ( const Attachment::AttachmentsList& attach_list )
{
    Attachment::AttachmentsList::const_iterator itr_new     = attach_list.begin();
    Attachment::AttachmentsList::const_iterator itr_exist   = m_vProcess_list.begin();
    //
    for (;itr_new != attach_list.end(); ++itr_new)
    {
        bool b_allow_to_add = true;
        //
        for (itr_exist = m_vProcess_list.begin(); itr_exist != m_vProcess_list.end(); ++itr_exist)
        {
            if ( (*itr_new)->getID() == (*itr_exist)->getID() )
            {
                b_allow_to_add = false;
                break;
            };
        };
        //
        if(b_allow_to_add)
            m_vProcess_list.push_back(*itr_new);
    };
    //
    m_bFinishedCorrectly = true;
}

void CryptoThread::reset()
{
    m_vProcess_list.erase( m_vProcess_list.begin(), m_vProcess_list.end() );
    m_enWhatToDo = UNKNOWN_ACTION;
}

void CryptoThread::run()
{
    if (UNKNOWN_ACTION == m_enWhatToDo)
        return; //nothing to do
    //
    m_bContinueWork = true;
    //
    if (m_vProcess_list.size() == 0)
        return; //nothing to do
    //
    QString str_direction;
    //
    switch(m_enWhatToDo)
    {
    case ENCRYPT_IT:
        str_direction = "Encrypt ";
        break;
    case DECRYPT_IT:
        str_direction = "Decrypt ";
        break;
    //case RE_ENCRYPT_IT,
    case PROTECT_IT:
        str_direction = "Protect ";
        break;
    case UNPROTECT_IT:
        str_direction = "Unprotect ";
        break;
    case UNKNOWN_ACTION:
        return;
    default:
        m_bContinueWork = false;
        Q_ASSERT(FALSE);
        break;
    };
    //
    Attachment::AttachmentsList::const_iterator itr = m_vProcess_list.begin();
    //
    unsigned int ui_current_number = 0;
    emit setValue(ui_current_number);
    //
    for (;itr != m_vProcess_list.end(); ++itr)
    {
        if (false == m_bContinueWork)
            break;
        //
        QString str_message = str_direction;
        str_message += (*itr)->getName();
        str_message += "...";
        emit setCurrentLabel(str_message);
        //
        bool b_resut = false;
        switch(m_enWhatToDo)
        {
        case ENCRYPT_IT:
            b_resut = (*itr)->encrypt_it();
            break;
        case DECRYPT_IT:
            b_resut = (*itr)->decrypt_it();
            break;
        //case RE_ENCRYPT_IT,
        case PROTECT_IT:
            b_resut = (*itr)->protect_it(true);
            break;
        case UNPROTECT_IT:
            b_resut = (*itr)->protect_it(false);
            break;
        case UNKNOWN_ACTION:
            m_bContinueWork = false;
            continue;
            break;
        default:
            Q_ASSERT(FALSE);
        };
        //
        if (false == b_resut)
        {
            m_bFinishedCorrectly = false;
            break;
        };
        //
        emit setValue(++ui_current_number);
    };
    //
    reset();
    emit stopThis();
}

void CryptoThread::OnTerminate ()
{
    m_bContinueWork = false;
}
