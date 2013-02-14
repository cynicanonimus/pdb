#ifndef ATTACHMENTCLICKATIONDIALOG_H
#define ATTACHMENTCLICKATIONDIALOG_H
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
#include <QDialog>

namespace Ui {
class AttachmentClickAtionDialog;
}

class AttachmentClickAtionDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AttachmentClickAtionDialog(bool b_rename_enabled, QWidget *parent = 0);
    ~AttachmentClickAtionDialog();
    //
    //if want use more than once
    void    reset();
    bool    isDownload();
    bool    isOpen();
    bool    isRename();
    //
    void    keyPressEvent(QKeyEvent * event);
    //
private slots:
    void    onClickOpen     ();
    void    onClickDownload ();
    void    onClickCancel   ();
    void    onClickRename   ();

private:
    enum en_ATTACH_USER_CHOICE
    {
        en_OPEN_ATTACH,
        en_DOWNLOAD_ATTACH,
        en_RENAME_ATTACH,
        en_DO_NOTHING
    };
    //
    en_ATTACH_USER_CHOICE m_enUserChoice;
    //
    Ui::AttachmentClickAtionDialog *ui;
};

#endif // ATTACHMENTCLICKATIONDIALOG_H
