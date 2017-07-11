#include "titkinform.h"
#include "ui_titkinform.h"
#include "qsqlrelationaltablemodel.h"
#include "qdebug.h"
#include "dafgraph.h"
#include "dafnodetype.h"
#include "dafedgetype.h"
#include "qlist.h"
#include "dafnode.h"
#include "dafedge.h"
#include "datamodel.h"
#include "QSqlQuery"
#include "QList"
#include "QSqlError"
#include <QStringList>
//test
TitkinForm::TitkinForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TitkinForm)
{
    ui->setupUi(this);

}

TitkinForm::~TitkinForm()
{
    delete ui;
}
QString getQuantByID(int quant)
{
    QSqlQuery q;
    QString str = "SELECT quant_name FROM quantifiers WHERE id="+QString::number(quant);
    if(q.exec(str))
    {
        q.next();
        return q.value(0).toString();
    }
    else
        qDebug()<<"SQL getQuantByID"<<q.lastError().text();
}

QStringList getNames(QString aID, QString bID)
{
    QSqlQuery q;
    QStringList ret;
    QString str = "SELECT a.node_name, b.node_name FROM nodes AS a, nodes AS b WHERE a.id=%1 AND b.id=%2;";
    str=str.arg(aID).arg(bID);
    if(q.exec(str))
    {
        q.next();
        ret<<q.value(0).toString()<<q.value(1).toString();
    }
    else
        qDebug()<<"SQL get names "<<q.lastError().text();
    return ret;
}

int TitkinForm::insert_edge(QString a,QString ac,QString c,VzRule *r)
{    
    QString str="SELECT id FROM edges WHERE id_node_src="+a;
    str+=" AND id_edge_type="+ac;
    str+=" AND id_node_dst="+c;
    QSqlQuery query,q2,q3;
    if(query.exec(str))
    {
        if(!query.next())
        {

            str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) VALUES("+a+","+ac+","+
                    c+",1,"+QString::number(r->getId())+","+QString::number(r->getM_AC_Quant())+")";
            if(q2.exec(str))
            {
                str="SELECT max(id) FROM edges";
                if(q3.exec(str))
                {
                    q3.next();
                    QStringList ans=getNames(a,c);
                    ui->textEdit->append(QString("    Добавленно (THEN) '%1' --'%2','%5'--> '%3', ID=%4")
                                         .arg(ans.at(0))
                                         .arg(r->getM_edgeACName())
                                         .arg(ans.at(1))
                                         .arg(q3.value(0).toString())
                                         .arg(getQuantByID(r->getM_AC_Quant()))
                                         );
                    return q3.value(0).toInt();
                }
                else
                    qDebug()<<q3.lastError().text()<<"MAX ERROR";

            }
            else
                qDebug()<<q2.lastError().text()<<"insert ERROR";
        }
        else
        {
            qDebug()<< QString::number(__LINE__) << ": !next";
            qDebug() << "SQL Error: " << query.lastError().text();
            qDebug() << "SQL querry: " << str;
        }
    }
    //else qDebug()<<"ERROR check m_ins "<<query.lastError().text();
    else
    {
        qDebug()<< QString::number(__LINE__) << ": ERROR check m_ins";
        qDebug() << "SQL Error: " << query.lastError().text();
        qDebug() << "SQL querry: " << str;
    }
    return -1;
}
QStringList TitkinForm::insert_node(QString type, QString name,QString a, QString ad, QString c, QString dc,
                       QString author, VzRule *r)
{
    qDebug()<<"Fuck up 1";
    QStringList ret;
    QSqlQuery query,q2;
    QString str,d,logstr;
    str="SELECT id FROM nodes WHERE id_node_type=%1 AND node_name='%2'";
    str=str.arg(type,name);
    if(query.exec(str))
    {
        if(query.next())
            return QStringList("-1");
        else
        {
            str="INSERT INTO nodes (id_node_type,node_name,author,status) VALUES('%1','%2','%3',%4)";
            str=str.arg(type,name,author,QString::number(r->getId()));
            if(!q2.exec(str))
            {
                qDebug()<<"ERROR insert node 2"<<q2.lastError().text();
                return QStringList("-1");
            }
            else
            {
                //nothing
            }
            str = "SELECT id,node_name FROM nodes WHERE id in(SELECT max(id) FROM nodes)";
            if(q2.exec(str))
            {
                q2.next();
                d=q2.value(0).toString();
                ui->textEdit->append("    Add concept "+q2.value(1).toString());
            }
            else
                qDebug()<<"ERROR kujyhg"<<q2.lastError().text();
            //add AD if need
            if(r->getM_AD_typeId()>0)
            {
                str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) "
                    "VALUES(%1,%2,%3,1,%5,%4)";
                if(r->getM_AD_direct())
                {
                    str=str.arg(a,ad,d,QString::number(r->getM_AD_Quant()),QString::number(r->getId()));
                    logstr="    Добавлена роль: '%1' --'%2','%4'--> '%3', ID=%5";
                    logstr=logstr.arg(r->getM_A_name(),r->getM_edgeADName(),r->getM_D_name(),
                                      getQuantByID(r->getM_AD_Quant()));
                }
                else
                {
                    str=str.arg(d,ad,a,QString::number(r->getM_AD_Quant()),QString::number(r->getId()));
                    logstr="    Добавлена роль: '%1' --'%2','%4'--> '%3', ID=%5";
                    logstr=logstr.arg(r->getM_D_name(),r->getM_edgeADName(),r->getM_A_name(),
                                      getQuantByID(r->getM_AD_Quant()));
                }
                if(!q2.exec(str))
                {
                    qDebug()<<"ERROR insert node 3"<<q2.lastError().text();
                    return QStringList("-1");
                }
                else
                {
                    str="SELECT max(id) FROM edges";
                    if(q2.exec(str))
                    {
                        q2.next();
                        ui->textEdit->append(logstr.arg(q2.value(0).toString()));
                        ret.append(q2.value(0).toString());
                    }
                    else
                        qDebug()<<"ERROR sfgcx"<<q2.lastError().text();
                }
            }
            else
            {
                //nothing
            }
            //add DC if need
            if(r->getM_DC_typeId()>0)
            {
                str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) "
                    "VALUES(%1,%2,%3,1,%5,%4)";
                if(r->getM_DC_direct())
                {
                    str=str.arg(d,dc,c,QString::number(r->getM_DC_Quant()),QString::number(r->getId()));
                    logstr="    Добавлена роль: '%1' --'%2','%4'--> '%3', ID=%5";
                    logstr=logstr.arg(r->getM_D_name(),r->getM_edgeDCName(),r->getM_C_name(),
                                      getQuantByID(r->getM_DC_Quant()));
                }
                else
                {
                    str=str.arg(c,dc,d,QString::number(r->getM_DC_Quant()),QString::number(r->getId()));
                    logstr="    Добавлена роль: '%1' --'%2','%4'--> '%3', ID=%5";
                    logstr=logstr.arg(r->getM_C_name(),r->getM_edgeDCName(),r->getM_D_name(),
                                      getQuantByID(r->getM_DC_Quant()));
                }
                if(!q2.exec(str))
                {
                    qDebug()<<"ERROR insert node 3"<<q2.lastError().text();
                    return QStringList("-1");
                }
                else
                {
                    str="SELECT max(id) FROM edges";
                    if(q2.exec(str))
                    {
                        q2.next();
                        ui->textEdit->append(logstr.arg(q2.value(0).toString()));
                        ret.append(q2.value(0).toString());
                    }
                    else
                        qDebug()<<"ERROR sfgcx"<<q2.lastError().text();
                }
            }
            else
            {
                //nothing
            }
            //add BD if need
            if(r->getM_BD_typeId()>0)
            {
                str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) "
                    "VALUES(%1,%2,%3,1,%5,%4)";
                if(r->getM_BD_direct())
                {
                    str=str.arg(QString::number(r->getM_B_typeId()),QString::number(r->getM_BC_typeId()),d,
                                QString::number(r->getM_BD_Quant()),QString::number(r->getId()));
                    logstr="    Добавлена роль: '%1' --'%2','%4'--> '%3', ID=%5";
                    logstr=logstr.arg(r->getM_B_name(),r->getM_edgeBDName(),r->getM_D_name(),
                                      getQuantByID(r->getM_BD_Quant()));
                }
                else
                {
                    str=str.arg(d,QString::number(r->getM_BD_typeId()),QString::number(r->getM_B_typeId()),
                                QString::number(r->getM_BD_Quant()),QString::number(r->getId()));
                    logstr="    Добавлена роль: '%1' --'%2','%4'--> '%3', ID=%5";
                    logstr=logstr.arg(r->getM_D_name(),r->getM_edgeBDName(),r->getM_B_name(),
                                      getQuantByID(r->getM_BD_Quant()));
                }
                if(!q2.exec(str))
                {
                    qDebug()<<"ERROR insert node 3"<<q2.lastError().text();
                    return QStringList("-1");
                }
                else
                {
                    str="SELECT max(id) FROM edges";
                    if(q2.exec(str))
                    {
                        q2.next();
                        ui->textEdit->append(logstr.arg(q2.value(0).toString()));
                        ret.append(q2.value(0).toString());
                    }
                    else
                        qDebug()<<"ERROR sfgcx"<<q2.lastError().text();
                }
            }
            else
            {
                //nothing
            }
            return ret;

        }
    }
    else
        qDebug()<<"insert iuffcv ERROR"<<query.lastError().text();
}

