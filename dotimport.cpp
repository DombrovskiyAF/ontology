#include "dotimport.h"



dotimport::dotimport(DataModel *sqlmodel, QWidget *parent)
{
    model = sqlmodel;
}

bool dotimport::ImportFromDot(QString FName,QTableWidget *edgestable,QTableWidget *nodesstable,QTableWidget *quanttable)
{
    Edgestable = new QTableWidget ; Edgestable = edgestable;
    Nodesstable = new QTableWidget ; Nodesstable = nodesstable;
    QFile file(FName);
    QRegExp nodes("\"[А-Яа-я\u0451a-zA-Z0-9_ .]+\"+[^-,]");
    QRegExp edgesfrom("\"[А-Яа-яa-zA-Z0-9_ .]+\"-");
    QRegExp labels("label=[А-Яа-яa-zA-Z0-9_ .]+");
    QStringList nodes_list,quant_list;
    QStringList nodes_id_list,quant_id_list;
    for(int i =0;i<model->m_nodeTypes->rowCount(QModelIndex());i++)
    {
        QModelIndex index = model->m_nodeTypes->index(i,nodetype_name);
        QModelIndex index2 = model->m_nodeTypes->index(i,nodetype_id);
        QVariant var_name = model->m_nodeTypes->data(index);
        QVariant var_id = model->m_nodeTypes->data(index2);
        nodes_list.append(var_name.toString());
        nodes_id_list.append(var_id.toString());

    }
    for(int i =0;i<model->m_quantTypes->rowCount(QModelIndex());i++)
    {
        QModelIndex index = model->m_quantTypes->index(i,nodetype_name);
        QModelIndex index2 =  model->m_quantTypes->index(i,nodetype_id);
        QVariant var_name =  model->m_quantTypes->data(index);
        QVariant var_id =  model->m_quantTypes->data(index2);
        quant_list.append(var_name.toString());
        quant_id_list.append(var_id.toString());

    }
    QTextStream stream(&file);
    QString dot_string;
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        qDebug() << "error open file";
        return false;
    }
    while(stream.atEnd()!=true)
    {

    dot_string = stream.readLine();
    qDebug()<< dot_string;
    int num;
    if(dot_string.contains("->"))
    {
        int pos = 0, count = 0;
        QString Findlabel;
        QString status = "1";

                    while((pos = labels.indexIn(dot_string, pos)) != -1)
                    {
                        Findlabel = labels.cap().remove("[");
                        Findlabel.remove("label=");
                        pos += labels.matchedLength();
                        count++;
                        //qDebug().noquote()<< Findlabel;
                    }
        QString quant_id;
        for(int i = 0; i<Edgestable->rowCount();i++)
        {
            if(Edgestable->item(i,1)->text()==Findlabel
                    &(dot_string.contains("color="+Edgestable->item(i,2)->text())))
            {
                for(int i = 0 ; i < quant_list.size(); i++){
                    if(dot_string.contains(quanttable->item(i,1)->text()))
                    {
                        quant_id = quant_id_list.at(quant_list.indexOf(quanttable->item(i,0)->text()));
                    }}
                if(dot_string.contains(Edgestable->item(i,3)->text()))
                {
                    qDebug()<<Edgestable->item(i,3)->text();
                    status="0";
                }
                QString from;
                QString to;

                from = dot_string.left(dot_string.indexOf("-"));
                from.remove("\"");
                //qDebug().noquote()<< from +"-"+"� ЕБ� О ИЗ";

                to = dot_string.right(dot_string.size()-1 - dot_string.indexOf(">"));
                to = to.left(to.indexOf("["));
                to.remove("\"");
                //qDebug().noquote()<< to +"-"+"� ЕБ� О В";
                QSqlQuery queryid_edge_type;
                int id;
                     if (!queryid_edge_type.exec("SELECT edge_types.id FROM edge_types WHERE (((edge_name) =\'"+Edgestable->item(i,0)->text()+"\'));"))


                     {


                         QMessageBox::information(0, "SQL INSERT:", queryid_edge_type.lastError().text());
                     }
                     while (queryid_edge_type.next()) {
                              id = queryid_edge_type.value(0).toInt();
                              qDebug() << id;
                          }
                     qDebug() << "1";
                     QSqlQuery queryid_node_type;
                     int id2;
                          if (!queryid_node_type.exec("SELECT nodes.id FROM nodes WHERE (((node_name) =\'"+from+"\'));"))


                          {

                              QMessageBox::information(0, "SQL INSERT:", queryid_edge_type.lastError().text());
                          }
                          while (queryid_node_type.next()) {
                                   id2 = queryid_node_type.value(0).toInt();
                                   qDebug() << id2;
                               }
                          QSqlQuery queryid_node_type_to;
                          int id3;
                               if (!queryid_node_type_to.exec("SELECT nodes.id FROM nodes WHERE (((node_name) ='"+to+"'));"))


                               {

                                   QMessageBox::information(0, "SQL INSERT:", queryid_edge_type.lastError().text());
                               }
                               while (queryid_node_type_to.next()) {
                                        id3 = queryid_node_type_to.value(0).toInt();
                                        qDebug() << id3;
                                    }

                     QSqlQuery query;
                     QString str,s1,s2,s3;
                    s1.setNum(id2);s2.setNum(id);s3.setNum(id3);
                     QString strF ="INSERT INTO edges "
                             "(id_node_src,id_edge_type,id_node_dst,quant_id,capacity,status,isNew) VALUES "
                             "(%1, %2, %3, '%4', %5, %6, %7);";
                     qDebug()<< quant_id;
                     str = strF.arg(s1)
                               .arg(s2)
                               .arg(s3)
                               .arg(quant_id)
                               .arg(0)
                               .arg(status)
                               .arg(0);

                     if (!query.exec(str))
                     {
                         QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
                     }
                     //qDebug().noquote()<< str;
             break;



            }
        }
    }
    else
    {
        QString status_node = "0";
    for(int i = 0; i<Nodesstable->rowCount();i++)
    {
        if(dot_string.contains(Nodesstable->item(i,1)->text())&(dot_string.contains("color="+Nodesstable->item(i,2)->text()))&(dot_string.contains("fillcolor="+Nodesstable->item(i,3)->text())))
        {
            if(dot_string.contains("style=dotted"))
            {
                status_node="1";
            }
            int pos = 0, count = 0;
            QString FindNode;

                        while((pos = nodes.indexIn(dot_string, pos)) != -1)
                        {
                            FindNode = nodes.cap().remove("[");
                            FindNode.remove("\"");
                            pos += nodes.matchedLength();
                            count++;
                            //qDebug().noquote()<< FindNode +"-"+ Nodesstable->item(i,0)->text();
                        }

                       QSqlQuery query;
                       QString str;
                       QString strF ="INSERT INTO nodes (id_node_type, node_name, brightness, status, author, rem)"
                                                 "VALUES(%1, '%2', %3, %4, '%5', '%6');";
                       int indexofnode = nodes_list.indexOf(Nodesstable->item(i,0)->text());
                       QString num = nodes_id_list.at(indexofnode);
                      // qDebug().noquote()<< num;
                       str = strF.arg(num.toInt())
                                    .arg(FindNode)
                                    .arg(1)
                                    .arg(status_node.toInt())
                                    .arg("current")
                                    .arg("none");
                      if (!query.exec(str))
                      {
                          qDebug() << str;
                          qDebug() << "Unable to make insert operation";
                          //QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
                      }
                    break;
       }
    }}
 }

return true;

}
