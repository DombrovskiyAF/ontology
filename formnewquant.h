#ifndef FORMNEWQUANT_H
#define FORMNEWQUANT_H

#include <QDialog>

namespace Ui {
class formNewQuant;
}

class formNewQuant : public QDialog
{
    Q_OBJECT

public:
    explicit formNewQuant(QWidget *parent = 0);
    ~formNewQuant();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::formNewQuant *ui;
};

#endif // FORMNEWQUANT_H
