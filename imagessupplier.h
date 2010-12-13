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

class ImagesSupplier : public QObject
{
public:
    ImagesSupplier(QRgb color = 0, QSize size = QSize(0, 0));
    void init(ImagesSupplier* is);
    bool openForegroundMovie(const QString& file);
    bool openBackgroundMovie(const QString& file);
    bool openForegroundImage(const QString& file);
    bool openBackgroundImage(const QString& file);
    bool save(const QString& file);
    const Mat& getFgImage(bool isPaused);
    const Mat& getBgImage(bool isPaused);
    double getFrameTime();
    void cutFrames(int n);
    QImage getForegroundIcon();
    QImage getBackgroundIcon();
    bool isMovie();    
    bool hasMoreImages();
    void saveFrame(const Mat& img);
    int getProgress();

private:
    void createImage(Mat& img, Size size);
    void init(QRgb c, QSize size);
    bool getFrame(VideoCapture& cap, Mat& mat);
    bool openImage(const QString& file, Mat& image);
    bool openMovie(const QString& file, VideoCapture& capture);
    bool openVideoWriter(const QString& file, const Mat& img);    

    QMutex mutex;
    double frameTime;
    VideoCapture fgCapture;
    VideoCapture bgCapture;
    VideoWriter videoWriter;
    Mat fgImage;
    Mat bgImage;
    bool fgIsMovie;
    bool bgIsMovie;
    bool fgOpened;
    bool bgOpened;
    bool fgFinished;
    bool bgFinished;
    QString fgFile;
    QString bgFile;
    QString outFile;
    QRgb color;
};

#endif // IMAGESSUPPLIER_H
