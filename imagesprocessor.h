#ifndef IMAGESPROCESSOR_H
#define IMAGESPROCESSOR_H

#include <QDebug>
#include <QImage>
#include <QMutex>
#include <QColor>
#include <cmath>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;

class ImagesProcessor
{
public:

    ImagesProcessor(QSize size = QSize(100, 100));
    static QImage fromCvMat(const Mat& mat);
    QImage scaledFromCvMat(const Mat& mat);
    static void fill(Mat& mat, QRgb color);    
    static void resize(const Mat &mat, Mat &out, double fact);
    static void to3Chanels(const Mat &mat, Mat &out);
    void setSize(QSize size);
    QSize getSize();

private:
    QMutex mutex;
    QSize imageSize;    
};

#endif // IMAGESPROCESSOR_H
