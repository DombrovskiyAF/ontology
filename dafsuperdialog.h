#ifndef DAFSUPERDIALOG_H
#define DAFSUPERDIALOG_H

#include <QDialog>

namespace Ui {
class DafSuperDialog;
}

class DafSuperDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DafSuperDialog(QWidget *parent = 0);
    ~DafSuperDialog();

public slots:
    void start();
    void obhod();

private slots:
   // void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pbAddNode_clicked();

    void on_pbViewGraph_clicked();

    void on_pbSaveNewToDB_clicked();

private:
    Ui::DafSuperDialog *ui;
};

#endif // DAFSUPERDIALOG_H
