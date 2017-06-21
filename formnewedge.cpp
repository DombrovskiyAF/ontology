#include "formnewedge.h"
#include "ui_formnewedge.h"

#include "formedges.h"
#include "formnodes.h"
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

FormNewEdge::FormNewEdge(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormNewEdge)
{
    ui->setupUi(this);
    setModal(1);

    mapper_src = new QDataWidgetMapper(this);
    mapper_src_priv = new QDataWidgetMapper(this);
    mapper_type = new QDataWidgetMapper(this);
    mapper_type_priv = new QDataWidgetMapper(this);
    mapper_dst = new QDataWidgetMapper(this);
    mapper_dst_priv = new QDataWidgetMapper(this);
    mapper_quant = new QDataWidgetMapper(this);
    mapper_quant_priv = new QDataWidgetMapper(this);
    mapper_src_type = new QDataWidgetMapper(this);
    mapper_dst_type = new QDataWidgetMapper(this);

    connect(ui->cb_src, SIGNAL(currentIndexChanged(int)),ui->cb_src_private,SLOT(setCurrentIndex(int)));
    connect(ui->cb_type, SIGNAL(currentIndexChanged(int)),ui->cb_type_private,SLOT(setCurrentIndex(int)));
    connect(ui->cb_dst, SIGNAL(currentIndexChanged(int)),ui->cb_dst_private,SLOT(setCurrentIndex(int)));
    connect(ui->cb_quant, SIGNAL(currentIndexChanged(int)),ui->cb_quant_private,SLOT(setCurrentIndex(int)));

    connect(ui->push_ontree_src,SIGNAL(clicked(bool)),SLOT(open_tree()));
    connect(ui->push_ontree_dst,SIGNAL(clicked(bool)),SLOT(open_tree()));

    connect(ui->cb_type,SIGNAL(currentIndexChanged(int)),SLOT(currentIndexChanged(int)));
    connect(ui->cb_src,SIGNAL(currentIndexChanged(int)),SLOT(for_cb_src()));
    connect(ui->cb_dst,SIGNAL(currentIndexChanged(int)),SLOT(for_cb_dst()));

    connect(ui->cb_type,SIGNAL(currentIndexChanged(int)),SLOT(checking()));

}

FormNewEdge::~FormNewEdge()
{
    delete mapper_src;
    delete mapper_src_priv;
    delete mapper_type;
    delete mapper_type_priv;
    delete mapper_dst;
    delete mapper_dst_priv;
    delete mapper_quant;
    delete mapper_quant_priv;
    delete mapper_src_type;
    delete mapper_dst_type;
    delete ui;
}

void FormNewEdge::currentIndexChanged(int i){
    ui->cb_require_dst->setCurrentIndex(i);
    ui->cb_require_src->setCurrentIndex(i);
}

void FormNewEdge::open_tree(){
    if ((QPushButton*)sender()==ui->push_ontree_src){
        who_call=1;
        qDebug () << "1";
        tree->create_update_tree(who_call,ui->cb_require_src->currentText(),ui->cb_type->currentText(),ui->cb_require_dst->currentText());

    }else{
        who_call=2;
        qDebug () << "2";
        tree->create_update_tree(who_call,ui->cb_require_dst->currentText(),ui->cb_type->currentText(),ui->cb_require_src->currentText());

    }
    tree->on_open(ui->cb_type->currentIndex());
    tree->show();

}

void FormNewEdge::for_cb_src(){
    QAbstractItemModel * qaim_form_nodes = dataModel->m_formNodes->getmodel();
    QString temp_node_name = ui->cb_src->currentText();
    for (int j = 0;j<qaim_form_nodes->rowCount();j++){
        if(qaim_form_nodes->index(j,2).data(2).toString()==temp_node_name){
            ui->label_src->setText(qaim_form_nodes->index(j,1).data(2).toString());
            if (ui->label_src->text()!=ui->cb_require_src->currentText()){
                ui->label_src->styleSheet().clear();
                ui->label_src->setStyleSheet("background-color:red;");
            }else{
                ui->label_src->styleSheet().clear();
                ui->label_src->setStyleSheet("background-color:green;");
            }
            break;
        }
    }
}

