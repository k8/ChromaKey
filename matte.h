#ifndef MATTE_H
#define MATTE_H

#include <opencv/cv.h>
#include <QColor>
using namespace cv;

class Matte
{
public:
    Matte(Size size);
    void invert();
    void multiply(const Mat& a, const Mat& b, Mat& out);
    void scale(int white, int black);
    const Mat& getMat() {return mat;}
    uchar value(int i, int j) {return mat.at<uchar>(i,j);} // i == row number, j == col number
    void setValue(int i, int j, uchar value) {mat.at<uchar>(i,j) = value;} // i == row number, j == col number

protected:
    Mat mat;
};

class DifferenceMatte : public Matte
{        
public:
    DifferenceMatte(Size size);
    void compute(const Mat& in, int a, int b, int c);
};

#endif // MATTE_H
