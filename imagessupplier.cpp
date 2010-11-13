#include "imagessupplier.h"
#include "image.h"

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
    bool opened = fgCapture.open(file.toStdString());
    frameTime = 1000/fgCapture.get(CV_CAP_PROP_FPS);
    getFrame(fgCapture, fgImage);
    fgIsMovie = true;
    return opened;
}

bool ImagesSupplier::openBackgroundMovie(const QString &file)
{
    if (frameTime == 1)
        frameTime = 1000/bgCapture.get(CV_CAP_PROP_FPS);
    bool opened = bgCapture.open(file.toStdString());
    getFrame(bgCapture, bgImage);
    bgIsMovie = true;
    return opened;
}

bool ImagesSupplier::openForegroundImage(const QString &file)
{
    fgImage = imread(file.toStdString());
    fgIsMovie = false;
    return true;
}

bool ImagesSupplier::openBackgroundImage(const QString &file)
{
    bgImage = imread(file.toStdString());
    bgIsMovie = false;
    return true;
}

const Mat& ImagesSupplier::getForegroundImage(bool isPaused)
{
    if (fgIsMovie && ! isPaused)
    {
        getFrame(fgCapture, fgImage);
    }
    return fgImage;
}

const Mat& ImagesSupplier::getBackgroundImage(bool isPaused)
{
    if (bgIsMovie && ! isPaused)
    {
        getFrame(bgCapture, bgImage);
    }
    return bgImage;
}

double ImagesSupplier::getFrameTime()
{
    return frameTime;
}

void ImagesSupplier::cutFrames(int n)
{
    for (int i = 0; i < n; i++)
    {
        Mat frame;
        fgCapture >> frame;
        bgCapture >> frame;
    }
}

QImage ImagesSupplier::getForegroundIcon()
{
    Mat icon;
    resize(fgImage, icon, Size(50, 50));
    return fromCvMat(icon);
}

QImage ImagesSupplier::getBackgroundIcon()
{
    Mat icon;
    resize(bgImage, icon, Size(50, 50));
    return fromCvMat(icon);
}

bool ImagesSupplier::isMovie()
{
    return fgIsMovie || bgIsMovie;
}

void ImagesSupplier::getFrame(VideoCapture &cap, Mat &mat)
{
    Mat frame;
    if (cap.grab() && cap.retrieve(frame))
        frame.copyTo(mat);
}
