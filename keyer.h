#ifndef KEYER_H
#define KEYER_H

#include <opencv/cv.h>

#include "matte.h"
#include "keyingparameters.h"

class Keyer
{
public:
    Keyer();
    void keying(KeyingParameters* kp, const Mat &a, const Mat &b, Mat &out);
    virtual ~Keyer();

protected:
    void prepareSize(const Mat& a, const Mat& b);
    void resize(const Mat &mat, Mat &out, double fact);
    virtual void computeMatte() {}
    virtual void prepareOutput(Mat& out);

protected:
    KeyingParameters* kp;
    Matte* matte;
    Mat fg;
    Mat bg;
};

#endif // KEYER_H
