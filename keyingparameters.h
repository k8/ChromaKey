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
        KA_DM
    };
    enum ColorName
    {
        C_RED,
        C_GREEN,
        C_BLUE,
        C_CYAN_GREEN,
        C_CYAN_BLUE,
        C_MAGENTA_RED,
        C_MAGENTA_BLUE,
        C_YELLOW_RED,
        C_YELLOW_GREEN
    };

    KeyingParameters(KeyingAlgorithm keyingAlgorithm,
                     QRgb color,
                     int hue,
                     int saturation,
                     int value,
                     int white,
                     int black,
                     bool matteVisible,
                     bool despill);
    KeyingParameters(KeyingParameters& other);
    KeyingParameters(KeyingParameters* other);

    KeyingAlgorithm getKeyingAlgorithm() {QMutexLocker locker(&mutex); return keyingAlgorithm; }
    QColor getColor() {QMutexLocker locker(&mutex); return QColor(color); }
    int getHue() {QMutexLocker locker(&mutex); return hue; }
    int getSaturation() {QMutexLocker locker(&mutex); return saturation; }
    int getValue() {QMutexLocker locker(&mutex); return value; }
    int getWhite() {QMutexLocker locker(&mutex); return white; }
    int getBlack() {QMutexLocker locker(&mutex); return black; }
    bool isMatteVisible() {QMutexLocker locker(&mutex); return matteVisible; }
    bool getDespill() {QMutexLocker locker(&mutex); return despill; }
    ColorName getFirstColor() {QMutexLocker locker(&mutex); return firstColor; }

public slots:
    void setKeyingAlgorithm(KeyingAlgorithm ka);
    void setColor(QRgb c);
    void setHue(int h);
    void setSaturation(int s);
    void setValue(int v);
    void setWhite(int w);
    void setBlack(int b);
    void setMatteVisible(bool v);
    void setDespill(bool d);
    void setFirstColor(ColorName color);
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
    int white;
    int black;
    bool matteVisible;
    bool despill;
    ColorName firstColor;
    QMutex mutex;
};

#endif // KEYINGPARAMETERS_H
