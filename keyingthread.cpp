#include "keyingthread.h"
#include <QMutexLocker>
#include <QDebug>

KeyingThread::KeyingThread(ImagesSupplier *is, ImagesProcessor *ip)
    :
    imagesSupplier(is),
    imagesProcessor(ip)
{
}

KeyingThread::~KeyingThread()
{
    delete imagesProcessor;
    delete imagesSupplier;
}

void KeyingThread::keying(cv::Mat &out)
{
    imagesProcessor->keying(imagesSupplier->getFgImage(isPaused()), imagesSupplier->getBgImage(isPaused()), out);
}
