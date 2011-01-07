#include "matte.h"
#include <QDebug>
#include <opencv/highgui.h>

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
    for (int i = 0; i < mat.rows; i++)
    {
        for (int j = 0; j < mat.cols; j++)
        {
            double fac = mat.at<uchar>(i,j)/255.0;
            out.at<Vec3b>(i,j)[0] = a.at<Vec3b>(i,j)[0]*fac+b.at<Vec3b>(i,j)[0]*(1-fac);
            out.at<Vec3b>(i,j)[1] = a.at<Vec3b>(i,j)[1]*fac+b.at<Vec3b>(i,j)[1]*(1-fac);
            out.at<Vec3b>(i,j)[2] = a.at<Vec3b>(i,j)[2]*fac+b.at<Vec3b>(i,j)[2]*(1-fac);
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

void DifferenceMatte::compute(const Mat &a)
{
    for (int i=0; i<a.rows; i++)
    {
        for (int j=0; j<a.cols; j++)
        {
            Vec3b elem = a.at<Vec3b>(i,j);
            int value = elem[1]-max(elem[2], elem[2]);
            if (value < 0) value = 0;
            mat.at<uchar>(i, j) = value;
        }
    }
}
