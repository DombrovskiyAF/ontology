#include "similarity.h"
#include "ui_similarity.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QList>
#include <QDebug>
#include <datamodel.h>
#include <QSqlQuery>
#include <QSqlError>

Similarity::Similarity(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Similarity)
{
    ui->setupUi(this);

    children = 1; parents = 0;
}

Similarity::~Similarity()
{
    delete ui;
}

//void Similarity::on_push_add_line_clicked()
//{
//    QHBoxLayout *qhbl = new QHBoxLayout;                              // Новый горизонтальный слой
//    list_of_lays << qhbl;                                             // Запишем его в list_of_lays
//    ui->gridLayout->addLayout(qhbl,parents,0,Qt::AlignCenter);
//    QVector <QLineEdit*> qvle;
//    for (int i=0;i<children;i++){         // По общему количеству детей
//        QLineEdit *qle= new QLineEdit;    // Создаём необходимое количество лайнэдитов
//        qhbl->addWidget(qle);
//        qvle << qle;                                     //В этот лист записываем указатели на лайнэдиты
//    }
//    vec_of_parents << qvle;     //В вектор с предками вставляем новый вектор с детьми
//    ++parents; //Увеличиваем число предков (строк)
//}

//void Similarity::on_push_add_children_clicked()
//{
//    for (int i=0;i<list_of_lays.count();i++){ //в каждом ряду
//        QLineEdit *qle= new QLineEdit;        //добавляем по одному ребёнку
//        list_of_lays[i]->addWidget(qle);
//        vec_of_parents[i] << qle;             // и в наш массив аналогично
//    }
//    ++children; //Увеличиваем число детей (столбцов, лайнэдитов)
//}

//void Similarity::on_push_cancel_clicked()
//{
//    hide();
//}

//void Similarity::on_push_go_clicked()
//{
//    int rowCount = dataModel->m_edges->rowCount();
//    for (int i=0;i<parents;i++){
//        for (int j=0;j<children;j++){
//            //qDebug () << vec_of_parents[i][j]->text();

//            QString pred;
//            for (int k=1;k<rowCount;k++){
//            if (dataModel->m_edges->index(k,3).data(2).toString()==vec_of_parents[i][j]->text()){
//                qDebug () << vec_of_parents[i][j]->text() <<"->-"<< dataModel->m_edges->index(k,1).data(2).toString();
//                pred = dataModel->m_edges->index(k,1).data(2).toString();
//            }
//            }

//            int bol=0; do{
//            bol=0;
//            for (int k=1;k<rowCount;k++){
//                if (dataModel->m_edges->index(k,3).data(2).toString()==pred){
//                    qDebug () <<"->"<< dataModel->m_edges->index(k,1).data(2).toString();
//                    pred = dataModel->m_edges->index(k,1).data(2).toString();
//                    bol=1;
//                }
//            }
//            }while(bol);

//        }
//    }
//}

void Similarity::on_push_similarity_clicked()
{
    int count_of_nodes = dataModel->m_nodes->rowCount();//,i=0;
    int count_of_edges = dataModel->m_edges->rowCount();//,j=0;

    for (int i=0;i<count_of_nodes;i++){


        QSqlQuery query;
        QString str;
        QString strF ="UPDATE nodes SET "
                "nodes.lvl = %1 "
                "WHERE nodes.id=%7;";

        str = strF.arg(QString::number(if_have(dataModel->m_nodes->index(i,2).data(2).toString())))
                     .arg(dataModel->m_nodes->index(i,0).data(2).toString());

        if (!query.exec(str))
        {
          qDebug() << str;
          qDebug() << query.lastError();
          //QMessageBox::information(0, "SQL UPDATE:", query.lastError().text());
        }
        else
        {
          qDebug() << "SUCCESS!!!";
          qDebug() << str;
          //ui->tvNodes->setModel(dataModel->m_nodes);
          dataModel->m_nodes->select();
        }
    }


//    for (int j=0;j<count_of_edges;j++){
//        if_have(dataModel->m_nodes->index(i,2).data(2),dataModel->m_edges->index(j,1).data(2));
//    }
}

int Similarity::if_have(QString src)
{
    int count_of_edges = dataModel->m_edges->rowCount(),lvl=0;//,j=0;
    for (int j=0;j<count_of_edges;j++){
        if (src==dataModel->m_edges->index(j,1).data(2).toString()){
            lvl++;
            lvl+=if_have(dataModel->m_edges->index(j,3).data(2).toString());
            break;
        }
    }
    return lvl;
}


//    do{
//        if (dataModel->m_nodes->index(i,2).data(2)==(dataModel->m_edges->index(j))
//    }while()
//qDebug () << dataModel->m_nodes->index(1,1).data(2).toString();
    //                qDebug () << dataModel->m_edges->index(j,1).data(2);
