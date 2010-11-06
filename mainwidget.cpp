#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    ui->colorLabel->setPalette(QPalette(QColor(0, 0, 0)));
    movieThread.openForegroundMovie("input.avi");
//    movieThread.openForegroundMovie("../chroma_key/ChromaKey/DSC_0007.AVI");
    movieThread.openBackgroundMovie("new_york.avi");
    connect(&movieThread, SIGNAL(frameReady(const QImage&)), this, SLOT(prepareFrame(const QImage&)));
    connect(ui->movieLabel, SIGNAL(colorChanged(QRgb)), this, SLOT(changeColor(QRgb)));
    connect(ui->segmentationCheck, SIGNAL(toggled(bool)), &movieThread, SLOT(setSegmentaion(bool)));
    connect(ui->hueSlider, SIGNAL(valueChanged(int)), &movieThread, SLOT(setHue(int)));
    connect(ui->saturationSlider, SIGNAL(valueChanged(int)), &movieThread, SLOT(setSaturation(int)));
    connect(ui->valueSlider, SIGNAL(valueChanged(int)), &movieThread, SLOT(setValue(int)));
    movieThread.start();
}

MainWidget::~MainWidget()
{
    movieThread.stop();
    movieThread.wait();
    delete ui;
}

void MainWidget::changeColor(QRgb color)
{
    ui->colorLabel->setPalette(QColor(color));
    movieThread.setColor(color);
}

void MainWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWidget::prepareFrame(const QImage &frame)
{
    ui->movieLabel->setPixmap(QPixmap::fromImage(frame));
}

void MainWidget::on_playPauseButton_clicked()
{
    if (movieThread.paused())
        ui->playPauseButton->setText("Play");
    else
        ui->playPauseButton->setText("Pause");
}
