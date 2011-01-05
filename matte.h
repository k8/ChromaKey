#ifndef MATTE_H
#define MATTE_H

#include <opencv/cv.h>
using namespace cv;

class Matte
{
public:
    Matte(Size size);
    void invert();
    void multiply(const Mat& a, const Mat& b, Mat& out);
protected:
    Mat mat;
};

class DifferenceMatte : public Matte
{
public:
    DifferenceMatte(Size size);
    void compute(const Mat& in);
};

#endif // MATTE_H
