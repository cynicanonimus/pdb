#include "imagelabel.h"

ImageLabel::ImageLabel(QWidget *parent) :
    QLabel(parent)
{
    m_ptrBand = NULL;
}

ImageLabel::~ImageLabel()
{
    if (m_ptrBand)
    {
        delete m_ptrBand;
    }
}

void ImageLabel::mousePressEvent(QMouseEvent *event)
{
    m_pBeginPoint = event->pos();
    //
    if (m_ptrBand)
    {
        m_ptrBand->hide();
        delete m_ptrBand;
    }
    //
    m_ptrBand = new QRubberBand(QRubberBand::Rectangle, this);
    m_ptrBand->setGeometry(QRect(m_pBeginPoint, QSize()));
    m_ptrBand->show();
}

void ImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_ptrBand)
        m_ptrBand->setGeometry(QRect(m_pBeginPoint, event->pos()).normalized());
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->pos() == m_pBeginPoint)
    {
        if (m_ptrBand)
        {
            m_ptrBand->hide();
            delete m_ptrBand;
            m_ptrBand = NULL;
            emit CropAllowed(false);
        };
    }else
    {
        if (m_ptrBand)
        {
            emit CropAllowed(true);
        };
    };
}
