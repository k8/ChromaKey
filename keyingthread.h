#ifndef KEYINGTHREAD_H
#define KEYINGTHREAD_H

#include "pausablethread.h"
#include "keyingparameters.h"
#include "imagessupplier.h"
#include "opencv/cv.h"

class KeyingThread : public PausableThread
{
    Q_OBJECT
public:
    KeyingThread(ImagesSupplier* is, KeyingParameters* kp);
    virtual ~KeyingThread();
    void keying(cv::Mat& out);

signals:
    void progressChanged(int percent);

protected:
    virtual bool nextIteration() = 0;

protected:
    ImagesSupplier* imagesSupplier;
    KeyingParameters* keyingParameters;
};

#endif // KEYINGTHREAD_H
