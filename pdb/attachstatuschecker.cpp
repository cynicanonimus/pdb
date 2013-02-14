#include "attachstatuschecker.h"


AttachStatusChecker::AttachStatusChecker()
{
    m_bAtLeastOneAttachmentIsDeleted           = false;
    m_bAtLeastOneAttachmentIsOK                = false;
    //
    m_bAtLeastOneAttachmentOKAndUnprotected    = false;
    m_bAtLeastOneAttachmentOKAndProtected      = false;
    //
    m_bAtLeastOneAttachmentOKAndDecrypted      = false;
    m_bAtLeastOneAttachmentOKAndEncrypted      = false;
    //
    m_bAtLeastOneAttachmentIsNotReady          = false;
    //
    m_bParentNodeHasOkStatus                   = false;
    //
    m_iAttachmentsAmount                       = 0;

}

