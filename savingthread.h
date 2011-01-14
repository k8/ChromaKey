#ifndef SAVINGTHREAD_H
#define SAVINGTHREAD_H

#include "keyingthread.h"

class SavingThread : public KeyingThread
{
    Q_OBJECT

public:
    SavingThread(ImagesSupplier* is, KeyingParameters* kp);

protected:
    bool nextIteration();
};

#endif // SAVINGTHREAD_H
