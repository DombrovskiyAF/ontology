#include "formtypes.h"
#include "ui_formtypes.h"
#include "datamodel.h"
#include "formnewquant.h"

#include <QSqlRelationalDelegate>
#include <QDataWidgetMapper>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>
#include <QMessageBox>

#include <formnewnodetype.h>
#include <formnewedgetype.h>

FormTypes::FormTypes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormTypes)
{
    ui->setupUi(this);
    mapper = new QDataWidgetMapper(this);
    mapper2 = new QDataWidgetMapper(this);

    ui->tvNodeTypes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvNodeTypes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvEdgeTypes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvEdgeTypes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvQuantTypes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvQuantTypes->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tvNodeTypes,SIGNAL(clicked(QModelIndex)),this,SLOT(on_n_select(QModelIndex)));
    connect(ui->push_n_revert,SIGNAL(clicked()),this,SLOT(on_push_n_revert()));
    connect(ui->push_n_save,SIGNAL(clicked()),this,SLOT(on_push_n_save()));
    connect(ui->push_n_del,SIGNAL(clicked()),this,SLOT(on_push_n_del()));
    connect(ui->line_node_name,SIGNAL(textEdited(QString)),SLOT(on_n_changed()));

    connect(ui->tvQuantTypes,SIGNAL(clicked(QModelIndex)),this,SLOT(on_q_select(QModelIndex)));
    connect(ui->push_q_add,SIGNAL(clicked()),this,SLOT(on_push_q_add()));
    connect(ui->push_q_revert,SIGNAL(clicked()),this,SLOT(on_push_q_revert()));
    connect(ui->push_q_save,SIGNAL(clicked()),this,SLOT(on_push_q_save()));
    connect(ui->push_q_del,SIGNAL(clicked()),this,SLOT(on_push_q_del()));
    connect(ui->line_quant_name,SIGNAL(textEdited(QString)),SLOT(on_q_changed()));

    connect(ui->tvEdgeTypes,SIGNAL(clicked(QModelIndex)),this,SLOT(on_e_select(QModelIndex)));
    connect(ui->push_e_add,SIGNAL(clicked()),this,SLOT(on_push_e_add()));
    connect(ui->push_e_revert,SIGNAL(clicked()),this,SLOT(on_push_e_revert()));
    connect(ui->push_e_save,SIGNAL(clicked()),this,SLOT(on_push_e_save()));
    connect(ui->push_e_del,SIGNAL(clicked()),this,SLOT(on_push_e_del()));
    connect(ui->cb_src, SIGNAL(currentIndexChanged(int)),ui->cb_src_private,SLOT(setCurrentIndex(int)));
    connect(ui->cb_dst, SIGNAL(currentIndexChanged(int)),ui->cb_dst_private,SLOT(setCurrentIndex(int)));
    connect(ui->line_edge_name,SIGNAL(textEdited(QString)),SLOT(on_e_changed()));
    connect(ui->cb_src,SIGNAL(currentIndexChanged(int)),SLOT(on_e_changed()));
    connect(ui->cb_dst,SIGNAL(currentIndexChanged(int)),SLOT(on_e_changed()));

    ui->cb_dst_private->hide();
    ui->cb_src_private->hide();

    ui->line_edge_name->setPlaceholderText("Enter edge name");
    ui->line_node_name->setPlaceholderText("Enter node name");

    connect(formnewquant,SIGNAL(accepted()),SLOT(update_only()));
}

FormTypes::~FormTypes()
{
    delete ui;
}

void FormTypes::setNodeTypesModel(QAbstractItemModel *model)
{
    ui->tvNodeTypes->setModel(model);
    ui->tvNodeTypes->setColumnHidden(2,1);
}

void FormTypes::setEdgeTypesModel(QAbstractItemModel *model)
{
    ui->tvEdgeTypes->setModel(model);

    mapper->setModel(dataModel->m_edgeTypes);

    ui->cb_src->setModel(dataModel->m_nodeTypes);
    ui->cb_src->setModelColumn(1);
    mapper->addMapping(ui->cb_src, 1);

    ui->cb_dst->setModel(dataModel->m_nodeTypes);
    ui->cb_dst->setModelColumn(1);
    mapper->addMapping(ui->cb_dst,1);

    mapper2->setModel(dataModel->m_edgeTypes);

    ui->cb_src_private->setModel(dataModel->m_nodeTypes);
    mapper2->addMapping(ui->cb_src_private, 1);

    ui->cb_dst_private->setModel(dataModel->m_nodeTypes);
    mapper2->addMapping(ui->cb_dst_private,1);
}

