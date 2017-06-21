#include "formnodes.h"
#include "ui_formnodes.h"
#include "datamodel.h"
#include <QSqlRelationalDelegate>
#include <QSqlQuery>
#include <QDebug>
#include <QHeaderView>
#include <QSqlError>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QModelIndex>

FormNodes::FormNodes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormNodes)
{
    ui->setupUi(this);
    connect(ui->tvNodes,SIGNAL(pressed(QModelIndex)),this,SLOT(on_select(QModelIndex)));
    connect(ui->tvNEXT,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_edge_next_select(QModelIndex)));
    connect(ui->tvPREV,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(on_edge_prev_select(QModelIndex)));


    connect(ui->pbAdd,SIGNAL(clicked()),this,SLOT(on_add()));
    connect(ui->pbRevert,SIGNAL(clicked()),this,SLOT(on_revert()));
    connect(ui->pbDelete,SIGNAL(clicked()),this,SLOT(on_delete()));
    connect(ui->pbSave,SIGNAL(clicked()),this,SLOT(on_save()));
    connect(ui->lineFindText,SIGNAL(textEdited(QString)),this,SLOT(on_push_find()));

    QObject::connect(ui->cbNodeType, SIGNAL(currentIndexChanged(int)),ui->cbNodeType_private,SLOT(setCurrentIndex(int)));

    connect(ui->check_brightness_filter,SIGNAL(clicked(bool)),this,SLOT(on_push_filter()));
    connect(ui->check_status_filter,SIGNAL(clicked(bool)),this,SLOT(on_push_filter()));
    connect(ui->check_strong_search,SIGNAL(stateChanged(int)),this,SLOT(on_push_find()));

    ui->tvNodes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvNodes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvNEXT->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvNEXT->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tvPREV->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvPREV->setEditTriggers(QAbstractItemView::NoEditTriggers);

    m_mapper = new QDataWidgetMapper(this);
    m_mapper2 = new QDataWidgetMapper(this);

    ui->tvNodes->setItemDelegate( new QSqlRelationalDelegate( ui->tvNodes));


    m_mapper->setItemDelegate(new QSqlRelationalDelegate(this));
    edit_flag = 0;

    ui->tvNodes->setSortingEnabled(true);

    ui->lineEditAuthor->setPlaceholderText("Enter author name");
    ui->lineEditName->setPlaceholderText("Enter node name, then Save");
    ui->lineFindText->setPlaceholderText("Enter consept's name");

    pal_lineEdit = new QPalette(this->palette());

    ui->cbNodeType_private->hide();

}

FormNodes::~FormNodes()
{
    delete ui;
    delete m_mapper;
}

void FormNodes::setNodesModel(QAbstractItemModel *model)
{
    ui->tvNodes->setModel(model);
    const QModelIndex &parent = QModelIndex();
    while (model->canFetchMore(parent))
        model->fetchMore(parent);
    //ui->tvNodes->model()->canFetchMore();
    //ui->tvNodes->setColumnHidden(nodes_author,1);
    //ui->tvNodes->setColumnHidden(nodes_rem,1);

    ui->tvNodes->setItemDelegate(new QSqlRelationalDelegate(ui->tvNodes));
    m_mapper->setModel(model);

    ui->tvNodes->setCurrentIndex(ui->tvNodes->model()->index(0,0));
    emit on_select(ui->tvNodes->model()->index(0,0));
}

void FormNodes::on_pbTest_clicked()
{
    //QString name = ui->lineEditName->text();
    //QString typeName = ui->lineEditTypeName->text();
    //QString aut = QString::fromLocal8Bit("аффтор");

    //dataModel->addNodeToModel(name, typeName, 1, 1, aut, "remark");

    //dataModel->submitNodes();
}

