#include "realtimethread.h"

RealTimeThread::RealTimeThread(ImagesSupplier *is, ImagesProcessor *ip)
    : KeyingThread(is, ip)
{
}

void RealTimeThread::waitForFrame()
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

bool RealTimeThread::nextIteration()
{
    time.restart();
    cv::Mat outFrame;
    keying(outFrame);
    emit frameReady(imagesProcessor->fromCvMat(outFrame), imagesProcessor->scaledFromCvMat(outFrame));
    if (! imagesSupplier->hasMoreImages())
    {
        emit noMoreFrames();
    }
    waitForFrame();
    return true;
}