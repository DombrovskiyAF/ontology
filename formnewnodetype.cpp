#include "formnewnodetype.h"
#include "ui_formnewnodetype.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include <datamodel.h>

FormNewNodeType::FormNewNodeType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormNewNodeType)
{
    ui->setupUi(this);
}

FormNewNodeType::~FormNewNodeType()
{
    delete ui;
}

void FormNewNodeType::on_push_add_clicked()
{
    if (!dataModel->db.isOpen()){
        QMessageBox::information(0, "SQL INSERT:", "Database isn't open!");
    }
    else{
        dataModel->insertNodeType(ui->line_node_name->text());
        ui->line_node_name->clear();
        hide();
    }
}

void FormNewNodeType::on_push_cancel_clicked()
{
    ui->line_node_name->clear();
    hide();
}
