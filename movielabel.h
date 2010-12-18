#ifndef MOVIELABEL_H
#define MOVIELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include "imagesprocessor.h"

class MovieLabel : public QLabel
{
    Q_OBJECT
public:
    MovieLabel(QWidget* parent);   
    void init(QRgb color, QSize size, ImagesProcessor* ip);
    void setImagesProcessor(ImagesProcessor* ip);
    void setPixmap(const QPixmap &);
    void setPixmaps(const QPixmap& big, const QPixmap& small);

signals:
    void colorChanged(QRgb color);

protected:
    QPixmap scaledPixmap(const QPixmap& pix);
    void scalePixmap();
    void mousePressEvent(QMouseEvent *ev);
    void resizeEvent(QResizeEvent * ev);

private:
    QPixmap orgPixmap;
    ImagesProcessor* imagesProcessor;
};

#endif // MOVIELABEL_H
