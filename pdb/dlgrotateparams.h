#ifndef DLGROTATEPARAMS_H
#define DLGROTATEPARAMS_H

#include <QDialog>

namespace Ui {
class DlgRotateParams;
}

class DlgRotateParams : public QDialog
{
    Q_OBJECT
    
public:
    explicit DlgRotateParams(QWidget *parent = 0);
    ~DlgRotateParams();

public:

    int     getAngle    () const        {return m_iAngle;}
    bool    isClockWise () const        {return m_bClockWise;}
    
private slots:
    void onOK();

private:

    int     m_iAngle;
    bool    m_bClockWise;

    Ui::DlgRotateParams *ui;
};

#endif // DLGROTATEPARAMS_H
