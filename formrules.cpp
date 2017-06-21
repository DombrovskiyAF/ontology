#include "formrules.h"
#include "ui_formrules.h"
#include "datamodel.h"

#include "QDebug"
#include <QString>

#include <QSqlRelationalDelegate>
#include <QMessageBox>
#include "QSqlRecord"
#include <QVector>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QStringList>


FormRules::FormRules(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormRules)
{
    ui->setupUi(this);
    connect(this,SIGNAL(signalsendfields(int)),ruleeditwiget,SLOT(slotgetfield(int)));
    connect(ruleaddwiget,SIGNAL(signaladdrule(QString,QString,QString,QString)),
            this,SLOT(slotaddrule(QString,QString,QString,QString)));
    connect(ruleeditwiget,SIGNAL(signaleditrule(QString,QString,QString,QString)),
            this,SLOT(sloteditrule(QString,QString,QString,QString)));

    ui->tvRules->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tvRules->setEditTriggers(QAbstractItemView::NoEditTriggers);


    m_mapper = new QDataWidgetMapper(this);
    ui->l_done1->setVisible(0);
    ui->l_done2->setVisible(0);



}
QVector<int> edgeType;

QList <QStandardItem*> row;
FormRules::~FormRules()
{
    delete ui;
    delete m_mapper;
}


QString FormRules::replaceNumbersEdges(int i, QString string)
{
    switch(i)
    {
    case 1 :
    {
        QSqlQuery query;
        QString str="SELECT rule_name FROM rules WHERE id="+string+";";
        if(query.exec(str))
        {
            query.next();
            return query.value(0).toString();
        }
    }
        break;
    case 3 :
    {
        QSqlQuery query;
        QString str="SELECT edge_name FROM edge_types WHERE id="+string+";";
        if(query.exec(str))
        {
            query.next();
            return query.value(0).toString();
        }
    }
        break;
    case 2 :
    {
        QSqlQuery query,q2;
        QString str="SELECT node_type_id,node_name FROM rules_nodes WHERE id="+string+";";
        if(query.exec(str))
        {
            query.next();
            str="SELECT node_type_name FROM node_types WHERE id="+query.value(0).toString()+";";
            if(q2.exec(str))
            {
                q2.next();
                return q2.value(0).toString()+" "+query.value(1).toString();
            }
        }
        else qDebug()<<"DB error2";
    }
        break;
    case 4 :
    {
        QSqlQuery query,q2;
        QString str="SELECT node_type_id,node_name FROM rules_nodes WHERE id="+string+";";
        if(query.exec(str))
        {
            query.next();
            str="SELECT node_type_name FROM node_types WHERE id="+query.value(0).toString()+";";
            if(q2.exec(str))
            {
                q2.next();
                return q2.value(0).toString()+" "+query.value(1).toString();
            }
        }
        else qDebug()<<"DB error4";
    }
        break;
    default:
        return string;
        break;
    }
}


QString FormRules::replaceNumbersNodes(int i, QString string) //заменяем цифры из таблиц словами из связанных с ними таблиц
{
    switch(i)
    {
    case 1 :
    {
        QSqlQuery query;
        QString str="SELECT rule_name FROM rules WHERE id="+string+";";
        if(query.exec(str))
        {
            query.next();
            return query.value(0).toString();
        }
    }
        break;
    case 2 :
    {
        QSqlQuery query;
        QString str="SELECT node_type_name FROM node_types WHERE id="+string+";";
        if(query.exec(str))
        {
            query.next();
            return query.value(0).toString();
        }
        else qDebug()<<"DB error5";
    }
        break;
    default:
        return string;
        break;
    }
}

void FormRules::setRulesModel(QSqlRelationalTableModel *model)
{    
    tmp=model;
    ui->tvRules->setModel(tmp);
    ui->tvRules->setItemDelegate(new QSqlRelationalDelegate(ui->tvRules)); 
    ui->tvRules->setColumnHidden(3,true);
    tmp->sort(0,Qt::AscendingOrder);
    m_mapper->setModel(tmp);
    tmp->setHeaderData(0,Qt::Horizontal,"ID");
    tmp->setHeaderData(1,Qt::Horizontal,"Правило");
    tmp->setHeaderData(2,Qt::Horizontal,"Автор");
    tmp->select();
}


