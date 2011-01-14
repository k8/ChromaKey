#include "keyingparameters.h"

#include <QDebug>
#include <QList>

KeyingParameters::KeyingParameters(KeyingAlgorithm keyingAlgorithm, QRgb color, int hue, int saturation, int value, int alpha, int white, int black, bool matteVisible)
    :
    keyingAlgorithm(keyingAlgorithm),
    color(color),
    hue(hue),
    saturation(saturation),
    value(value),
    alpha(alpha),
    white(white),
    black(black),
    matteVisible(matteVisible)
{
}

void KeyingParameters::setKeyingAlgorithm(KeyingAlgorithm ka)
{
    QMutexLocker locker(&mutex);
    keyingAlgorithm = ka;
    emit parameterChanged();
}

void KeyingParameters::setColor(QRgb c)
{
    QMutexLocker locker(&mutex);
    color = c;
    setColors(c);
    emit parameterChanged();
}

void KeyingParameters::setHue(int h)
{
    QMutexLocker locker(&mutex);
    hue = h;
    emit parameterChanged();
}

void KeyingParameters::setSaturation(int s)
{
    QMutexLocker locker(&mutex);
    saturation = s;
    emit parameterChanged();
}

void KeyingParameters::setValue(int v)
{
    QMutexLocker locker(&mutex);
    value = v;
    emit parameterChanged();
}

void KeyingParameters::setAlpha(int a)
{
    QMutexLocker locker(&mutex);
    alpha = a;
    emit parameterChanged();
}

void KeyingParameters::setWhite(int w)
{
    QMutexLocker locker(&mutex);
    white = w;
    emit parameterChanged();
}

void KeyingParameters::setBlack(int b)
{
    QMutexLocker locker(&mutex);
    black = b;
    emit parameterChanged();
}

void KeyingParameters::setMatteVisible(bool v)
{
    QMutexLocker locker(&mutex);
    matteVisible = v;
    emit parameterChanged();
}

void KeyingParameters::setFirstColor(ColorName color)
{
    QMutexLocker locker(&mutex);
    firstColor = color;
    emit parameterChanged();
}

void KeyingParameters::setSecondColor(ColorName color)
{
    QMutexLocker locker(&mutex);
    secondColor = color;
    emit parameterChanged();
}

void KeyingParameters::setColors(QColor color)
{
    QList<Color> list;
    list.append(Color(color.red(), C_RED));
    list.append(Color(color.green(), C_GREEN));
    list.append(Color(color.blue(), C_BLUE));
    qSort(list);
    firstColor = list.at(0).index;
    secondColor = C_MAX;
    qDebug() << color.red() << color.green() << color.blue();
    qDebug() << list.at(0).value << list.at(1).value << list.at(2).value;
    if (abs(list.at(0).value-list.at(1).value) < 20)
        secondColor = list.at(2).index;
}
