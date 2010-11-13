#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QPixmap>
#include <QFileDialog>
#include <QColorDialog>
#include <QColor>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);    
    ui->playPauseButton->setDisabled(true);
    imagesSupplier = new ImagesSupplier();
    keyingThread = new KeyingThread(imagesSupplier);
    ui->colorButton->setPalette(QPalette(Qt::white));
    setForegroundIcon(imagesSupplier->getForegroundIcon());
    setBackgroundIcon(imagesSupplier->getBackgroundIcon());
    QPixmap pix(ui->movieLabel->width(), ui->movieLabel->height());
    pix.fill();
    ui->movieLabel->setPixmap(pix);
    changeColor(qRgb(255, 255, 255));
//    ui->colorButton->setDisabled(true);
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

MainWidget::~MainWidget()
{
    keyingThread->stop();
    keyingThread->wait();
    delete imagesSupplier;
    delete keyingThread;
    delete ui;
}

void MainWidget::setForegroundIcon(const QImage& img)
{
    ui->fgButton->setIcon(QPixmap::fromImage(img));
}

void MainWidget::setBackgroundIcon(const QImage& img)
{
    ui->bgButton->setIcon(QPixmap::fromImage(img));
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
    QPixmap pix(ui->colorButton->width(), ui->colorButton->height());
    pix.fill(QColor(color));
    ui->colorButton->setIcon(pix);;
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

void MainWidget::pause()
{
    keyingThread->pause();
    ui->playPauseButton->setText("Play");
}

void MainWidget::play()
{
    keyingThread->play();
    ui->playPauseButton->setText("Pause");
}

void MainWidget::prepareFrame(const QImage &frame)
{
    ui->movieLabel->setPixmap(QPixmap::fromImage(frame));
}

void MainWidget::on_playPauseButton_clicked()
{
//    if (!keyingThread->isRunning())
//    {
//        keyingThread->start();
//        time.start();
//    }
    if (keyingThread->isPaused())
    {
        play();
    }
    else
    {
        pause();
    }
}

void MainWidget::on_fgButton_clicked()
{    
    QString file = QFileDialog::getOpenFileName(this, "Open foreground file", QDir::currentPath(), "Movies (*.avi);;Images (*.jpg)");
    if (file != QString())
    {
        if (file.contains(".avi"))
        {
            imagesSupplier->openForegroundMovie(file);
        }
        else
        {
            imagesSupplier->openForegroundImage(file);
        }
        setForegroundIcon(imagesSupplier->getForegroundIcon());
        updateMovieLabel();
    }
}

void MainWidget::on_bgButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Open background file", QDir::currentPath(), "Movies (*.avi);;Images (*.jpg)");
    if (file != QString())
    {
        if (file.contains(".avi"))
        {
            imagesSupplier->openBackgroundMovie(file);
        }
        else
        {
            imagesSupplier->openBackgroundImage(file);
        }
        setBackgroundIcon(imagesSupplier->getBackgroundIcon());
        updateMovieLabel();

    }
}

void MainWidget::on_colorButton_clicked()
{
    QColor color = QColorDialog::getColor(keyingThread->getColor());
    changeColor(color.rgb());
}
