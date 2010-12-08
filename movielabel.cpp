#include "movielabel.h"
#include <QDebug>
#include <QRgb>

MovieLabel::MovieLabel(QWidget* parent)
    : QLabel(parent)
{
    setCursor(Qt::CrossCursor);    
}

void MovieLabel::setImagesProcessor(ImagesProcessor *ip)
{
    imagesProcessor = ip;
    imagesProcessor->setSize(size());
}

void MovieLabel::setPixmap(const QPixmap & pix)
{
    orgPixmap = pix;
    QLabel::setPixmap(pix);
//    scalePixmap();
}

void MovieLabel::setPixmaps(const QPixmap &big, const QPixmap &small)
{
    orgPixmap = big;
    QLabel::setPixmap(small);
}

QPixmap MovieLabel::scaledPixmap(const QPixmap& pix)
{
    if (pix.isNull())
        return pix;
    if (pix.width()/(double)width() < pix.height()/(double)height())
        return pix.scaledToHeight(height());
    else
        return pix.scaledToWidth(width());
}

void MovieLabel::scalePixmap()
{
    QLabel::setPixmap(scaledPixmap(orgPixmap));
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

void MovieLabel::resizeEvent(QResizeEvent * event)
{
    imagesProcessor->setSize(event->size());
    scalePixmap();
}
