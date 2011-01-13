#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QPixmap>
#include <QFileDialog>
#include <QColorDialog>
#include <QColor>
#include <QMessageBox>

#include "imagesprocessor.h"
#include "filesavingdialog.h"


MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    filesPath = QDir::currentPath()+QDir::separator()+"img";
    QRgb color = qRgb(0, 0, 0);
    QSize movieSize(300, 200);    

    imagesSupplier = new ImagesSupplier(color, movieSize);
    keyingParameters = new KeyingParameters(algorithmName(ui->tabWidget->currentIndex()),
                                            color,
                                            ui->hueSlider->value(),
                                            ui->saturationSlider->value(),
                                            ui->valueSlider->value(),
                                            ui->luminanceSlider->value(),
                                            ui->blueSlider->value(),
                                            ui->redSlider->value(),
                                            ui->alphaSpinBox->value(),
                                            false,
                                            ui->dmSlider->value(),
                                            ui->dmSlider2->value(),
                                            ui->showDmBox->isChecked());
    initColorNames();
    ImagesProcessor* imagesProcessor = new ImagesProcessor(keyingParameters, movieSize);
    keyingThread = new RealTimeThread(imagesSupplier, imagesProcessor);
    ui->movieLabel->init(color, movieSize, imagesProcessor);

    connectObjects();

    setForegroundIcon(imagesSupplier->getForegroundIcon(ui->fgButton->size()));
    setBackgroundIcon(imagesSupplier->getBackgroundIcon(ui->bgButton->size()));
    changeColor(color);
    showPlayPauseButton(false);


//    <ycbcr testing settings>
//    openFile("img/input.avi", true);
//    openFile("img/new_york.avi", false);
//    ui->ycbcrButton->click();
//    changeColor(qRgb(45, 188, 179));
//    ui->alphaSpinBox->setValue(295);
//    connect(keyingThread, SIGNAL(started()), keyingThread, SLOT(update()));
//    </ycbcr testing settings>

    keyingThread->start();
}

MainWidget::~MainWidget()
{
    keyingThread->stop();
    keyingThread->wait();
    delete keyingThread;
    delete ui;
}

void MainWidget::openFile(const QString &file, bool fg)
{
    bool opened = false;
    if (fg)
    {
        if (imagesSupplier->openForeground(file))
        {
            setForegroundIcon(imagesSupplier->getForegroundIcon(ui->fgButton->size()));
            opened = true;
        }
    }
    else
    {
        if (imagesSupplier->openBackground(file))
        {
            setBackgroundIcon(imagesSupplier->getBackgroundIcon(ui->bgButton->size()));
            opened = true;
        }
    }
    if (opened)
    {
        updateMovieLabel();
    }
    else
    {
        showOpenFailMessage(file);
    }
}

void MainWidget::connectObjects()
{
    connect(ui->movieSlider, SIGNAL(sliderReleased()), this, SLOT(shiftMovie()));

    connect(keyingThread, SIGNAL(frameReady(const QImage&, const QImage&)), this, SLOT(prepareFrame(const QImage&, const QImage&)));
    connect(keyingThread, SIGNAL(noMoreFrames()), this, SLOT(movieFinished()));
    connect(keyingParameters, SIGNAL(parameterChanged()), keyingThread, SLOT(update()));

    connect(ui->movieLabel, SIGNAL(colorChanged(QRgb)), this, SLOT(changeColor(QRgb)));

    connect(ui->hueSlider, SIGNAL(valueChanged(int)), keyingParameters, SLOT(setHue(int)));
    connect(ui->saturationSlider, SIGNAL(valueChanged(int)), keyingParameters, SLOT(setSaturation(int)));
    connect(ui->valueSlider, SIGNAL(valueChanged(int)), keyingParameters, SLOT(setValue(int)));

    connect(ui->luminanceSlider, SIGNAL(valueChanged(int)), keyingParameters, SLOT(setLuminance(int)));
    connect(ui->blueSlider, SIGNAL(valueChanged(int)), keyingParameters, SLOT(setBlue(int)));
    connect(ui->redSlider, SIGNAL(valueChanged(int)), keyingParameters, SLOT(setRed(int)));

    connect(ui->alphaSpinBox, SIGNAL(valueChanged(int)), keyingParameters, SLOT(setAlpha(int)));

    connect(ui->dmSlider, SIGNAL(valueChanged(int)), keyingParameters, SLOT(setWhite(int)));
    connect(ui->dmSlider2, SIGNAL(valueChanged(int)), keyingParameters, SLOT(setBlack(int)));
    connect(ui->showDmBox, SIGNAL(toggled(bool)), keyingParameters, SLOT(setMatteVisible(bool)));

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
    showPlayPauseButton(imagesSupplier->isMovie());
    keyingThread->update();
}

