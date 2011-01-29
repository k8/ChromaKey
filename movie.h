#ifndef MOVIE_H
#define MOVIE_H

#include "image.h"
#include <opencv/highgui.h>

class Movie : public Image
{
public:
    Movie(const QString& file);
    Movie(const Movie& other);
    ~Movie();
    const Mat& get(bool next);
    bool isMovie()
    {
        return true;
    }
    bool isFinished() {return finished;}
    int getFPS() {return capture->get(CV_CAP_PROP_FPS);}
    int getFOURCC() {return capture->get(CV_CAP_PROP_FOURCC);}
    int getProgress() { return round(capture->get(CV_CAP_PROP_POS_AVI_RATIO)*100.0); }
    void setProgress(int p);

protected:
    bool getFrame();

private:
    VideoCapture* capture;
    bool finished;
};

class MovieSaver : public ImageSaver
{
public:
    MovieSaver(const QString& file, Movie* movie, Size size);
    ~MovieSaver();
    bool save(const Mat &image);
    int getProgress() { return movie->getProgress(); }

protected:
    bool openVideoWriter(const QString& file);

private:
    VideoWriter videoWriter;
    Movie* movie;
};

#endif // MOVIE_H
