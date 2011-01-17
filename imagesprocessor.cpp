#include "imagesprocessor.h"
#include <QDebug>
#include <QColor>
#include <QMutexLocker>
#include <QMap>
#include <cmath>

#include "matte.h"
#include "keyerfactory.h"

ImagesProcessor::ImagesProcessor(QSize size)
    : imageSize(size)
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

void ImagesProcessor::to3Chanels(const Mat &mat, Mat &out)
{
    if (mat.type() != 0)
        return;
    out.create(mat.size(), CV_8UC3);
    for (int i=0; i<out.rows; i++)
    {
      for (int j=0; j<out.cols; j++)
      {
          Vec3b& elem = out.at<Vec3b>(i,j);
          int v = mat.at<uchar>(i,j);
          elem[0] = v;
          elem[1] = v;
          elem[2] = v;
      }
  }
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
