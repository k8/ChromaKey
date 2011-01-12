#ifndef KEYINGPARAMETERS_H
#define KEYINGPARAMETERS_H

#include <QObject>
#include <QColor>
#include <QMutexLocker>
#include <QMutex>

class KeyingParameters : public QObject
{
    Q_OBJECT

public:
    enum KeyingAlgorithm
    {
        KA_HSV,
        KA_DM,
        KA_YCbCr,
    };
    enum ColorName
    {
        C_RED,
        C_GREEN,
        c_BLUE
    };

    KeyingParameters(KeyingAlgorithm keyingAlgorithm,
                     QRgb color,
                     int hue,
                     int saturation,
                     int value,
                     int luminance,
                     int blue,
                     int red,
                     int alpha,
                     bool segmentation,
                     int white,
                     int black,
                     bool matteVisible);
    KeyingAlgorithm getKeyingAlgorithm() {QMutexLocker locker(&mutex); return keyingAlgorithm; }
    QColor getColor() {QMutexLocker locker(&mutex); return QColor(color); }
    int getHue() {QMutexLocker locker(&mutex); return hue; }
    int getSaturation() {QMutexLocker locker(&mutex); return saturation; }
    int getValue() {QMutexLocker locker(&mutex); return value; }
    int getLuminance() {QMutexLocker locker(&mutex); return luminance; }
    int getBlue() {QMutexLocker locker(&mutex); return blue; }
    int getRed() {QMutexLocker locker(&mutex); return red; }
    int getAlpha() {QMutexLocker locker(&mutex); return alpha;}
    bool getSegmentation() {QMutexLocker locker(&mutex); return segmentation; }
    int getWhite() {QMutexLocker locker(&mutex); return white; }
    int getBlack() {QMutexLocker locker(&mutex); return black; }
    bool getMatteVisible() {QMutexLocker locker(&mutex); return matteVisible; }

public slots:
    void setKeyingAlgorithm(KeyingAlgorithm ka);
    void setColor(QRgb c);
    void setHue(int h);
    void setSaturation(int s);
    void setValue(int v);
    void setLuminance(int l);
    void setBlue(int b);
    void setRed(int r);
    void setAlpha(int a);
    void setSegmentaion(bool s);
    void setWhite(int w);
    void setBlack(int b);
    void setMatteVisible(bool v);

signals:
    void parameterChanged();

private:
    KeyingAlgorithm keyingAlgorithm;
    QRgb color;
    int hue;
    int saturation;
    int value;
    int luminance;
    int blue;
    int red;
    int alpha;
    bool segmentation;
    int white;
    int black;
    bool matteVisible;

    QMutex mutex;
};

#endif // KEYINGPARAMETERS_H
