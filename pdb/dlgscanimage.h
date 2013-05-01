#ifndef DLGSCANIMAGE_H
#define DLGSCANIMAGE_H
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
#include <QByteArray>

//
QT_FORWARD_DECLARE_CLASS(ImageLabel)
QT_FORWARD_DECLARE_CLASS(QToolBar)
QT_FORWARD_DECLARE_CLASS(QAction)
QT_FORWARD_DECLARE_CLASS(QComboBox)
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QScrollArea)
QT_FORWARD_DECLARE_CLASS(QPushButton)
QT_FORWARD_DECLARE_CLASS(QSpinBox)
QT_FORWARD_DECLARE_CLASS(QScrollBar)
QT_FORWARD_DECLARE_CLASS(QCheckBox)
QT_FORWARD_DECLARE_CLASS(QToolButton)
//
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
    QToolButton*    m_ptrZoom;
    QAction*        m_ptrZoomIn;
    QAction*        m_ptrZoomOut;
    QAction*        m_ptrZoom_1_1;
    //
    QAction*        m_ptrCrop;
    //
    QLabel*         m_ptrWidthLabel;
    QSpinBox*       m_ptrWidthBox;
    //
    QLabel*         m_SpaceLbl;
    QCheckBox*      m_Proportional;
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
