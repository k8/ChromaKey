#include "imagesprocessor.h"
#include <QDebug>
#include <QColor>
#include <QMutexLocker>
#include <cmath>

ImagesProcessor::ImagesProcessor(KeyingParameters *keyingParams, QSize size)
    : kp(keyingParams), imageSize(size)
{
}

QImage ImagesProcessor::scaledFromCvMat(const Mat& inMat)
{
    Mat mat;
    QSize size = getSize();
    double scaleFactor = min(size.width()/(double)inMat.size().width, size.height()/(double)inMat.size().height);
    resize(inMat, mat, scaleFactor);
    return fromCvMat(mat);
}

QImage ImagesProcessor::fromCvMat(const Mat& mat)
{
    if(mat.channels()==1)
    {
        Mat m;
        cvtColor(mat, m, CV_GRAY2RGB);
        QImage img(m.data, m.size().width, m.size().height, m.step, QImage::Format_RGB888);
        return img.copy(0,0,img.width(),img.height());
    }
    else
    {
        QImage img(mat.data, mat.size().width, mat.size().height, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
}

void ImagesProcessor::fill(Mat &image, QRgb color)
{
    for (int i=0; i<image.rows; i++)
    {
      for (int j=0; j<image.cols; j++)
      {
          Vec3b& elem = image.at<Vec3b>(i,j);
          elem[0] = qBlue(color);
          elem[1] = qGreen(color);
          elem[2] = qRed(color);
      }
  }
}

void ImagesProcessor::resize(const Mat &mat, Mat &out, double fact)
{
    if (fact == 1)
    {
        out = mat;
        return ;
    }
    int width = mat.cols*fact;
    int height = mat.rows*fact;
    cv::resize(mat, out, Size(width, height));
}

void ImagesProcessor::prepareSize(const Mat &a, const Mat &b, Mat &aout, Mat &bout)
{
    b.copyTo(bout);
    if (a.cols <= b.cols && a.rows <= b.rows)
    {
        double widthFact = (double)a.cols/(b.cols);
        double heightFact = (double)a.rows/(b.rows);
        resize(b, bout, max(widthFact, heightFact));
    }
    Mat a2;
    a.copyTo(a2);
    if (a.cols > bout.cols)
    {
        resize(a, a2, (double)bout.cols/a.cols);
    }
    a2.copyTo(aout);
    if (a2.rows > bout.rows)
    {
        resize(a2, aout, (double)bout.rows/a2.rows);
    }
}

void ImagesProcessor::segmentation(Mat &in)
{
    Mat image;
    cvtColor(in, image, CV_BGR2HSV);
    for (int i=0; i<image.rows; i++)
    {
      for (int j=0; j<image.cols; j++)
      {
          Vec3b& hsvElem = image.at<Vec3b>(i,j);
          hsvElem[0] = hsvElem[0]/10*10;
          hsvElem[1] = hsvElem[1]/80*80;
          hsvElem[2] = hsvElem[1]/80*80;
      }
    }
    cvtColor(image, in, CV_HSV2BGR);
}

void ImagesProcessor::keying(const Mat &fg, const Mat &bg, Mat &out)
{
    switch (kp->getKeyingAlgorithm())
    {
    case KeyingParameters::KA_HSV:
        keyingHSV(fg, bg, out);
        break;
    case KeyingParameters::KA_YCbCr:
        keyingYCbCr(fg, bg, out);
        break;
    }
}

void ImagesProcessor::keyingHSV(const Mat &fg, const Mat &bg, Mat &out)
{    
    Mat a;
    Mat b;
    prepareSize(fg, bg, a, b);
    Mat image = a;
    if (kp->getSegmentation())
        segmentation(image);
    Mat mask = Mat::ones(image.rows, image.cols, CV_8UC1);
    QColor col = kp->getColor();
    int h = col.hue();
    int s = col.saturation();
    int v = col.value();
    Mat imgHSV;
    cvtColor(image, imgHSV, CV_BGR2HSV);
    int hue = kp->getHue();
    int saturation = kp->getSaturation();
    int value = kp->getValue();
    for (int i=0; i<image.rows; i++)
    {
        for (int j=0; j<image.cols; j++)
        {
          {                          
            Vec3b& elem = imgHSV.at<Vec3b>(i,j);
            if ((abs(elem[0]*2-h) < hue &&
                 abs(elem[1]-s) < saturation &&
                 abs(elem[2]-v) < value))
            {
              mask.at<uchar>(i,j) = 0;
            }
          }
        }
    }
    b.copyTo(out);
    Rect r(out.cols-image.cols, out.rows-image.rows, image.cols, image.rows);
    Mat roi(out, r);
    image.copyTo(roi, mask);
}

double keyValue(int in, int fix, int tol)
{
    double k = 0;
    int H = fix+tol;
    int L = fix-tol;
    if (in > H)
        k = 1;
    else if (in < L)
        k = 0;
    else
        k = (in-L)/(double)(H-L);
    return k;
}

double abs(double x)
{
    if (x < 0)
        x = -x;
    return x;
}

void ImagesProcessor::keyingYCbCr(const Mat &fg, const Mat &bg, Mat &out)
{
    double alpha = (kp->getAlpha()/180.0)*M_PI;
    Mat a;
    Mat b;
    prepareSize(fg, bg, a, b);
    Mat image;
    a.copyTo(image);
    if (kp->getSegmentation())
        segmentation(image);
    Color col(kp->getColor().rgb());
    Mat fgYCrCb;
    cvtColor(image, fgYCrCb, CV_BGR2YCrCb);
    b.copyTo(out);
    Rect r(out.cols-image.cols, out.rows-image.rows, image.cols, image.rows);
    Mat roi(out, r);
    Mat bgYCrCb;
    cvtColor(roi, bgYCrCb, CV_BGR2YCrCb);

    int y = col.Y();
    int cr = col.Cr();
    int cb = col.Cb();

    YCbCrCordinate key_cord(cb, cr);
    double sin_tau = key_cord.sin();
    double cos_tau = key_cord.cos();

    double tan_alpha = tan(alpha/2.0);
    if (tan_alpha == 0)
        tan_alpha = 0.000001;

    double min =  0;

    int count = 0;
    for (int i=0; i<image.rows; i++)
    {
        for (int j=0; j<image.cols; j++)
        {
            Vec3b& elem = fgYCrCb.at<Vec3b>(i,j);
            Vec3b& bgElem = bgYCrCb.at<Vec3b>(i,j);
            int ey = elem[0];
            int ecr = elem[1];
            int ecb = elem[2];

            double ecb_norm = YCbCrCordinate::normalized(ecb);
            double ecr_norm = YCbCrCordinate::normalized(ecr);

            double X = ecb_norm*cos_tau+ecr_norm*sin_tau;
            double Z = ecr_norm*cos_tau-ecb_norm*sin_tau;

            double K_FG = 0;
            min =  abs(Z)/tan_alpha;
            if (X > min)
            {
                K_FG = X-min;
            }
            double CB_FG = ecb-K_FG*cos_tau;
            double CR_FG = ecr-K_FG*sin_tau;

            if (K_FG > 0)
            {                
                bgElem = elem;                
                bgElem[0] = bgElem[0]*(1-K_FG)+ey*K_FG;
                bgElem[1] = CR_FG;
                bgElem[2] = CB_FG;
                double Y_norm = YCbCrCordinate::normalized(ey);
//                bgElem[0] = Y_norm-kp->getLuminance()*K_FG;
//                bgElem[1] = bgElem[1]*(1-Cr_FG)+ecr*Cr_FG;
//                bgElem[2] = bgElem[2]*(1-Cb_FG)+ecb*Cb_FG;
                if (i == j)
                    qDebug() << X << min << K_FG << CR_FG << CB_FG;
                count++;
            }
            else
            {
            }
        }
    }   
    cvtColor(bgYCrCb, roi, CV_YCrCb2BGR);
}

void ImagesProcessor::setSize(QSize size)
{
    QMutexLocker locker(&mutex);
    imageSize = size;
}

QSize ImagesProcessor::getSize()
{
    QMutexLocker locker(&mutex);
    return imageSize;
}
