#include "savingthread.h"

SavingThread::SavingThread(ImagesSupplier *is, ImagesProcessor *ip)
    : KeyingThread(is, ip)
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
