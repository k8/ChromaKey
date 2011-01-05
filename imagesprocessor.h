#ifndef IMAGESPROCESSOR_H
#define IMAGESPROCESSOR_H

#include "keyingparameters.h"
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

    ImagesProcessor(KeyingParameters* keyingParams = 0, QSize size = QSize(100, 100));
    static QImage fromCvMat(const Mat& mat);
    QImage scaledFromCvMat(const Mat& mat);
    static void fill(Mat& mat, QRgb color);
    void keying(const Mat& fg, const Mat& bg, Mat& out);
    void setSize(QSize size);
    QSize getSize();

protected:
    void keyingHSV(const Mat& fg, const Mat& bg, Mat& out);
    void keyingYCbCr(const Mat& fg, const Mat& bg, Mat& out);
    void keyingDM(const Mat& fg, const Mat& bg, Mat& out);
    void resize(const Mat& mat, Mat& out, double fact);
    void prepareSize(const Mat& a, const Mat& b, Mat& c, Mat& d);
    void segmentation(Mat& in);

private:
    QMutex mutex;
    KeyingParameters* kp;
    QSize imageSize;    

    Mat tmp; // temporary - for difference matte

    class Color : public QColor
    {
    public:
        Color() : y(0), cr(0), cb(0), ycrcb(false), RGB(true)
        {
        }
        Color(int Y, int Cr, int Cb) : y(Y), cr(Cr), cb(Cb), ycrcb(true), RGB(false) {}
        Color(const QRgb& c) : QColor(c), y(0), cr(0), cb(0), ycrcb(false), RGB(true) {}
        Color toRgb() {prepareRGB(); return *this;}
        int Y() {prepareYCrCb(); return y;}
        int Cr() {prepareYCrCb(); return cr;}
        int Cb() {prepareYCrCb(); return cb;}
        int R() {prepareRGB(); return red();}
        int G() {prepareRGB(); return green();}
        int B() {prepareRGB(); return blue();}


        void convertToYCrCb()
        {
            Mat rgb(1, 1, CV_8UC3);
            Vec3b& elem = rgb.at<Vec3b>(0,0);
            elem[0] = blue();
            elem[1] = green();
            elem[2] = red();
            Mat yrb(1, 1, CV_8UC3);
            cvtColor(rgb, yrb, CV_BGR2YCrCb);
            elem = yrb.at<Vec3b>(0,0);
            y = elem[0];
            cr = elem[1];
            cb = elem[2];
        }
        void convertToRGB()
        {
            Mat yrb(1, 1, CV_8UC3);
            Vec3b& elem = yrb.at<Vec3b>(0,0);
            elem[0] = y;
            elem[1] = cr;
            elem[2] = cb;
            Mat rgb;
            cvtColor(yrb, rgb, CV_YCrCb2BGR);
            elem = rgb.at<Vec3b>(0,0);
            int b = elem[0];
            int g = elem[1];
            int r = elem[2];
            setRgb(qRgb(r, g, b));
        }
        void prepareYCrCb()
        {
            if (! ycrcb)
            {
                convertToYCrCb();
                ycrcb = true;
                RGB = false;
            }
        }
        void prepareRGB()
        {
            if (! RGB)
            {
                convertToRGB();
                RGB = true;
                ycrcb = false;
            }
        }

        int y;
        int cr;
        int cb;

        bool ycrcb;
        bool RGB;
        static const int delta = 128;
    };

    class YCbCrCordinate
    {
    public:
        YCbCrCordinate(int cb, int cr)
        {
            b = normalized(cb);
            r = normalized(cr);
            diag = sqrt(b*b+r*r);
        }
        double sin() {return -r/diag;}
        double cos() {return b/diag;}
        static double normalized(int value)
        {
            return ((value)/127.5)-1;
        }
    private:
        double b;
        double r;
        double diag;
    };
};

#endif // IMAGESPROCESSOR_H