void FormNodes::on_select(QModelIndex current)
{
    ui->cbNodeType->setCurrentIndex(ui->cbNodeType->findText((ui->tvNodes->model()->data(ui->tvNodes->model()->index(current.row(),1),2)).toString(),(Qt::MatchFixedString) ));


    ui->lineEditName->setText(ui->tvNodes->model()->data(   ui->tvNodes->model()->index(current.row(),2)   , 2).toString());
    ui->lineEditAuthor->setText(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(current.row(),5)  , 2).toString());
    ui->textEditRem->setText(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(current.row(),6)  , 2).toString());
    ui->spinBoxId->setValue(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(current.row(),0)  , 2).toInt());
    ui->spinBoxBrig->setValue(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(current.row(),3)  , 2).toInt());
    ui->spinBoxStatus->setValue(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(current.row(),4)  , 2).toInt());


    m_mapper->setModel(dataModel->m_nodes);
    ui->cbNodeType->setModel(dataModel->m_nodeTypes);
    ui->cbNodeType->setModelColumn(1);
    m_mapper->addMapping(ui->cbNodeType, 1);

    m_mapper2->setModel(dataModel->m_nodeTypes);
    ui->cbNodeType_private->setModel(dataModel->m_nodeTypes);
    m_mapper2->addMapping(ui->cbNodeType_private, 2);


//    ui->cbNodeType->setEnabled(0);
//    ui->lineEditAuthor->setEnabled(0);
//    ui->lineEditName->setEnabled(0);
//    ui->textEditRem->setEnabled(0);
//    ui->spinBoxBrig->setEnabled(0);
//    ui->spinBoxStatus->setEnabled(0);



    edit_flag = 0;

    ui->pbDelete->setEnabled(1);

//////////////////////////////////////////////////////////ВХОДЯЩИЕ � ЁБ� А
    QSqlQuery query;
    QString str;
//    QString strF ="SELECT edges.id, edges.id_node_src, edges.id_edge_type, edges.id_node_dst "
//            "FROM edges "
//            "WHERE (((edges.id_node_dst)=%1));";

    QString strF = "SELECT edges.id, node_types.node_type_name , nodes.node_name, edge_types.edge_name,edges.id_node_src "
                    "FROM edges, edge_types, nodes,node_types "
                    "WHERE (((edges.id_node_dst)=%1)) "
                    "AND (((edges.id_edge_type)=(edge_types.id))) "
                    "AND (((edges.id_node_src)=(nodes.id))) "
                    "AND (((node_types.id)=(nodes.id_node_type)));";
//, edges.id_node_dst ,  edges.id_edge_type,  edges.id_node_src,
  str = strF.arg(QString::number(ui->spinBoxId->value()));

  if (!query.exec(str))
  {
      qDebug() << str;
      qDebug() << query.lastError();
      qDebug() << "Unable to make insert operation";
  }
  else
  {

      QSqlQueryModel *model_prev = new QSqlQueryModel;
      model_prev->setQuery(str);
   //   model_prev->setHeaderData(0, Qt::Horizontal, tr("Name"));
    //  model_prev->setHeaderData(1, Qt::Horizontal, tr("Salary"));

      ui->tvPREV->setModel(model_prev);
      ui->tvPREV->setEditTriggers(QAbstractItemView::NoEditTriggers);
      ui->tvPREV->setItemDelegate( new QSqlRelationalDelegate( ui->tvPREV));
      m_mapper->setItemDelegate(new QSqlRelationalDelegate(this));
  }


  /////////////////////////////////////////////////ИСХОДЯЩИЕ � ЁБ� А
      QSqlQuery query2;
      QString str2;
      QString strF2 = "SELECT edges.id, edge_types.edge_name , node_types.node_type_name , nodes.node_name, edges.id_node_dst "
                      "FROM edges, edge_types, nodes,node_types "
                      "WHERE (((edges.id_node_src)=%1)) "
                      "AND (((edges.id_edge_type)=(edge_types.id))) "
                      "AND (((edges.id_node_dst)=(nodes.id))) "
                      "AND (((node_types.id)=(nodes.id_node_type)));";
