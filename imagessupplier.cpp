#include "imagessupplier.h"
#include "image.h"
#include <QDebug>
#include <QMutexLocker>

ImagesSupplier::ImagesSupplier()
    : fgIsMovie(false), bgIsMovie(false),
      fgFinished(false), bgFinished(false),
      frameTime(1)
{
    fgImage = Mat::ones(Size(640, 480), CV_8UC3);
    bgImage = Mat::ones(Size(640, 480), CV_8UC3);
    fill(fgImage, qRgb(255, 255, 255));
    fill(bgImage, qRgb(255, 255, 255));
}

void ImagesSupplier::init(ImagesSupplier *is)
{
    if (is->fgIsMovie)
        openForegroundMovie(is->fgFile);
    else
        openForegroundImage(is->fgFile);
    if (is->bgIsMovie)
        openBackgroundMovie(is->bgFile);
    else
        openBackgroundImage(is->bgFile);
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
        fgFile = file;
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
        bgFile = file;
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
        fgFile = file;
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
        bgFile = file;
    }
    return opened;
}

bool ImagesSupplier::save(const QString &file)
{
    outFile = file;
}

const Mat& ImagesSupplier::getForegroundImage(bool isPaused)
{
    QMutexLocker locker(&mutex);
    if (fgIsMovie && ! isPaused && ! fgFinished)
    {
        if (! getFrame(fgCapture, fgImage))
            fgFinished = true;
    }
    return fgImage;
}

const Mat& ImagesSupplier::getBackgroundImage(bool isPaused)
{
    QMutexLocker locker(&mutex);
    if (bgIsMovie && ! isPaused && ! bgFinished)
    {
        if (getFrame(bgCapture, bgImage))
            bgFinished = true;
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

bool ImagesSupplier::hasMoreImages()
{
  bool more = false;
  if (fgIsMovie && ! fgFinished)
      more = true;
  if (bgIsMovie && ! bgFinished)
      more = true;
  return more;
}

void ImagesSupplier::saveFrame(const Mat &img)
{
    if (isMovie())
    {
        if (! videoWriter.isOpened())
        {
            if (openVideoWriter(outFile, img))
                qDebug() << outFile << "opened";
            else
                qDebug() << outFile << "not opened";
        }
        videoWriter << img;
        imwrite("frame.jpg", img);
    }
    else
    {
        if (imwrite(outFile.toStdString(), img))
            qDebug() << outFile << "saved";
        else
            qDebug() << outFile << "not saved";
    }
}

int ImagesSupplier::getProgress()
{
    int progress = 100;
    if (fgIsMovie)
    {
        progress = fgCapture.get(CV_CAP_PROP_POS_AVI_RATIO)*100.0;
    }
    else if (bgIsMovie)
    {
        progress = bgCapture.get(CV_CAP_PROP_POS_AVI_RATIO)*100.0;
    }
    qDebug() << fgCapture.get(CV_CAP_PROP_POS_AVI_RATIO);
    qDebug() << bgCapture.get(CV_CAP_PROP_POS_AVI_RATIO);
    qDebug() << progress;
    return progress;
}

bool ImagesSupplier::getFrame(VideoCapture &cap, Mat &mat)
{
    Mat frame;
    try
    {
        if (cap.grab() && cap.retrieve(frame))
        {
            frame.copyTo(mat);
            return true;
        }
        else
        {
            return false;
        }
    }
    catch (cv::Exception& e )
    {
        qDebug() << e.what();
    }
    return false;
}

bool ImagesSupplier::openImage(const QString& file, Mat& image)
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

bool ImagesSupplier::openMovie(const QString& file, VideoCapture &capture)
{
    VideoCapture tmp;
    bool opened = tmp.open(file.toStdString());
    if (opened)
    {
        capture = tmp;
    }
    return opened;
}

bool ImagesSupplier::openVideoWriter(const QString &file, const Mat &img)
{
   int fourcc = bgCapture.get(CV_CAP_PROP_FOURCC);
   int fps = bgCapture.get(CV_CAP_PROP_FPS);
   if (fgIsMovie)
   {
       if (! bgIsMovie)
        fourcc = fgCapture.get(CV_CAP_PROP_FOURCC);
       fps = fgCapture.get(CV_CAP_PROP_FPS);
   }
   return videoWriter.open(file.toStdString(), fourcc, fps, Size(img.cols, img.rows));
}
