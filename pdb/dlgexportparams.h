#ifndef DLGEXPORTPARAMS_H
#define DLGEXPORTPARAMS_H

#include <QDialog>
#include <QKeyEvent>

namespace Ui {
class DlgExportParams;
}

class DlgExportParams : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgExportParams(QWidget *parent = 0);
    ~DlgExportParams();
    //
    void            reset          ();
    bool            isExportAll    () const;
    bool            isExportCurrent() const;
    const QString&  getExportPath  () const;
    //
    void    keyPressEvent(QKeyEvent * event);

private slots:
    void    onClickExportAll     ();
    void    onClickExportCurrent ();
    void    onClickCancel        ();
    void    onChangeExportPath   (QString);
    void    onClickChangeExportPath();
    
private:
    Ui::DlgExportParams *ui;


private:

    enum en_EXPORT_TYPE
    {
        en_EXPORT_ALL,
        en_EXPORT_CURRENT,
        en_DO_NOTHING
    };
    //
    en_EXPORT_TYPE  m_enUserChoice;
    QString         m_strExportPath;
};

#endif // DLGEXPORTPARAMS_H