void FormNewEdge::for_cb_dst()
{
    QAbstractItemModel * qaim_form_nodes = dataModel->m_formNodes->getmodel();
    QString temp_node_name = ui->cb_dst->currentText();
    for (int j = 0;j<qaim_form_nodes->rowCount();j++){
        if(qaim_form_nodes->index(j,2).data(2).toString()==temp_node_name){
            ui->label_dst->setText(qaim_form_nodes->index(j,1).data(2).toString());
            if (ui->label_dst->text()!=ui->cb_require_dst->currentText()){
                ui->label_dst->styleSheet().clear();
                ui->label_dst->setStyleSheet("background-color:red;");
            }else{
                ui->label_dst->styleSheet().clear();
                ui->label_dst->setStyleSheet("background-color:green;");
            }
            break;
        }
    }
}

void FormNewEdge::setmappers()
{


//    mapper_type->setModel(dataModel->m_edges);
//    ui->cb_src_type->setModel(dataModel->m_edgeTypes);
//    ui->cb_src_type->setModelColumn(1);
//    mapper_type->addMapping(ui->cb_src_type, 1);

//    mapper_type->setModel(dataModel->m_edges);
//    ui->cb_dst_type->setModel(dataModel->m_edgeTypes);
//    ui->cb_dst_type->setModelColumn(3);
//    mapper_type->addMapping(ui->cb_dst_type, 1);


    mapper_src_type->setModel(dataModel->m_edges);
    ui->cb_require_src->setModel(dataModel->m_edgeTypes);
    ui->cb_require_src->setModelColumn(1);
    mapper_src_type->addMapping(ui->cb_require_src, 1);

    mapper_dst->setModel(dataModel->m_edges);
    ui->cb_require_dst->setModel(dataModel->m_edgeTypes);
    ui->cb_require_dst->setModelColumn(3);
    mapper_dst->addMapping(ui->cb_require_dst, 1);







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

    //tree->create_update_tree(0);
}

void FormNewEdge::from_child(QString str)
{
    if (who_call==1){
        for (int i=0;i<ui->cb_src->count();i++){
            if (ui->cb_src->itemText(i)==str) {ui->cb_src->setCurrentIndex(i);break;}

        }
    }else{
        for (int i=0;i<ui->cb_dst->count();i++){
            if (ui->cb_dst->itemText(i)==str) {ui->cb_dst->setCurrentIndex(i);break;}

        }
    }
}

void FormNewEdge::on_kkk_clicked()
{
    if ((ui->label_src->styleSheet()!="background-color:red;") && (ui->label_dst->styleSheet()!="background-color:red;")){
        if (!dataModel->db.isOpen())
        {
            QMessageBox::information(0, "SQL INSERT:", "Database isn't open!");
        }
        else
        {
//            QSqlQuery query;
//            QString str;
//            QString strF ="INSERT INTO edges "
//                    "(id_node_src,id_edge_type,id_node_dst,quant_id,capacity,status,isNew) VALUES "
//                    "(%1, %2, %3, '%4', %5, %6, %7);";
//            str = strF.arg(ui->cb_src_private->currentText())
//                      .arg(ui->cb_type_private->currentText())
//                      .arg(ui->cb_dst_private->currentText())
//                      .arg(ui->cb_quant_private->currentText())
//                      .arg(ui->spin_capacity->value())
//                      .arg(ui->spin_status->value())
//                      .arg(ui->spin_isNew->value());

//            if (!query.exec(str))
//            {
//                QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
//            }
//            else
//            {
//    //            QMessageBox::information(0, "SQL INSERT:", "Operation successfully!");
//                dataModel->m_edges->select();
//                //hide();
//            }

            dataModel->insertEdge(ui->cb_src_private->currentText(),ui->cb_type_private->currentText(),ui->cb_dst_private->currentText(),ui->cb_quant_private->currentText(),QString::number(ui->spin_capacity->value()),QString::number(ui->spin_status->value()),QString::number(ui->spin_isNew->value()));
        }
    }else{
        QMessageBox::warning(0,"Warning:","Incorrect types!");
    }
}

void FormNewEdge::on_cancel_clicked()
{
    hide();
}

void FormNewEdge::checking()
{
    if (ui->label_src->text()!=ui->cb_require_src->currentText()){
        ui->label_src->styleSheet().clear();
        ui->label_src->setStyleSheet("background-color:red;");
    }else{
        ui->label_src->styleSheet().clear();
        ui->label_src->setStyleSheet("background-color:green;");
    }
    if (ui->label_dst->text()!=ui->cb_require_dst->currentText()){
        ui->label_dst->styleSheet().clear();
        ui->label_dst->setStyleSheet("background-color:red;");
    }else{
        ui->label_dst->styleSheet().clear();
        ui->label_dst->setStyleSheet("background-color:green;");
    }
}
