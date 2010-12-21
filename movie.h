#ifndef MOVIE_H
#define MOVIE_H

#include "image.h"
#include <opencv/highgui.h>

class Movie : public Image
{
public:
    Movie();
    bool open(const QString& file) = 0;
    const Mat& get() = 0;
    QImage getResizedQImage(const QSize& size) = 0;

protected:
    bool getFrame();

private:
    VideoCapture capture;
};

#endif // MOVIE_H
