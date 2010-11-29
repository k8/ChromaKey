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
    KeyingThread(ImagesSupplier* is, ImagesProcessor* ip = 0, bool save = false);
    ~KeyingThread();
    void init(KeyingThread* kt);
    void stop();    
    void pause();
    void play();
    void update();
    bool isStopped();
    bool isPaused();
    QColor getColor() {QMutexLocker locker(&mutex); return QColor(color); }
    int getHue() {QMutexLocker locker(&mutex); return hue; }
    int getSaturation() {QMutexLocker locker(&mutex); return saturation; }
    int getValue() {QMutexLocker locker(&mutex); return value; }
    bool getSegmentation() {QMutexLocker locker(&mutex); return segmentation; }
        
public slots:
    void setColor(QRgb c);
    void setHue(int h);
    void setSaturation(int s);
    void setValue(int v);
    void setSegmentaion(bool s);

signals:
    void progressChanged(int percent);
    void frameReady(const QImage&, const QImage&);
    void finished();

protected:
    void run();
    void waitForFrame();

private:
    ImagesSupplier* imagesSupplier;
    ImagesProcessor* imagesProcessor;
    QMutex mutex;
    QWaitCondition playContition;
    bool stopped;
    bool played;
    QTime time;
    bool save;


    QRgb color;
    int hue;
    int saturation;
    int value;
    bool segmentation;
};

#endif // KEYINGTHREAD_H