//QStringList TitkinForm::insert_node(QString type, QString name,QString a, QString ad, QString c, QString dc,
//                                    QString author, VzRule *r)
//{
//    QStringList ret;
//    QSqlQuery query,q2;
//    QString str,d;
//    //1>>check
//    if(r->getM_AD_direct() && r->getM_DC_direct())
//    str="SELECT id FROM edges WHERE id_node_src="+a+" AND id_edge_type="+ad+
//            " AND id_node_dst in (SELECT id_node_src FROM edges WHERE id_edge_type="+dc+" AND id_node_dst="+c+")";
//    //2><check
//    if(r->getM_AD_direct() && !r->getM_DC_direct())
//    str="SELECT id FROM edges WHERE id_node_src="+a+" AND id_edge_type="+ad+
//            " AND id_node_dst in (SELECT id_node_dst FROM edges WHERE id_edge_type="+dc+" AND id_node_src="+c+")";
//    //3<>check
//    if(!r->getM_AD_direct() && r->getM_DC_direct())
//    str="SELECT id FROM edges WHERE id_node_dst="+a+" AND id_edge_type="+ad+
//            " AND id_node_src in (SELECT id_node_src FROM edges WHERE id_edge_type="+dc+" AND id_node_dst="+c+")";
//    //4<<check
//    if(!r->getM_AD_direct() && !r->getM_DC_direct())
//    str="SELECT id FROM edges WHERE id_node_dst="+a+" AND id_edge_type="+ad+
//            " AND id_node_src in (SELECT id_node_dst FROM edges WHERE id_edge_type="+dc+" AND id_node_src="+c+")";
//    if(query.exec(str))
//    {
//        if(!query.next())
//        {