void MainWidget::changeColor(QRgb color)
{
    QPixmap pix(ui->colorButton->width(), ui->colorButton->height());
    pix.fill(QColor(color));
    ui->colorButton->setIcon(pix);;
    keyingParameters->setColor(color);
    updateColorCodes();
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
    ui->playPauseButton->setText("Play ");
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

void MainWidget::initColorNames()
{
    QStringList colorsList;
    int i = 0;
    colorsList.push_back("Red");
    colorCodes[i] = KeyingParameters::C_RED;
    colorIndexes[KeyingParameters::C_RED] = i++;
    colorsList.push_back("Green");
    colorCodes[i] = KeyingParameters::C_GREEN;
    colorIndexes[KeyingParameters::C_GREEN] = i++;
    colorsList.push_back("Blue");
    colorCodes[i] = KeyingParameters::C_BLUE;
    colorIndexes[KeyingParameters::C_BLUE] = i++;

    ui->mainColorBox->addItems(colorsList);

    colorsList.push_back("Maximum");
    colorCodes[i] = KeyingParameters::C_MAX;
    colorIndexes[KeyingParameters::C_MAX] = i++;
    ui->secondColorBox->addItems(colorsList);
}

KeyingParameters::KeyingAlgorithm MainWidget::algorithmName(int index)
{

    switch (index)
    {
    case 0:
        return KeyingParameters::KA_HSV;
    case 1:
        return KeyingParameters::KA_DM;
    case 2:
        return KeyingParameters::KA_YCbCr;
    }
}

void MainWidget::showPlayPauseButton(bool show)
{
    ui->playPauseButton->setVisible(show);
}

void MainWidget::prepareFrame(const QImage &big, const QImage &small)
{
    ui->movieLabel->setPixmaps(QPixmap::fromImage(big), QPixmap::fromImage(small));
}

void MainWidget::movieFinished()
{
    showPlayPauseButton(false);
    pause();
}

void MainWidget::on_playPauseButton_clicked()
{
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
    QString file = QFileDialog::getOpenFileName(this, "Open foreground file", filesPath, "Movies (*.avi);;Images (*.jpg)");
    if (file != QString())
    {
        openFile(file, true);
    }
}

void MainWidget::on_bgButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Open background file", filesPath, "Movies (*.avi);;Images (*.jpg)");
    if (file != QString())
    {
        openFile(file, false);
    }
}

void MainWidget::on_colorButton_clicked()
{
    QColor color = QColorDialog::getColor(keyingParameters->getColor());
    changeColor(color.rgb());
}

void MainWidget::on_saveButton_clicked()
{
    QString filter = "Images (*.jpg)";
    if (imagesSupplier->isMovie())
        filter = "Movies (*.avi)";
    QString file = QFileDialog::getSaveFileName(this, "Save file", filesPath, filter);
    if (file != QString())
    {
        FileSavingDialog* savingDialog = new FileSavingDialog(imagesSupplier, keyingParameters, file, this);
        savingDialog->show();
    }
}

void MainWidget::on_tabWidget_currentChanged(int index)
{
    keyingParameters->setKeyingAlgorithm(algorithmName(index));
}

void MainWidget::shiftMovie()
{
    imagesSupplier->setProgress(ui->movieSlider->value());
    keyingThread->update();
}

void MainWidget::updateColorCodes()
{
    qDebug() << "first index: " << colorIndexes[keyingParameters->getFirstColor()];
    qDebug() << "second index: " << colorIndexes[keyingParameters->getSecondColor()];

    ui->mainColorBox->setCurrentIndex(colorIndexes[keyingParameters->getFirstColor()]);
    ui->secondColorBox->setCurrentIndex(colorIndexes[keyingParameters->getSecondColor()]);
}

void MainWidget::on_mainColorBox_currentIndexChanged(int index)
{
    keyingParameters->setFirstColor(colorCodes[index]);
}

void MainWidget::on_secondColorBox_currentIndexChanged(int index)
{
    keyingParameters->setSecondColor(colorCodes[index]);
}
