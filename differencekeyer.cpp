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
    int f, s, t;
    setIndexesDespill(f, s, t);

    for (int i=0; i<fg.rows; i++)
    {
        for (int j=0; j<fg.cols; j++)
        {
            Vec3b& elem = fg.at<Vec3b>(i,j);
            if (elem[f] > elem[s])
            {
                elem[f] = elem[s];
            }
            if (elem[t] > elem[s])
            {
                elem[t] = elem[s];
            }
        }
    }
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
        b = 0;
        c = 1;
        break;
    case KeyingParameters::C_GREEN:
        a = 1;
        b = 0;
        c = 2;
        break;
    case KeyingParameters::C_BLUE:
        a = 0;
        b = 2;
        c = 1;
        break;
    case KeyingParameters::C_CYAN_BLUE:
        a = 0;
        b = 2;
        c = 2;
        break;
    case KeyingParameters::C_CYAN_GREEN:
        a = 1;
        b = 2;
        c = 2;
        break;
    case KeyingParameters::C_MAGENTA_BLUE:
        a = 0;
        b = 1;
        c = 1;
        break;
    case KeyingParameters::C_MAGENTA_RED:
        a = 2;
        b = 1;
        c = 1;
        break;
    case KeyingParameters::C_YELLOW_RED:
        a = 2;
        b = 0;
        c = 0;
        break;
    case KeyingParameters::C_YELLOW_GREEN:
        a = 1;
        b = 0;
        c = 0;
        break;
    }
}

void DifferenceKeyer::setIndexesDespill(int& a, int& b, int& c)
{
    KeyingParameters::ColorName firstColor = kp->getFirstColor();
    KeyingParameters::ColorName secondColor = kp->getSecondColor();

    switch (firstColor)
    {
    case KeyingParameters::C_RED:
        a = 2;
        b = 0;
        c = 2;
        break;
    case KeyingParameters::C_GREEN:
        a = 1;
        b = 2;
        c = 1;
        break;
    case KeyingParameters::C_BLUE:
        a = 0;
        b = 2;
        c = 0;
        break;
    case KeyingParameters::C_CYAN_BLUE:
        a = 0;
        b = 2;
        c = 1;
        break;
    case KeyingParameters::C_CYAN_GREEN:
        a = 1;
        b = 2;
        c = 0;
        break;
    case KeyingParameters::C_MAGENTA_BLUE:
        a = 0;
        b = 1;
        c = 2;
        break;
    case KeyingParameters::C_MAGENTA_RED:
        a = 2;
        b = 1;
        c = 0;
        break;
    case KeyingParameters::C_YELLOW_RED:
        a = 2;
        b = 0;
        c = 1;
        break;
    case KeyingParameters::C_YELLOW_GREEN:
        a = 1;
        b = 0;
        c = 2;
        break;
    }
}
