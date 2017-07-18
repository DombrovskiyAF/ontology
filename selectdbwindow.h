#ifndef SELECTDBWINDOW_H
#define SELECTDBWINDOW_H

#include <QDialog>

namespace Ui {
class selectdbwindow;
}

class selectdbwindow : public QDialog
{
    Q_OBJECT

public:
    explicit selectdbwindow(QWidget *parent = 0);
    ~selectdbwindow();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_rb_odbc_clicked();

    void on_rb_mysql_clicked();

    void on_rb_postgree_clicked();

private:
    Ui::selectdbwindow *ui;
};

#endif // SELECTDBWINDOW_H
