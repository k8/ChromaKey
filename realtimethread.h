#ifndef REALTIMETHREAD_H
#define REALTIMETHREAD_H

#include "keyingthread.h"
#include <QTime>

class RealTimeThread : public KeyingThread
{
    Q_OBJECT

public:
    RealTimeThread(ImagesSupplier* is, ImagesProcessor* ip);

signals:
    void frameReady(const QImage&, const QImage&);
    void noMoreFrames();

protected:
    void waitForFrame();
    bool nextIteration();

private:
    QTime time;

};

#endif // REALTIMETHREAD_H
