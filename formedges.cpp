#include "formedges.h"
#include "ui_formedges.h"
#include "QMenu"
#include "datamodel.h"
#include <formnewedge.h>
#include <QSqlRelationalDelegate>
#include <QSqlQuery>

#include <QSqlError>
#include <QDebug>
#include <QList>
#include <QMessageBox>
#include <QSortFilterProxyModel>

FormEdges::FormEdges(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormEdges)
{
    ui->setupUi(this);

    ui->tvEdges->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvEdges->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvEdges->setSortingEnabled(1);



    //QObject::connect(ui->tvEdges,SIGNAL(clicked(QModelIndex)),this,SLOT(on_select(QModelIndex)));
    QObject::connect(ui->tvEdges,SIGNAL(pressed(QModelIndex)),this,SLOT(on_select(QModelIndex)));
    QObject::connect(ui->push_add,SIGNAL(clicked()),this,SLOT(on_add()));
    QObject::connect(ui->push_save,SIGNAL(clicked()),this,SLOT(on_save()));
    QObject::connect(ui->push_revert,SIGNAL(clicked()),this,SLOT(on_revert()));
    QObject::connect(ui->push_del,SIGNAL(clicked()),this,SLOT(on_del()));


    QObject::connect(ui->cb_src, SIGNAL(currentIndexChanged(int)),ui->cb_src_private,SLOT(setCurrentIndex(int)));
    QObject::connect(ui->cb_type, SIGNAL(currentIndexChanged(int)),ui->cb_type_private,SLOT(setCurrentIndex(int)));
    QObject::connect(ui->cb_dst, SIGNAL(currentIndexChanged(int)),ui->cb_dst_private,SLOT(setCurrentIndex(int)));
    QObject::connect(ui->cb_quant, SIGNAL(currentIndexChanged(int)),ui->cb_quant_private,SLOT(setCurrentIndex(int)));

//    QObject::connect(ui->treeWidget_src,SIGNAL(clicked(QModelIndex)),SLOT(src_tree_clicked(QModelIndex)));

// Р В¤РЎС“Р Р…Р С”РЎвЂ Р С‘РЎРЏ Р Т‘Р В»РЎРЏ Р С”Р Р…Р С•Р С—Р С•Р С”
    connect(ui->cb_dst,SIGNAL(currentIndexChanged(int)),SLOT(data_canged()));
    connect(ui->cb_src,SIGNAL(currentIndexChanged(int)),SLOT(data_canged()));
    connect(ui->cb_type,SIGNAL(currentIndexChanged(int)),SLOT(data_canged()));
    connect(ui->cb_quant,SIGNAL(currentIndexChanged(int)),SLOT(data_canged()));
    connect(ui->spin_capacity,SIGNAL(valueChanged(int)),SLOT(data_canged()));
    connect(ui->spin_status,SIGNAL(valueChanged(int)),SLOT(data_canged()));
    connect(ui->spin_isNew,SIGNAL(valueChanged(int)),SLOT(data_canged()));

//    connect(ui->pushButton,SIGNAL(clicked()),SLOT(create_update_tree()));


    mapper_src = new QDataWidgetMapper(this);
    mapper_src_priv = new QDataWidgetMapper(this);
    mapper_dst = new QDataWidgetMapper(this);
    mapper_dst_priv = new QDataWidgetMapper(this);
    mapper_quant = new QDataWidgetMapper(this);
    mapper_quant_priv = new QDataWidgetMapper(this);
    mapper_type = new QDataWidgetMapper(this);
    mapper_type_priv = new QDataWidgetMapper(this);

    formnewedge = new FormNewEdge;

    ui->cb_src_private->hide();
    ui->cb_type_private->hide();
    ui->cb_dst_private->hide();
    ui->cb_quant_private->hide();
    ui->cb_src11->hide();
    ui->cb_dst11->hide();


    add_flag = 0;
    edit_flag = 0;

//    ui->treeWidget_src->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(ui->treeWidget_src,SIGNAL(customContextMenuRequested(QPoint)),SLOT(on_right_click(QPoint)));
}

FormEdges::~FormEdges()
{
    delete mapper_src;
    delete mapper_src_priv;
    delete mapper_dst;
    delete mapper_dst_priv;
    delete mapper_quant;
    delete mapper_quant_priv;
    delete mapper_type;
    delete mapper_type_priv;

    delete formnewedge;

    delete ui;
}

