#ifndef ATTACHIMPORTORREPLACEDLG_H
#define ATTACHIMPORTORREPLACEDLG_H
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
//Class implements dialog for making import of attachment or making replacement of attach
//
#include <QObject>
#include <QStringList>
#include <QComboBox>
#include <QCheckBox>

class QFileDialog;

class AttachImportOrReplaceDlg : public QObject
{
    Q_OBJECT
public:
    explicit AttachImportOrReplaceDlg(const QString& str_header,  bool b_allow_multiple_files, QObject *parent = NULL);
             ~AttachImportOrReplaceDlg();
    
signals:
    
public slots:


public:
    bool                exec                            ( const QString& str_path,
                                                          const QString  str_action_button_text,
                                                          bool           b_delete_files_after_attachment,
                                                          bool           b_protect_attachment,
                                                          bool           b_encrypt_attachment);

    const QStringList&  getFileList                     () const                    {return m_FileList;}
    bool                getDeleteFilesAfterAttachment   () const                    {return m_bDeleteFilesAfterAttachment;}
    bool                getProtectAttachment            () const                    {return m_bProtectAttachment;}
    bool                getEncryptAttachment            () const                    {return m_bEncryptAttachment;}
    const QString&      getLastAttachmentFromPath       () const                    {return m_strLastAttachmentFromPath;}
    unsigned int        getTypeOfEncrypt                () const                    {return m_uiTypeOfEncrypt;}

private slots:
    void                onChangeEncryptType             (int);
    void                onChangeEncryptCheckBox         (int);

private:

    void                fillEncryptionTypes              ();

private:

    QComboBox*      m_ptrEncryptType;
    QCheckBox*      m_ptrEncryptUploadEnable;
    QString         m_strDlgHeader;
    QFileDialog*    m_dlgFileDlg;
    QStringList     m_FileList;
    //
    bool            m_bAllowMultipleFiles;
    bool            m_bDeleteFilesAfterAttachment;
    bool            m_bProtectAttachment;
    bool            m_bEncryptAttachment;
    QString         m_strLastAttachmentFromPath;
    unsigned int    m_uiTypeOfEncrypt;
};

#endif // ATTACHIMPORTORREPLACEDLG_H
