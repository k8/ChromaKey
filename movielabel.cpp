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
    const QPixmap* pix = pixmap();
    if (pix)
    {
        QRgb color = pix->toImage().pixel(ev->pos());
        emit colorChanged(color);
    }
}
