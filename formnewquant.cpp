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
        QSqlQuery query;
        QString str;
        QString strF ="INSERT INTO quantifiers (quant_name) VALUES('%1');";
        str = strF.arg(ui->name->text());

        if (!query.exec(str))
        {
            QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
        }
        else
        {
//            QMessageBox::information(0, "SQL INSERT:", "Operation successfully!");
            dataModel->m_quantTypes->select();
            ui->name->clear();
        }
    }
}

void formNewQuant::on_buttonBox_rejected()
{
    ui->name->clear();
    hide();
}
