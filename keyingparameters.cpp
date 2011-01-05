#include "keyingparameters.h"

KeyingParameters::KeyingParameters(KeyingAlgorithm keyingAlgorithm, QRgb color, int hue, int saturation, int value, int luminance, int blue, int red, int alpha, bool segmentation, int threshold)
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
    threshold(threshold)
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

void KeyingParameters::setThreshold(int t)
{
    QMutexLocker locker(&mutex);
    threshold = t;
    emit parameterChanged();
}