void FormTypes::setQuantTypesModel(QAbstractItemModel *model)
{
    ui->tvQuantTypes->setModel(model);
}


void FormTypes::on_db_open()
{
    ui->push_n_add->setEnabled(1);
    ui->push_e_add->setEnabled(1);
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////

// Выбор типа узла / Ds,jh nbgf epkf
void FormTypes::on_n_select(QModelIndex current)
{
    ui->spin_n_id->setValue(ui->tvNodeTypes->model()->data(ui->tvNodeTypes->model()->index(current.row(),0), 2).toInt());
    ui->line_node_name->setText(ui->tvNodeTypes->model()->data(ui->tvNodeTypes->model()->index(current.row(),1), 2).toString());
    ui->push_n_del->setEnabled(1);
    ui->push_n_save->setEnabled(0);
    ui->push_n_revert->setEnabled(0);
}

// Добавление типа узла / Lj,fdktybt nbgf epkf
void FormTypes::on_push_n_add_clicked()
{
    formnewnodetype->show();
}

//Отмена изменений типа узла
void FormTypes::on_push_n_revert()
{
    ui->line_node_name->setText(ui->tvNodeTypes->model()->data(ui->tvNodeTypes->model()->index(ui->tvNodeTypes->currentIndex().row(),1), 2).toString());
    ui->push_n_save->setEnabled(0);
    ui->push_n_revert->setEnabled(0);
}

// Сохранение типа узла / Cj[hfytybt nbgf epkf
void FormTypes::on_push_n_save(){
    if (!dataModel->db.isOpen()){
        QMessageBox::information(0, "SQL UPDATE:", "Database isn't open!");
    }else{
        if (dataModel->updateNodeType(ui->line_node_name->text(),QString::number(ui->spin_n_id->value()))){
            dataModel->m_nodeTypes->select();
            dataModel->m_edgeTypes->select();
        }
    }
    ui->push_n_del->setEnabled(0);
    ui->push_n_save->setEnabled(0);
    ui->push_n_revert->setEnabled(0);
}

// При удалении типа узла - удаление связных типов рёбер
//& удаление всех узлов удаленного типа узла
//& удаление всех рёбер связного типа
void FormTypes::del_edgetypes_for_del_nodetype()
{
    int i = 0;
    QList <QString> list_of_del_edgetype;
    while (ui->tvEdgeTypes->model()->index(i,0).data().toBool())
    {
        if ((ui->tvEdgeTypes->model()->index(i,1).data().toString())==(ui->tvNodeTypes->model()->index(ui->tvNodeTypes->currentIndex().row(),1).data().toString()) ||
            (ui->tvEdgeTypes->model()->index(i,3).data().toString())==(ui->tvNodeTypes->model()->index(ui->tvNodeTypes->currentIndex().row(),1).data().toString()))
        {
            list_of_del_edgetype << ui->tvEdgeTypes->model()->index(i,0).data().toString();
        }
        i++;
    }

    i = 0;
    while (i<list_of_del_edgetype.size())
    {
        QSqlQuery query;
        QString str;
        QString strF = "DELETE FROM edge_types WHERE id=%0;";
        str = strF.arg(list_of_del_edgetype.at(i));
        query.exec(str);

        strF = "DELETE FROM edges WHERE id_edge_type=%0;";
        str = strF.arg(list_of_del_edgetype.at(i));
        query.exec(str);
        i++;
    }

    i = 0;
    QList <QString> list_of_del_nodes;
    while (dataModel->m_nodes->index(i,0).data().toBool())
    {
//        qDebug() << dataModel->m_nodes->index(i,0).data().toString();
//        qDebug() << dataModel->m_nodes->index(i,1).data().toString();
//        qDebug() << ui->tvNodeTypes->model()->index(ui->tvNodeTypes->currentIndex().row(),1).data().toString();
        if ((dataModel->m_nodes->index(i,1).data().toString())==(ui->tvNodeTypes->model()->index(ui->tvNodeTypes->currentIndex().row(),1).data().toString()))
        {
            list_of_del_nodes << dataModel->m_nodes->index(i,0).data().toString();
//        qDebug() << dataModel->m_nodes->index(i,0).data().toString();
        }
        i++;
    }

    i = 0;
    while (i<list_of_del_nodes.size())
    {
        QSqlQuery query;
        QString str;
        QString strF = "DELETE FROM nodes WHERE id=%0;";
        str = strF.arg(list_of_del_nodes.at(i));
        query.exec(str);
        i++;
    }
}

void FormTypes::update_only()
{
//    dataModel->m_nodeTypes->select();
//    dataModel->m_edgeTypes->select();
//    dataModel->m_quantTypes->select();
//    ui->tvEdgeTypes->selectAll();
//    ui->tvNodeTypes->selectAll();
//    ui->tvQuantTypes->selectAll();
}

// Удаление типа узла / Elfktybt nbgf epkf
void FormTypes::on_push_n_del()
{
    del_edgetypes_for_del_nodetype();

    dataModel->m_nodeTypes->removeRow(ui->tvNodeTypes->currentIndex().row());
    dataModel->m_nodeTypes->submitAll();
    dataModel->m_edgeTypes->select();

    ui->push_n_del->setEnabled(0);
    ui->push_n_save->setEnabled(0);
    ui->push_e_revert->setEnabled(0);
}

// Изменение данных типа узла
void FormTypes::on_n_changed()
{
    if ((ui->line_node_name->text()) != (ui->tvNodeTypes->model()->data(ui->tvNodeTypes->model()->index(ui->tvNodeTypes->currentIndex().row(),1),2)).toString())
    {
        ui->push_n_save->setEnabled(1);
        ui->push_n_revert->setEnabled(1);
    }
    else
    {
        ui->push_n_save->setEnabled(0);
        ui->push_n_revert->setEnabled(0);
    }
}

//////////////////////////// КВАНТОРЫ

void FormTypes::on_q_select(QModelIndex current)
{
    ui->spin_q_id->setValue(ui->tvQuantTypes->model()->data(ui->tvQuantTypes->model()->index(current.row(),0), 2).toInt());
    ui->line_quant_name->setText(ui->tvQuantTypes->model()->data(ui->tvQuantTypes->model()->index(current.row(),1), 2).toString());
    ui->push_q_del->setEnabled(1);
    ui->push_q_save->setEnabled(0);
    ui->push_q_revert->setEnabled(0);
}

void FormTypes::on_push_q_add()
{
    formnewquant->show();
}

void FormTypes::on_push_q_revert()
{
    ui->line_quant_name->setText(ui->tvQuantTypes->model()->data(ui->tvQuantTypes->model()->index(ui->tvQuantTypes->currentIndex().row(),1), 2).toString());
    ui->push_q_save->setEnabled(0);
    ui->push_q_revert->setEnabled(0);
}

void FormTypes::on_push_q_save()
{
    if (!dataModel->db.isOpen()){
        QMessageBox::information(0, "SQL UPDATE:", "Database isn't open!");
    }
    else{
        QSqlQuery query;
        QString str;
        QString strF ="UPDATE quantifiers SET quantifiers.quant_name = '%1' WHERE (((quantifiers.id)=%2));";

        str = strF.arg(ui->line_quant_name->text())
                  .arg(ui->spin_q_id->value());

        if (!query.exec(str)){
            QMessageBox::information(0, "SQL UPDATE:", query.lastError().text());
        }
        else{
//          QMessageBox::information(0, "SQL UPDATE:", "Operation successfully!");
            dataModel->m_quantTypes->select();
        }
    }
    ui->push_q_del->setEnabled(0);
    ui->push_q_save->setEnabled(0);
    ui->push_q_revert->setEnabled(0);
}

void FormTypes::on_push_q_del()
{
    dataModel->m_quantTypes->removeRow(ui->tvQuantTypes->currentIndex().row());
    dataModel->m_quantTypes->submitAll();
    dataModel->m_quantTypes->select();

    ui->push_q_del->setEnabled(0);
    ui->push_q_save->setEnabled(0);
    ui->push_q_revert->setEnabled(0);
}

void FormTypes::on_q_changed()
{
    if ((ui->line_quant_name->text()) != (ui->tvQuantTypes->model()->data(ui->tvQuantTypes->model()->index(ui->tvQuantTypes->currentIndex().row(),1),2)).toString())
    {
        ui->push_q_save->setEnabled(1);
        ui->push_q_revert->setEnabled(1);
    }
    else
    {
        ui->push_q_save->setEnabled(0);
        ui->push_q_revert->setEnabled(0);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

// Выбор типа ребра
void FormTypes::on_e_select(QModelIndex current)
{
    ui->spin_e_id->setValue(ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(current.row(),0), 2).toInt());
    ui->line_edge_name->setText(ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(current.row(),2), 2).toString());

    ui->cb_src->setCurrentIndex(ui->cb_src->findText((ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(current.row(),1),2)).toString(),Qt::MatchFixedString));
    ui->cb_dst->setCurrentIndex(ui->cb_dst->findText((ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(current.row(),3),2)).toString(),Qt::MatchFixedString));

    ui->push_e_del->setEnabled(1);
    ui->push_e_save->setEnabled(0);
    ui->push_e_revert->setEnabled(0);

}

