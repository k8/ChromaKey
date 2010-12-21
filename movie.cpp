#include "movie.h"
#include <QDebug>

Movie::Movie()
{
}

bool Movie::open(const QString &file)
{
    VideoCapture tmp;
    bool opened = tmp.open(file.toStdString());
    if (opened)
    {
        capture = tmp;
    }
    return opened;
}

const Mat& Movie::get()
{
    getFrame();
    return image;
}

QImage Movie::getResizedQImage(const QSize& size)
{
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
