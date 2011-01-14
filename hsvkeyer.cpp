#include "hsvkeyer.h"

HSVKeyer::HSVKeyer()
{
}

void HSVKeyer::computeMatte()
{
    QColor col = kp->getColor();
    int h = col.hue();
    int s = col.saturation();
    int v = col.value();
    Mat imgHSV;
    cvtColor(fg, imgHSV, CV_BGR2HSV);
    int hue = kp->getHue();
    int saturation = kp->getSaturation();
    int value = kp->getValue();
    for (int i=0; i<imgHSV.rows; i++)
    {
        for (int j=0; j<imgHSV.cols; j++)
        {
          {
            Vec3b& elem = imgHSV.at<Vec3b>(i,j);
            if ((abs(elem[0]*2-h) < hue &&
                 abs(elem[1]-s) < saturation &&
                 abs(elem[2]-v) < value))
            {
                matte->setValue(i,j, 0);
            }
            else
            {
                matte->setValue(i,j, 255);
            }
          }
        }
    }
    matte->invert();
}
