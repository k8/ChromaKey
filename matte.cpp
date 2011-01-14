#include "matte.h"
#include <QDebug>
#include <opencv/highgui.h>
#include <QList>

Matte::Matte(Size size)
{
    mat.create(size, CV_8UC1);
}

void Matte::invert()
{
    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
            mat.at<uchar>(i,j) = 255-mat.at<uchar>(i,j);
        }
    }
}

void Matte::multiply(const Mat &a, const Mat &b, Mat &out)
{
    out = b;
    int x = b.rows-a.rows;
    int y = b.cols-a.cols;
    for (int i = x; i < b.rows; i++)
    {
        for (int j = y; j < b.cols; j++)
        {
            double fac = mat.at<uchar>(i-x,j-y)/255.0;
            out.at<Vec3b>(i,j)[0] = a.at<Vec3b>(i-x,j-y)[0]*fac+b.at<Vec3b>(i,j)[0]*(1-fac);
            out.at<Vec3b>(i,j)[1] = a.at<Vec3b>(i-x,j-y)[1]*fac+b.at<Vec3b>(i,j)[1]*(1-fac);
            out.at<Vec3b>(i,j)[2] = a.at<Vec3b>(i-x,j-y)[2]*fac+b.at<Vec3b>(i,j)[2]*(1-fac);
        }
    }
}

void Matte::scale(int white, int black)
{
    mat *= 256.0/white;
    mat -= black;
    mat *= 256.0/black;
}

DifferenceMatte::DifferenceMatte(Size size)
    : Matte(size)
{

}

void DifferenceMatte::compute(const Mat &img, int a, int b, int c)
{
    for (int i=0; i<img.rows; i++)
    {
        for (int j=0; j<img.cols; j++)
        {
            Vec3b elem = img.at<Vec3b>(i,j);
            int value = elem[a]-max(elem[b], elem[c]);
            if (value < 0) value = 0;
            mat.at<uchar>(i, j) = value;
        }
    }
}
