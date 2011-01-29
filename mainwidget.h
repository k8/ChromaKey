#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QMap>
#include "realtimethread.h"
#include "imagessupplier.h"
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
    void openFile(const QString& filename, bool fg);
    void connectObjects();
    void setForegroundIcon(const QImage& img);
    void setBackgroundIcon(const QImage& img);
    void updateMovieLabel();
    void changeEvent(QEvent *e);
    void pause();
    void play();
    void showFailMessage(const QString& text);
    void showOpenFailMessage(const QString& file);
    void initColorNames();

    KeyingParameters::KeyingAlgorithm algorithmName(int index);

protected slots:
    void prepareFrame(const QImage& big, const QImage& small);
    void changeColor(QRgb color);

private:
    Ui::MainWidget *ui;
    ImagesSupplier* imagesSupplier;
    RealTimeThread* keyingThread;
    KeyingParameters* keyingParameters;
    QString filesPath;
    QMap<int, KeyingParameters::ColorName> colorCodes;
    QMap<KeyingParameters::ColorName, int> colorIndexes;

private slots:
    void on_mainColorBox_currentIndexChanged(int index);
    void on_tabWidget_currentChanged(int index);
    void showMovieMenu(bool show);

    void on_colorButton_clicked();
    void on_bgButton_clicked();
    void on_fgButton_clicked();
    void on_playPauseButton_clicked();
    void on_saveButton_clicked();

    void shiftMovie();

    void updateColorCodes();
};

#endif // MAINWIDGET_H