//            str="INSERT INTO nodes (id_node_type,node_name,author,status) VALUES('%1','%2','%3',%4)";
//            str=str.arg(type).arg(name).arg(author).arg(QString::number(r->getId()));
//            if(!q2.exec(str))
//            {
//                qDebug()<<"ERROR insert node 2"<<q2.lastError().text();
//                return QStringList("-1");
//            }
//            /*str="SELECT id,id_node_type FROM nodes,node_types WHERE nodes.id in (SELECT max(id) FROM nodes) AND"
//                "nodes.id_node_type=node_types.id;";*/
//            str = "SELECT id,node_name FROM nodes WHERE id in(SELECT max(id) FROM nodes)";
//            if(q2.exec(str))
//            {
//                q2.next();
//                d=q2.value(0).toString();
//                ui->textEdit->append("    Add concept "+q2.value(1).toString());
//            }
//            else
//                qDebug()<<"ERROR kujyhg"<<q2.lastError().text();
//            //1>>
//            if(r->getM_AD_direct() && r->getM_DC_direct())
//            {
//            str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) VALUES(%1,%2,%3,1,%5,%4)";
//            str=str.arg(a).arg(ad).arg(d).arg(QString::number(r->getM_AD_Quant())).arg(QString::number(r->getId()));
//            if(!q2.exec(str))
//            {
//                qDebug()<<"ERROR insert node 3"<<q2.lastError().text();
//                return QStringList("-1");
//            }
//            else
//            {
//                str="SELECT max(id) FROM edges";
//                if(q2.exec(str))
//                {
//                    q2.next();
//                    ui->textEdit->append(QString("    Добавлена роль: '%1' --'%2','%5'--> '%3', ID=%4")
//                                         .arg(r->getM_A_name())
//                                         .arg(r->getM_edgeADName())
//                                         .arg(r->getM_D_name())
//                                         .arg(q2.value(0).toString())
//                                         .arg(getQuantByID(r->getM_AD_Quant()))
//                                         );
//                    ret.append(q2.value(0).toString());
//                }
//                else
//                    qDebug()<<"ERROR sfgcx"<<q2.lastError().text();
//            }
//            str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) VALUES(%1,%2,%3,1,%5,%4)";
//            str=str.arg(d).arg(dc).arg(c).arg(r->getM_DC_Quant()).arg(QString::number(r->getId()));
//                    if(!q2.exec(str))
//                    {
//                        qDebug()<<"ERROR insert node 4"<<q2.lastError().text();
//                        return QStringList("-1");
//                    }
//                    else
//                    {
//                        str="SELECT max(id) FROM edges";
//                        if(q2.exec(str))
//                {
//                    q2.next();
//                    ui->textEdit->append(QString("    Добавлена роль: %1 --'%2','%5'--> %3, "
//                                                 "ID=%4").arg(d).arg(dc).arg(c).arg(q2.value(0).toString())
//                                         .arg(getQuantByID(r->getM_DC_Quant())));
//                    ret.append(q2.value(0).toString());
//                }
//                else
//                    qDebug()<<"error sfgbjc"<<q2.lastError().text();
//            }
//            return ret;
//            }
//            //2><
//            if(r->getM_AD_direct() && !r->getM_DC_direct())
//            {
//                qDebug()<<"found";
//            str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) VALUES(%1,%2,%3,1,%5,%4)";
//            str=str.arg(a).arg(ad).arg(d).arg(QString::number(r->getM_AD_Quant())).arg(QString::number(r->getId()));
//            if(!q2.exec(str))
//            {
//                qDebug()<<"ERROR insert node 3"<<q2.lastError().text();
//                return QStringList("-1");
//            }
//            else
//            {
//                str="SELECT max(id) FROM edges";
//                if(q2.exec(str))
//                {
//                    q2.next();
//                    ui->textEdit->append(QString("    Добавлена роль: %1 --'%2','%5'--> %3, "
//                                                 "ID=%4").arg(a).arg(ad).arg(d).arg(q2.value(0).toString())
//                                         .arg(getQuantByID(r->getM_AD_Quant())));
//                    ret.append(q2.value(0).toString());
//                }
//                else
//                    qDebug()<<"fgvc"<<q2.lastError().text();
//            }
//            str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) VALUES(%1,%2,%3,1,%5,%4)";
//            str=str.arg(c).arg(dc).arg(d).arg(r->getM_DC_Quant()).arg(QString::number(r->getId()));
//            if(!q2.exec(str))
//            {
//                qDebug()<<"ERROR insert node 4"<<q2.lastError().text();
//                return QStringList("-1");
//            }
//            else
//            {
//                str="SELECT max(id) FROM edges";
//                if(q2.exec(str))
//                {
//                    q2.next();
//                    ui->textEdit->append(QString("    Добавлена роль: %1 --'%2','%5'--> %3, "
//                                                 "ID=%4").arg(c).arg(dc).arg(d).arg(q2.value(0).toString())
//                                         .arg(getQuantByID(r->getM_DC_Quant())));
//                    ret.append(q2.value(0).toString());
//                }
//                else
//                    qDebug()<<"wremd"<<q2.lastError().text();
//            }
//            return ret;
//            }
//            //3<>
//            if(!r->getM_AD_direct() && r->getM_DC_direct())
//            {
//            str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) VALUES(%1,%2,%3,1,%5,%4)";
//            str=str.arg(d).arg(ad).arg(a).arg(QString::number(r->getM_AD_Quant())).arg(QString::number(r->getId()));
//            if(!q2.exec(str))
//            {
//                qDebug()<<"ERROR insert node 3"<<q2.lastError().text();
//                return QStringList("-1");
//            }
//            else
//            {
//                str="SELECT max(id) FROM edges";
//                if(q2.exec(str))
//                {
//                    q2.next();
//                    ui->textEdit->append(QString("    Добавлена роль: %1 --'%2','%5'--> %3, "
//                                                 "ID=%4").arg(d).arg(ad).arg(a).arg(q2.value(0).toString())
//                                         .arg(getQuantByID(r->getM_AD_Quant())));
//                    ret.append(q2.value(0).toString());
//                }
//                else
//                    qDebug()<<"sdfgh"<<q2.lastError().text();
//            }
//            str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) VALUES(%1,%2,%3,1,%5,%4)";
//            str=str.arg(d).arg(dc).arg(c).arg(r->getM_DC_Quant()).arg(QString::number(r->getId()));
//            if(!q2.exec(str))
//            {
//                qDebug()<<"ERROR insert node 4"<<q2.lastError().text();
//                return QStringList("-1");
//            }
//            else
//            {
//                str="SELECT max(id) FROM edges";
//                if(q2.exec(str))
//                {
//                    q2.next();
//                    ui->textEdit->append(QString("    Добавлена роль: %1 --'%2','%5'--> %3, "
//                                                 "ID=%4").arg(d).arg(dc).arg(c).arg(q2.value(0).toString())
//                                         .arg(getQuantByID(r->getM_DC_Quant())));
//                    ret.append(q2.value(0).toString());
//                }
//                else
//                    qDebug()<<",mgdcdf"<<q2.lastError().text();
//            }
//            return ret;
//            }
//            //4<<
//            if(!r->getM_AD_direct() && !r->getM_DC_direct())
//            {
//            str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) VALUES(%1,%2,%3,1,%5,%4)";
//            str=str.arg(d).arg(ad).arg(a).arg(QString::number(r->getM_AD_Quant())).arg(QString::number(r->getId()));
//            if(!q2.exec(str))
//            {
//                qDebug()<<"ERROR insert node 3"<<q2.lastError().text();
//                return QStringList("-1");
//            }
//            else
//            {
//                str="SELECT max(id) FROM edges";
//                if(q2.exec(str))
//                {
//                    q2.next();
//                    ui->textEdit->append(QString("    Добавлена роль: %1 --'%2','%5'--> %3, "
//                                                 "ID=%4").arg(d).arg(ad).arg(a).arg(q2.value(0).toString())
//                                         .arg(getQuantByID(r->getM_AD_Quant())));
//                    ret.append(q2.value(0).toString());
//                }
//                else
//                    qDebug()<<"gkoiuytf"<<q2.lastError().text();
//            }
//            str="INSERT INTO edges (id_node_src,id_edge_type,id_node_dst,isNew,status,quant_id) VALUES(%1,%2,%3,1,%5,%4)";
//            str=str.arg(c).arg(dc).arg(d).arg(r->getM_DC_Quant()).arg(QString::number(r->getId()));
//            if(!q2.exec(str))
//            {
//                qDebug()<<"ERROR insert node 4"<<q2.lastError().text();
//                return QStringList("-1");
//            }
//            else
//            {
//                str="SELECT max(id) FROM edges";
//                if(q2.exec(str))
//                {
//                    q2.next();
//                    ui->textEdit->append(QString("    Добавлена роль: %1 --'%2','%5'--> %3, "
//                                                 "ID=%4").arg(c).arg(dc).arg(d).arg(q2.value(0).toString())
//                                         .arg(getQuantByID(r->getM_DC_Quant())));
//                    ret.append(q2.value(0).toString());
//                }
//                else
//                    qDebug()<<"edfhhgfccc"<<q2.lastError().text();
//            }
//            return ret;
//            }

