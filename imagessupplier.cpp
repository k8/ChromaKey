#include "imagessupplier.h"
#include "imagesprocessor.h"
#include "imagefactory.h"
#include <QDebug>
#include <QMutexLocker>

ImagesSupplier::ImagesSupplier(QRgb c, QSize size)
    : frameTime(1),
      color(c)
{
    Size imSize(size.width(), size.height());
    fgPic = new Image(imSize, c);
    bgPic = new Image(imSize, c);
}

void ImagesSupplier::init(ImagesSupplier *is)
{
//    if (is->fgIsMovie)
//        openForegroundMovie(is->fgFile);
//    else
//        openForegroundImage(is->fgFile);
//    if (is->bgIsMovie)
//        openBackgroundMovie(is->bgFile);
//    else
//        openBackgroundImage(is->bgFile);
}

bool ImagesSupplier::openBackground(const QString &file)
{
    bgPic = ImageFactory::create(file);
    prepareImages();
    return bgPic->isOpened();
}

bool ImagesSupplier::openForeground(const QString &file)
{
    fgPic = ImageFactory::create(file);
    prepareImages();
    return fgPic->isOpened();
}

void ImagesSupplier::prepareImages()
{
    if (fgPic->isOpened() && ! bgPic->isOpened())
    {
        bgPic = new Image(fgPic->getSize(), color);
    }
    if (bgPic->isOpened() && ! fgPic->isOpened())
    {
        fgPic = new Image(bgPic->getSize(), color);
    }
    if (bgPic->isMovie())
    {
        frameTime = 1000/(double)((Movie*)bgPic)->getFPS();
    }
    if (fgPic->isMovie())
    {
        frameTime = 1000/(double)((Movie*)fgPic)->getFPS();
    }
}

bool ImagesSupplier::save(const QString &file)
{
    outFile = file;
    return true;
}

const Mat& ImagesSupplier::getFgImage(bool isPaused)
{
    QMutexLocker locker(&mutex);
    return fgPic->get(! isPaused);
}

const Mat& ImagesSupplier::getBgImage(bool isPaused)
{
    QMutexLocker locker(&mutex);
    return bgPic->get(! isPaused);
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
        fgPic->get(true);
        bgPic->get(true);
    }
}

QImage ImagesSupplier::getForegroundIcon(const QSize &size)
{
    QMutexLocker locker(&mutex);
    return fgPic->getResizedQImage(size);
}

QImage ImagesSupplier::getBackgroundIcon(const QSize &size)
{
    QMutexLocker locker(&mutex);
    return bgPic->getResizedQImage(size);
}

bool ImagesSupplier::isMovie()
{
    QMutexLocker locker(&mutex);    
    return fgPic->isMovie() || bgPic->isMovie();
}

bool ImagesSupplier::hasMoreImages()
{
  return ! fgPic->isFinished() || ! bgPic->isFinished();
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
    if (fgPic->isMovie())
    {
        progress = ((Movie*)fgPic)->getProgress();
    }
    else if (bgPic->isMovie())
    {
        progress = ((Movie*)bgPic)->getProgress();
    }
    return progress;
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
//   int fourcc = bgCapture.get(CV_CAP_PROP_FOURCC);
//   int fps = bgCapture.get(CV_CAP_PROP_FPS);
//   if (fgIsMovie)
//   {
//       fourcc = fgCapture.get(CV_CAP_PROP_FOURCC);
//       fps = fgCapture.get(CV_CAP_PROP_FPS);
//   }
//   if (fourcc ==  CV_FOURCC('M','J','P','G'))
//       fourcc = CV_FOURCC('F', 'L', 'V', '1');
//   return videoWriter.open(file.toStdString(), fourcc, fps, Size(img.cols, img.rows));
}
