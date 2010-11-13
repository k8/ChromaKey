#ifndef IMAGE_H
#define IMAGE_H

#include <QImage>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;

QImage fromCvMat(const Mat& mat);
void fill(Mat& mat, QRgb color);
void resize(const Mat& mat, Mat& out, double fact);
void prepareSize(const Mat& a, const Mat& b, Mat& c, Mat& d);
void segmentation(Mat& in);
void keying(const Mat& fg, const Mat& bg, Mat& out, QRgb color, int hue, int saturation, int value, bool segm);

#endif // IMAGE_H
