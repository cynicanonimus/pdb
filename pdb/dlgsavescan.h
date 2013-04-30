#ifndef DLGSAVESCAN_H
#define DLGSAVESCAN_H

#include <QDialog>

namespace Ui {
class DlgSaveScan;
}

class DlgSaveScan : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgSaveScan(QWidget *parent = 0);
    ~DlgSaveScan();

public:
    
    int             getImageQuality   () const {return m_iImageQuality;}
    const QString&  getAttachmentName () const {return m_strAttachmentName;}
    const QString&  getAttachmentExt  () const {return m_strExtention;}

private slots:
    void onSave();

private:
    Ui::DlgSaveScan *ui;

private:
    int         m_iImageQuality;
    QString     m_strAttachmentName;
    QString     m_strExtention;
    //
};

#endif // DLGSAVESCAN_H
