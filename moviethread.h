#ifndef MOVIETHREAD_H
#define MOVIETHREAD_H

#include <QThread>
#include <QMutex>
#include <QRgb>
#include <QWaitCondition>
#include "image.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>

class MovieThread : public QThread
{
    Q_OBJECT
public:
    MovieThread();
    bool openForegroundMovie(const QString& file);
    bool openBackgroundMovie(const QString& file);
    void stop();    
    bool paused();


public slots:
    void setTolerance(int t);
    void setDarkTolerance(int t);
    void setColor(QRgb c);
    void setDarkColor(QRgb c);

signals:
    void frameReady(const QImage&);

protected:
    void run();

private:
    QMutex mutex;
    QWaitCondition pauseContition;
    cv::VideoCapture fgCapture;
    cv::VideoCapture bgCapture;
    int tolerance;
    int darkTolerance;
    QRgb color;
    QRgb darkColor;
    bool stopped;
    bool play;
};

#endif // MOVIETHREAD_H
