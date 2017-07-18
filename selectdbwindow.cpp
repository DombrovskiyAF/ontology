#include "selectdbwindow.h"
#include "ui_selectdbwindow.h"
#include "datamodel.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlDatabase>

selectdbwindow::selectdbwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectdbwindow)
{
    ui->setupUi(this);
    ui->port->hide();
    ui->user->hide();
    ui->pwd->hide();
    ui->db->hide();
    ui->host->hide();
    ui->label->hide();
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
}

selectdbwindow::~selectdbwindow()
{
    delete ui;
}

void selectdbwindow::on_buttonBox_accepted()
{
    if (ui->rb_mysql->isChecked()){

    }
    else if (ui->rb_odbc->isChecked()) {
        QFileDialog fd;
        fd.setDefaultSuffix("db");
        fd.setWindowTitle("РћС‚РєСЂС‹С‚СЊ Р±Р°Р·Сѓ РґР°РЅРЅС‹С…");
        fd.setNameFilter("Р‘Р°Р·С‹ РґР°РЅРЅС‹С… Access (*.mdb)");
        if (fd.exec() == QDialog::Accepted) {
            if (fd.selectedFiles().count() > 0) {
                dataModel->m_dbName = fd.selectedFiles().at(0);
                dataModel->connectDB(dataModel->m_dbName);
                dataModel->createModel();
                dataModel->setupView();
            }
        }
    }
    else if (ui->rb_postgree->isChecked()) {
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL",ui->host->text());
        db.setHostName(ui->host->text());
        db.setDatabaseName(ui->db->text());
        db.setUserName(ui->user->text());
        db.setPassword(ui->pwd->text());
        db.setPort(5050);
        if (!db.open()) {
            qDebug() << "Cannot open database:" << db.lastError();
            return;
        }else{
            qDebug() << "Ok";
        }
    }
    hide();
}

void selectdbwindow::on_buttonBox_rejected(){
    hide();
}

void selectdbwindow::on_rb_odbc_clicked(){
    if (ui->rb_odbc->isChecked()){
        ui->port->hide();
        ui->user->hide();
        ui->pwd->hide();
        ui->db->hide();
        ui->host->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_5->hide();
    }
}

void selectdbwindow::on_rb_postgree_clicked(){
    if (ui->rb_postgree->isChecked()){
        ui->port->show();
        ui->user->show();
        ui->pwd->show();
        ui->db->show();
        ui->host->show();
        ui->label->show();
        ui->label_2->show();
        ui->label_3->show();
        ui->label_4->show();
        ui->label_5->show();
    }
}

void selectdbwindow::on_rb_mysql_clicked(){
    if (ui->rb_mysql->isChecked()){
        ui->port->hide();
        ui->user->hide();
        ui->pwd->hide();
        ui->db->hide();
        ui->host->hide();
        ui->label->hide();
        ui->label_2->hide();
        ui->label_3->hide();
        ui->label_4->hide();
        ui->label_5->hide();
    }
}
