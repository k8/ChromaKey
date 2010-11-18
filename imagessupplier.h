#ifndef IMAGESSUPPLIER_H
#define IMAGESSUPPLIER_H

#include <QObject>
#include <QImage>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <QMutex>
using namespace cv;

class ImagesSupplier : public QObject
{
public:
    ImagesSupplier();
    bool openForegroundMovie(const QString& file);
    bool openBackgroundMovie(const QString& file);
    bool openForegroundImage(const QString& file);
    bool openBackgroundImage(const QString& file);
    const Mat& getForegroundImage(bool isPaused);
    const Mat& getBackgroundImage(bool isPaused);
    double getFrameTime();
    void cutFrames(int n);
    QImage getForegroundIcon();
    QImage getBackgroundIcon();
    bool isMovie();    

private:
    void getFrame(VideoCapture& cap, Mat& mat);
    bool openImage(const QString file, Mat& image);
    bool openMovie(const QString file, VideoCapture& capture);

    QMutex mutex;
    double frameTime;
    VideoCapture fgCapture;
    VideoCapture bgCapture;
    Mat fgImage;
    Mat bgImage;
    bool fgIsMovie;
    bool bgIsMovie;
};

#endif // IMAGESSUPPLIER_H
