#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QSize>
#include <QImage>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QDebug>
using namespace cv;

#include "imagesprocessor.h"

class Image : public QObject
{
public:
    Image() {}
    Image(const QString&);
    Image(const Size& size, const QRgb& color);
    Image(const Image& other);
    virtual ~Image() {}
    Size getSize() {return image.size();}
    virtual const Mat& get(bool next);
    virtual QImage getResizedQImage(const QSize& size);
    bool isOpened() {return opened;}
    virtual bool isMovie()
    {
        return false;
    }
    virtual bool isFinished() {return true;}
    const QString& getFile() {return file;}

protected:
    Mat image;
    bool opened;
    QString file;
};

class ImageSaver
{
public:
    ImageSaver(const QString& file);
    virtual ~ImageSaver() {}
    virtual bool save(const Mat& image);
    virtual int getProgress() { return 100;}
protected:
    QString file;
};

#endif // IMAGE_H
