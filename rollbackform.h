#ifndef ROLLBACKFORM_H
#define ROLLBACKFORM_H

#include <QWidget>

namespace Ui {
class RollbackForm;
}

class RollbackForm : public QWidget
{
    Q_OBJECT

public:
    explicit RollbackForm(QWidget *parent = 0);
    ~RollbackForm();

private slots:
    void on_p_dellAll_clicked();

    void on_pb_delN_clicked();

private:
    Ui::RollbackForm *ui;
};

#endif // ROLLBACKFORM_H
