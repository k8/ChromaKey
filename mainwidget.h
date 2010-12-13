#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "filesavingdialog.h"
#include "keyingthread.h"
#include "imagessupplier.h"
#include "image.h"
#include "keyingparameters.h"

namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:
    void connectObjects();
    void setForegroundIcon(const QImage& img);
    void setBackgroundIcon(const QImage& img);
    void updateMovieLabel();
    void changeEvent(QEvent *e);
    void pause();
    void play();
    void showFailMessage(const QString& text);
    void showOpenFailMessage(const QString& file);

protected slots:
    void prepareFrame(const QImage& big, const QImage& small);
    void changeColor(QRgb color);
    void savingFinished();

private:
    Ui::MainWidget *ui;
    FileSavingDialog* savingDialog;
    ImagesSupplier* imagesSupplier;
    ImagesProcessor* imagesProcessor;
    KeyingThread* keyingThread;
    KeyingParameters* keyingParameters;

private slots:
    void on_ycbcrButton_clicked();
    void on_hsvButton_clicked();
    void updateColor(int);
    void showPlayPauseButton(bool show);
    void movieFinished();

    void on_colorButton_clicked();
    void on_bgButton_clicked();
    void on_fgButton_clicked();
    void on_playPauseButton_clicked();
    void on_saveButton_clicked();
};

#endif // MAINWIDGET_H
