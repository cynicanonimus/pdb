#ifndef DLGSCANIMAGE_H
#define DLGSCANIMAGE_H

#include "imagelabel.h"

#include <QDialog>
#include <QToolBar>
#include <QAction>
#include <QComboBox>
#include <QLabel>
#include <QScrollArea>
#include <QScrollBar>
#include <QSpinBox>
#include <QPushButton>
//#include <QMouseEvent>
#include <QByteArray>

namespace Ui {
class DlgScanImage;
}

class DlgScanImage : public QDialog
{
    Q_OBJECT
    
public:
    explicit            DlgScanImage    (QWidget *parent = 0);
                        ~DlgScanImage   ();
    //
    const QByteArray&   getImage        () const                                          {return m_baImageAsArray;}
    const QString&      getImageName    () const                                          {return m_strImageName;}
    //
private:
    void                makeToolBar     ();
    void                setLinks        ();
    void                scaleImage      (double factor);
    void                adjustScrollBar (QScrollBar* scrollBar, double factor);
    //
    void                debugOpen       ();
    //
private slots:

    void        onRotate    ();
    //
    void        onZoomIn    ();
    void        onZoomOut   ();
    void        onZoom_1_1  ();
    //
    void        onWidthChanged(int);
    void        onHeightChanged(int);
    //
    void        onCrop      ();
    //
    void        onResize    ();
    //
    void        onSave      ();
    //
    void        onCropAllowed(bool);

private:
    //
    QLabel*         m_ptrScanLabel;
    QComboBox*      m_ptrScanMode;
    QAction*        m_ptrScan;
    //
    QAction*        m_ptrRotate;
    //
    QAction*        m_ptrZoomIn;
    QAction*        m_ptrZoomOut;
    QAction*        m_ptrZoom_1_1;
    //
    QAction*        m_ptrCrop;
    //
    QLabel*         m_ptrWidthLabel;
    QSpinBox*       m_ptrWidthBox;
    //
    QLabel*         m_ptrHeightLabel;
    QSpinBox*       m_ptrHeightBox;
    //
    bool            m_bIgnoreResize;
    //
    double          m_d_WH_Ratio;
    //
    QPushButton*    m_pResize;
    //
    QAction*        m_ptrSave;
    QAction*        m_ptrExit;
    //
    // draw_params.png
    //
    QToolBar*       m_ptrToolBar;
    //
    QScrollArea*    m_ptrScrollArea;
    ImageLabel*     m_ptrImageLabel;
    //
    QPixmap*        m_ptrImage;
    QByteArray      m_baImageAsArray;
    QString         m_strImageName;
    //
    double          m_dscaleFactor;
    //
    Ui::DlgScanImage *ui;
};

#endif // DLGSCANIMAGE_H
