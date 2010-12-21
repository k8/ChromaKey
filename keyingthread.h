#ifndef KEYINGTHREAD_H
#define KEYINGTHREAD_H

#include "pausablethread.h"
#include "imagesprocessor.h"
#include "imagessupplier.h"
#include "picture.h"
#include "opencv/cv.h"

class KeyingThread : public PausableThread
{
    Q_OBJECT
public:
    KeyingThread(ImagesSupplier* is, ImagesProcessor* ip);
    KeyingThread(Picture* fgPic, Picture* bgPic);
    ~KeyingThread();
    void keying(cv::Mat& out);

protected:
    virtual bool nextIteration() = 0;

protected:
    ImagesSupplier* imagesSupplier;
    ImagesProcessor* imagesProcessor;
    Picture* foregroundPicture;
    Picture* backgroundPicture;
};

#endif // KEYINGTHREAD_H