// edges.id_node_src, edges.id_edge_type, edges.id_node_dst,
    str2 = strF2.arg(QString::number(ui->spinBoxId->value()));

    if (!query2.exec(str))
    {
        qDebug() << str2;
        qDebug() << query2.lastError();
        qDebug() << "Unable to make insert operation";

        QStandardItemModel *models = new QStandardItemModel(1,1, ui->tvNEXT);
        ui->tvNEXT->setModel(models);
        ui->tvPREV->setModel(models);
        QStandardItem *newItem = new QStandardItem("ERROR. Please reload database...");
        models->setItem(0, 0, newItem);

    }
    else
    {

        QSqlQueryModel *model_next = new QSqlQueryModel;

        model_next->setQuery(str2);

        //model_next

        ui->tvNEXT->setItemDelegate( new QSqlRelationalDelegate(ui->tvNEXT));
        ui->tvNEXT->setModel(model_next);
        ui->tvNEXT->setEditTriggers(QAbstractItemView::NoEditTriggers);
    }

///////////////////////////////////////////////////////////////////////////////////////
    ui->tvPREV->hideColumn(4);
    ui->tvNEXT->hideColumn(4);

    add_flag = 0;

}

void FormNodes::on_save()
{
            QSqlQuery query;
            QString str;
            QString strF ="UPDATE nodes SET "
                    "nodes.id_node_type = %1, "
                    "nodes.node_name = '%2', "
                    "nodes.brightness = %3, "
                    "nodes.status = %4, "
                    "nodes.author = '%5', "
                    "nodes.rem = '%6' "
                    "WHERE nodes.id=%7;";

            str = strF.arg(ui->cbNodeType_private->currentText())
                         .arg(ui->lineEditName->text())
                         .arg(QString::number(ui->spinBoxBrig->value()))
                         .arg(QString::number(ui->spinBoxStatus->value()))
                         .arg(ui->lineEditAuthor->text())
                         .arg(ui->textEditRem->toPlainText())
                         .arg(QString::number(ui->spinBoxId->value()));

            if (!query.exec(str))
            {
              qDebug() << str;
              qDebug() << query.lastError();
              QMessageBox::information(0, "SQL UPDATE:", query.lastError().text());
            }
            else
            {
              qDebug() << "SUCCESS!!!";
              qDebug() << str;
              ui->tvNodes->setModel(dataModel->m_nodes);
              dataModel->m_nodes->select();
//              QMessageBox::information(0, "SQL UPDATE:", "Operation successfully!");
            }

            ui->tvNodes->update(ui->tvNodes->currentIndex());
}




void FormNodes::on_revert()
{
    int row = ui->tvNodes->currentIndex().row();
    ui->lineEditName->setText(ui->tvNodes->model()->data(   ui->tvNodes->model()->index(row,2)   , 2).toString());
    ui->lineEditAuthor->setText(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(row,5)  , 2).toString());
    ui->textEditRem->setText(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(row,6)  , 2).toString());
    ui->spinBoxId->setValue(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(row,0)  , 2).toInt());
    ui->spinBoxBrig->setValue(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(row,3)  , 2).toInt());
    ui->spinBoxStatus->setValue(ui->tvNodes->model()->data(  ui->tvNodes->model()->index(row,4)  , 2).toInt());

//    ui->cbNodeType->setCurrentIndex(0);
//    ui->lineEditAuthor->setText("");
//    ui->lineEditName->setText("");
//    ui->textEditRem->setText("");
//    ui->spinBoxBrig->setValue(0);
//    ui->spinBoxStatus->setValue(0);

//    ui->tvNEXT->setModel(0);
//    ui->tvPREV->setModel(0);
//    ui->tvNodes->clearSelection();
}