//        }
//        else qDebug()<<query.value(0).toString();
//    }
//    else qDebug()<<"ERROR insert node 1"<<query.lastError().text();
//    return QStringList("-1");
//}

void TitkinForm::logStrIf(int id_AB, int id_BC)
{
    QString logStr="    Найдено (IF): ";
    QSqlQuery query;
    QString str;

    str="SELECT edge_types.edge_name, nFirst.node_name, nSecond.node_name, nFirst.id, nSecond.id, quantifiers.quant_name FROM edges, edge_types, quantifiers, nodes AS nFirst, nodes AS nSecond WHERE edges.id="+
                QString::number(id_AB) + " AND edges.id_edge_type=edge_types.id AND nFirst.id=edges.id_node_src AND nSecond.id=edges.id_node_dst AND quantifiers.id=edges.quant_id";

    if(query.exec(str))
    {
        while(query.next())
        {
            logStr += "[Триплет 1: " + query.value(1).toString() + " (ID= " + QString::number(query.value(3).toInt()) + ") -> " +
                                       query.value(0).toString() + " (ID= " + QString::number(id_AB) + ", QUANT= "+ query.value(5).toString() + ") -> " +
                                       query.value(2).toString() + " (ID= " + QString::number(query.value(4).toInt()) + "] ";
        }
    }
    else
        qDebug()<<"afafsdfzz"<<query.lastError().text();
    query.clear();

    str="SELECT edge_types.edge_name, nFirst.node_name, nSecond.node_name, nFirst.id, nSecond.id, quantifiers.quant_name FROM edges, edge_types, quantifiers, nodes AS nFirst, nodes AS nSecond WHERE edges.id="+
                QString::number(id_BC) + " AND edges.id_edge_type=edge_types.id AND nFirst.id=edges.id_node_src AND nSecond.id=edges.id_node_dst AND quantifiers.id=edges.quant_id";
    if(query.exec(str))
    {
        while(query.next())
        {
            logStr += "[Триплет 2: " + query.value(1).toString() + " (ID= " + QString::number(query.value(3).toInt()) + ") -> " +
                                       query.value(0).toString() + " (ID= " + QString::number(id_BC) + ", QUANT= "+ query.value(5).toString() + ") -> " +
                                       query.value(2).toString() + " (ID= " + QString::number(query.value(4).toInt()) + "] ";
        }
    }
    else
        qDebug()<<"fdfijhvcxbb"<<query.lastError().text();
    ui->textEdit->append(logStr);
    qDebug()<< QString::number(__LINE__) << ": " << logStr;
    return;
}

