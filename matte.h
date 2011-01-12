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

protected:
    Mat mat;
};

class DifferenceMatte : public Matte
{        
public:
    DifferenceMatte(Size size);
    void compute(const Mat& in, QColor color);

private:    
    class Color
    {
    public:
        Color(int value, int index): value(value), index(index) {}
        int value;
        int index;
    };
    friend bool operator <(const Color& a, const Color& b)
    {
        return a.value > b.value;
    }

    void setIndexes(QColor color, int& a, int& b, int& c);
};

#endif // MATTE_H
