#include "filesavingdialog.h"
#include "ui_filesavingdialog.h"
#include "imagesprocessor.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QCloseEvent>


FileSavingDialog::FileSavingDialog(ImagesSupplier *is, KeyingParameters* kp, const QString& file, QWidget *parent)
    :
    QDialog(parent),
    ui(new Ui::FileSavingDialog)
{
    ui->setupUi(this);
    QStringList list = file.split(QDir::separator());
    setWindowTitle("Saving "+list.last());
    fileName = list.last();
    saveSupplier = new ImagesSupplier();
    saveSupplier->init(is->getForegroundFile(), is->getBackgroundFile());
    saveThread = new SavingThread(saveSupplier, new ImagesProcessor(kp));
    saveThread->play();
    connect(saveThread, SIGNAL(finished()), this, SLOT(savingFinished()));
    ui->label->setText("Saving "+file);
    connect(saveThread, SIGNAL(progressChanged(int)), ui->progressBar, SLOT(setValue(int)));
    saveSupplier->save(file);
    saveThread->start();    
}

FileSavingDialog::~FileSavingDialog()
{
    saveThread->stop();
    saveThread->wait();
    delete saveThread;
    delete saveSupplier;
    delete ui;
}

void FileSavingDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FileSavingDialog::closeEvent(QCloseEvent *event)
{
    if (saveThread->isRunning())
    {
        QMessageBox msgBox;
        msgBox.setText("Do you want to stop saving file "+fileName+"?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if (ret == QMessageBox::Yes)
        {
             event->accept();
             emit finished();
        }
        else
        {
            event->ignore();
        }
    }
    else
    {
        event->accept();
    }
}

void FileSavingDialog::savingFinished()
{
    close();
    emit finished();
}