void FormNodes::on_delete()
{

    int count = ui->tvNodes->selectionModel()->selectedRows().count();
    for( int i = 0; i < count; i++)
    ui->tvNodes->model()->removeRow( ui->tvNodes->selectionModel()->selectedRows().at(i).row(), QModelIndex());


    int i = 0;
    QList <QString> list_of_del_next;
    while (ui->tvNEXT->model()->index(i,0).data().toBool())
    {
        list_of_del_next << ui->tvNEXT->model()->index(i,0).data().toString();
        i++;
    }

    i = 0;
    while (i<list_of_del_next.size())
    {

        QSqlQuery query;
        QString str;
        QString strF = "DELETE FROM edges WHERE id=%0;";

        str = strF.arg(list_of_del_next.at(i));

        query.exec(str);

        i++;
    }

    qDebug() << "=====================================";

    i = 0;
    QList <QString> list_of_del_prev;
    while (ui->tvPREV->model()->index(i,0).data().toBool())
    {
        list_of_del_prev << ui->tvPREV->model()->index(i,0).data().toString();
        i++;
    }


    i = 0;
    while (i<list_of_del_prev.size())
    {
        QSqlQuery query2;
        QString str2;
        QString strF2 = "DELETE FROM edges WHERE id=%0;";

        str2 = strF2.arg(list_of_del_prev.at(i));

        query2.exec(str2);

        i++;
    }

    int row = ui->tvNodes->currentIndex().row();
    dataModel->m_nodes->removeRow(row);
    dataModel->m_nodes->submitAll();
    dataModel->m_edges->select();

    ui->lineEditAuthor->clear();
    ui->lineEditName->clear();
    ui->textEditRem->clear();
    ui->pbDelete->setEnabled(0);

    ui->spinBoxBrig->setValue(0);
    ui->spinBoxId->setValue(0);
    ui->spinBoxStatus->setValue(0);

    ui->tvNEXT->setModel(0);
    ui->tvPREV->setModel(0);

}

void FormNodes::on_edge_prev_select(QModelIndex current3)
{
    int i = 0;
    while (i<ui->tvNodes->model()->rowCount())
    {
        qDebug() << ui->tvPREV->model()->index(current3.row(),4).data(2).toInt();
        if (ui->tvPREV->model()->index(current3.row(),4).data(2).toInt()==ui->tvNodes->model()->index(i,0).data(2).toInt())
        {
           qDebug() << ui->tvPREV->model()->index(current3.row(),4).data(2).toInt();
          // qDebug() << ui->tvNodes->model()->index(i,0).data(2).toInt();
           ui->tvNodes->setCurrentIndex(ui->tvNodes->model()->index(i,0));
           emit on_select(ui->tvNodes->model()->index(i,0));
           break;
        }
        i++;
    }
}

void FormNodes::on_edge_next_select(QModelIndex current2)
{
     int i = 0;
     while (i<ui->tvNodes->model()->rowCount())
     {
         if (ui->tvNEXT->model()->index(current2.row(),4).data(2).toInt()==ui->tvNodes->model()->index(i,0).data(2).toInt())
         {
            qDebug() << ui->tvNEXT->model()->index(current2.row(),4).data(2).toInt();
            qDebug() << ui->tvNodes->model()->index(i,0).data(2).toInt();
            ui->tvNodes->setCurrentIndex(ui->tvNodes->model()->index(i,0));
            emit on_select(ui->tvNodes->model()->index(i,0));
            break;
         }
         i++;
     }

}


void FormNodes::activate()
{
    ui->tvNodes->setEnabled(1);
    ui->pbAdd->setEnabled(1);
    ui->pbDelete->setEnabled(1);
    ui->tvNEXT->setEnabled(1);
    ui->tvPREV->setEnabled(1);
}

void FormNodes::deactivate()
{
    ui->tvNodes->setEnabled(0);
    ui->pbAdd->setEnabled(0);
    ui->pbDelete->setEnabled(0);
}

