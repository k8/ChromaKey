#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QPixmap>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);    
    ui->playPauseButton->setDisabled(true);
}

MainWidget::~MainWidget()
{
    keyingThread->stop();
    keyingThread->wait();
    delete ui;
}

void MainWidget::init(ImagesSupplier* is)
{
    imagesSupplier = is;
    keyingThread = new KeyingThread(is);    
    ui->colorLabel->setPalette(QPalette(Qt::white));
    setForegroundIcon(imagesSupplier->getForegroundIcon());
    setBackgroundIcon(imagesSupplier->getBackgroundIcon());
    QPixmap pix(ui->movieLabel->width(), ui->movieLabel->height());
    pix.fill();
    ui->movieLabel->setPixmap(pix);
    connect(keyingThread, SIGNAL(frameReady(const QImage&)), this, SLOT(prepareFrame(const QImage&)));
    connect(ui->movieLabel, SIGNAL(colorChanged(QRgb)), this, SLOT(changeColor(QRgb)));
    connect(ui->segmentationCheck, SIGNAL(toggled(bool)), keyingThread, SLOT(setSegmentaion(bool)));
    connect(ui->hueSlider, SIGNAL(valueChanged(int)), keyingThread, SLOT(setHue(int)));
    connect(ui->saturationSlider, SIGNAL(valueChanged(int)), keyingThread, SLOT(setSaturation(int)));
    connect(ui->valueSlider, SIGNAL(valueChanged(int)), keyingThread, SLOT(setValue(int)));
    connect(keyingThread, SIGNAL(finished()), this, SLOT(movieFinished()));
    keyingThread->start();
    time.start();
}

void MainWidget::setForegroundIcon(const QImage& img)
{
    ui->fgLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWidget::setBackgroundIcon(const QImage& img)
{
    ui->bgLabel->setPixmap(QPixmap::fromImage(img));
}

void MainWidget::updateMovieLabel()
{
    keyingThread->update();
    if (imagesSupplier->isMovie())
    {
        ui->playPauseButton->setDisabled(false);
    }
    else
    {
        ui->playPauseButton->setDisabled(true);
    }
}

void MainWidget::changeColor(QRgb color)
{
    ui->colorLabel->setPalette(QColor(color));
    keyingThread->setColor(color);
}

void MainWidget::movieFinished()
{
    qDebug() << time.elapsed();
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
    QString text = "Pause";
    if (!keyingThread->isRunning())
    {
        keyingThread->start();
        time.start();
    }
    else
    {
        if (keyingThread->isPaused())
        {
            keyingThread->play();
        }
        else
        {
            keyingThread->pause();
            text = "Play";
        }
    }
    ui->playPauseButton->setText(text);
}
