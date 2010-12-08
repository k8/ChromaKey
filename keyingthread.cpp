#include "keyingthread.h"
#include <QMutexLocker>
#include <QDebug>

KeyingThread::KeyingThread(ImagesSupplier *is, ImagesProcessor *ip, bool save)
    :
    imagesSupplier(is),
    imagesProcessor(ip),
    save(save),
    stopped(true), played(true),
    keyingAlgorithm(KA_HSV),
    hue(0), saturation(0), value(0),
    luminance(0), blue(0), red(0),
    alpha(360),
    segmentation(false)
{
    if (! imagesProcessor)
        imagesProcessor = new ImagesProcessor();
    color = qRgb(0, 0, 0);
}

KeyingThread::~KeyingThread()
{
}

void KeyingThread::init(KeyingThread *kt)
{
    keyingAlgorithm = kt->getKeyingAlgorithm();
    color = kt->getColor().rgb();
    hue = kt->getHue();
    saturation = kt->getSaturation();
    value = kt->getValue();
    luminance = kt->getLuminance();
    blue = kt->getBlue();
    red = kt->getRed();
    segmentation = kt->getSegmentation();
}

void KeyingThread::stop()
{
    QMutexLocker locker(&mutex);
    playContition.wakeAll();
    stopped = true;
    played = true;
}

void KeyingThread::setKeyingAlgorithm(KeyingAlgorithm ka)
{
    QMutexLocker locker(&mutex);
    keyingAlgorithm = ka;
    playContition.wakeAll();
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

void KeyingThread::setLuminance(int l)
{
    QMutexLocker locker(&mutex);
    luminance = l;
    playContition.wakeAll();
}

void KeyingThread::setBlue(int b)
{
    QMutexLocker locker(&mutex);
    blue = b;
    playContition.wakeAll();
}

void KeyingThread::setRed(int r)
{
    QMutexLocker locker(&mutex);
    red = r;
    playContition.wakeAll();
}

void KeyingThread::setAlpha(double a)
{
    QMutexLocker locker(&mutex);
    alpha = a;
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
                qDebug() << "pause";
                playContition.wait(&mutex);
            }
        }
        else
            paused = false;
        Mat fgFrame = imagesSupplier->getForegroundImage(paused);
        Mat bgFrame = imagesSupplier->getBackgroundImage(paused);
        time.restart();
        mutex.lock();
        KeyingAlgorithm ka = keyingAlgorithm;
        int c = color;
        int h = hue;
        int s = saturation;
        int v = value;
        int l = luminance;
        int b = blue;
        int r = red;
        double a = alpha;
        bool segm = segmentation;
        mutex.unlock();
        Mat outFrame;
        switch (ka)
        {
        case KA_HSV:
            imagesProcessor->keyingHSV(fgFrame, bgFrame, outFrame, c, h, s, v, segm);
            break;
        case KA_YCbCr:
            imagesProcessor->keyingYCbCr(fgFrame, bgFrame, outFrame, c, l, b, r, a, segm);
            break;
        }
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
            waitForFrame();
        }
    }
    qDebug() << "finished";
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
