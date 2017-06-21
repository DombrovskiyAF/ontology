#include "rollbackform.h"
#include "ui_rollbackform.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
RollbackForm::RollbackForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RollbackForm)
{
    ui->setupUi(this);
}

RollbackForm::~RollbackForm()
{
    delete ui;
}

void RollbackForm::on_p_dellAll_clicked()
{
    QSqlQuery query;
    QString str="DELETE FROM nodes WHERE status>0";
    if(query.exec(str))
    {
        str="DELETE FROM edges WHERE status>0";
        if(query.exec(str))
            QMessageBox::information(0, "Удаление", "Удаление прошло без ошибок");
        else
            qDebug()<<query.lastError().text();
    }
    else
        qDebug()<<query.lastError().text();
}

void RollbackForm::on_pb_delN_clicked()
{
    QSqlQuery query;
    QString str="DELETE FROM nodes WHERE status="+QString::number(ui->sb_ruleID->value());
    if(query.exec(str))
    {
        str="DELETE FROM edges WHERE status="+QString::number(ui->sb_ruleID->value());
        if(query.exec(str))
            QMessageBox::information(0, "Удаление", "Удаление прошло без ошибок");
        else
            qDebug()<<query.lastError().text();
    }
    else
        qDebug()<<query.lastError().text();
}