void FormEdges::setEdgesModel(QAbstractItemModel *model)
{
    ui->tvEdges->setModel(model);
    ui->tvEdges->model()->setHeaderData(0,Qt::Horizontal,"Id",2);
    ui->tvEdges->model()->setHeaderData(1,Qt::Horizontal,"Src node name",2);
    ui->tvEdges->model()->setHeaderData(2,Qt::Horizontal,"Type of edge",2);
    ui->tvEdges->model()->setHeaderData(3,Qt::Horizontal,"Dst node name",2);
    ui->tvEdges->model()->setHeaderData(4,Qt::Horizontal,"Type of quant",2);
    ui->tvEdges->model()->setHeaderData(5,Qt::Horizontal,"Capacity",2);
    ui->tvEdges->model()->setHeaderData(6,Qt::Horizontal,"Status",2);
    ui->tvEdges->model()->setHeaderData(7,Qt::Horizontal,"IsNew",2);
    const QModelIndex &parent = QModelIndex();
    while (model->canFetchMore(parent))
    model->fetchMore(parent);
    //create_update_tree();

    mapper_src->setModel(dataModel->m_edges);
    ui->cb_src->setModel(dataModel->m_nodes);
    ui->cb_src->setModelColumn(2);
    mapper_src->addMapping(ui->cb_src, 1);

    mapper_src_priv->setModel(dataModel->m_edges);
    ui->cb_src_private->setModel(dataModel->m_nodes);
    mapper_src_priv->addMapping(ui->cb_src_private, 1);

        mapper_quant->setModel(dataModel->m_edges);
        ui->cb_quant->setModel(dataModel->m_quantTypes);
        ui->cb_quant->setModelColumn(1);
        mapper_quant->addMapping(ui->cb_quant, 4);

        mapper_quant_priv->setModel(dataModel->m_edges);
        ui->cb_quant_private->setModel(dataModel->m_quantTypes);
        mapper_quant_priv->addMapping(ui->cb_quant_private, 4);

    mapper_type->setModel(dataModel->m_edges);
    ui->cb_type->setModel(dataModel->m_edgeTypes);
    ui->cb_type->setModelColumn(2);
    mapper_type->addMapping(ui->cb_type, 1);

    mapper_dst->setModel(dataModel->m_edges);
    ui->cb_dst->setModel(dataModel->m_nodes);
    ui->cb_dst->setModelColumn(2);
    mapper_dst->addMapping(ui->cb_dst, 1);

    mapper_type_priv->setModel(dataModel->m_edges);
    ui->cb_type_private->setModel(dataModel->m_edgeTypes);
    mapper_type_priv->addMapping(ui->cb_type_private, 1);

    mapper_dst_priv->setModel(dataModel->m_edges);
    ui->cb_dst_private->setModel(dataModel->m_nodes);
    mapper_dst_priv->addMapping(ui->cb_dst_private, 1);

    formnewedge->setmappers();
}

// Выбор ребра
void FormEdges::on_select(QModelIndex current)
{
    ui->spin_id->setValue(ui->tvEdges->model()->data(  ui->tvEdges->model()->index(current.row(),0)  , 2).toInt());
    ui->cb_src->setCurrentIndex(ui->cb_src->findText((ui->tvEdges->model()->data(ui->tvEdges->model()->index(current.row(),1),2)).toString(),(Qt::MatchFixedString) ));
    //ui->cb_type->setCurrentIndex(ui->cb_type->findText((ui->tvEdges->model()->data(ui->tvEdges->model()->index(current.row(),2),2)).toString(),(Qt::MatchFixedString) ));



    QSqlQuery query2;
    QString str2;
    QString strF2 = "SELECT edge_types.id, edge_types.id_node_type_src, edge_types.id_node_type_dst "
                    "FROM edge_types "
                    "WHERE (((edge_types.id_node_type_src)=%1)) "
                    "AND (((edge_types.id_node_type_dst)=%2));";
//                    "AND (((edges.id_node_dst)=(nodes.id))) "
//                    "AND (((node_types.id)=(nodes.id_node_type)));";
// edges.id_node_src, edges.id_edge_type, edges.id_node_dst,
//  str2 = strF2.arg(QString::number(ui->spinBoxId->value()))
//              .arg(QString::number(ui->spinBoxId->value()));

//  if (!query2.exec(str))
//  {
//      qDebug() << str2;
//      qDebug() << query2.lastError();
//      qDebug() << "Unable to make insert operation";

//      QStandardItemModel *models = new QStandardItemModel(1,1, ui->tvNEXT);
//      ui->tvNEXT->setModel(models);
//      ui->tvPREV->setModel(models);
//      QStandardItem *newItem = new QStandardItem("ERROR. Please reload database...");
//      models->setItem(0, 0, newItem);

//  }
//  else
//  {

//      QSqlQueryModel *model_next = new QSqlQueryModel;

//      model_next->setQuery(str2);

//      //model_next

//      ui->tvNEXT->setItemDelegate( new QSqlRelationalDelegate(ui->tvNEXT));
//      ui->tvNEXT->setModel(model_next);
//      ui->tvNEXT->setEditTriggers(QAbstractItemView::NoEditTriggers);
//  }




    ui->cb_dst->setCurrentIndex(ui->cb_dst->findText((ui->tvEdges->model()->data(ui->tvEdges->model()->index(current.row(),3),2)).toString(),(Qt::MatchFixedString) ));
    ui->cb_quant->setCurrentIndex(ui->cb_quant->findText((ui->tvEdges->model()->data(ui->tvEdges->model()->index(current.row(),4),2)).toString(),(Qt::MatchFixedString) ));
    ui->spin_capacity->setValue(ui->tvEdges->model()->data(  ui->tvEdges->model()->index(current.row(),5)  , 2).toInt());
    ui->spin_status->setValue(ui->tvEdges->model()->data(  ui->tvEdges->model()->index(current.row(),6)  , 2).toInt());
    ui->spin_isNew->setValue(ui->tvEdges->model()->data(  ui->tvEdges->model()->index(current.row(),7)  , 2).toInt());
    ui->push_del->setEnabled(1);
    ui->push_revert->setEnabled(0);
    ui->push_save->setEnabled(0);
}

