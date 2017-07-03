#include "tree.h"
#include "ui_tree.h"
#include <datamodel.h>
#include <formedges.h>
#include <formnewedge.h>
#include <formnodes.h>
#include <QDebug>
#include <QPalette>

Tree::Tree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tree)
{
    ui->setupUi(this);
    setModal(1);
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(itemClicked(QTreeWidgetItem*,int)));
    mapper_type = new QDataWidgetMapper(this);
    mapper_src = new QDataWidgetMapper(this);
    mapper_dst = new QDataWidgetMapper(this);
    connect(ui->cb_type,SIGNAL(currentIndexChanged(int)),SLOT(currentIndexChanged(int)));
}

Tree::~Tree()
{
    delete mapper_type;
    delete mapper_src;
    delete mapper_dst;

    delete ui;
}

void Tree::create_update_tree(int who,QString src,QString type,QString dst)
{
    ui->treeWidget->clear();
    ListOfNodesForTree.clear();
    ListOfSrcEdgesForTree.clear();
    ListOfDstEdgesForTree.clear();

    whoiscall=who;
    if (who==1){
        ui->cb_dst_type->setStyleSheet("background-color:white;");
        ui->cb_src_type->setStyleSheet("background-color:lightgreen;");
    }else{
        ui->cb_src_type->setStyleSheet("background-color:white;");
        ui->cb_dst_type->setStyleSheet("background-color:lightgreen;");
    }



    mapper_type->setModel(dataModel->m_edges);
    ui->cb_type->setModel(dataModel->m_edgeTypes);
    ui->cb_type->setModelColumn(2);
    mapper_type->addMapping(ui->cb_type, 1);

    mapper_type->setModel(dataModel->m_edges);
    ui->cb_src_type->setModel(dataModel->m_edgeTypes);
    ui->cb_src_type->setModelColumn(1);
    mapper_type->addMapping(ui->cb_src_type, 1);

    mapper_type->setModel(dataModel->m_edges);
    ui->cb_dst_type->setModel(dataModel->m_edgeTypes);
    ui->cb_dst_type->setModelColumn(3);
    mapper_type->addMapping(ui->cb_dst_type, 1);







       int count = dataModel->m_nodes->rowCount();
       for (int i = 0;i<count;i++){// Заполняется лист всех узлов
           if (dataModel->m_nodes->index(i,1).data(2).toString()==src){
                //ListOfNodesForTree << dataModel->m_nodes->index(i,2).data(2).toString();
                int prov=0;
                for (int y=0;y<dataModel->m_edges->rowCount();y++){
                    if ((dataModel->m_edges->index(y,1).data(2).toString()==dataModel->m_nodes->index(i,2).data(2).toString()) && (dataModel->m_edges->index(y,2).data(2).toString()==type)){
                        prov=1;
                    }
                }
                if (prov==0) ListOfSrcEdgesForTree << dataModel->m_nodes->index(i,2).data(2).toString();
           }

       }

//       count = dataModel->m_edges->rowCount();
//       for (int i = 0;i<count;i++){// Заполняется лист конечных узлов
//           if ((dataModel->m_edges->index(i,2).data(2).toString()==type) && (dataModel->m_edges->index(i,3).data(2).toString()==src))
//            ListOfDstEdgesForTree << dataModel->m_edges->index(i,1).data(2).toString();
//       }



//       count = ListOfNodesForTree.size(); // По числу всех узлов
//       for (int i = 0;i<count;i++)
//       {
//           bool flag = 0;
//           int count2 = ListOfDstEdgesForTree.size();
//           for (int j = 0;j<count2;j++)
//           {// если узел (каждый из Всех) равен узлу(конечному) то он
//               //не записывается в исходные узлы (пропускается)
//               if (ListOfNodesForTree.at(i)==ListOfDstEdgesForTree.at(j)) //
//               {
//                    flag = 1;
//               }
//           }
//           if (flag==0) ListOfSrcEdgesForTree << ListOfNodesForTree.at(i);
//       }

       // добавляем все исходные узлы на дерево
       count = ListOfSrcEdgesForTree.size();
       for (int i = 0;i<count;i++)
       {
           QTreeWidgetItem *topLevelItem = new QTreeWidgetItem(ui->treeWidget);
           // вешаем его на наше дерево в качестве топ узла.
           ui->treeWidget->addTopLevelItem(topLevelItem);
           // укажем текст итема
           topLevelItem->setText(0, ListOfSrcEdgesForTree.at(i));

           QAbstractItemModel * qaim_form_nodes = dataModel->m_formNodes->getmodel();
           QString temp_node_name = ListOfSrcEdgesForTree.at(i);
           for (int j = 0;j<qaim_form_nodes->rowCount();j++){

               if(qaim_form_nodes->index(j,2).data(2).toString()==temp_node_name){
                   QString temp_node_type = qaim_form_nodes->index(j,1).data(2).toString();
                   topLevelItem->setText(1, qaim_form_nodes->index(j,1).data(2).toString());
                   if((temp_node_type==ui->cb_src_type->currentText()) && (who==1)) topLevelItem->setBackgroundColor(1,Qt::green);
                   if((temp_node_type==ui->cb_dst_type->currentText()) && (who==2)) topLevelItem->setBackgroundColor(1,Qt::green);
                   break;
               }
               qDebug () << topLevelItem->text(1) << " " << ui->cb_src_type->currentText();
           }
       }
}

void Tree::itemClicked(QTreeWidgetItem *current, int cur_i)
{
    if (current->childCount()==0)
    {
        QAbstractItemModel * qaim_form_edges = dataModel->m_formEdges->getmodel();
        QAbstractItemModel * qaim_form_nodes = dataModel->m_formNodes->getmodel();
        for (int i = 0;i<qaim_form_edges->rowCount();i++){
            if (qaim_form_edges->index(i,3).data(2).toString() == current->text(0) && (qaim_form_edges->index(i,2).data(2).toString()==ui->cb_type->currentText())){

                QTreeWidgetItem *item=new QTreeWidgetItem(current);
                item->setText(0, qaim_form_edges->index(i,1).data(2).toString());

                QString temp_node_name = qaim_form_edges->index(i,1).data(2).toString();
                for (int j = 0;j<qaim_form_nodes->rowCount();j++){
                    if(qaim_form_nodes->index(j,2).data(2).toString()==temp_node_name){
                        QString temp_node_type = qaim_form_nodes->index(j,1).data(2).toString();
                        item->setText(1, qaim_form_nodes->index(j,1).data(2).toString());
                        if((temp_node_type==ui->cb_src_type->currentText()) && (whoiscall==1)) item->setBackgroundColor(1,Qt::green);
                        if((temp_node_type==ui->cb_dst_type->currentText()) && (whoiscall==2)) item->setBackgroundColor(1,Qt::green);

                        break;
                    }
                }
            }
        }
    }
}

void Tree::currentIndexChanged(int i)
{
    ui->cb_src_type->setCurrentIndex(i);
    ui->cb_dst_type->setCurrentIndex(i);
}

void Tree::on_kk_clicked()
{
    dataModel->m_formEdges->formnewedge->from_child(ui->treeWidget->currentItem()->text(0));
    this->hide();
}

void Tree::on_no_clicked()
{
    this->hide();
    int a=2,b=4;
}

void Tree::on_open(int i){
    ui->cb_type->setCurrentIndex(i);
}
