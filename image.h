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
void segmentation(Mat& mat);
void keying(Mat& fg, Mat& bg, QRgb color, int hue, int saturation, int value, bool segm);

#endif // IMAGE_H
