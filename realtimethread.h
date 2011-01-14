#ifndef REALTIMETHREAD_H
#define REALTIMETHREAD_H

#include "keyingthread.h"
#include "imagesprocessor.h"
#include <QTime>

class RealTimeThread : public KeyingThread
{
    Q_OBJECT

public:
    RealTimeThread(ImagesSupplier* is, ImagesProcessor* ip, KeyingParameters* kp);
    ~RealTimeThread();

signals:
    void frameReady(const QImage&, const QImage&);
    void noMoreFrames();

protected:
    void waitForFrame();
    bool nextIteration();

private:
    QTime time;
    ImagesProcessor* imagesProcessor;
};

#endif // REALTIMETHREAD_H
