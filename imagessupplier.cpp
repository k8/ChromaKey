#include "imagessupplier.h"
#include "image.h"
#include <QDebug>
#include <QMutexLocker>

ImagesSupplier::ImagesSupplier(QRgb c, QSize size)
    : frameTime(1),
      fgIsMovie(false), bgIsMovie(false),
      fgOpened(false), bgOpened(false),
      fgFinished(true), bgFinished(true),
      color(c)
{
    Size imSize(size.width(), size.height());
    createImage(fgImage, imSize);
    createImage(bgImage, imSize);
}

void ImagesSupplier::createImage(Mat &img, Size size)
{
    img = Mat::ones(size, CV_8UC3);
    ImagesProcessor::fill(img, color);
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
        fgFinished = false;
        fgFile = file;
        fgOpened = true;
        if (! bgOpened)
        {
            createImage(bgImage, fgImage.size());
        }
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
        bgFinished = false;
        bgFile = file;
        bgOpened = true;
        if (! fgOpened)
        {
            createImage(fgImage, bgImage.size());
        }
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
        fgFinished = true;
        fgFile = file;
        fgOpened = true;
        if (! bgOpened)
        {
            createImage(bgImage, fgImage.size());
        }
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
        bgFinished = true;
        bgFile = file;
        bgOpened = true;
        if (! fgOpened)
        {
            createImage(fgImage, bgImage.size());
        }
    }
    return opened;
}

bool ImagesSupplier::save(const QString &file)
{
    outFile = file;
    return true;
}

const Mat& ImagesSupplier::getFgImage(bool isPaused)
{
    QMutexLocker locker(&mutex);
    if (fgIsMovie && ! isPaused && ! fgFinished)
    {
        if (! getFrame(fgCapture, fgImage))
            fgFinished = true;
    }
    return fgImage;
}

const Mat& ImagesSupplier::getBgImage(bool isPaused)
{
    QMutexLocker locker(&mutex);
    if (bgIsMovie && ! isPaused && ! bgFinished)
    {
        if (! getFrame(bgCapture, bgImage))
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
    return ImagesProcessor::fromCvMat(icon);
}

QImage ImagesSupplier::getBackgroundIcon()
{
    QMutexLocker locker(&mutex);
    Mat icon;
    resize(bgImage, icon, Size(50, 50));
    return ImagesProcessor::fromCvMat(icon);
}

bool ImagesSupplier::isMovie()
{
    QMutexLocker locker(&mutex);
    return fgIsMovie || bgIsMovie;
}

bool ImagesSupplier::hasMoreImages()
{
  return ! (fgFinished || bgFinished);
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
//    qDebug() << fgCapture.get(CV_CAP_PROP_POS_AVI_RATIO);
//    qDebug() << bgCapture.get(CV_CAP_PROP_POS_AVI_RATIO);
//    qDebug() << progress;
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
//    qDebug() << "CV_FOURCC('P','I','M','1')    " << CV_FOURCC('P','I','M','1')    ;
//    qDebug() << "CV_FOURCC('M','J','P','G')    " << CV_FOURCC('M','J','P','G')    ;
//    qDebug() << "CV_FOURCC('M', 'P', '4', '2') " << CV_FOURCC('M', 'P', '4', '2') ;
//    qDebug() << "CV_FOURCC('D', 'I', 'V', '3') " << CV_FOURCC('D', 'I', 'V', '3') ;
//    qDebug() << "CV_FOURCC('D', 'I', 'V', 'X') " << CV_FOURCC('D', 'I', 'V', 'X') ;
//    qDebug() << "CV_FOURCC('U', '2', '6', '3') " << CV_FOURCC('U', '2', '6', '3') ;
//    qDebug() << "CV_FOURCC('I', '2', '6', '3') " << CV_FOURCC('I', '2', '6', '3') ;
//    qDebug() << "CV_FOURCC('F', 'L', 'V', '1') " << CV_FOURCC('F', 'L', 'V', '1') ;
   int fourcc = bgCapture.get(CV_CAP_PROP_FOURCC);
   int fps = bgCapture.get(CV_CAP_PROP_FPS);    
   if (fgIsMovie)
   {
       fourcc = fgCapture.get(CV_CAP_PROP_FOURCC);
       fps = fgCapture.get(CV_CAP_PROP_FPS);
   }
   if (fourcc ==  CV_FOURCC('M','J','P','G'))
       fourcc = CV_FOURCC('F', 'L', 'V', '1');
   return videoWriter.open(file.toStdString(), fourcc, fps, Size(img.cols, img.rows));
}
