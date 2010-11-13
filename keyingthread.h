#ifndef KEYINGTHREAD_H
#define KEYINGTHREAD_H

#include <QThread>
#include <QMutex>
#include <QTime>
#include <QColor>
#include <QRgb>
#include <QWaitCondition>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "image.h"
#include "imagessupplier.h"

class KeyingThread : public QThread
{
    Q_OBJECT
public:
    KeyingThread(ImagesSupplier* is);
    void stop();    
    void pause();
    void play();
    void update();
    bool isStopped();
    bool isPaused();
    QColor getColor() {return QColor(color); }

public slots:
    void setColor(QRgb c);
    void setHue(int h);
    void setSaturation(int s);
    void setValue(int v);
    void setSegmentaion(bool s);

signals:
    void frameReady(const QImage&);
    void finished();

protected:
    void run();
    void waitForFrame();

private:
    ImagesSupplier* imagesSupplier;
    QMutex mutex;
    QWaitCondition playContition;
    bool stopped;
    bool played;
    QTime time;
    QRgb color;
    int hue;
    int saturation;
    int value;
    bool segmentation;
};

#endif // KEYINGTHREAD_H
