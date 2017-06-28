#include "formnewquant.h"
#include "ui_formnewquant.h"
#include "datamodel.h"

#include <QSqlRelationalDelegate>
#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QMessageBox>

formNewQuant::formNewQuant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formNewQuant)
{
    ui->setupUi(this);
    setModal(1);
}

formNewQuant::~formNewQuant()
{
    delete ui;
}

void formNewQuant::on_buttonBox_accepted()
{
    if (!dataModel->db.isOpen())
    {
        QMessageBox::information(0, "SQL INSERT:", "Database isn't open!");
    }
    else
    {
        dataModel->insertQuantType(ui->name->text());
        ui->name->clear();
        hide();
    }
}

void formNewQuant::on_buttonBox_rejected()
{
    ui->name->clear();
    hide();
}
