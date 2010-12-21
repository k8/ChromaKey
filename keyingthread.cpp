#include "keyingthread.h"
#include <QMutexLocker>
#include <QDebug>

KeyingThread::KeyingThread(ImagesSupplier *is, ImagesProcessor *ip)
    :
    imagesSupplier(is),
    imagesProcessor(ip)
{
}

KeyingThread::KeyingThread(Picture *fgPic, Picture *bgPic)
    :
    foregroundPicture(fgPic),
    backgroundPicture(bgPic)
{

}

KeyingThread::~KeyingThread()
{
}

void KeyingThread::keying(cv::Mat &out)
{
//    imagesProcessor->keying(foregroundPicture->get(), backgroundPicture->get(), out);
    imagesProcessor->keying(imagesSupplier->getFgImage(isPaused()), imagesSupplier->getBgImage(isPaused()), out);
}
