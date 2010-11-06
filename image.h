#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;

class Image : public QImage
{
public:
    Image();
};

QImage fromCvMat(const cv::Mat& mat);
void keying(Mat& fg, Mat& bg, int eps, int eps2, QRgb color, QRgb darkColor);

#endif // IMAGE_H
