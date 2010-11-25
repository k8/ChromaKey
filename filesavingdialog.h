#ifndef FILESAVINGDIALOG_H
#define FILESAVINGDIALOG_H

#include <QDialog>
#include "imagessupplier.h"
#include "keyingthread.h"

namespace Ui {
    class FileSavingDialog;
}

class FileSavingDialog : public QDialog {
    Q_OBJECT
public:
    FileSavingDialog(ImagesSupplier* is, KeyingThread* kt, const QString& file, QWidget *parent = 0);
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
    KeyingThread* saveThread;
};

#endif // FILESAVINGDIALOG_H
