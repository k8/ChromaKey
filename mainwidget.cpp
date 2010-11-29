#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QPixmap>
#include <QFileDialog>
#include <QColorDialog>
#include <QColor>
#include <QMessageBox>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget),
    savingDialog(0)
{
    ui->setupUi(this);    
    QRgb color = qRgb(0, 0, 0);
    QSize movieSize(300, 200);
    ui->playPauseButton->setDisabled(true);
    imagesSupplier = new ImagesSupplier(color, movieSize);
    imagesProcessor = new ImagesProcessor(ui->movieLabel->size());
    keyingThread = new KeyingThread(imagesSupplier, imagesProcessor);
    ui->movieLabel->setImagesProcessor(imagesProcessor);
    ui->colorButton->setPalette(QPalette(Qt::white));
    setForegroundIcon(imagesSupplier->getForegroundIcon());
    setBackgroundIcon(imagesSupplier->getBackgroundIcon());
    QPixmap pix(movieSize);
    pix.fill(color);
    ui->movieLabel->setMinimumSize(movieSize);
    ui->movieLabel->setPixmap(pix);
    changeColor(color);
    connect(keyingThread, SIGNAL(frameReady(const QImage&, const QImage&)), this, SLOT(prepareFrame(const QImage&, const QImage&)));
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
    delete keyingThread;
    delete imagesSupplier;
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

void MainWidget::savingFinished()
{
    delete savingDialog;
    savingDialog = 0;
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

void MainWidget::showFailMessage(const QString &text)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void MainWidget::showOpenFailMessage(const QString &file)
{
    showFailMessage("Failed to open file "+file+".");
}

void MainWidget::prepareFrame(const QImage &big, const QImage &small)
{
    ui->movieLabel->setPixmaps(QPixmap::fromImage(big), QPixmap::fromImage(small));
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
        bool opened = false;
        if (file.contains(".avi"))
        {
            if (imagesSupplier->openForegroundMovie(file))
                opened = true;
        }
        else
        {
            if (imagesSupplier->openForegroundImage(file))
                opened = true;
        }
        if (opened)
        {
            setForegroundIcon(imagesSupplier->getForegroundIcon());
            updateMovieLabel();
        }
        else
        {
            showOpenFailMessage(file);
        }
    }
}

void MainWidget::on_bgButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Open background file", QDir::currentPath(), "Movies (*.avi);;Images (*.jpg)");
    if (file != QString())
    {
        bool opened = false;
        if (file.contains(".avi"))
        {
            if (imagesSupplier->openBackgroundMovie(file))
                opened = true;
        }
        else
        {
            if (imagesSupplier->openBackgroundImage(file))
                opened = true;
        }
        if (opened)
        {
            setBackgroundIcon(imagesSupplier->getBackgroundIcon());
            updateMovieLabel();
        }
        else
        {
            showOpenFailMessage(file);
        }
    }
}

void MainWidget::on_colorButton_clicked()
{
    QColor color = QColorDialog::getColor(keyingThread->getColor());
    changeColor(color.rgb());
}

void MainWidget::on_saveButton_clicked()
{
    QString filter = "Images (*.jpg)";
    if (imagesSupplier->isMovie())
        filter = "Movies (*.avi)";    QString file = QFileDialog::getSaveFileName(this, "Save file", QDir::currentPath(), filter);
    if (file != QString())
    {
        savingDialog = new FileSavingDialog(imagesSupplier, keyingThread, file, this);
        savingDialog->show();
        connect(savingDialog, SIGNAL(finished()), this, SLOT(savingFinished()));
    }
}
