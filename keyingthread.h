#ifndef KEYINGTHREAD_H
#define KEYINGTHREAD_H

#include "pausablethread.h"
#include "image.h"
#include "imagessupplier.h"
#include "opencv/cv.h"

class KeyingThread : public PausableThread
{
    Q_OBJECT
public:
    KeyingThread(ImagesSupplier* is, ImagesProcessor* ip);
    ~KeyingThread();
    void keying(cv::Mat& out);

protected:
    virtual bool nextIteration() = 0;

protected:
    ImagesSupplier* imagesSupplier;
    ImagesProcessor* imagesProcessor;
};

#endif // KEYINGTHREAD_H
