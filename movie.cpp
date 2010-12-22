#include "movie.h"
#include <QDebug>

Movie::Movie(const QString &file)
    : finished(false)
{
    VideoCapture tmp;
    opened = tmp.open(file.toStdString());
    if (opened)
    {
        capture = tmp;
        this->file = file;
        get(true);
    }
}

Movie::Movie(const Movie &other)
{
    capture = capture;
    finished = finished;
}

const Mat& Movie::get(bool next)
{
    if (next && ! getFrame())
    {
        finished = true;
    }
    return image;
}

bool Movie::getFrame()
{
    Mat frame;
    try
    {
        if (capture.grab() && capture.retrieve(frame))
        {
            frame.copyTo(image);
            return true;
        }
    }
    catch (cv::Exception& e )
    {
        qDebug() << e.what();
    }
    return false;
}

MovieSaver::MovieSaver(const QString &file, Movie *movie, Size size)
    : ImageSaver(file), movie(movie)
{
    int fourcc = movie->getFOURCC();
    if (fourcc ==  CV_FOURCC('M','J','P','G'))
        fourcc = CV_FOURCC('F', 'L', 'V', '1');
    videoWriter.open(file.toStdString(), fourcc, movie->getFPS(), size);
}

bool MovieSaver::save(const Mat &image)
{
    if (videoWriter.isOpened())
    {
        videoWriter << image;
        return true;
    }
    return false;
}

