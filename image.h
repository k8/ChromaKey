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
    Image() {}
    Image(const QString&);
    Image(const Size& size, const QRgb& color);
    Size getSize() {return image.size();}
    virtual const Mat& get(bool next);
    virtual QImage getResizedQImage(const QSize& size);
    bool isOpened() {return opened;}
    virtual bool isMovie() {return false;}
    virtual bool isFinished() {return true;}
    const QString& getFile() {return file;}
    int getFPS() {return 1;}
    int getProgress() { return 100;}

protected:
    Mat image;
    bool opened;
    QString file;
};

#endif // IMAGE_H