// Добавление типа ребра
void FormTypes::on_push_e_add_clicked()
{
    formnewedgetype->show();
}

// Отмена изменений типа ребра
void FormTypes::on_push_e_revert()
{
    QModelIndex current = ui->tvEdgeTypes->currentIndex();
    ui->spin_e_id->setValue(ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(current.row(),0), 2).toInt());
    ui->line_edge_name->setText(ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(current.row(),2), 2).toString());

    ui->cb_src->setCurrentIndex(ui->cb_src->findText((ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(current.row(),1),2)).toString(),Qt::MatchFixedString));
    ui->cb_dst->setCurrentIndex(ui->cb_dst->findText((ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(current.row(),3),2)).toString(),Qt::MatchFixedString));

    ui->push_e_save->setEnabled(0);
    ui->push_e_revert->setEnabled(0);

}

// Сохранение типа ребра
void FormTypes::on_push_e_save()
{
            QSqlQuery query;
            QString str;
            QString strF ="UPDATE edge_types SET "
                    "edge_types.id_node_type_src=%0, "
                    "edge_types.edge_name='%1', "
                    "edge_types.id_node_type_dst=%2 "
                    "WHERE (((edge_types.id)=%4));";
            str = strF.arg(ui->cb_src_private->currentText())
                      .arg(ui->line_edge_name->text())
                      .arg(ui->cb_dst_private->currentText())
                      .arg(ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(ui->tvEdgeTypes->currentIndex().row(),0), 2).toString());
            if (!query.exec(str))
            {
                QMessageBox::information(0, "SQL UPDATE:", query.lastError().text());
            }
            else
            {
//                QMessageBox::information(0, "SQL UPDATE:", "Operation successfully!");
                dataModel->m_edgeTypes->select();
            }

            ui->push_e_del->setEnabled(0);
            ui->push_e_save->setEnabled(0);
            ui->push_e_revert->setEnabled(0);

}

