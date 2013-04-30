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
    dropSelection();
    //
    m_ptrBand = new QRubberBand(QRubberBand::Rectangle, this);
    m_ptrBand->setGeometry(QRect(m_pBeginPoint, QSize()));
    m_ptrBand->show();
    //
    event->accept();
}

void ImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_ptrBand)
        m_ptrBand->setGeometry(QRect(m_pBeginPoint, event->pos()).normalized());
    //
    event->accept();
}

void ImageLabel::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->pos() == m_pBeginPoint)
    {
        dropSelection();
    }else
    {
        if (m_ptrBand)
        {
            emit CropAllowed(true);
        };
    };
    //
    event->accept();
}

void ImageLabel::dropSelection ()
{
    if (m_ptrBand)
    {
        m_ptrBand->hide();
        delete m_ptrBand;
        m_ptrBand = NULL;
        emit CropAllowed(false);
    };
}

const QRect* ImageLabel::getSelection ()
{
    const QRect* selection_band = NULL;
    //
    if (m_ptrBand)
    {
        selection_band = &(m_ptrBand->geometry());
    };
    //
    return selection_band;
}
