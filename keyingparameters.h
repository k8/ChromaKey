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
        C_BLUE,
        C_MAX
    };

    KeyingParameters(KeyingAlgorithm keyingAlgorithm,
                     QRgb color,
                     int hue,
                     int saturation,
                     int value,
                     int alpha,
                     int white,
                     int black,
                     int ys,
                     bool matteVisible);
    KeyingParameters(KeyingParameters& other);
    KeyingParameters(KeyingParameters* other);

    KeyingAlgorithm getKeyingAlgorithm() {QMutexLocker locker(&mutex); return keyingAlgorithm; }
    QColor getColor() {QMutexLocker locker(&mutex); return QColor(color); }
    int getHue() {QMutexLocker locker(&mutex); return hue; }
    int getSaturation() {QMutexLocker locker(&mutex); return saturation; }
    int getValue() {QMutexLocker locker(&mutex); return value; }
    int getAlpha() {QMutexLocker locker(&mutex); return alpha;}
    int getWhite() {QMutexLocker locker(&mutex); return white; }
    int getBlack() {QMutexLocker locker(&mutex); return black; }
    int getYs() {QMutexLocker locker(&mutex); return ys; }
    bool isMatteVisible() {QMutexLocker locker(&mutex); return matteVisible; }
    ColorName getFirstColor() {QMutexLocker locker(&mutex); return firstColor; }
    ColorName getSecondColor() {QMutexLocker locker(&mutex); return secondColor; }

public slots:
    void setKeyingAlgorithm(KeyingAlgorithm ka);
    void setColor(QRgb c);
    void setHue(int h);
    void setSaturation(int s);
    void setValue(int v);
    void setAlpha(int a);
    void setWhite(int w);
    void setBlack(int b);
    void setYs(int ys);
    void setMatteVisible(bool v);
    void setFirstColor(ColorName color);
    void setSecondColor(ColorName color);
    void setColors(QColor color);

signals:
    void parameterChanged();

private:
    class Color
    {
    public:
        Color(int value, ColorName index): value(value), index(index) {}
        int value;
        ColorName index;
    };
    friend bool operator <(const Color& a, const Color& b)
    {
        return a.value > b.value;
    }

    KeyingAlgorithm keyingAlgorithm;
    QRgb color;
    int hue;
    int saturation;
    int value;
    int alpha;
    int white;
    int black;
    int ys;
    bool matteVisible;
    ColorName firstColor;
    ColorName secondColor;
    QMutex mutex;
};

#endif // KEYINGPARAMETERS_H
