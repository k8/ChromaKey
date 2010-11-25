#include "filesavingdialog.h"
#include "ui_filesavingdialog.h"
#include <QFileDialog>

FileSavingDialog::FileSavingDialog(ImagesSupplier *is, KeyingThread *kt, const QString& file, QWidget *parent)
    :
    QDialog(parent),
    ui(new Ui::FileSavingDialog)
{
//    setAttribute(Qt::WA_DeleteOnClose, true);
    ui->setupUi(this);
    saveSupplier = new ImagesSupplier();
    saveSupplier->init(is);
    saveThread = new KeyingThread(saveSupplier, true);
    saveThread->init(kt);
    connect(saveThread, SIGNAL(finished()), this, SLOT(savingFinished()));
    ui->label->setText("Saving "+file);
    connect(saveThread, SIGNAL(progressChanged(int)), ui->progressBar, SLOT(setValue(int)));
    saveSupplier->save(file);
    saveThread->start();    
}

FileSavingDialog::~FileSavingDialog()
{
    delete ui;
    delete saveSupplier;
    delete saveThread;
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

void FileSavingDialog::savingFinished()
{
    close();
    emit finished();
}
