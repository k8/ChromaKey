#include "differencekeyer.h"

#include <QDebug>
#include <QMap>

DifferenceKeyer::DifferenceKeyer()
{
}

void DifferenceKeyer::computeMatte()
{
    int f, s, t;
    setIndexes(f, s, t);
    for (int i=0; i<fg.rows; i++)
    {
        for (int j=0; j<fg.cols; j++)
        {
            Vec3b elem = fg.at<Vec3b>(i,j);
            int value = elem[f]-max(elem[s], elem[t]);
            if (value < 0) value = 0;
            matte->setValue(i, j, value);
        }
    }
    matte->scale(kp->getWhite(), kp->getBlack());
    if (kp->getDespill())
    {
        despill();
    }
}

void DifferenceKeyer::despill()
{
    for (int i=0; i<fg.rows; i++)
    {
        for (int j=0; j<fg.cols; j++)
        {
            Vec3b& elem = fg.at<Vec3b>(i,j);
            if (elem[1] > elem[2])
            {
                elem[1] = elem[2];
            }
            if (elem[0] > elem[2])
            {
                elem[0] = elem[2];
            }
        }
    }
    double n = fg.rows*fg.cols;
}

void DifferenceKeyer::setIndexes(int& a, int& b, int& c)
{
    QMap<KeyingParameters::ColorName, int> indexes;
    indexes[KeyingParameters::C_BLUE] = 0;
    indexes[KeyingParameters::C_GREEN] = 1;
    indexes[KeyingParameters::C_RED] = 2;

    KeyingParameters::ColorName firstColor = kp->getFirstColor();
    KeyingParameters::ColorName secondColor = kp->getSecondColor();

    switch (firstColor)
    {
    case KeyingParameters::C_RED:
        a = 2;
        if (secondColor == KeyingParameters::C_MAX) { b = 0; c = 1; }
        else {b = indexes[secondColor]; c = b; }
        break;
    case KeyingParameters::C_GREEN:
        a = 1;
        if (secondColor == KeyingParameters::C_MAX) { b = 2; c = 0; }
        else {b = indexes[secondColor]; c = b; }
        break;
    case KeyingParameters::C_BLUE:
        a = 0;
        if (secondColor == KeyingParameters::C_MAX) { b = 1; c = 2; }
        else {b = indexes[secondColor]; c = b; }
        break;
    }
}