void TitkinForm::useRuleType1(VzRule *r,QList<int> *inEdges,QList<int> *outEdges)
{
    QSqlQuery query;
    QString ag,bg,cg,str;
    /////////////////////////////////
   /////////////->.->///////////////
  /////////////////////////////////
    if(r->getM_AB_direct() && r->getM_BC_direct())
    {
        str="SELECT edges.id_node_src,edges.id_node_dst,edges.id FROM nodes,edges WHERE edges.id_edge_type="+
                    QString::number(r->getM_AB_typeId())+
                    " AND edges.id_node_src=nodes.id AND edges.quant_id="+QString::number(r->getM_AB_Quant())+" AND nodes.id_node_type="+QString::number(r->getM_A_typeId());
        if(query.exec(str))
        {
            while(query.next())
            {
                ag=query.value(0).toString();
                str="SELECT edges.id_node_src,edges.id_node_dst,edges.id FROM nodes,edges WHERE edges.id_edge_type="+QString::number(r->getM_BC_typeId());
                str+=" AND edges.id_node_src="+query.value(1).toString();
                str+=" AND edges.quant_id="+QString::number(r->getM_BC_Quant())+" AND edges.id_node_src=nodes.id AND nodes.id_node_type="+QString::number(r->getM_B_typeId());
                QSqlQuery query2,query3;
                if(query2.exec(str))
                {
                    while(query2.next())
                    {
                        if(inEdges->contains(query.value(2).toInt()) || inEdges->contains(query2.value(2).toInt()))
                        {
                            bg=query2.value(0).toString();
                            str="SELECT id FROM nodes WHERE id="+query2.value(1).toString();
                            str+=" AND id_node_type="+QString::number(r->getM_C_typeId());
                            if(query3.exec(str))
                            {
                                while(query3.next())
                                {
                                    cg=query3.value(0).toString();
                                    int t;
                                    if(r->getM_AC_direct())
                                    {
                                        logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                        t=insert_edge(ag,QString::number(r->getM_AC_typeId()),cg,r);
                                    }
                                    if(!r->getM_AC_direct())
                                    {
                                        logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                        t=insert_edge(cg,QString::number(r->getM_AC_typeId()),ag,r);
                                    }
                                    if (t>0)
                                        outEdges->append(t);
                                }
                            }
                            else
                                qDebug()<<"ERROR select internal 2"<<query3.lastError().text();
                        }
                    }
                }
                else
                    qDebug()<<"ERROR select internal"<<query2.lastError().text();
            }
        }
        else
            qDebug()<<"ERROR select 2"<<query.lastError().text();

    }
    /////////////////////////////////
   /////////////<-.->///////////////
  /////////////////////////////////
    if(!r->getM_AB_direct() && r->getM_BC_direct())
    {
        str="SELECT edges.id_node_src,edges.id_node_dst,edges.id FROM nodes,edges WHERE edges.id_edge_type="+
                    QString::number(r->getM_AB_typeId())+
                    " AND edges.quant_id="+QString::number(r->getM_AB_Quant())+" AND edges.id_node_dst=nodes.id AND nodes.id_node_type="+QString::number(r->getM_A_typeId());
        if(query.exec(str))
        {
            while(query.next())
            {
                ag=query.value(1).toString();
                str="SELECT edges.id_node_src,edges.id_node_dst,edges.id FROM nodes,edges WHERE edges.id_edge_type="+QString::number(r->getM_BC_typeId());
                str+=" AND edges.quant_id="+QString::number(r->getM_BC_Quant())+" AND edges.id_node_src="+query.value(0).toString();
                str+=" AND edges.id_node_src=nodes.id AND nodes.id_node_type="+QString::number(r->getM_B_typeId());
                QSqlQuery query2,query3;
                if(query2.exec(str))
                {
                    while(query2.next())
                    {
                        if(inEdges->contains(query.value(2).toInt()) || inEdges->contains(query2.value(2).toInt()))
                            //ИЛИ потому что в переданном списке ребер может быть только одно из пары
                        {
                            bg=query2.value(0).toString();
                            str="SELECT id FROM nodes WHERE id="+query2.value(1).toString();
                            str+=" AND id_node_type="+QString::number(r->getM_C_typeId());
                            if(query3.exec(str))
                            {
                                while(query3.next())
                                {
                                    //QString logStr = "<- " + QString::number(query.value(2).toInt() )+ " <- -> " + QString::number(query2.value(2).toInt()) + " ->";
                                    //ui->textEdit->append(logStr);
                                    cg=query3.value(0).toString();
                                    int t;
                                    if(r->getM_AC_direct())
                                    {
                                        logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                        t=insert_edge(ag,QString::number(r->getM_AC_typeId()),cg,r);
                                    }
                                    if(!r->getM_AC_direct())
                                    {
                                        logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                        t=insert_edge(cg,QString::number(r->getM_AC_typeId()),ag,r);
                                    }
                                    if (t>0)
                                        outEdges->append(t);
                                }
                            }
                            else
                                qDebug()<<"ERROR select internal 2"<<query3.lastError().text();
                        }
                    }
                }
                else
                    qDebug()<<"ERROR select internal"<<query2.lastError().text();
            }
        }
        else
            qDebug()<<"ERROR select 2"<<query.lastError().text();

    }
    /////////////////////////////////
   /////////////->.<-///////////////
  /////////////////////////////////
   if(r->getM_AB_direct() && !r->getM_BC_direct())
    {
        str="SELECT edges.id_node_src,edges.id_node_dst,edges.id FROM nodes,edges WHERE edges.id_edge_type="+
                    QString::number(r->getM_AB_typeId())+
                    " AND edges.quant_id="+QString::number(r->getM_AB_Quant())+" AND edges.id_node_src=nodes.id AND nodes.id_node_type="+QString::number(r->getM_A_typeId());
        if(query.exec(str))
        {
            while(query.next())
            {
                ag=query.value(0).toString();
                str="SELECT edges.id_node_src,edges.id_node_dst,edges.id FROM nodes,edges WHERE edges.id_edge_type="+QString::number(r->getM_BC_typeId());
                str+=" AND edges.id_node_dst="+query.value(1).toString();
                str+=" AND edges.quant_id="+QString::number(r->getM_BC_Quant())+" AND edges.id_node_dst=nodes.id AND nodes.id_node_type="+QString::number(r->getM_B_typeId());
                QSqlQuery query2,query3;
                if(query2.exec(str))
                {
                    while(query2.next())
                    {
                        if(inEdges->contains(query.value(2).toInt()) || inEdges->contains(query2.value(2).toInt()))
                        {
                            bg=query2.value(1).toString();
                            str="SELECT id FROM nodes WHERE id="+query2.value(0).toString();
                            str+=" AND id_node_type="+QString::number(r->getM_C_typeId());
                            if(query3.exec(str))
                            {
                                while(query3.next())
                                {
                                    //QString logStr = "-> " + QString::number(query.value(2).toInt() )+ " -> <-" + QString::number(query2.value(2).toInt())+ " <-";
                                    //ui->textEdit->append(logStr);
                                    cg=query3.value(0).toString();
                                    int t;
                                    if(r->getM_AC_direct())
                                    {
                                        logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                        t=insert_edge(ag,QString::number(r->getM_AC_typeId()),cg,r);
                                    }
                                    if(!r->getM_AC_direct())
                                    {
                                        logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                        t=insert_edge(cg,QString::number(r->getM_AC_typeId()),ag,r);
                                    }
                                    if (t>0)
                                        outEdges->append(t);
                                }
                            }
                            else
                                qDebug()<<"ERROR select internal 2"<<query3.lastError().text();
                        }
                    }
                }
                else
                    qDebug()<<"ERROR select internal"<<query2.lastError().text();
            }
        }
        else
            qDebug()<<"ERROR select 2"<<query.lastError().text();

    }

   /////////////////////////////////
  /////////////<-.<-///////////////
 /////////////////////////////////
   if(!r->getM_AB_direct() && !r->getM_BC_direct())
   {
       str="SELECT edges.id_node_src,edges.id_node_dst,edges.id FROM nodes,edges WHERE edges.id_edge_type="+
                   QString::number(r->getM_AB_typeId())+
                   " AND edges.id_node_dst=nodes.id AND edges.quant_id="+QString::number(r->getM_AB_Quant())+" AND nodes.id_node_type="+QString::number(r->getM_A_typeId());
       if(query.exec(str))
       {
           while(query.next())
           {
               ag=query.value(1).toString();
               qDebug() << "ag= " << ag;
               str="SELECT edges.id_node_src,edges.id_node_dst,edges.id FROM nodes,edges WHERE edges.id_edge_type="+QString::number(r->getM_BC_typeId());
               str+=" AND edges.id_node_dst="+query.value(0).toString();
               str+=" AND edges.quant_id="+QString::number(r->getM_BC_Quant())+" AND edges.id_node_dst=nodes.id AND nodes.id_node_type="+QString::number(r->getM_B_typeId());
               QSqlQuery query2,query3;
               qDebug()<<str;
               if(query2.exec(str))
               {
                   while(query2.next())
                   {
                       if(inEdges->contains(query.value(2).toInt()) || inEdges->contains(query2.value(2).toInt()))
                       {
                           bg=query2.value(1).toString();
                           qDebug() << "bg= " << bg;
                           str="SELECT id FROM nodes WHERE id="+query2.value(0).toString();
                           str+=" AND id_node_type="+QString::number(r->getM_C_typeId());
                           if(query3.exec(str))
                           {
                               while(query3.next())
                               {
                                   cg=query3.value(0).toString();
                                   qDebug() << "cg= " << cg;
                                   int t;
                                   if(r->getM_AC_direct())
                                   {
                                       qDebug() <<"insert edge " << ag << " -> " << cg;
                                       logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                       t=insert_edge(ag,QString::number(r->getM_AC_typeId()),cg,r);
                                   }
                                   if(!r->getM_AC_direct())
                                   {
                                       qDebug() <<"insert edge " << ag << " <- " << cg;
                                       logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                       t=insert_edge(cg,QString::number(r->getM_AC_typeId()),ag,r);
                                   }
                                   if (t>0)
                                       outEdges->append(t);
                               }
                           }
                           else
                               qDebug()<<"ERROR select internal 2"<<query3.lastError().text();
                       }
                   }
               }
               else
                   qDebug()<<"ERROR select internal"<<query2.lastError().text();
           }
       }
       else
           qDebug()<<"ERROR select 2"<<query.lastError().text();

   }
}

