#include "moviethread.h"
#include <QMutexLocker>
#include <QDebug>
#include <QTime>

MovieThread::MovieThread()
    : stopped(false), play(true),
    hue(0), saturation(0), value(0),  segmentation(false)
{
    color = qRgb(0, 0, 0);
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
    pauseContition.wakeAll();
    stopped = true;
    play = true;
}

void MovieThread::setColor(QRgb c)
{
    QMutexLocker locker(&mutex);
    color = c;
    pauseContition.wakeAll();
}

void MovieThread::setHue(int h)
{
    QMutexLocker locker(&mutex);
    hue = h;
    pauseContition.wakeAll();
}


void MovieThread::setSaturation(int s)
{
    QMutexLocker locker(&mutex);
    saturation = s;
    pauseContition.wakeAll();
}

void MovieThread::setValue(int v)
{
    QMutexLocker locker(&mutex);
    value = v;
    pauseContition.wakeAll();
}

void MovieThread::setSegmentaion(bool s)
{
    QMutexLocker locker(&mutex);
    segmentation = s;
    pauseContition.wakeAll();
}

bool MovieThread::paused()
{
    QMutexLocker locker(&mutex);
    play = !play;
    if (play)
        pauseContition.wakeAll();
    return !play;
}

void MovieThread::run()
{
    using namespace cv;
    double frameTime = 1000/fgCapture.get(CV_CAP_PROP_FPS);
    QTime time;
    int waitTime = frameTime;
    while (! stopped)
    {
        Mat fgFrame;
        Mat bgFrame;
        if (waitTime < 0)
        {
            for (int i = 0; i <= abs(waitTime)/frameTime; i++)
            {
                fgCapture >> fgFrame;
                bgCapture >> bgFrame;
            }
        }
        if (fgCapture.grab() && fgCapture.retrieve(fgFrame) && bgCapture.grab() &&bgCapture.retrieve(bgFrame))
        {
            do
            {
                if (! play)
                {
                    QMutexLocker locker(&mutex);
                    pauseContition.wait(&mutex);
                }
                time.restart();
                Mat outFrame;
                mutex.lock();
                int c = color;
                int h = hue;
                int s = saturation;
                int v = value;
                bool segm = segmentation;
                mutex.unlock();
                keying(fgFrame, bgFrame, outFrame, c, h, s, v, segm);
                emit frameReady(fromCvMat(outFrame));
                qDebug() << time.elapsed();
                waitTime = frameTime-time.elapsed();
                if (waitTime > 0)
                    usleep(waitTime*1000);
            }
            while (! play);
        }
        else
        {
            break;
        }
    }
}