QVector<int> FormRules::setEdgeTypeCB(QSqlRelationalTableModel *model) //todo
{
   /* model->sort(0,Qt::AscendingOrder);
    model->select();
    int i=0;
    while(!model->record(i).isNull(0))
    {
        edgeType.append(model->record(i).value(0).toInt());
        i++;
    }*/
    return edgeType;

}


void FormRules::on_tvRules_clicked(const QModelIndex &index)
{
    qDebug()<< index;

    ui->cb_A->clear();
    ui->cb_B->clear();
    ui->cb_C->clear();
    ui->cb_A_2->clear();
    ui->cb_B_2->clear();
    ui->cb_C_2->clear();
    ui->cb_D->clear();
    ui->cb_AB->clear();
    ui->cb_AB_2->clear();
    ui->cb_BC->clear();
    ui->cb_BC_2->clear();
    ui->cb_AC->clear();
    ui->cb_AD->clear();
    ui->cb_DC->clear();
    ui->cb_BD->clear();
    ui->cb_quant_AB->clear();
    ui->cb_quant_AB_2->clear();
    ui->cb_quant_AC->clear();
    ui->cb_quant_AD->clear();
    ui->cb_quant_BC->clear();
    ui->cb_quant_BC_2->clear();
    ui->cb_quant_DC->clear();
    ui->cb_quant_BD->clear();

    //new
    QSqlQuery q;
    QString str="SELECT * FROM node_types";
    if(q.exec(str))
    while(q.next())
    {
        ui->cb_A->addItem(q.value("node_type_name").toString(),q.value("id").toInt());
        ui->cb_B->addItem(q.value("node_type_name").toString(),q.value("id").toInt());
        ui->cb_C->addItem(q.value("node_type_name").toString(),q.value("id").toInt());
        ui->cb_A_2->addItem(q.value("node_type_name").toString(),q.value("id").toInt());
        ui->cb_B_2->addItem(q.value("node_type_name").toString(),q.value("id").toInt());
        ui->cb_D->addItem(q.value("node_type_name").toString(),q.value("id").toInt());
        ui->cb_C_2->addItem(q.value("node_type_name").toString(),q.value("id").toInt());
    }
    else qDebug()<<"on_tvRules_clicked"<<q.lastError().text();
    str="SELECT * FROM quantifiers";
    if(q.exec(str))
    while(q.next())
    {
        ui->cb_quant_AB->addItem(q.value("quant_name").toString(),q.value("id").toInt());
        ui->cb_quant_AB_2->addItem(q.value("quant_name").toString(),q.value("id").toInt());
        ui->cb_quant_AC->addItem(q.value("quant_name").toString(),q.value("id").toInt());
        ui->cb_quant_AD->addItem(q.value("quant_name").toString(),q.value("id").toInt());
        ui->cb_quant_BC->addItem(q.value("quant_name").toString(),q.value("id").toInt());
        ui->cb_quant_BC_2->addItem(q.value("quant_name").toString(),q.value("id").toInt());
        ui->cb_quant_DC->addItem(q.value("quant_name").toString(),q.value("id").toInt());
        ui->cb_quant_BD->addItem(q.value("quant_name").toString(),q.value("id").toInt());
    }
    else qDebug()<<"on_tvRules_clicked"<<q.lastError().text();
    dataModel->LoadRulesList();
    QList<VzRule*> *rules = dataModel->getRulesList();
    QList<VzRule*>::iterator i;
    for (i = rules->begin(); i != rules->end(); ++i)
    {
        VzRule* r = *i;
        if(r->getId()==tmp->record(index.row()).value(0).toInt())
        {
            if(r->getM_AB_typeId()>0)
            {
                if(r->getRuleType()==1)
                {
                    ui->tabWidget->setCurrentWidget(ui->tab_3);
                    ui->cb_A->setCurrentIndex(ui->cb_A->findData(r->getM_A_typeId()));
                    ui->cb_B->setCurrentIndex(ui->cb_B->findData(r->getM_B_typeId()));
                    ui->cb_C->setCurrentIndex(ui->cb_C->findData(r->getM_C_typeId()));
                    selected1();
                    ui->cb_AB->setCurrentIndex(ui->cb_AB->findData(r->getM_AB_typeId()));
                    ui->cb_BC->setCurrentIndex(ui->cb_BC->findData(r->getM_BC_typeId()));
                    ui->cb_AC->setCurrentIndex(ui->cb_AC->findData(r->getM_AC_typeId()));
                    ui->cb_AB_direction->setCurrentIndex(r->getM_AB_direct() ? 0 : 1);
                    ui->cb_BC_direction->setCurrentIndex(r->getM_BC_direct() ? 0 : 1);
                    ui->cb_AC_direction->setCurrentIndex(r->getM_AC_direct() ? 0 : 1);
                    ui->cb_quant_AB->setCurrentIndex(r->getM_AB_Quant()-1);
                    ui->cb_quant_BC->setCurrentIndex(r->getM_BC_Quant()-1);
                    ui->cb_quant_AC->setCurrentIndex(r->getM_AC_Quant()-1);
                }
            else if(r->getRuleType()==2)
            {
                ui->tabWidget->setCurrentWidget(ui->tab_4);
                ui->cb_A_2->setCurrentIndex(ui->cb_A_2->findData(r->getM_A_typeId()));
                ui->cb_B_2->setCurrentIndex(ui->cb_B_2->findData(r->getM_B_typeId()));
                ui->cb_C_2->setCurrentIndex(ui->cb_C_2->findData(r->getM_C_typeId()));
                ui->cb_D->setCurrentIndex(ui->cb_D->findData(r->getM_D_typeId()));
                selected2();
                ui->cb_AB_2->setCurrentIndex(ui->cb_AB_2->findData(r->getM_AB_typeId()));
                ui->cb_BC_2->setCurrentIndex(ui->cb_BC_2->findData(r->getM_BC_typeId()));
                ui->cb_AD->setCurrentIndex(ui->cb_AD->findData(r->getM_AD_typeId()));
                ui->cb_DC->setCurrentIndex(ui->cb_DC->findData(r->getM_DC_typeId()));
                ui->cb_BD->setCurrentIndex(ui->cb_BD->findData(r->getM_BD_typeId()));
                ui->cb_AB_direction_2->setCurrentIndex(r->getM_AB_direct() ? 0 : 1);
                ui->cb_BC_direction_2->setCurrentIndex(r->getM_BC_direct() ? 0 : 1);
                ui->cb_AD_direction->setCurrentIndex(r->getM_AD_direct() ? 0 : 1);
                ui->cb_DC_direction->setCurrentIndex(r->getM_DC_direct() ? 0 : 1);
                ui->cb_BD_direction->setCurrentIndex(r->getM_BD_direct() ? 0 : 1);
                ui->cb_quant_AB_2->setCurrentIndex(r->getM_AB_Quant()-1);
                ui->cb_quant_BC_2->setCurrentIndex(r->getM_BC_Quant()-1);
                ui->cb_quant_AD->setCurrentIndex(r->getM_AD_Quant()-1);
                ui->cb_quant_DC->setCurrentIndex(r->getM_DC_Quant()-1);
                ui->cb_quant_BD->setCurrentIndex(r->getM_BD_Quant()-1);
                ui->cb_adON->setChecked(r->getM_AD_typeId()>0 ? true : false);
                ui->cb_bdON->setChecked(r->getM_BD_typeId()>0 ? true : false);
                ui->cb_dcON->setChecked(r->getM_DC_typeId()>0 ? true : false);
            }
            }

        }
    }
}