void TitkinForm::useRuleType2(VzRule *r,QList<int> *inEdges,QList<int> *outEdges)
{
    QString ag,bg,cg,aName,bName,cName,str;
    QSqlQuery query,query2,query3;
    //1>>
    if(r->getM_AB_direct() && r->getM_BC_direct())
    {
    str="SELECT edges.id_node_src,edges.id_node_dst,edges.id,nodes.node_name FROM nodes,edges WHERE edges.id_edge_type="+
            QString::number(r->getM_AB_typeId())+
            " AND edges.quant_id="+QString::number(r->getM_AB_Quant())+" AND edges.id_node_src=nodes.id AND nodes.id_node_type="+QString::number(r->getM_A_typeId());
    if(query.exec(str))
    {
        while(query.next())
        {
            ag=query.value(0).toString();
            aName=query.value(3).toString();
            str="SELECT edges.id_node_src,edges.id_node_dst,edges.id,nodes.node_name FROM nodes,edges WHERE edges.id_edge_type="+QString::number(r->getM_BC_typeId());
            str+=" AND edges.quant_id="+QString::number(r->getM_BC_Quant())+" AND edges.id_node_src="+query.value(1).toString();
            str+=" AND edges.id_node_src=nodes.id AND nodes.id_node_type="+QString::number(r->getM_B_typeId());
            if(query2.exec(str))
            {
                while(query2.next())
                {
                    if(inEdges->contains(query.value(2).toInt()) || inEdges->contains(query2.value(2).toInt()))
                    {
                        bg=query2.value(0).toString();
                        bName=query2.value(3).toString();
                        str="SELECT id,node_name FROM nodes WHERE id="+query2.value(1).toString();
                        str+=" AND id_node_type="+QString::number(r->getM_C_typeId());
                        if(query3.exec(str))
                        {
                            while(query3.next())
                            {
                                cg=query3.value(0).toString();
                                cName=query3.value(1).toString();
                                QString name=r->getTemplate();
                                name.replace("%A",aName);
                                name.replace("%B",bName);
                                name.replace("%C",cName);
                                QStringList tt=insert_node(QString::number(r->getM_D_typeId()),name,ag,QString::number(r->getM_AD_typeId()),
                                                   cg,QString::number(r->getM_DC_typeId()),"rule_"+QString::number(r->getId()),r);
                                if (tt.size()>1)
                                {
                                    logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                    qDebug()<<"Fuck up 2";
                                    for(int i=0;i<tt.length();i++)
                                        outEdges->append(tt.at(i).toInt());
                                    qDebug()<<"Fuck up 3";
                                }
                            }
                        }
                        else
                            qDebug()<<"ERROR select internal 2"<<query3.lastError().text();
                    }
                }
            }
            else
                qDebug()<<"ERROR select internal"<<query2.lastError().text();
        }
    }
    else
        qDebug()<<"ERROR select 2"<<query.lastError().text();
    }
    //2><
    if(r->getM_AB_direct() && !r->getM_BC_direct())
    {
    str="SELECT edges.id_node_src,edges.id_node_dst,edges.id,nodes.node_name FROM nodes,edges WHERE edges.id_edge_type="+
            QString::number(r->getM_AB_typeId())+
            " AND edges.quant_id="+QString::number(r->getM_AB_Quant())+" AND edges.id_node_src=nodes.id AND nodes.id_node_type="+QString::number(r->getM_A_typeId());
    if(query.exec(str))
    {
        while(query.next())
        {
            ag=query.value(0).toString();
            aName=query.value(3).toString();
            str="SELECT edges.id_node_src,edges.id_node_dst,edges.id,nodes.node_name FROM nodes,edges WHERE edges.id_edge_type="+QString::number(r->getM_BC_typeId());
            str+=" AND edges.quant_id="+QString::number(r->getM_BC_Quant())+" AND edges.id_node_dst="+query.value(1).toString();
            str+=" AND edges.id_node_dst=nodes.id AND nodes.id_node_type="+QString::number(r->getM_B_typeId());
            if(query2.exec(str))
            {
                while(query2.next())
                {
                    if(inEdges->contains(query.value(2).toInt()) || inEdges->contains(query2.value(2).toInt()))
                    {
                        bg=query2.value(1).toString();
                        bName=query2.value(3).toString();
                        str="SELECT id,node_name FROM nodes WHERE id="+query2.value(0).toString();
                        str+=" AND id_node_type="+QString::number(r->getM_C_typeId());
                        if(query3.exec(str))
                        {
                            while(query3.next())
                            {
                                cg=query3.value(0).toString();
                                cName=query3.value(1).toString();
                                QString name=r->getTemplate();
                                name.replace("%A",aName);
                                name.replace("%B",bName);
                                name.replace("%C",cName);
                                QStringList tt=insert_node(QString::number(r->getM_D_typeId()),name,ag,QString::number(r->getM_AD_typeId()),
                                                   cg,QString::number(r->getM_DC_typeId()),"rule_"+QString::number(r->getId()),r);
                                if (tt.size()>1)
                                {
                                    logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                    outEdges->append(tt.at(0).toInt());
                                    outEdges->append(tt.at(1).toInt());
                                }
                            }
                        }
                        else
                            qDebug()<<"ERROR select internal 2"<<query3.lastError().text();
                    }
                }
            }
            else
                qDebug()<<"ERROR select internal"<<query2.lastError().text();
        }
    }
    else
        qDebug()<<"ERROR select 2"<<query.lastError().text();
    }
    //3<>
    if(!r->getM_AB_direct() && r->getM_BC_direct())
    {
    str="SELECT edges.id_node_src,edges.id_node_dst,edges.id,nodes.node_name FROM nodes,edges WHERE edges.id_edge_type="+
            QString::number(r->getM_AB_typeId())+
            " AND edges.quant_id="+QString::number(r->getM_AB_Quant())+" AND edges.id_node_dst=nodes.id AND nodes.id_node_type="+QString::number(r->getM_A_typeId());
    if(query.exec(str))
    {
        while(query.next())
        {
            ag=query.value(1).toString();
            aName=query.value(3).toString();
            str="SELECT edges.id_node_src,edges.id_node_dst,edges.id,nodes.node_name FROM nodes,edges WHERE edges.id_edge_type="+QString::number(r->getM_BC_typeId());
            str+=" AND edges.quant_id="+QString::number(r->getM_BC_Quant())+" AND edges.id_node_src="+query.value(0).toString();
            str+=" AND edges.id_node_src=nodes.id AND nodes.id_node_type="+QString::number(r->getM_B_typeId());
            if(query2.exec(str))
            {
                while(query2.next())
                {
                    if(inEdges->contains(query.value(2).toInt()) || inEdges->contains(query2.value(2).toInt()))
                    {
                        bg=query2.value(0).toString();
                        bName=query2.value(3).toString();
                        str="SELECT id,node_name FROM nodes WHERE id="+query2.value(1).toString();
                        str+=" AND id_node_type="+QString::number(r->getM_C_typeId());
                        if(query3.exec(str))
                        {
                            while(query3.next())
                            {
                                cg=query3.value(0).toString();
                                cName=query3.value(1).toString();
                                QString name=r->getTemplate();
                                name.replace("%A",aName);
                                name.replace("%B",bName);
                                name.replace("%C",cName);
                                QStringList tt=insert_node(QString::number(r->getM_D_typeId()),name,ag,QString::number(r->getM_AD_typeId()),
                                                   cg,QString::number(r->getM_DC_typeId()),"rule_"+QString::number(r->getId()),r);
                                if (tt.size()>1)
                                {
                                    logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                    outEdges->append(tt.at(0).toInt());
                                    outEdges->append(tt.at(1).toInt());
                                }
                            }
                        }
                        else
                            qDebug()<<"ERROR select internal 2"<<query3.lastError().text();
                    }
                }
            }
            else
                qDebug()<<"ERROR select internal"<<query2.lastError().text();
        }
    }
    else
        qDebug()<<"ERROR select 2"<<query.lastError().text();
    }
    //4<<
    if(!r->getM_AB_direct() && !r->getM_BC_direct())
    {
    str="SELECT edges.id_node_src,edges.id_node_dst,edges.id,nodes.node_name FROM nodes,edges WHERE edges.id_edge_type="+
            QString::number(r->getM_AB_typeId())+
            " AND edges.quant_id="+QString::number(r->getM_AB_Quant())+" AND edges.id_node_dst=nodes.id AND nodes.id_node_type="+QString::number(r->getM_A_typeId());
    if(query.exec(str))
    {
        while(query.next())
        {
            ag=query.value(1).toString();
            aName=query.value(3).toString();
            str="SELECT edges.id_node_src,edges.id_node_dst,edges.id,nodes.node_name FROM nodes,edges WHERE edges.id_edge_type="+QString::number(r->getM_BC_typeId());
            str+=" AND edges.id_node_dst="+query.value(1).toString();
            str+=" AND edges.quant_id="+QString::number(r->getM_BC_Quant())+" AND edges.id_node_dst=nodes.id AND nodes.id_node_type="+QString::number(r->getM_B_typeId());
            if(query2.exec(str))
            {
                while(query2.next())
                {
                    if(inEdges->contains(query.value(2).toInt()) || inEdges->contains(query2.value(2).toInt()))
                    {
                        bg=query2.value(1).toString();
                        bName=query2.value(3).toString();
                        str="SELECT id,node_name FROM nodes WHERE id="+query2.value(0).toString();
                        str+=" AND id_node_type="+QString::number(r->getM_C_typeId());
                        if(query3.exec(str))
                        {
                            while(query3.next())
                            {
                                cg=query3.value(0).toString();
                                cName=query3.value(1).toString();
                                QString name=r->getTemplate();
                                name.replace("%A",aName);
                                name.replace("%B",bName);
                                name.replace("%C",cName);
                                QStringList tt=insert_node(QString::number(r->getM_D_typeId()),name,ag,QString::number(r->getM_AD_typeId()),
                                                   cg,QString::number(r->getM_DC_typeId()),"rule_"+QString::number(r->getId()),r);
                                if (tt.size()>1)
                                {
                                    logStrIf(query.value(2).toInt(), query2.value(2).toInt());
                                    outEdges->append(tt.at(0).toInt());
                                    outEdges->append(tt.at(1).toInt());
                                }
                            }
                        }
                        else
                            qDebug()<<"ERROR select internal 2"<<query3.lastError().text();
                    }
                }
            }
            else
                qDebug()<<"ERROR select internal"<<query2.lastError().text();
        }
    }
    else
        qDebug()<<"ERROR select 2"<<query.lastError().text();
    }
}

