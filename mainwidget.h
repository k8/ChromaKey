#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QTime>
#include "keyingthread.h"
#include "imagessupplier.h"

namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
    void setForegroundIcon(const QImage& img);
    void setBackgroundIcon(const QImage& img);
    void updateMovieLabel();

protected:
    void changeEvent(QEvent *e);
    void pause();
    void play();
    void showFailMessage(const QString& text);
    void showOpenFailMessage(const QString& file);

protected slots:
    void prepareFrame(const QImage& frame);
    void changeColor(QRgb color);
    void movieFinished();

private:
    Ui::MainWidget *ui;
    ImagesSupplier* imagesSupplier;
    KeyingThread* keyingThread;
    QTime time;

private slots:
    void on_saveButton_clicked();
    void on_colorButton_clicked();
    void on_bgButton_clicked();
    void on_fgButton_clicked();
    void on_playPauseButton_clicked();
};

#endif // MAINWIDGET_H
