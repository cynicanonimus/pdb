#ifndef ATTACHIMPORTORREPLACEDLG_H
#define ATTACHIMPORTORREPLACEDLG_H
//
//Class implements dialog for making import of attachment or making replacement of attach
//
#include <QObject>
#include <QStringList>

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

private:

    QString         m_strDlgHeader;
    QFileDialog*    m_dlgFileDlg;
    QStringList     m_FileList;
    //
    bool            m_bAllowMultipleFiles;
    bool            m_bDeleteFilesAfterAttachment;
    bool            m_bProtectAttachment;
    bool            m_bEncryptAttachment;
    QString         m_strLastAttachmentFromPath;
};

#endif // ATTACHIMPORTORREPLACEDLG_H