void TitkinForm::useRules(QList<int> *inEdges, QList<int> *outEdges)
{
    QList<VzRule*> *rules = dataModel->getRulesList();

    QList<VzRule*>::iterator i;
    for (i = rules->begin(); i != rules->end(); ++i)
    {
        VzRule* r = *i;
        ui->textEdit->append("Use rule "+QString::number(r->getId()));
        qDebug() << "Use rule " + QString::number(r->getId());
        if(r->getRuleType()==1)
            useRuleType1(r, inEdges, outEdges);
        if(r->getRuleType()==2)
            useRuleType2(r, inEdges, outEdges);
    }
}

void TitkinForm::on_pbStart_clicked()
{
    QList<int> allEdges;
    QString str="SELECT * FROM edges";
    QSqlQuery query,q;
    if(query.exec(str))
        while(query.next())
        {
            allEdges.append(query.value("id").toInt());
            str="UPDATE edges SET isNew=isNew+1 WHERE id="+query.value("id").toString();
            if(!q.exec(str))
                qDebug()<<"ERROR inc in pbStart"<<q.lastError().text();

        }
    else
        qDebug()<<"ERROR loading all edges"<<query.lastError().text();
    QList<int> inEdges = allEdges,outEdges;

    while(inEdges.size() > 0)
    {
        qDebug()<<inEdges;
        ui->textEdit->append("Start using rules");
        useRules(&inEdges, &outEdges);
        inEdges = outEdges;
        outEdges.clear();
        ui->textEdit->append("**********");
    }

}

void TitkinForm::on_pbLoadRules_clicked()
{
    dataModel->LoadRulesList();
    qDebug()<<"load";
    ui->textEdit->append("loaded");
}

void TitkinForm::on_pb_startNew_clicked()
{
    QList<int> allEdges;
    QString str="SELECT * FROM edges WHERE isNew=0";
    QSqlQuery query,q;
    if(query.exec(str))
        while(query.next())
        {
            allEdges.append(query.value("id").toInt());
            str="UPDATE edges SET isNew=isNew+1 WHERE id="+query.value("id").toString();
            if(!q.exec(str))
                qDebug()<<"ERROR inc in pbStartNew"<<q.lastError().text();
        }
    else
        qDebug()<<"ERROR loading all edges"<<query.lastError().text();
    QList<int> inEdges = allEdges,outEdges;
    while(inEdges.size() > 0)
    {
        qDebug()<<inEdges;
        useRules(&inEdges, &outEdges);
        inEdges = outEdges;
        outEdges.clear();
    }
}