void FormRules::on_pb_confirm_clicked()
{    
    QMessageBox* pmbx =new QMessageBox(QMessageBox::Question,"Удаление","Точно удалить?",
    QMessageBox::Yes|QMessageBox::No);
    int n = pmbx->exec();
    delete pmbx;
    if (n == QMessageBox::Yes) {
    QString str="DELETE FROM rules_edges WHERE id_rules="+
            tmp->data(tmp->index(ui->tvRules->currentIndex().row(),0)).toString()+";";
    QSqlQuery query;
    if(!query.exec(str))
        qDebug()<<"ERROR rule edges deleting";
    str="DELETE FROM rules_nodes WHERE id_rule="+
                tmp->data(tmp->index(ui->tvRules->currentIndex().row(),0)).toString()+";";
    if(!query.exec(str))
        qDebug()<<"ERROR rule nodes deleting";
    tmp->removeRow(ui->tvRules->currentIndex().row());
    tmp->submitAll();
    }
}

///////////////////////////////////////////////////
////////////////// NEW VERSION/////////////////////
///////////////////////////////////////////////////



void FormRules::selected1() //selected concepts
{
    ui->cb_AB->clear();
    ui->cb_BC->clear();
    ui->cb_AC->clear();
    QSqlQuery q;
    QString str="SELECT id,edge_name FROM edge_types WHERE (id_node_type_src=%1 AND id_node_type_dst=%2) OR"
            " (id_node_type_src=%2 AND id_node_type_dst=%1)";
    str=str.arg(ui->cb_A->currentData().toString()).arg(ui->cb_B->currentData().toString());
    if(q.exec(str))
    while(q.next())
    {
        ui->cb_AB->addItem(q.value("edge_name").toString(),q.value("id").toInt());
    }
    else qDebug()<<"on_pushButton_clicked1"<<q.lastError().text();
    str="SELECT id,edge_name FROM edge_types WHERE (id_node_type_src=%1 AND id_node_type_dst=%2) OR ("
            "id_node_type_src=%2 AND id_node_type_dst=%1)";
    str=str.arg(ui->cb_B->currentData().toString()).arg(ui->cb_C->currentData().toString());
    if(q.exec(str))
    while(q.next())
    {
        ui->cb_BC->addItem(q.value("edge_name").toString(),q.value("id").toInt());
    }
    else qDebug()<<"on_pushButton_clicked2"<<q.lastError().text();
    str="SELECT id,edge_name FROM edge_types WHERE (id_node_type_src=%1 AND id_node_type_dst=%2) OR "
            "(id_node_type_src=%2 AND id_node_type_dst=%1)";
    str=str.arg(ui->cb_A->currentData().toString()).arg(ui->cb_C->currentData().toString());
    if(q.exec(str))
    while(q.next())
    {
        ui->cb_AC->addItem(q.value("edge_name").toString(),q.value("id").toInt());
    }
    else qDebug()<<"on_pushButton_clicked3"<<q.lastError().text();
    }

