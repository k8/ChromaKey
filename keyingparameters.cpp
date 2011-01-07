#include "keyingparameters.h"

KeyingParameters::KeyingParameters(KeyingAlgorithm keyingAlgorithm, QRgb color, int hue, int saturation, int value, int luminance, int blue, int red, int alpha, bool segmentation, int white, int black, bool matteVisible)
    :
    keyingAlgorithm(keyingAlgorithm),
    color(color),
    hue(hue),
    saturation(saturation),
    value(value),
    luminance(luminance),
    blue(blue),
    red(red),
    alpha(alpha),
    segmentation(segmentation),
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

void KeyingParameters::setLuminance(int l)
{
    QMutexLocker locker(&mutex);
    luminance = l;
    emit parameterChanged();
}

void KeyingParameters::setBlue(int b)
{
    QMutexLocker locker(&mutex);
    blue = b;
    emit parameterChanged();
}

void KeyingParameters::setRed(int r)
{
    QMutexLocker locker(&mutex);
    red = r;
    emit parameterChanged();
}

void KeyingParameters::setAlpha(int a)
{
    QMutexLocker locker(&mutex);
    alpha = a;
    emit parameterChanged();
}

void KeyingParameters::setSegmentaion(bool s)
{
    QMutexLocker locker(&mutex);
    segmentation = s;
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
