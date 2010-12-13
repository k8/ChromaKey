#ifndef KEYINGTHREAD_H
#define KEYINGTHREAD_H

#include <QThread>
#include <QMutex>
#include <QTime>
#include <QColor>
#include <QRgb>
#include <QWaitCondition>
#include "image.h"
#include "imagessupplier.h"

class KeyingThread : public QThread
{
    Q_OBJECT
public:
    KeyingThread(ImagesSupplier* is, ImagesProcessor* ip = 0, bool save = false);
    ~KeyingThread();
    void stop();    
    void pause();
    void play();
    void update();
    bool isStopped();
    bool isPaused();

public slots:
    void wake();

signals:
    void progressChanged(int percent);
    void frameReady(const QImage&, const QImage&);
    void finished();
    void noMoreFrames();

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
};

#endif // KEYINGTHREAD_H