void FormRules::on_pushButton_2_clicked() //add rule T1
{
    QSqlQuery q;
    QString a,b,c;
    QString str = "INSERT INTO rules_nodes (id_rule,node_type_id,node_is_create,node_name) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            ui->cb_A->currentData().toString()+",0,'A')";
    if(!q.exec(str))
        qDebug()<<"ERROR pb2 "<<q.lastError().text();
    else {
        str="SELECT max(id) FROM rules_nodes";
        if(q.exec(str))
        {
            q.next();
            a=q.value(0).toString();
        }
    }
    str = "INSERT INTO rules_nodes (id_rule,node_type_id,node_is_create,node_name) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            ui->cb_B->currentData().toString()+",0,'B')";
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    else {
        str="SELECT max(id) FROM rules_nodes";
        if(q.exec(str))
        {
            q.next();
            b=q.value(0).toString();
        }
    }
    str = "INSERT INTO rules_nodes (id_rule,node_type_id,node_is_create,node_name) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            ui->cb_C->currentData().toString()+",0,'C')";
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    else {
        str="SELECT max(id) FROM rules_nodes";
        if(q.exec(str))
        {
            q.next();
            c=q.value(0).toString();
        }
    }


    if(ui->cb_AB_direction->currentIndex()==0){
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            a+","+
            ui->cb_AB->currentData().toString()+","+
            b+",0,"+
                QString::number(ui->cb_quant_AB->currentIndex()+1)+")";
    }
    else {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            b+","+
            ui->cb_AB->currentData().toString()+","+
            a+",0,"+
                QString::number(ui->cb_quant_AB->currentIndex()+1)+")";
    }
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    if(ui->cb_BC_direction->currentIndex()==0){
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            b+","+
            ui->cb_BC->currentData().toString()+","+
            c+",0,"+
                QString::number(ui->cb_quant_BC->currentIndex()+1)+")";
    }
    else {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            c+","+
            ui->cb_BC->currentData().toString()+","+
            b+",0,"+
                QString::number(ui->cb_quant_BC->currentIndex()+1)+")";
    }
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    if(ui->cb_AC_direction->currentIndex()==0){
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            a+","+
            ui->cb_AC->currentData().toString()+","+
            c+",1,"+
                QString::number(ui->cb_quant_AC->currentIndex()+1)+")";
    }
    else {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            c+","+
            ui->cb_AC->currentData().toString()+","+
            a+",1,"+
                QString::number(ui->cb_quant_AC->currentIndex()+1)+")";
    }
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    ui->l_done1->setVisible(1);


}

