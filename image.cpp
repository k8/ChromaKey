#include "image.h"
#include <QDebug>
#include <QColor>

QImage fromCvMat(const Mat& mat)
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

void segmentation(Mat &in)
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

void keying(const Mat &fg, const Mat &bg, Mat& out, QRgb color, int hue, int saturation, int value, bool segm)
{    
    double scaleFact = (double)bg.cols/fg.cols;
    int width = fg.cols*scaleFact;
    int height = fg.rows*scaleFact;
    Mat image;
    resize(fg, image, Size(width, height));
    if (segm)
        segmentation(image);
    Mat mask = Mat::ones(image.rows, image.cols, CV_8UC1);
    QColor col(color);
    col.toHsv();
    int h = col.hue();
    int s = col.saturation();
    int v = col.value();
    Mat imgHSV;
    cvtColor(image, imgHSV, CV_BGR2HSV);
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
    bg.copyTo(out);
    Mat roi(out, Rect(out.cols-image.cols, out.rows-image.rows, image.cols, image.rows));
    image.copyTo(roi, mask);
}