// Удаление типа ребра
void FormTypes::on_push_e_del()
{
    dataModel->m_edgeTypes->removeRow(ui->tvEdgeTypes->currentIndex().row());
    dataModel->m_edgeTypes->submitAll();

    ui->push_e_del->setEnabled(0);
    ui->push_e_save->setEnabled(0);
    ui->push_e_revert->setEnabled(0);

    //    ui->tvEdgeTypes->clearFocus();
}

// Изменение данных типа ребра
void FormTypes::on_e_changed()
{
    if (((ui->cb_src->currentText()) != (ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(ui->tvEdgeTypes->currentIndex().row(),1),2)).toString()) ||
       ((ui->line_edge_name->text()) != (ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(ui->tvEdgeTypes->currentIndex().row(),2),2)).toString()) ||
       ((ui->cb_dst->currentText()) != (ui->tvEdgeTypes->model()->data(ui->tvEdgeTypes->model()->index(ui->tvEdgeTypes->currentIndex().row(),3),2)).toString()))
    {
        ui->push_e_save->setEnabled(1);
        ui->push_e_revert->setEnabled(1);
    }
    else
    {
        ui->push_e_save->setEnabled(0);
        ui->push_e_revert->setEnabled(0);
    }
}

/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
/////////////////////////////////////////////////////