void FormRules::selected2() //selected 2
{
    ui->cb_AB_2->clear();
    ui->cb_BC_2->clear();
    ui->cb_AD->clear();
    ui->cb_DC->clear();
    ui->cb_BD->clear();
    QSqlQuery q;
    QString str=QString("SELECT id,edge_name FROM edge_types WHERE (id_node_type_src=%2 AND id_node_type_dst=%1)"
                        " OR (id_node_type_src=%1 AND id_node_type_dst=%2)").arg(ui->cb_B_2->currentData().toString()).arg(ui->cb_A_2->currentData().toString());
        if(q.exec(str))
        while(q.next())
        {
            ui->cb_AB_2->addItem(q.value("edge_name").toString(),q.value("id").toInt());
        }
        else qDebug()<<"on_pushButton_clicked4"<<q.lastError().text();
        str=QString("SELECT id,edge_name FROM edge_types WHERE (id_node_type_src=%2 AND id_node_type_dst=%1)"
                    " OR (id_node_type_src=%1 AND id_node_type_dst=%2)").arg(ui->cb_C_2->currentData().toString()).arg(ui->cb_B_2->currentData().toString());
        if(q.exec(str))
        while(q.next())
        {
            ui->cb_BC_2->addItem(q.value("edge_name").toString(),q.value("id").toInt());
        }
        else qDebug()<<"on_pushButton_clicked5"<<q.lastError().text();
        str=QString("SELECT id,edge_name FROM edge_types WHERE (id_node_type_src=%2 AND id_node_type_dst=%1)"
                " OR (id_node_type_src=%1 AND id_node_type_dst=%2)").arg(ui->cb_A_2->currentData().toString()).arg(ui->cb_D->currentData().toString());
        if(q.exec(str))
        while(q.next())
        {
            ui->cb_AD->addItem(q.value("edge_name").toString(),q.value("id").toInt());
        }
        else qDebug()<<"on_pushButton_clicked6"<<q.lastError().text();

        str=QString("SELECT id,edge_name FROM edge_types WHERE (id_node_type_src=%2 AND id_node_type_dst=%1)"
                " OR (id_node_type_src=%1 AND id_node_type_dst=%2)").arg(ui->cb_D->currentData().toString()).arg(ui->cb_C_2->currentData().toString());
        if(q.exec(str))
        while(q.next())
        {
            ui->cb_DC->addItem(q.value("edge_name").toString(),q.value("id").toInt());
        }
        else qDebug()<<"on_pushButton_clicked7"<<q.lastError().text();

        str=QString("SELECT id,edge_name FROM edge_types WHERE (id_node_type_src=%2 AND id_node_type_dst=%1)"
                " OR (id_node_type_src=%1 AND id_node_type_dst=%2)").arg(ui->cb_B_2->currentData().toString()).arg(ui->cb_D->currentData().toString());
        if(q.exec(str))
        while(q.next())
        {
            ui->cb_BD->addItem(q.value("edge_name").toString(),q.value("id").toInt());
        }
        else qDebug()<<"on_pushButton_clicked8"<<q.lastError().text();
}

