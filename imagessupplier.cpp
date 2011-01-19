#include "imagessupplier.h"
#include "imagesprocessor.h"
#include "imagefactory.h"
#include <QDebug>
#include <QMutexLocker>

ImagesSupplier::ImagesSupplier(QRgb c, QSize size)
    : frameTime(1),
      color(c),
      saver(0)
{
    Size imSize(size.width(), size.height());
    fgPic = new Image(imSize, c);
    bgPic = new Image(imSize, c);
}

ImagesSupplier::~ImagesSupplier()
{
    if (saver)
    {
        delete saver;
    }

}

void ImagesSupplier::init(const QString &fgFile, const QString &bgFile)
{
    openForeground(fgFile);
    openBackground(bgFile);
}

bool ImagesSupplier::openBackground(const QString &file)
{
    QMutexLocker locker(&mutex);
    return open(&bgPic, file);
}

bool ImagesSupplier::openForeground(const QString &file)
{
    QMutexLocker locker(&mutex);
    return open(&fgPic, file);
}

const QString& ImagesSupplier::getForegroundFile()
{
    QMutexLocker locker(&mutex);
    return fgPic->getFile();
}

const QString& ImagesSupplier::getBackgroundFile()
{
    QMutexLocker locker(&mutex);
    return bgPic->getFile();
}

bool ImagesSupplier::open(Image **img, const QString &file)
{
    Image* tmp = ImageFactory::createImage(file);
    bool opened = false;
    if (tmp->isOpened())
    {
        opened = true;
        delete *img;
        *img = tmp;
        prepareImages();
    }
    return opened;
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
    if (saver != 0)
    {
        delete saver;
    }
    saver = ImageFactory::createSaver(file, fgPic, bgPic);
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
    QMutexLocker locker(&mutex);
    if (saver && saver->getProgress() == 100)
        return false;
    return ! fgPic->isFinished() || ! bgPic->isFinished();
}

void ImagesSupplier::saveFrame(const Mat &img)
{
    QMutexLocker locker(&mutex);
    saver->save(img);
}

int ImagesSupplier::getProgress()
{
    int p = 0;
    QMutexLocker locker(&mutex);
    if (fgPic->isMovie())
    {
        p = (static_cast<Movie*>(fgPic))->getProgress()+1;
    }
    return p;
}

void ImagesSupplier::setProgress(int p)
{
    QMutexLocker locker(&mutex);
    if (fgPic->isMovie())
    {
        (static_cast<Movie*>(fgPic))->setProgress(p);
    }
    if (bgPic->isMovie())
    {
        (static_cast<Movie*>(bgPic))->setProgress(p);
    }
}
