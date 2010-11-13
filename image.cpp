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

void fill(Mat &image, QRgb color)
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

void resize(const Mat &mat, Mat &out, double fact)
{
    if (fact == 1)
        return;
    int width = mat.cols*fact;
    int height = mat.rows*fact;
    resize(mat, out, Size(width, height));
}

void prepareSize(const Mat &a, const Mat &b, Mat &aout, Mat &bout)
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
    Mat a;
    Mat b;
    prepareSize(fg, bg, a, b);
    Mat image;
    a.copyTo(image);
    if (segm)
        segmentation(image);
    Mat blur;
    GaussianBlur(image, blur, Size(13, 13), 1.5, 1.5);
    Mat mask = Mat::ones(image.rows, image.cols, CV_8UC1);
    blur = image-blur;
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
    b.copyTo(out);
    Rect r(out.cols-image.cols, out.rows-image.rows, image.cols, image.rows);
    Mat roi(out, r);
    image.copyTo(roi, mask);
}