void FormRules::on_pb_add2_clicked() //add rule T2
{
    QSqlQuery q;
    QString a,b,c,d;
    QString str = "INSERT INTO rules_nodes (id_rule,node_type_id,node_is_create,node_name) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            ui->cb_A_2->currentData().toString()+",0,'A')";
    if(!q.exec(str))
        qDebug()<<"ERROR add2 "<<q.lastError().text();
    else {
        str="SELECT max(id) FROM rules_nodes";
        if(q.exec(str))
        {
            q.next();
            a=q.value(0).toString();
        }
    }
    str = "INSERT INTO rules_nodes (id_rule,node_type_id,node_is_create,node_name) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            ui->cb_B_2->currentData().toString()+",0,'B')";
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    else {
        str="SELECT max(id) FROM rules_nodes";
        if(q.exec(str))
        {
            q.next();
            b=q.value(0).toString();
        }
    }
    str = "INSERT INTO rules_nodes (id_rule,node_type_id,node_is_create,node_name) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            ui->cb_C_2->currentData().toString()+",0,'C')";
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    else {
        str="SELECT max(id) FROM rules_nodes";
        if(q.exec(str))
        {
            q.next();
            c=q.value(0).toString();
        }
    }
    str = "INSERT INTO rules_nodes (id_rule,node_type_id,node_is_create,node_name) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            ui->cb_D->currentData().toString()+QString(",1,'D')");
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    else {
        str="SELECT max(id) FROM rules_nodes";
        if(q.exec(str))
        {
            q.next();
            d=q.value(0).toString();
        }
    }

    if(ui->cb_AB_direction_2->currentIndex()==0)
    {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            a+","+
            ui->cb_AB_2->currentData().toString()+","+
            b+",0,"+
            QString::number(ui->cb_quant_AB_2->currentIndex()+1)+")";
    }
    else {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            b+","+
            ui->cb_AB_2->currentData().toString()+","+
            a+",0,"+
                QString::number(ui->cb_quant_AB_2->currentIndex()+1)+")";
    }
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    if(ui->cb_BC_direction_2->currentIndex()==0)
    {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            b+","+
            ui->cb_BC_2->currentData().toString()+","+
            c+",0,"+
                QString::number(ui->cb_quant_BC_2->currentIndex()+1)+")";
    }
    else {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            c+","+
            ui->cb_BC_2->currentData().toString()+","+
            b+",0,"+
                QString::number(ui->cb_quant_BC_2->currentIndex()+1)+")";
    }
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    if(ui->cb_adON->isChecked())
    {
    if(ui->cb_AD_direction->currentIndex()==0)
    {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            a+","+
            ui->cb_AD->currentData().toString()+","+
            d+",1,"+
                QString::number(ui->cb_quant_AD->currentIndex()+1)+")";
    }
    else {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            d+","+
            ui->cb_AD->currentData().toString()+","+
            a+",1,"+
                QString::number(ui->cb_quant_AD->currentIndex()+1)+")";
    }
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    }
    if(ui->cb_dcON->isChecked())
    {
    if(ui->cb_DC_direction->currentIndex()==0)
    {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            d+","+
            ui->cb_DC->currentData().toString()+","+
            c+",1,"+
                QString::number(ui->cb_quant_DC->currentIndex()+1)+")";
    }
    else {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            c+","+
            ui->cb_DC->currentData().toString()+","+
            d+",1,"+
                QString::number(ui->cb_quant_DC->currentIndex()+1)+")";
    }
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    }
    if(ui->cb_bdON->isChecked())
    {
    if(ui->cb_BD_direction->currentIndex()==0)
    {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            b+","+
            ui->cb_BD->currentData().toString()+","+
            d+",1,"+
                QString::number(ui->cb_quant_BD->currentIndex()+1)+")";
    }
    else {
        str = "INSERT INTO rules_edges (id_rules,node_id_src,edge_type_id,node_id_dst,edge_is_create,rule_quant_id) values ("+
            tmp->record(ui->tvRules->currentIndex().row()).value(0).toString()+","+
            d+","+
            ui->cb_BD->currentData().toString()+","+
            b+",1,"+
                QString::number(ui->cb_quant_BD->currentIndex()+1)+")";
    }
    if(!q.exec(str))
        qDebug()<<q.lastError().text();
    }
    ui->l_done2->setVisible(1);
}