// Р СџРЎР‚Р С‘ Р Т‘Р С•Р В±Р В°Р Р†Р В»Р ВµР Р…Р С‘Р С‘
void FormEdges::on_add(){
    formnewedge->show();
}

// Обновление ребра
void FormEdges::on_save()
{
    if (!dataModel->db.isOpen()){
        QMessageBox::information(0, "SQL UPDATE:", "Database isn't open!");
    }else{
        if (dataModel->updateEdge(ui->cb_src_private->currentText(),
                                  ui->cb_type_private->currentText(),
                                  ui->cb_dst_private->currentText(),
                                  ui->cb_quant_private->currentText(),
                                  QString::number(ui->spin_capacity->value()),
                                  QString::number(ui->spin_status->value()),
                                  QString::number(ui->spin_isNew->value()),
                                  ui->tvEdges->model()->data(ui->tvEdges->model()->index(ui->tvEdges->currentIndex().row(),0), 2).toString()))
        {
            dataModel->m_edges->select();
        }
    }
    ui->push_save->setEnabled(0);
    ui->push_revert->setEnabled(0);
    ui->push_del->setEnabled(0);
    //create_update_tree();
}


// Р СџРЎР‚Р С‘ Р С•РЎвЂљР СР ВµР Р…Р Вµ
void FormEdges::on_revert()
{
    QModelIndex current = ui->tvEdges->currentIndex();
    ui->cb_src->setCurrentIndex(ui->cb_src->findText((ui->tvEdges->model()->data(ui->tvEdges->model()->index(current.row(),1),2)).toString(),(Qt::MatchFixedString) ));
    ui->cb_type->setCurrentIndex(ui->cb_type->findText((ui->tvEdges->model()->data(ui->tvEdges->model()->index(current.row(),2),2)).toString(),(Qt::MatchFixedString) ));
    ui->cb_dst->setCurrentIndex(ui->cb_dst->findText((ui->tvEdges->model()->data(ui->tvEdges->model()->index(current.row(),3),2)).toString(),(Qt::MatchFixedString) ));
    ui->cb_quant->setCurrentIndex(ui->cb_dst->findText((ui->tvEdges->model()->data(ui->tvEdges->model()->index(current.row(),4),2)).toString(),(Qt::MatchFixedString) ));
    ui->spin_capacity->setValue(ui->tvEdges->model()->data(  ui->tvEdges->model()->index(current.row(),5)  , 2).toInt());
    ui->spin_status->setValue(ui->tvEdges->model()->data(  ui->tvEdges->model()->index(current.row(),6)  , 2).toInt());
    ui->spin_isNew->setValue(ui->tvEdges->model()->data(  ui->tvEdges->model()->index(current.row(),7)  , 2).toInt());

    ui->push_save->setEnabled(0);
    ui->push_revert->setEnabled(0);
}


// Р СџРЎР‚Р С‘ РЎС“Р Т‘Р В°Р В»Р ВµР Р…Р С‘Р С‘
void FormEdges::on_del()
{
    //dataModel->m_edges->removeRow(ui->tvEdges->currentIndex().row());
    //dataModel->m_edges->removeRows(ui->tvEdges->se);

    int count = ui->tvEdges->selectionModel()->selectedRows().count();
    for( int i = 0; i < count; i++)
    ui->tvEdges->model()->removeRow( ui->tvEdges->selectionModel()->selectedRows().at( i).row(), QModelIndex());

    dataModel->m_edges->submitAll();

    ui->push_del->setEnabled(0);
    ui->push_save->setEnabled(0);
    ui->push_revert->setEnabled(0);


    const QModelIndex &parent = QModelIndex();
    while (ui->tvEdges->model()->canFetchMore(parent))
       ui->tvEdges->model()->fetchMore(parent);
    //create_update_tree();
}


