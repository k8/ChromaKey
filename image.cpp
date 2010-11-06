#include "image.h"
#include <QDebug>

Image::Image()
{
}

QImage fromCvMat(const cv::Mat& mat)
{
    if(mat.channels()==1)
    {
        cv::Mat m;
        cv::cvtColor(mat, m, CV_GRAY2RGB);
        QImage img(m.data, m.size().width, m.size().height, m.step, QImage::Format_RGB888);
        return img.copy(0,0,img.width(),img.height());
    }
    else
    {
        QImage img(mat.data, mat.size().width, mat.size().height, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
}

void keying(Mat& fg, Mat& bg, int eps, int eps2, QRgb color, QRgb darkColor)
{
  int b = 165;
  int g = 170;
  int r = 43;

//  int miniB = 136;
//  int miniG = 141;
//  int miniR = 23;

  int miniB = qBlue(color);
  int miniG = qGreen(color);
  int miniR = qRed(color);

//  int maxiB = 182;
//  int maxiG = 188;
//  int maxiR = 61;

  int maxiB = qBlue(darkColor);
  int maxiG = qGreen(darkColor);
  int maxiR = qRed(darkColor);

  double scaleFact = (double)bg.cols/fg.cols;
  int width = fg.cols*scaleFact;
  int height = fg.rows*scaleFact;
  Mat image;
  resize(fg, image, Size(width, height));
  Mat mask = Mat::ones(image.rows, image.cols, CV_8UC1);
  for (int i=0; i<image.rows; i++)
  {
    for (int j=0; j<image.cols; j++)
    {
      {
        Vec3b& elem = image.at<Vec3b>(i,j);
//	int alpha = elem[0]-elem[1]/2.0-elem[2]/2.0;
//	if (alpha < 0)
//        if (!(abs(elem[0]-b) < eps && abs(elem[1]-g) < eps && abs(elem[2]-r) < eps))
        if ((abs(elem[0]-miniB) < eps && abs(elem[1]-miniG) < eps && abs(elem[2]-miniR) < eps) || (abs(elem[0]-maxiB) < eps2 && abs(elem[1]-maxiG) < eps2 && abs(elem[2]-maxiR) < eps2))
        {
          mask.at<uchar>(i,j) = 0;
        }
      }
    }
  }
  Mat roi(bg, Rect(bg.cols-image.cols, bg.rows-image.rows, image.cols, image.rows));
  image.copyTo(roi, mask);
}