void FormNodes::on_push_filter()
{
    if ((ui->check_brightness_filter->isChecked()==1) && (ui->check_status_filter->isChecked()==1))
    {
        ui->tvNodes->setModel(0);
        ui->tvNodes->setModel(dataModel->m_nodes);

        bool match = false;
        int filter = ui->spin_brightness->value();
        int filter2 = ui->spin_status->value();

        for( int i = 0; i < ui->tvNodes->model()->rowCount(); ++i )
        {
            match = false;

            QModelIndex item = ui->tvNodes->model()->index(i, 3);
            QModelIndex item2 = ui->tvNodes->model()->index(i, 4);

            if((item.data(2).toInt() == filter) && (item2.data(2).toInt()== filter2))
            {
                match = true;
            }
            ui->tvNodes->setRowHidden( i, !match );
        }
    }
    else if ((ui->check_brightness_filter->isChecked()==1) && (ui->check_status_filter->isChecked()==0))
    {
        ui->tvNodes->setModel(0);
        ui->tvNodes->setModel(dataModel->m_nodes);

        bool match = false;
        int filter = ui->spin_brightness->value();
        for( int i = 0; i < ui->tvNodes->model()->rowCount(); ++i )
        {
            match = false;
            QModelIndex item = ui->tvNodes->model()->index(i, 3);
            qDebug() << item.data();
            if(item.data(2).toInt()== filter)
            {
                match = true;
            }
            ui->tvNodes->setRowHidden( i, !match );
        }
    }
    else if ((ui->check_brightness_filter->isChecked()==0) && (ui->check_status_filter->isChecked()==1))
    {
        ui->tvNodes->setModel(0);
        ui->tvNodes->setModel(dataModel->m_nodes);

        bool match = false;
        int filter = ui->spin_status->value();
        for( int i = 0; i < ui->tvNodes->model()->rowCount(); ++i )
        {
            match = false;
            QModelIndex item = ui->tvNodes->model()->index(i, 4);
            qDebug() << item.data();
            if(item.data(2).toInt()== filter)
            {
                match = true;
            }
            ui->tvNodes->setRowHidden( i, !match );
        }

    }
    else
    {
        ui->tvNodes->setModel(0);
        ui->tvNodes->setModel(dataModel->m_nodes);
    }

}

void FormNodes::on_push_find()
{
    if (ui->lineFindText->text()!="")
    {
    if (ui->check_strong_search->isChecked())
    {
      if (ui->lineFindText->text()!="")
      {
        QVariant val = ui->lineFindText->text(); // строка для поиска
        QModelIndexList list = ui->tvNodes->model()->match(ui->tvNodes->model()->index(0, 2), 0, val,1, Qt::MatchFixedString);
        if (!list.isEmpty())
        {
            ui->tvNodes->selectionModel()->setCurrentIndex(list.first(),QItemSelectionModel::Rows|QItemSelectionModel::Select|QItemSelectionModel::ClearAndSelect);
            ui->tvNodes->scrollTo(ui->tvNodes->model()->index(list.first().row(), 2));
            emit on_select(ui->tvNodes->model()->index(list.first().row(), 2));
            pal_lineEdit->setColor(QPalette::Base, Qt::green); //цвет фона
            ui->lineFindText->setPalette(*pal_lineEdit);
            return;
        }
        pal_lineEdit->setColor(QPalette::Base, Qt::red); //цвет фона
        ui->lineFindText->setPalette(*pal_lineEdit);
      }
      else
      {
            pal_lineEdit->setColor(QPalette::Base, Qt::white); //цвет фона
            ui->lineFindText->setPalette(*pal_lineEdit);
      }
    }
    else
    {
      if (ui->lineFindText->text()!="")
      {
        QVariant val = ui->lineFindText->text(); // строка для поиска
        QModelIndexList list = ui->tvNodes->model()->match(ui->tvNodes->model()->index(0, 2), 0, val,1, Qt::MatchContains);
        if (!list.isEmpty())
        {
            ui->tvNodes->selectionModel()->setCurrentIndex(list.first(),QItemSelectionModel::Rows|QItemSelectionModel::Select|QItemSelectionModel::ClearAndSelect);
            ui->tvNodes->scrollTo(ui->tvNodes->model()->index(list.first().row(), 2));
            emit on_select(ui->tvNodes->model()->index(list.first().row(), 2));
            pal_lineEdit->setColor(QPalette::Base, Qt::green); //цвет фона
            ui->lineFindText->setPalette(*pal_lineEdit);
            return;
        }
        pal_lineEdit->setColor(QPalette::Base, Qt::red); //цвет фона
        ui->lineFindText->setPalette(*pal_lineEdit);
      }
      else
      {
            pal_lineEdit->setColor(QPalette::Base, Qt::white); //цвет фона
            ui->lineFindText->setPalette(*pal_lineEdit);
      }
    }
    }
}

void FormNodes::on_tvNodes_activated(const QModelIndex &index)
{
    ui->tvNodes->setModel(dataModel->m_nodes);
    dataModel->m_nodes->select();
}

QAbstractItemModel *FormNodes::getmodel(){
    return ui->tvNodes->model();
}

void FormNodes::on_pbAdd_clicked(){
    formnewnode->show();
}
