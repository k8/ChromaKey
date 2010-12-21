#ifndef MOVIE_H
#define MOVIE_H

#include "image.h"
#include <opencv/highgui.h>

class Movie : public Image
{
public:
    Movie(const QString& file);
    const Mat& get(bool next);
    bool isMovie() {return true;}
    bool isFinished() {return finished;}
    int getFPS() {return capture.get(CV_CAP_PROP_FPS);}
    int getProgress() { return capture.get(CV_CAP_PROP_POS_AVI_RATIO)*100.0; }

protected:
    bool getFrame();

private:
    VideoCapture capture;
    bool finished;
};

#endif // MOVIE_H
