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

void Movie::setProgress(int p)
{
    int fpp = capture.get(CV_CAP_PROP_FRAME_COUNT)/100.0;
    capture.set(CV_CAP_PROP_POS_FRAMES, fpp*p);
    getFrame();
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
    if (fourcc ==  875967048)
        fourcc = CV_FOURCC('F', 'L', 'V', '1');
    qDebug() << fourcc;
    videoWriter.open(file.toStdString(), fourcc, movie->getFPS(), size);
}

MovieSaver::~MovieSaver()
{
}

bool MovieSaver::save(const Mat &image)
{
    Mat frame = image;
    ImagesProcessor::to3Chanels(image, frame);
    if (videoWriter.isOpened())
    {
        videoWriter << frame;
        return true;
    }
    return false;
}

