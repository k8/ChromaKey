#include "moviethread.h"
#include <QMutexLocker>
#include <QDebug>

MovieThread::MovieThread()
    : tolerance(0), darkTolerance(0), color(0), darkColor(0), stopped(false), play(true)
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

void MovieThread::setTolerance(int t)
{
    QMutexLocker locker(&mutex);
    tolerance = t;
}

void MovieThread::setDarkTolerance(int t)
{
    QMutexLocker locker(&mutex);
    darkTolerance = t;
}

void MovieThread::setColor(QRgb c)
{
    QMutexLocker locker(&mutex);
    color = c;
}

void MovieThread::setDarkColor(QRgb c)
{
    QMutexLocker locker(&mutex);
    darkColor = c;
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
                int t1 = tolerance;
                int t2 = darkTolerance;
                int c1 = color;
                int c2 = darkColor;
                mutex.unlock();
                keying(a, b, t1, t2, c1, c2);
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
