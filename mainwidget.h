#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "moviethread.h"

namespace Ui {
    class MainWidget;
}

class MainWidget : public QWidget {
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();

protected:
    void changeEvent(QEvent *e);

protected slots:
    void prepareFrame(const QImage& frame);
    void changeColor(QRgb color);

private:
    Ui::MainWidget *ui;
    MovieThread movieThread;

private slots:
    void on_playPauseButton_clicked();
};

#endif // MAINWIDGET_H
