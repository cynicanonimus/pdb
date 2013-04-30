#ifndef IMAGELABEL_H
#define IMAGELABEL_H

#include <QWidget>
#include <QLabel>
#include <QRubberBand>
//
#include <QMouseEvent>


class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ImageLabel(QWidget *parent = 0);
    ~ImageLabel();
    //
    void mousePressEvent    (QMouseEvent *);
    void mouseMoveEvent     (QMouseEvent *);
    void mouseReleaseEvent  (QMouseEvent *);

signals:
    void CropAllowed(bool);
    
public slots:

private:

    QRubberBand*    m_ptrBand;
    QPoint          m_pBeginPoint;
};

#endif // IMAGELABEL_H
