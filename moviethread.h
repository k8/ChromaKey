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
    void setColor(QRgb c);
    void setHue(int h);
    void setSaturation(int s);
    void setValue(int v);
    void setSegmentaion(bool s);

signals:
    void frameReady(const QImage&);

protected:
    void run();

private:
    QMutex mutex;
    QWaitCondition pauseContition;
    bool stopped;
    bool play;
    cv::VideoCapture fgCapture;
    cv::VideoCapture bgCapture;
    QRgb color;
    int hue;
    int saturation;
    int value;
    bool segmentation;
};

#endif // MOVIETHREAD_H
