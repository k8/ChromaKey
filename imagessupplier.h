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
    Q_OBJECT

public:
    ImagesSupplier(QRgb color = 0, QSize size = QSize(0, 0));
    ~ImagesSupplier();
    void init(const QString& fgFile, const QString& bgFile);

    bool openForeground(const QString& file);
    bool openBackground(const QString& file);

    const QString& getForegroundFile();
    const QString& getBackgroundFile();

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
    void setProgress(int p);

private:
    bool open(Image** img, const QString& file);
    void prepareImages();

    void init(QRgb c, QSize size);

    QMutex mutex;
    double frameTime;
    QString outFile;
    QRgb color;
    VideoWriter videoWriter;

    Image* fgPic;    
    Image* bgPic;
    ImageSaver* saver;
};

#endif // IMAGESSUPPLIER_H
