#ifndef SEMCHFORM_H
#define SEMCHFORM_H

#include <QDialog>

namespace Ui {
class SemchForm;
}

class SemchForm : public QDialog
{
    Q_OBJECT

public:
    explicit SemchForm(QWidget *parent = 0);
    ~SemchForm();

private:
    Ui::SemchForm *ui;
};

#endif // SEMCHFORM_H
