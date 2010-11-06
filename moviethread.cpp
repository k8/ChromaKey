#include "moviethread.h"
#include <QMutexLocker>
#include <QDebug>

MovieThread::MovieThread()
    : stopped(false), play(true),
    color(0), hue(0), saturation(0), value(0),  segmentation(false)
{
}

bool MovieThread::openForegroundMovie(const QString &file)
{
    return fgCapture.open(file.toStdString());
}

bool MovieThread::openBackgroundMovie(const QString &file)
{
    return bgCapture.open(file.toStdString());
}

void MovieThread::stop()
{
    QMutexLocker locker(&mutex);
    stopped = true;
    play = true;
}

void MovieThread::setColor(QRgb c)
{
    QMutexLocker locker(&mutex);
    color = c;
}

void MovieThread::setHue(int h)
{
    QMutexLocker locker(&mutex);
    hue = h;
}


void MovieThread::setSaturation(int s)
{
    QMutexLocker locker(&mutex);
    saturation = s;
}

void MovieThread::setValue(int v)
{
    QMutexLocker locker(&mutex);
    value = v;
}

void MovieThread::setSegmentaion(bool s)
{
    QMutexLocker locker(&mutex);
    segmentation = s;
}

bool MovieThread::paused()
{
    QMutexLocker locker(&mutex);
    if (play)
    {
        play = false;
    }
    else
    {
        play = true;
        //pauseContition.wakeAll();
    }
    return ! play;
}

void MovieThread::run()
{
    using namespace cv;
    while (! stopped)
    {
//        while (!play)
//        {
//            QMutexLocker locker(&mutex);
//            pauseContition.wait(&mutex);
//        }
        Mat fgFrame;
        Mat bgFrame;
        if (fgCapture.grab() && fgCapture.retrieve(fgFrame) && bgCapture.grab() &&bgCapture.retrieve(bgFrame))
        {
            int next = true;                        
            do
            {
                Mat a, b;
                fgFrame.copyTo(a);
                bgFrame.copyTo(b);
                mutex.lock();
                int c = color;
                int h = hue;
                int s = saturation;
                int v = value;
                bool segm = segmentation;
                mutex.unlock();
                keying(a, b, c, h, s, v, segm);
                emit frameReady(fromCvMat(b));
                usleep(30000);                
                mutex.lock();
                next = play;
                mutex.unlock();
            }
            while (! next);
        }
        else
        {
            break;
        }
    }
}
