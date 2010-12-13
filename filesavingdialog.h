#ifndef FILESAVINGDIALOG_H
#define FILESAVINGDIALOG_H

#include <QDialog>
#include "imagessupplier.h"
#include "keyingparameters.h"
#include "savingthread.h"

namespace Ui {
    class FileSavingDialog;
}

class FileSavingDialog : public QDialog {
    Q_OBJECT
public:
    FileSavingDialog(ImagesSupplier* is, KeyingParameters* kp, const QString& file, QWidget *parent = 0);
    ~FileSavingDialog();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *);

signals:
    void finished();

protected slots:
    void savingFinished();

private:
    Ui::FileSavingDialog *ui;
    ImagesSupplier* saveSupplier;
    SavingThread* saveThread;
};

#endif // FILESAVINGDIALOG_H
