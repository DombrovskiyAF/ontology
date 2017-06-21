#include "formnewnode.h"
#include "ui_formnewnode.h"
#include <datamodel.h>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

FormNewNode::FormNewNode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormNewNode)
{
    ui->setupUi(this);

    connect(ui->cbNodeType, SIGNAL(currentIndexChanged(int)),ui->cbNodeType_private,SLOT(setCurrentIndex(int)));
    ui->cbNodeType_private->hide();
}

FormNewNode::~FormNewNode()
{
    delete ui;
}

void FormNewNode::on_setup_view()
{
    m_mapper->setModel(dataModel->m_nodes);
    ui->cbNodeType->setModel(dataModel->m_nodeTypes);
    ui->cbNodeType->setModelColumn(1);
    m_mapper->addMapping(ui->cbNodeType, 1);

    m_mapper2->setModel(dataModel->m_nodeTypes);
    ui->cbNodeType_private->setModel(dataModel->m_nodeTypes);
    m_mapper2->addMapping(ui->cbNodeType_private, 2);
}

void FormNewNode::on_push_add_clicked()
{

    if (dataModel->insertNode(ui->cbNodeType_private->currentText(),ui->lineEditName->text(),QString::number(ui->spinBoxBrig->value()),QString::number(ui->spinBoxStatus->value()),ui->lineEditAuthor->text(),ui->textEditRem->toPlainText())){
        ui->lineEditName->clear();
        ui->textEditRem->clear();
        dataModel->m_nodes->select();
    }

//    QSqlQuery query;
//    QString str;
//    QString strF ="INSERT INTO nodes (id_node_type, node_name, brightness, status, author, rem)"
//                              "VALUES(%1, '%2', %3, %4, '%5', '%6');";
//    str = strF.arg(ui->cbNodeType_private->currentText())
//                 .arg(ui->lineEditName->text())
//                 .arg(QString::number(ui->spinBoxBrig->value()))
//                 .arg(QString::number(ui->spinBoxStatus->value()))
//                 .arg(ui->lineEditAuthor->text())
//                 .arg(ui->textEditRem->toPlainText());

//    if (!query.exec(str))
//    {
//      qDebug() << str;
//      qDebug() << "Unable to make insert operation";
//      QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
//    }else{
//        ui->lineEditName->clear();
//        //ui->lineEditAuthor->clear();
//        ui->textEditRem->clear();
//        dataModel->m_nodes->select();
//        //hide();


////    //      QMessageBox::information(0, "SQL INSERT:", "Operation successfully!");
////      QVariant val = ui->lineEditName->text(); // строка для поиска
////      QModelIndexList list = ui->tvNodes->model()->match(ui->tvNodes->model()->index(0, 2), 0, val,1, Qt::MatchFixedString);
////      if (!list.isEmpty())
////      {
////          ui->tvNodes->selectionModel()->setCurrentIndex(list.first(),QItemSelectionModel::Rows|QItemSelectionModel::Select|QItemSelectionModel::ClearAndSelect);
////          ui->tvNodes->scrollTo(ui->tvNodes->model()->index(list.first().row(), 2));
////          emit on_select(ui->tvNodes->model()->index(list.first().row(), 2));
////      }
//    }



}

void FormNewNode::on_push_cancel_clicked()
{
    ui->lineEditName->clear();
    ui->lineEditAuthor->clear();
    ui->textEditRem->clear();
    hide();
}
