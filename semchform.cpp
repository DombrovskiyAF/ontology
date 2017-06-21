#include "semchform.h"
#include "ui_semchform.h"

SemchForm::SemchForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SemchForm)
{
    ui->setupUi(this);
}

SemchForm::~SemchForm()
{
    delete ui;
}