void FormRules::on_l_done1_linkHovered(const QString &link)
{
    ui->l_done1->setVisible(0);
}

void FormRules::on_l_done2_linkHovered(const QString &link)
{
    ui->l_done2->setVisible(0);
}

void FormRules::on_pushButton_4_clicked()
{
    QSqlQuery query;
    QString str;
    str="DELETE FROM rules_nodes WHERE id_rule="+tmp->record(ui->tvRules->currentIndex().row()).value(0).toString();
    if(!query.exec(str))
        qDebug()<<"ERROR changing"<<query.lastError();
    else {
    str="DELETE FROM rules_edges WHERE id_rules="+tmp->record(ui->tvRules->currentIndex().row()).value(0).toString();
    if(!query.exec(str))
        qDebug()<<"ERROR changing"<<query.lastError();
    else
    {
    on_pushButton_2_clicked();
    }
    }
}

void FormRules::on_pb_edit2_clicked()
{
    QSqlQuery query;
    QString str;
    str="DELETE FROM rules_nodes WHERE id_rule="+tmp->record(ui->tvRules->currentIndex().row()).value(0).toString();
    if(!query.exec(str))
        qDebug()<<"ERROR changing"<<query.lastError();
    else {
    str="DELETE FROM rules_edges WHERE id_rules="+tmp->record(ui->tvRules->currentIndex().row()).value(0).toString();
    if(!query.exec(str))
        qDebug()<<"ERROR changing"<<query.lastError();
    else
    {
    on_pb_add2_clicked();
    }
    }
}

void FormRules::on_cb_A_currentIndexChanged(const QString &arg1)
{
    selected1();

}

void FormRules::on_cb_B_currentIndexChanged(const QString &arg1)
{
    selected1();
}

void FormRules::on_cb_C_currentIndexChanged(const QString &arg1)
{
    selected1();
}

void FormRules::on_cb_A_2_currentIndexChanged(const QString &arg1)
{
    selected2();
}

void FormRules::on_cb_B_2_currentIndexChanged(const QString &arg1)
{
    selected2();
}

void FormRules::on_cb_C_2_currentIndexChanged(const QString &arg1)
{
    selected2();
}

void FormRules::on_cb_D_currentIndexChanged(const QString &arg1)
{
    selected2();
}

void FormRules::on_pb_addedit_clicked()
{
     ruleaddwiget->show();
}
void FormRules::slotaddrule(QString name, QString author, QString remark, QString tmplt)
{ 
    int maxc=tmp->rowCount();
    tmp->insertRow(maxc);
    tmp->setData(tmp->index(maxc,1),name,Qt::EditRole);
    tmp->setData(tmp->index(maxc,2),author,Qt::EditRole);
    tmp->setData(tmp->index(maxc,3),remark,Qt::EditRole);
    tmp->setData(tmp->index(maxc,4),tmplt,Qt::EditRole);
    tmp->submitAll();
    tmp->sort(0,Qt::AscendingOrder);

}

void FormRules::on_pushButton_clicked()
{
    ruleeditwiget->show();
    emit(signalsendfields(tmp->record(ui->tvRules->currentIndex().row()).value(0).toInt()));
}
void FormRules::sloteditrule(QString name, QString author, QString remark, QString tmplt)
{
    int max=ui->tvRules->currentIndex().row();
    tmp->setData(tmp->index(max,1),name,Qt::EditRole);
    tmp->setData(tmp->index(max,2),author,Qt::EditRole);
    tmp->setData(tmp->index(max,3),remark,Qt::EditRole);
    tmp->setData(tmp->index(max,4),tmplt,Qt::EditRole);
    tmp->submitAll();
    ruleeditwiget->close();
}
