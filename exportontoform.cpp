#include "exportontoform.h"
#include "ui_exportontoform.h"

ExportOntoForm::ExportOntoForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExportOntoForm)
{
    ui->setupUi(this);
}

ExportOntoForm::~ExportOntoForm()
{
    delete ui;
}
