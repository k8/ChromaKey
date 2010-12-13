#ifndef SAVINGTHREAD_H
#define SAVINGTHREAD_H

#include "keyingthread.h"

class SavingThread : public KeyingThread
{
    Q_OBJECT

public:
    SavingThread(ImagesSupplier* is, ImagesProcessor* ip);

signals:
    void progressChanged(int percent);

protected:
    bool nextIteration();
};

#endif // SAVINGTHREAD_H
