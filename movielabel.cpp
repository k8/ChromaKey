#include "movielabel.h"
#include <QDebug>
#include <QRgb>

MovieLabel::MovieLabel(QWidget* parent)
    : QLabel(parent)
{
    setCursor(Qt::CrossCursor);    
}

void MovieLabel::paintEvent(QPaintEvent * event)
{
    const QPixmap* pix = pixmap();
    if (pix)
    {
        if (minimumWidth() < pix->width() && maximumHeight() < pix->height())
        {
            setMinimumSize(pix->size());
            setMaximumSize(pix->size());
        }
    }    
    QLabel::paintEvent(event);
}

void MovieLabel::mousePressEvent(QMouseEvent *ev)
{
    qDebug() << width() << height();
    const QPixmap* pix = pixmap();
    if (pix)
    {
        double factX = (double)pix->width()/width();
        double factY = (double)pix->height()/height();
        QRgb color = pix->toImage().pixel(ev->x()*factX, ev->y()*factY);
        emit colorChanged(color);
    }
}
