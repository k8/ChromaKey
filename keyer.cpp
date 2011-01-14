#include "keyer.h"

#include "imagesprocessor.h"

Keyer::Keyer()
    : kp(0), matte(0)
{
}

void Keyer::keying(KeyingParameters* kp, const Mat& a, const Mat &b, Mat &out)
{
    this->kp = kp;
    prepareSize(a, b);
    matte = new Matte(fg.size());
    computeMatte();
    prepareOutput(out);
}

Keyer::~Keyer()
{
    delete matte;
}

void Keyer::prepareSize(const Mat &a, const Mat &b)
{
    b.copyTo(bg);
    if (a.cols <= b.cols && a.rows <= b.rows)
    {
        double widthFact = (double)a.cols/(b.cols);
        double heightFact = (double)a.rows/(b.rows);
        ImagesProcessor::resize(b, bg, max(widthFact, heightFact));
    }
    Mat a2;
    a.copyTo(a2);
    if (a.cols > bg.cols)
    {
        ImagesProcessor::resize(a, a2, (double)bg.cols/a.cols);
    }
    a2.copyTo(fg);
    if (a2.rows > bg.rows)
    {
        ImagesProcessor::resize(a2, fg, (double)bg.rows/a2.rows);
    }
}

void Keyer::prepareOutput(Mat &out)
{
    if (kp->isMatteVisible())
    {
        out = matte->getMat();
    }
    else
    {
        matte->invert();
        matte->multiply(fg, bg, out);
    }
}
