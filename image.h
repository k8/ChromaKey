#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <QMutex>
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
    void keying(const Mat& fg, const Mat& bg, Mat& out, QRgb color, int hue, int saturation, int value, bool segm);
    void setSize(QSize size);
    QSize getSize();

private:
    void resize(const Mat& mat, Mat& out, double fact);
    void prepareSize(const Mat& a, const Mat& b, Mat& c, Mat& d);
    void segmentation(Mat& in);

    QSize imageSize;
    QMutex mutex;
};

#endif // IMAGE_H
