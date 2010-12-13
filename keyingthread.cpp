#include "keyingthread.h"
#include <QMutexLocker>
#include <QDebug>

KeyingThread::KeyingThread(ImagesSupplier *is, ImagesProcessor *ip, bool save)
    :
    imagesSupplier(is),
    imagesProcessor(ip),
    stopped(true), played(true),
    save(save)
{
}

KeyingThread::~KeyingThread()
{
}

void KeyingThread::stop()
{
    QMutexLocker locker(&mutex);
    playContition.wakeAll();
    stopped = true;
    played = true;
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

void KeyingThread::wake()
{
    playContition.wakeAll();
}

void KeyingThread::run()
{
    qDebug() << "keying thread started";
    using namespace cv;
    {
        QMutexLocker locker(&mutex);
        stopped = false;
        played = false;
    }
    while (! isStopped())
    {
        bool paused = isPaused();
        if (! save)
        {
            if (isPaused())
            {
                QMutexLocker locker(&mutex);
                playContition.wait(&mutex);
            }
        }
        else
            paused = false;
        Mat fgFrame = imagesSupplier->getForegroundImage(paused);
        Mat bgFrame = imagesSupplier->getBackgroundImage(paused);
        time.restart();
        Mat outFrame;
        imagesProcessor->keying(fgFrame, bgFrame, outFrame);
        if (save)
        {
            imagesSupplier->saveFrame(outFrame);
            emit progressChanged(imagesSupplier->getProgress());
            if (! imagesSupplier->hasMoreImages())
            {
                qDebug() << "saving finised";
                break;
            }
        }
        else
        {
            emit frameReady(imagesProcessor->fromCvMat(outFrame), imagesProcessor->scaledFromCvMat(outFrame));
            if (! imagesSupplier->hasMoreImages())
            {
                emit noMoreFrames();
            }
            waitForFrame();
        }
    }
    qDebug() << "keying thread finished";
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
