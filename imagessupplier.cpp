#include "imagessupplier.h"
#include "image.h"
#include <QDebug>
#include <QMutexLocker>

ImagesSupplier::ImagesSupplier()
    : fgIsMovie(false), bgIsMovie(false), frameTime(1)
{
    fgImage = Mat::ones(Size(640, 480), CV_8UC3);
    bgImage = Mat::ones(Size(640, 480), CV_8UC3);
    fill(fgImage, qRgb(255, 255, 255));
    fill(bgImage, qRgb(255, 255, 255));
}

bool ImagesSupplier::openForegroundMovie(const QString &file)
{
    QMutexLocker locker(&mutex);
    bool opened = openMovie(file, fgCapture);
    if (opened)
    {
        frameTime = 1000/fgCapture.get(CV_CAP_PROP_FPS);
        getFrame(fgCapture, fgImage);
        fgIsMovie = true;
    }
    return opened;
}

bool ImagesSupplier::openBackgroundMovie(const QString &file)
{
    QMutexLocker locker(&mutex);
    bool opened = openMovie(file, bgCapture);
    if (opened)
    {
        if (frameTime == 1)
            frameTime = 1000/bgCapture.get(CV_CAP_PROP_FPS);
        getFrame(bgCapture, bgImage);
        bgIsMovie = true;
    }
    return opened;
}

bool ImagesSupplier::openForegroundImage(const QString &file)
{
    QMutexLocker locker(&mutex);
    bool opened = openImage(file, fgImage);
    if (opened)
    {
        fgIsMovie = false;
    }
    return opened;
}

bool ImagesSupplier::openBackgroundImage(const QString &file)
{
    QMutexLocker locker(&mutex);
    bool opened = openImage(file, bgImage);
    if (opened)
    {
        bgIsMovie = false;
    }
    return opened;
}

const Mat& ImagesSupplier::getForegroundImage(bool isPaused)
{
    QMutexLocker locker(&mutex);
    if (fgIsMovie && ! isPaused)
    {
        getFrame(fgCapture, fgImage);
    }
    return fgImage;
}

const Mat& ImagesSupplier::getBackgroundImage(bool isPaused)
{
    QMutexLocker locker(&mutex);
    if (bgIsMovie && ! isPaused)
    {
        getFrame(bgCapture, bgImage);
    }
    return bgImage;
}

double ImagesSupplier::getFrameTime()
{
    QMutexLocker locker(&mutex);
    return frameTime;
}

void ImagesSupplier::cutFrames(int n)
{
    QMutexLocker locker(&mutex);
    for (int i = 0; i < n; i++)
    {
        Mat frame;
        getFrame(fgCapture, frame);
        getFrame(bgCapture, frame);
    }
}

QImage ImagesSupplier::getForegroundIcon()
{
    QMutexLocker locker(&mutex);
    Mat icon;
    resize(fgImage, icon, Size(50, 50));
    return fromCvMat(icon);
}

QImage ImagesSupplier::getBackgroundIcon()
{
    QMutexLocker locker(&mutex);
    Mat icon;
    resize(bgImage, icon, Size(50, 50));
    return fromCvMat(icon);
}

bool ImagesSupplier::isMovie()
{
    QMutexLocker locker(&mutex);
    return fgIsMovie || bgIsMovie;
}

void ImagesSupplier::getFrame(VideoCapture &cap, Mat &mat)
{
    Mat frame;
    try
    {
        if (cap.grab() && cap.retrieve(frame))
            frame.copyTo(mat);
    }
    catch (cv::Exception& e )
    {
        qDebug() << e.what();
    }
}

bool ImagesSupplier::openImage(const QString file, Mat& image)
{
    bool opened = true;
    Mat tmp = imread(file.toStdString());
    if (tmp.data == NULL)
    {
        opened = false;
    }
    else
    {
        image = tmp;
    }
    return opened;
}

bool ImagesSupplier::openMovie(QString file, VideoCapture &capture)
{
    VideoCapture tmp;
    bool opened = tmp.open(file.toStdString());
    if (opened)
    {
        capture = tmp;
    }
    return opened;
}