void FormEdges::on_node_input()
{

}

void FormEdges::on_node_output()
{

}



void FormEdges::open_add()
{
    ui->push_add->setEnabled(1);
}

void FormEdges::open_src()
{
//    if (ui->treeWidget_src->isHidden())
//    {
//        ui->treeWidget_src->show();
//    }
//    else
//    {
//        ui->treeWidget_src->hide();
//    }
}

void FormEdges::open_dst()
{
//    if (ui->treeWidget_dst->isHidden())
//    {
//        ui->treeWidget_dst->show();
//    }
//    else
//    {
//        //ui->treeWidget_dst->hide();
//    }
}



// Р СџРЎР‚Р С‘ Р С‘Р В·Р СР ВµР Р…Р ВµР Р…Р С‘Р С‘ Р Т‘Р В°Р Р…Р Р…РЎвЂ№РЎвЂ¦
void FormEdges::data_canged()
{
    if ((ui->cb_src->currentText()) != (ui->tvEdges->model()->data(ui->tvEdges->model()->index(ui->tvEdges->currentIndex().row(),1),2)).toString() ||
       (ui->cb_type->currentText()) != (ui->tvEdges->model()->data(ui->tvEdges->model()->index(ui->tvEdges->currentIndex().row(),2),2)).toString() ||
       (ui->cb_dst->currentText()) != (ui->tvEdges->model()->data(ui->tvEdges->model()->index(ui->tvEdges->currentIndex().row(),3),2)).toString() ||
       (ui->cb_quant->currentText()) != (ui->tvEdges->model()->data(ui->tvEdges->model()->index(ui->tvEdges->currentIndex().row(),4),2)).toString() ||
       (ui->spin_capacity->value()) != (ui->tvEdges->model()->data(  ui->tvEdges->model()->index(ui->tvEdges->currentIndex().row(),5)  , 2).toInt()) ||
       (ui->spin_status->value()) != (ui->tvEdges->model()->data(  ui->tvEdges->model()->index(ui->tvEdges->currentIndex().row(),6)  , 2).toInt()) ||
       (ui->spin_isNew->value()) != (ui->tvEdges->model()->data(  ui->tvEdges->model()->index(ui->tvEdges->currentIndex().row(),7)  , 2).toInt()))
    {
        ui->push_save->setEnabled(1);
        ui->push_revert->setEnabled(1);
    }
    else
    {
        ui->push_save->setEnabled(0);
        ui->push_revert->setEnabled(0);
    }
}

void FormEdges::src_tree_clicked(QModelIndex clicked)
{
   // qDebug() << clicked.data(2).toString();
}

void FormEdges::on_right_click(QPoint child)
{
   //qDebug() << ui->treeWidget_src->itemAt(child)->text(0);

   QPushButton *popupButton = new QPushButton/*(tr("Pop&up Button"))*/;
       QMenu *menu = new QMenu(this);
       menu->insertSeparator(0);

       QAction *remove = new QAction(tr("Remove node"), this);
       menu->addAction(remove);
       //node_for_delete = ui->treeWidget_src->itemAt(child)->text(0);
       connect(remove, SIGNAL(triggered()), SLOT(on_right_delete()));

       menu->insertSeparator(0);

       QAction *remove_all = new QAction(tr("Remove node with children"), this);
       menu->addAction(remove_all);
       //connect(remove_all, SIGNAL(triggered()), this, SLOT(on_right_delete_all()));

//       actionOpen->setShortcut(QKeySequence::Open);
       menu->insertSeparator(0);
       popupButton->setMenu(menu);menu->exec(QCursor::pos());

}

void FormEdges::on_right_delete()
{
    qDebug() << node_for_delete;

    QSqlQuery query;
    QString str;
    QString strF = "DELETE "
            "FROM [nodes] "
            "WHERE node_name='%0';";

    str = strF.arg(node_for_delete);



    if(!query.exec(str)) qDebug() << query.lastError().text();



    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    //// Р СџР  Р С›Р вЂР вЂєР вЂўР СљР С’ Р РЋ Р В­Р СћР С›Р в„ў Р РЋР СћР  Р С›Р В§Р С™Р С›Р в„ў  ///
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    dataModel->m_nodes->select();
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////
    //////////////////////////////////



    //create_update_tree();



    qDebug() << str;

}

//void FormEdges::on_right_delete_all(QString node)
//{

//    //create_update_tree();
//}

QAbstractItemModel *FormEdges::getmodel()
{
    return ui->tvEdges->model();
}
