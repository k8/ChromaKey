#include "movielabel.h"
#include <QDebug>
#include <QRgb>

MovieLabel::MovieLabel(QWidget* parent)
    : QLabel(parent)
{
    setCursor(Qt::CrossCursor);    
}

void MovieLabel::setPixmap(const QPixmap & pix)
{    
//    qDebug() << "pixmap size: " << pix.size();
    QLabel::setPixmap(scaledPixmap(pix));
}

QPixmap MovieLabel::scaledPixmap(const QPixmap& pix)
{
    if (pix.isNull())
        return pix;
    if (pix.width()/width() < pix.height()/height())
        return pix.scaledToHeight(height());
    else
        return pix.scaledToWidth(width());
}

void MovieLabel::paintEvent(QPaintEvent * event)
{
    const QPixmap* pix = pixmap();
    if (pix && !pix->isNull())
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
    if (pix && !pix->isNull())
    {
        double factX = (double)pix->width()/width();
        double factY = (double)pix->height()/height();
        QRgb color = pix->toImage().pixel(ev->x()*factX, ev->y()*factY);
        emit colorChanged(color);
    }
}

void MovieLabel::resizeEvent(QResizeEvent *)
{
    const QPixmap* pix = pixmap();
    if (pix && !pix->isNull())
    {
        setPixmap(*pix);
//        qDebug() << "size before: " << size();
        QLabel::resize(pixmap()->size());
//        qDebug() << "size after: " << size();
    }
}
