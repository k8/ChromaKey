#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QSize>
#include <QImage>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;

#include "imagesprocessor.h"

class Image : public QObject
{
public:
    Image();
    virtual bool open(const QString& file) = 0;
    virtual const Mat& get() = 0;
    virtual QImage getResizedQImage(const QSize& size);
protected:
    Mat image;
};

#endif // IMAGE_H
