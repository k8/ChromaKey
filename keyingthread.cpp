#include "keyingthread.h"
#include <QMutexLocker>
#include <QDebug>

#include "keyerfactory.h"

KeyingThread::KeyingThread(ImagesSupplier *is, KeyingParameters *kp)
    :
    imagesSupplier(is),
    keyingParameters(kp)
{
}

KeyingThread::~KeyingThread()
{
    delete imagesSupplier;
}

void KeyingThread::keying(cv::Mat &out)
{
    Keyer* keyer = KeyerFactory::createKeyer(keyingParameters->getKeyingAlgorithm());
    keyer->keying(keyingParameters, imagesSupplier->getFgImage(isPaused()), imagesSupplier->getBgImage(isPaused()), out);
    delete keyer;
}
