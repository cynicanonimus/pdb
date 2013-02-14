#ifndef ATTACHSTATUSCHECKER_H
#define ATTACHSTATUSCHECKER_H
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
