#ifndef PAUSABLETHREAD_H
#define PAUSABLETHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class PausableThread : public QThread
{
    Q_OBJECT
public:
    PausableThread();
    ~PausableThread();
    bool isStopped();
    bool isPaused();

public slots:
    void stop();
    void pause();
    void play();
    void update();

signals:
    void finished();
    void started();

protected:
    void run();
    virtual bool nextIteration() = 0;

private:
    QMutex mutex;
    QWaitCondition playContition;
    bool stopped;
    bool played;
};

#endif // PAUSABLETHREAD_H
