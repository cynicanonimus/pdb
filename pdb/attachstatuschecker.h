#ifndef ATTACHSTATUSCHECKER_H
#define ATTACHSTATUSCHECKER_H
/*
set of flags for checking attachment list
This class used for define status of attachments
for making right status of the menu items
*/
class AttachStatusChecker
{
public:
    AttachStatusChecker();
    //
    bool m_bAtLeastOneAttachmentIsDeleted;
    bool m_bAtLeastOneAttachmentIsOK;
    //
    bool m_bAtLeastOneAttachmentOKAndUnprotected;
    bool m_bAtLeastOneAttachmentOKAndProtected;
    //
    bool m_bAtLeastOneAttachmentOKAndDecrypted;
    bool m_bAtLeastOneAttachmentOKAndEncrypted;
    //
    bool m_bAtLeastOneAttachmentIsNotReady;
    //
    bool m_bParentNodeHasOkStatus;
    //
    int  m_iAttachmentsAmount;

};

#endif // ATTACHSTATUSCHECKER_H
