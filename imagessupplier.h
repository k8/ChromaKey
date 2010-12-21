#ifndef IMAGESSUPPLIER_H
#define IMAGESSUPPLIER_H

#include <QObject>
#include <QImage>
#include <QRgb>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QMutex>
#include <QSize>
using namespace cv;

#include "image.h"
#include "movie.h"

class ImagesSupplier : public QObject
{
public:
    ImagesSupplier(QRgb color = 0, QSize size = QSize(0, 0));
    void init(ImagesSupplier* is);

    bool openForeground(const QString& file);
    bool openBackground(const QString& file);

    bool save(const QString& file);
    const Mat& getFgImage(bool isPaused);
    const Mat& getBgImage(bool isPaused);
    double getFrameTime();
    void cutFrames(int n);
    QImage getForegroundIcon(const QSize& size);
    QImage getBackgroundIcon(const QSize& size);
    bool isMovie();    
    bool hasMoreImages();
    void saveFrame(const Mat& img);
    int getProgress();

private:
    void prepareImages();

    void init(QRgb c, QSize size);
    bool openVideoWriter(const QString& file, const Mat& img);    

    QMutex mutex;
    double frameTime;
    QString outFile;
    QRgb color;
    VideoWriter videoWriter;

    Image* fgPic;    
    Image* bgPic;
};

#endif // IMAGESSUPPLIER_H
