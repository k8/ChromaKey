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
        get(true);
    }
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
