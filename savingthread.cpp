#include "savingthread.h"

SavingThread::SavingThread(ImagesSupplier *is, KeyingParameters *kp)
    : KeyingThread(is, kp)
{
}

bool SavingThread::nextIteration()
{
    cv::Mat outFrame;
    keying(outFrame);
    imagesSupplier->saveFrame(outFrame);
    emit progressChanged(imagesSupplier->getProgress());
    return imagesSupplier->hasMoreImages();
}
