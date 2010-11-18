#include "keyingthread.h"
#include <QMutexLocker>
#include <QDebug>

KeyingThread::KeyingThread(ImagesSupplier *is)
    :
    imagesSupplier(is),
    stopped(true), played(true),
    hue(0), saturation(0), value(0),  segmentation(false)
{
    color = qRgb(0, 0, 0);
}

void KeyingThread::stop()
{
    QMutexLocker locker(&mutex);
    playContition.wakeAll();
    stopped = true;
    played = true;
}

void KeyingThread::setColor(QRgb c)
{
    QMutexLocker locker(&mutex);
    color = c;
    playContition.wakeAll();
}

void KeyingThread::setHue(int h)
{
    QMutexLocker locker(&mutex);
    hue = h;
    playContition.wakeAll();
}


void KeyingThread::setSaturation(int s)
{
    QMutexLocker locker(&mutex);
    saturation = s;
    playContition.wakeAll();
}

void KeyingThread::setValue(int v)
{
    QMutexLocker locker(&mutex);
    value = v;
    playContition.wakeAll();
}

void KeyingThread::setSegmentaion(bool s)
{
    QMutexLocker locker(&mutex);
    segmentation = s;
    playContition.wakeAll();
}

void KeyingThread::pause()
{
    QMutexLocker locker(&mutex);
    played = false;
}

void KeyingThread::play()
{
    QMutexLocker locker(&mutex);
    played = true;
    playContition.wakeAll();
}

void KeyingThread::update()
{
    QMutexLocker locker(&mutex);
    playContition.wakeAll();
}

bool KeyingThread::isStopped()
{
    QMutexLocker locker(&mutex);
    return stopped;
}

bool KeyingThread::isPaused()
{
    QMutexLocker locker(&mutex);
    return ! played;
}

void KeyingThread::run()
{
    using namespace cv;
    {
        QMutexLocker locker(&mutex);
        stopped = false;
        played = false;
    }
    while (! isStopped())
    {
        if (isPaused())
        {
            QMutexLocker locker(&mutex);
            playContition.wait(&mutex);
        }
        Mat fgFrame = imagesSupplier->getForegroundImage(isPaused());
        Mat bgFrame = imagesSupplier->getBackgroundImage(isPaused());
        time.restart();
        mutex.lock();
        int c = color;
        int h = hue;
        int s = saturation;
        int v = value;
        bool segm = segmentation;
        mutex.unlock();
        Mat outFrame;
        keying(fgFrame, bgFrame, outFrame, c, h, s, v, segm);
        emit frameReady(fromCvMat(outFrame));
        waitForFrame();
    }
    emit finished();
}

void KeyingThread::waitForFrame()
{
    if (imagesSupplier->isMovie())
    {
        int frameTime = imagesSupplier->getFrameTime();
        int waitTime = frameTime-time.elapsed();
        if (waitTime > 0)
            usleep(waitTime*1000);
        else
        {
            int framesToCut = abs(time.elapsed())/frameTime;
            imagesSupplier->cutFrames(framesToCut);
            int timeToSleep = framesToCut*frameTime-time.elapsed();
            if (timeToSleep > 0)
                usleep(timeToSleep);
        }

    }
}