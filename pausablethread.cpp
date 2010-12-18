#include "pausablethread.h"
#include <QMutexLocker>
#include <QDebug>

PausableThread::PausableThread()
    :
    stopped(false), played(false)
{
}

PausableThread::~PausableThread()
{
}

void PausableThread::stop()
{
    QMutexLocker locker(&mutex);
    playContition.wakeAll();
    stopped = true;
    played = true;
}

void PausableThread::pause()
{
    QMutexLocker locker(&mutex);
    played = false;
}

void PausableThread::play()
{
    QMutexLocker locker(&mutex);
    played = true;
    playContition.wakeAll();
}

void PausableThread::update()
{
    QMutexLocker locker(&mutex);
    playContition.wakeAll();
}

bool PausableThread::isStopped()
{
    QMutexLocker locker(&mutex);
    return stopped;
}

bool PausableThread::isPaused()
{
    QMutexLocker locker(&mutex);
    return ! played;
}

void PausableThread::run()
{
    emit started();
    while (! isStopped())
    {        
        if (isPaused())
        {
            QMutexLocker locker(&mutex);
            playContition.wait(&mutex);
        }
        if (! nextIteration())
            break;
    }
    emit finished();
}
