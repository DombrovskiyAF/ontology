#include "nodesedges.h"
#include "ui_nodesedges.h"

#include <QSqlRelationalTableModel>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QSqlRelationalDelegate>
#include <QtGui>

NodesEdges::NodesEdges(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodesEdges)
{
    ui->setupUi(this);

//    model_nodes = new QSqlRelationalTableModel;
//    model_edges = new QSqlRelationalTableModel;
    connect(ui->check_edges,SIGNAL(clicked(bool)),this,SLOT(check_edges(bool)));
    connect(ui->check_nodes,SIGNAL(clicked(bool)),this,SLOT(check_nodes(bool)));

    connect(ui->push_add_edge,SIGNAL(clicked()),this,SLOT(add_edge()));
    connect(ui->push_del_edge,SIGNAL(clicked()),this,SLOT(del_edge()));

    connect(ui->push_add_node,SIGNAL(clicked()),this,SLOT(add_node()));
    connect(ui->push_del_node,SIGNAL(clicked()),this,SLOT(del_node()));


    emit re();
}

NodesEdges::~NodesEdges()
{
    delete ui;
}

void NodesEdges::keyPressEvent(QKeyEvent *eventPress)
{
    qDebug()<<eventPress->key();
    if(eventPress->key()==16777223)
    {
        //QMessageBox::information(this,"Bar code","Bar code is readed succesesful.");
        if (ui->tv_edges->hasFocus())
        {
            emit del_edge();
        }
        if (ui->tv_nodes->hasFocus())
        {
            emit del_node();
        }
    }
}

void NodesEdges::re()
{
    model_edges = new QSqlRelationalTableModel;
    model_edges->setTable("edges");
    model_edges->setRelation(2, QSqlRelation("edge_types", "id", "edge_name"));
    model_edges->setRelation(1, QSqlRelation("nodes", "id", "node_name"));
    model_edges->setRelation(3, QSqlRelation("nodes", "id", "node_name"));
    model_edges->select();
    model_edges->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tv_edges->setModel(model_edges);
    model_edges->sort(0,Qt::SortOrder());

    ui->tv_edges->setItemDelegate(new QSqlRelationalDelegate(ui->tv_edges));

    model_nodes = new QSqlRelationalTableModel;
    model_nodes->setTable("nodes");
    model_nodes->setRelation(1, QSqlRelation("node_types", "id", "node_type_name"));
    model_nodes->select();
    model_nodes->setEditStrategy(QSqlTableModel::OnFieldChange);
    ui->tv_nodes->setModel(model_nodes);
    ui->tv_nodes->setItemDelegate(new QSqlRelationalDelegate(ui->tv_nodes));
    emit ui->tv_edges->scrollToBottom();
}

void NodesEdges::check_edges(bool a)
{
    switch (a)
    {
    case 0:
    {
        model_edges = new QSqlRelationalTableModel;
        model_edges->setTable("edges");
        model_edges->setRelation(2, QSqlRelation("edge_types", "id", "edge_name"));
        model_edges->setRelation(1, QSqlRelation("nodes", "id", "node_name"));
        model_edges->setRelation(3, QSqlRelation("nodes", "id", "node_name"));
        model_edges->select();
        model_edges->setEditStrategy(QSqlTableModel::OnFieldChange);
        ui->tv_edges->setModel(model_edges);
        ui->tv_edges->setItemDelegate(new QSqlRelationalDelegate(ui->tv_edges));
        break;
    }
    case 1:
    {
        model_edges = new QSqlRelationalTableModel;
        model_edges->setTable("edges");
        //model_edges->setRelation(2, QSqlRelation("edge_types", "id", "edge_name"));
        //model_edges->setRelation(1, QSqlRelation("nodes", "id", "node_name"));
        //model_edges->setRelation(3, QSqlRelation("nodes", "id", "node_name"));
        model_edges->select();
        model_edges->setEditStrategy(QSqlTableModel::OnFieldChange);
        ui->tv_edges->setModel(model_edges);
        ui->tv_edges->setItemDelegate(new QSqlRelationalDelegate(ui->tv_edges));
        break;
    }
    }

}


void NodesEdges::check_nodes(bool a)
{
    switch (a)
    {
    case 0:
    {
        model_nodes = new QSqlRelationalTableModel;
        model_nodes->setTable("nodes");
        model_nodes->setRelation(1, QSqlRelation("node_types", "id", "node_type_name"));
        model_nodes->select();
        model_nodes->setEditStrategy(QSqlTableModel::OnFieldChange);
        ui->tv_nodes->setModel(model_nodes);
        ui->tv_nodes->setItemDelegate(new QSqlRelationalDelegate(ui->tv_nodes));
        break;
    }
    case 1:
    {
        model_nodes = new QSqlRelationalTableModel;
        model_nodes->setTable("nodes");
        //model_nodes->setRelation(1, QSqlRelation("node_types", "id", "node_type_name"));
        model_nodes->select();
        model_nodes->setEditStrategy(QSqlTableModel::OnFieldChange);
        ui->tv_nodes->setModel(model_nodes);
        ui->tv_nodes->setItemDelegate(new QSqlRelationalDelegate(ui->tv_nodes));
        break;
    }
    }
}






void NodesEdges::add_edge()
{
    QSqlQuery query;
    QString str;

    str ="INSERT INTO edges (id_edge_type) values ('0');";
    if (!query.exec(str))
    {
        qDebug() << "Unable to make insert operation";
    }

    ui->check_edges->setChecked(1);
    emit check_edges(1);
    ui->tv_edges->sortByColumn(2,Qt::AscendingOrder);
    //ui->tv_edges->sortByColumn(0,Qt::AscendingOrder);
    emit ui->tv_edges->scrollToTop();
}

void NodesEdges::del_edge()
{
    int row;
    row = ui->tv_edges->currentIndex().row(); //# Попользователем, строки.

//    int n = QMessageBox::warning(0,
//                                 "Warning",
//                                 "Are you shue?",
//                                 "Yes",
//                                 "No",
//                                 QString(),
//                                 0,
//                                 1
//                                );
//    if(!n)
//    {
        model_edges->removeRows(row, 1);
        ui->check_edges->setChecked(1);
//    }
}








void NodesEdges::add_node()
{
    QSqlQuery query;
    QString str;
    str ="INSERT INTO nodes (node_name) values ('');";

    if (!query.exec(str))
    {
        qDebug() << "Unable to make insert operation";
    }

    ui->check_nodes->setChecked(1);
    emit check_nodes(1);
    //ui->tv_nodes->sortByColumn(1,Qt::AscendingOrder);
    ui->tv_nodes->sortByColumn(0,Qt::DescendingOrder);
    emit ui->tv_nodes->scrollToTop();
}

void NodesEdges::del_node()
{

    int row;
    row = ui->tv_nodes->currentIndex().row(); //# Попользователем, строки.

//    int n = QMessageBox::warning(0,
//                                 "Warning",
//                                 "Are you shue?",
//                                 "Yes",
//                                 "No",
//                                 QString(),
//                                 0,
//                                 1
//                                );
//    if(!n)
//    {
        model_nodes->removeRows(row, 1);
//    }
}
