#ifndef FILESAVINGDIALOG_H
#define FILESAVINGDIALOG_H

#include <QDialog>
#include "savingthread.h"

class ImagesSupplier;
class KeyingParameters;

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
    SavingThread* saveThread;
    QString fileName;
};

#endif // FILESAVINGDIALOG_H
