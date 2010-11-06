#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;

QImage fromCvMat(const Mat& mat);
void segmentation(Mat& in);
void keying(const Mat& fg, const Mat& bg, Mat& out, QRgb color, int hue, int saturation, int value, bool segm);

#endif // IMAGE_H
