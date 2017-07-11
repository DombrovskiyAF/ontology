#include "oggraph.h"

#include "ogedge.h"
#include "ogedgetype.h"
#include "ognode.h"
#include "ognodetype.h"
#include "datamodel.h"

#include <QDebug>
#include <QQueue>
#include <QStack>
#include <QSqlRelationalTableModel>
#include <QFile>

DafGraph::DafGraph()
{

}

DafGraph::~DafGraph()
{
    clearGraphData();
    clearGraphTypes();
}

void DafGraph::clearGraphData()
{
    QMapIterator<QString, DafEdge*> ie(m_edges);
    while (ie.hasNext())
    {
        ie.next();
        DafEdge* et = ie.value();
        delete et;
    }
    m_edges.clear();

    QMapIterator<QString, DafNode*> in(m_nodes);
    while (in.hasNext())
    {
        in.next();
        DafNode* nt = in.value();
        delete nt;
    }
    m_nodes.clear();
}

void DafGraph::clearGraphTypes()
{
    QMapIterator<QString, DafEdgeType*> ie(m_edgeTypes);
    while (ie.hasNext())
    {
        ie.next();
        DafEdgeType* et = ie.value();
        delete et;
    }
    m_edgeTypes.clear();

    QMapIterator<QString, DafNodeType*> in(m_nodeTypes);
    while (in.hasNext())
    {
        in.next();
        DafNodeType* nt = in.value();
        delete nt;
    }
    m_nodeTypes.clear();
}

int DafGraph::getNodeTypeId(QString nodeTypeName)
{
    DafNodeType *n = m_nodeTypes.value(nodeTypeName, NULL);
    if (n == NULL)
        return -1;

    return n->getId();
}

int DafGraph::getEdgeTypeId(QString nodeSrcTypeName, QString edgeTypeName, QString nodeDstTypeName)
{
    QString etn = nodeSrcTypeName + "->" + edgeTypeName + "->" +nodeDstTypeName;
    DafEdgeType *n = m_edgeTypes.value(etn, NULL);
    if (n == NULL)
        return -1;

    return n->getId();
}


/*
int DafGraph::loadRules()
{
    QSqlRelationalTableModel *modelRules;
    QSqlRelationalTableModel *modelRuleItems;

    modelRules = new QSqlRelationalTableModel();
    modelRules->setTable("rules");
    modelRules->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    if(!modelRules->select())
    {
        qDebug() << "Error: not select rules";
        return -1;
    }

    modelRuleItems = new QSqlRelationalTableModel(0);
    modelRuleItems->setTable("rules_items");
    modelRuleItems->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    if(!modelRuleItems->select())
    {
        qDebug() << "Error: not select rules_items";
        return -1;
    }

    int numRows;
    numRows = modelRules->rowCount(QModelIndex());
    qDebug() << "numRows= " << numRows;
    for (int row = 0; row < numRows; ++row)
    {
        QModelIndex index = modelRules->index(row, rules_id, QModelIndex());
        int id = modelRules->data(index, Qt::DisplayRole).toInt();

        index = modelRules->index(row, rules_name, QModelIndex());
        QString name = modelRules->data(index, Qt::DisplayRole).toString();

        index = modelRules->index(row, rules_author, QModelIndex());
        QString author = modelRules->data(index, Qt::DisplayRole).toString();

        index = modelRules->index(row, rules_rem, QModelIndex());
        QString rem = modelRules->data(index, Qt::DisplayRole).toString();

        VzRule* rule = new VzRule( id, name, author, rem);

        modelRuleItems->setFilter("id_rule = " + QString::number(id));
        int numRulesItemsRows = modelRuleItems->rowCount();
        qDebug() << "numRulesItemsRows= " << numRulesItemsRows;

        QVector<VzRulesItems*> ruleItems;
        for( int riRow = 0; riRow < numRulesItemsRows; ++riRow )
        {
            QModelIndex item =  modelRuleItems->index(riRow, rulesitems_id, QModelIndex());
            int id = modelRuleItems->data(item,Qt::DisplayRole).toInt();

            item =  modelRuleItems->index(riRow, rulesitems_id_rule, QModelIndex());
            int id_rule = modelRuleItems->data(item,Qt::DisplayRole).toInt();

            item =  modelRuleItems->index(riRow, rulesitems_node_type_id, QModelIndex());
            int node_type_id = modelRuleItems->data(item,Qt::DisplayRole).toInt();

            item =  modelRuleItems->index(riRow, rulesitems_node_name, QModelIndex());
            QString node_name = modelRuleItems->data(item,Qt::DisplayRole).toString();

            item =  modelRuleItems->index(riRow, rulesitems_node_isnew, QModelIndex());
            bool node_isnew =  modelRuleItems->data(item,Qt::DisplayRole).toBool();

            item =  modelRuleItems->index(riRow, rulesitems_edge_type_id, QModelIndex());
            int edge_type_id = modelRuleItems->data(item,Qt::DisplayRole).toInt();

            item =  modelRuleItems->index(riRow, rulesitems_edge_isnew, QModelIndex());
            bool edge_isnew =  modelRuleItems->data(item,Qt::DisplayRole).toBool();

            item =  modelRuleItems->index(riRow, rulesitems_node_dst, QModelIndex());
            int node_dst = modelRuleItems->data(item,Qt::DisplayRole).toInt();

            VzRulesItems* rulesItem = new VzRulesItems(id,    id_rule,    node_type_id, node_name,
                         node_isnew,    edge_type_id,    edge_isnew,    node_dst);

            ruleItems.append(rulesItem);
        }

        if (rule->createFuflet(&ruleItems, &nodeTypes, &edgeTypes))
        {
            rules.insert(id, rule);
        }
        else
        {
            delete rule;
        }

        ruleItems.clear();
    }

    delete modelRuleItems;
    delete modelRules;
    return 0;
}
*/

int DafGraph::UseRule(VzRule *r)
{
    qDebug() << "UseRule: " << r->getName();
    QMapIterator<QString, DafNode*> in(m_nodes);
    // перебор по всем узлам
    while (in.hasNext())
    {
        in.next();
        DafNode* nA = in.value();
        qDebug() << "UseRule: " << r->getName();
        // проверяем N.type == А.type ?
        if (nA->getNodeType()->getId() == r->getM_A_typeId())
        {
            qDebug() << "  find A: " << nA->getName();
            // перебор по всем А-B
            QMap <QString, DafEdge*> *edgesMap;
            if (r->getM_AB_direct())
              edgesMap = nA->getOutEdgesMap();
            else
              edgesMap = nA->getInEdgesMap();

            QMap <QString, DafEdge*>::iterator ieAB;
            for ( ieAB = edgesMap->begin(); ieAB != edgesMap->end(); ++ieAB)
            {
                DafEdge* eAB = *ieAB;
                qDebug() << "LINE: " << __LINE__ << "eAB->getEdgeType()->getId()= " <<eAB->getEdgeType()->getId() << ",  r->getM_AB_typeId()= " <<r->getM_AB_typeId();
                if(eAB->getEdgeType()->getId() == r->getM_AB_typeId())
                {
                    qDebug() << "    find AB: " << eAB->getEdgeType()->getName();
                    DafNode* nB;
                    if (r->getM_AB_direct())
                        nB = eAB->getDstNode();
                    else
                        nB = eAB->getSrcNode();
                    qDebug() << "      find B: " << nB->getName();
                    // перебор по всем B-C
                    QMap <QString, DafEdge*> *edgesBCMap;
                    if (r->getM_BC_direct())
                      edgesBCMap = nB->getOutEdgesMap();
                    else
                      edgesBCMap = nB->getInEdgesMap();

                    QMap <QString, DafEdge*>::iterator ieBC;
                    for(ieBC = edgesBCMap->begin(); ieBC != edgesBCMap->end(); ++ieBC)
                    {
                        DafEdge* eBC = *ieBC;
                        if(eBC->getEdgeType()->getId() == r->getM_BC_typeId())
                        {
                             qDebug() << "    find BC: " << eBC->getEdgeType()->getName();
                             DafNode* nC;
                             if (r->getM_BC_direct())
                                 nC = eBC->getDstNode();
                             else
                                 nC = eBC->getSrcNode();
                             qDebug() << "      find C: " << nC->getName();
                             qDebug() << "=======================";
                             qDebug() << nA->getName() << " " << eAB->getName() << " "
                                      << nB->getName() << " " << eBC->getName() << " "
                                      << nC->getName();
                             qDebug() << "=======================";

                             // проверка отсутствия условия "ТО" -----------------------------
                             // для типа "транзакция"
                             if(r->getRuleType() == rtTranz)
                             {
                                 QMap <QString, DafEdge*> *edgesACMap;
                                 DafNode *nL, *nR;
                                 if (r->getM_AC_direct())
                                 {
                                     nL = nA; nR = nC;
                                 }
                                 else
                                 {
                                     nL = nC; nR = nA;
                                 }
                                 edgesACMap = nL->getOutEdgesMap();

                                 QMap <QString, DafEdge*>::iterator ieLP;
                                 bool findElse = false;
                                 for(ieLP = edgesACMap->begin(); ieLP != edgesACMap->end(); ++ieLP)
                                 {
                                     DafEdge* eLP = *ieLP;
                                     if( eLP->getEdgeType()->getId() == r->getM_AC_typeId() &&
                                         eLP->getSrcNode()->getId() == nL->getId() &&
                                         eLP->getDstNode()->getId() == nR->getId()
                                       )
                                     {
                                         findElse = true;
                                         break;
                                     }
                                 }

                                 if(findElse)
                                 {
                                     qDebug() << "Ooops. A->C finded";
                                 }
                                 else
                                 {
                                     qDebug() << "ok. Add A->C ";
                                 }
                             }


                             // для типа "новый_концепт"
                             if(r->getRuleType() == rtNewConcept)
                             {
                                 bool findElse = false;
                                 QMap <QString, DafEdge*> *edgesADMap;
                                 if (r->getM_AD_direct())
                                 {
                                     edgesADMap = nA->getOutEdgesMap();
                                 }
                                 else
                                 {
                                     edgesADMap = nA->getInEdgesMap();
                                 }


                                 QMap <QString, DafEdge*>::iterator ieAD;
                                 for(ieAD = edgesADMap->begin(); ieAD != edgesADMap->end(); ++ieAD)
                                 {
                                     DafEdge* eAD = *ieAD;
                                     if( eAD->getEdgeType()->getId() == r->getM_AD_typeId() )
                                     {
                                         DafNode *nD;
                                         if (r->getM_AD_direct())
                                         {
                                             nD = eAD->getDstNode();
                                         }
                                         else
                                         {
                                             nD = eAD->getSrcNode();
                                         }
                                         QMap <QString, DafEdge*> *edgesDCMap;
                                         if(r->getM_DC_direct())
                                         {
                                             edgesDCMap = nD->getOutEdgesMap();
                                         }
                                         else
                                         {
                                             edgesDCMap = nD->getInEdgesMap();
                                         }
                                         QMap <QString, DafEdge*>::iterator ieDC;
                                         for(ieDC = edgesDCMap->begin(); ieDC != edgesDCMap->end(); ++ieDC)
                                         {
                                             DafEdge* eDC = *ieDC;
                                             if(eDC->getEdgeType()->getId() == r->getM_DC_typeId())
                                             {
                                                 findElse = true;
                                                 break;
                                             }
                                         }
                                     }
                                 }

                                 if(findElse)
                                 {
                                     qDebug() << "Ooops. A-D-C finded";
                                 }
                                 else
                                 {
                                     qDebug() << "ok. Add A-D-C";
                                 }
                             }

                        }
                    }
                }

            }




        }
    }


    return 0;
}

/*
int DafGraph::obhod(int rootNodeId)
{
    if (nodes.isEmpty())
        return -1;

    DafNode *rootNode = nodes.value(rootNodeId);
    if(rootNode == NULL)
        rootNode = nodes.first();

    qDebug() << "root= " << rootNode->getName();

    // пометить всех как new
    //??

    QQueue<DafNode*> queue;
    queue.enqueue(rootNode);


    while (!queue.isEmpty())
    {
        DafNode *node = queue.dequeue();
        node->incOpenCnt();

        //  обработать узел ...
        qDebug() << node->getName();

        // цикл по смежным узлам
        //QList<DafEdge*> edges;
        //getEdgeList(edges);
        QList <DafEdge*>::iterator edge_i;

        QList<DafEdge*> edges = node->getOutEdgeList();
        edge_i = edges.begin();
        for ( ; edge_i != edges.end(); ++edge_i)
        {
            DafEdge *de = *edge_i;
            DafNode *n = de->getDstNode();
            if(n->isNew())
            {
                n->incOpenCnt();
                queue.enqueue(n);
            }
        }

        edges = node->getInEdgeList();
        edge_i = edges.begin();
        for ( ; edge_i != edges.end(); ++edge_i)
        {
            DafEdge *de = *edge_i;
            DafNode *n = de->getSrcNode();
            if(n->isNew())
            {
                 n->incOpenCnt();
                 queue.enqueue(n);
            }
        }
    }

    return 0;
}
*/
int DafGraph::addNodeType( QString name, int id )
{
    DafNodeType* nodeType = new DafNodeType(name, id);
    m_nodeTypes.insert(name, nodeType);
    return 0;
}

int DafGraph::addEdgeType( QString name, QString srcNodeType, QString dstNodeType, QString mult, int id )
{
    DafNodeType *ntSrc = m_nodeTypes.value(srcNodeType);
    DafNodeType *ntDst = m_nodeTypes.value(dstNodeType);
    DafEdgeType *edgeType = new DafEdgeType(id, name, ntSrc, ntDst, mult);
    qDebug() << "edgeType->getName()= " << edgeType->getName();
    m_edgeTypes.insert(edgeType->getName(), edgeType);
    return 0;
}

int DafGraph::addNode( QString name, QString nodeTypeName, int brigh, int status, int id )
{
    DafNodeType *nodeType;
    nodeType = m_nodeTypes.value(nodeTypeName, NULL);
    if(nodeType == NULL)
    {
        qDebug() << "Error: nodeType not exist: " << nodeTypeName;
        return -1;
    }
    DafNode *node = new DafNode(id, name, nodeType, brigh, status);
    qDebug() << "node->getName()= " << node->getName();
    m_nodes.insert(node->getName(), node);
    return 0;
}

int DafGraph::addEdge( QString edgeTypeName, QString nodeSrcName, QString nodeDstName, int id )
{
    DafNode *nodeSrc = m_nodes.value(nodeSrcName);
    DafNode *nodeDst = m_nodes.value(nodeDstName);
    QString longName =  nodeSrc->getNodeType()->getName() + "->" +
                        edgeTypeName + "->" +
                        nodeDst->getNodeType()->getName();
    DafEdgeType *edgeType = m_edgeTypes.value(longName);
    DafEdge *edge = new DafEdge(id, edgeType, nodeSrc, nodeDst);
    edge->setName(nodeSrc->getName()+"->" +
                  edgeType->getName() + "->" +
                  nodeDst->getName());
    nodeSrc->addOutEdge(edge);
    nodeDst->addInEdge(edge);
    qDebug() << "edge->getName()= " << edge->getName();
    m_edges.insert(edge->getName(), edge);
    return 0;
}
/*
DafNode* DafGraph::addProduceNode(int type_id, QString name, int brigh, int status)
{
    DafNodeType *nodeType = nodeTypes.value(type_id);
    DafNode *node = new DafNode(-1, name, nodeType, brigh, status);
    node->setId(nodes.lastKey() + 1);
    m_nodes.insert(name, node);
    nodes.insert(node->getId(), node);
    return node;
}

DafEdge* DafGraph::addProduceNodeEdge(int type_id, DafNode* nodeSrc, DafNode* nodeDst)
{
    DafEdgeType *edgeType = edgeTypes.value(type_id);
    DafEdge *edge = new DafEdge(-1, edgeType, nodeSrc, nodeDst);
    edge->setName(nodeSrc->getName()+"->"+nodeDst->getName());
    nodeSrc->addOutEdge(edge);
    nodeDst->addInEdge(edge);
    edge->setId(edges.lastKey() + 1);
    m_edges.insert(edge->getName(), edge);
    edges.insert(edge->getId(), edge);
    return edge;
}
*/
int DafGraph::toFile(int rootNodeId)
{
/*
    if (nodes.isEmpty())
          return -1;

      DafNode *rootNode = nodes.value(rootNodeId);
      if(rootNode == NULL)
          rootNode = nodes.first(); //!!
      qDebug() << "Save to File" << endl;
      qDebug() << "root= " << rootNode->getName();



      QStack<DafNode*> stack;
      stack.push(rootNode);

    QString buffer = "digraph G\{";
      QFile file("graph.dot");
      QTextStream stream(&file);
      if (!file.open(QIODevice::WriteOnly|QIODevice::Text)) qDebug() << "error open file";
      else
          {

        stream << buffer << endl;
        stream.flush();
      while (!stack.isEmpty())
      {

          DafNode *node = stack.pop();
          node->incOpenCnt();


          qDebug() << node->getName();

          QList <DafEdge*>::iterator edge_i;


          QList<DafEdge*> edges = node->getOutEdgeList();
          edge_i = edges.begin();
          for ( ; edge_i != edges.end(); ++edge_i)
          {
              DafEdge *de = *edge_i;
              DafNode *n = de->getDstNode();
              if(n->isNew())
              {
                  buffer = "\"" +node->getName() + "\"" + "->" + "\"" + de->getDstNode()->getName() + "\"" + "[label=\"" + de->getEdgeType()->getName() + "\"]";
                  stream << buffer << endl;
                  qDebug()<< buffer << endl;
                  stream.flush();
                  n->incOpenCnt();
                  stack.push(n);
              }
          }

          edges = node->getInEdgeList();
          edge_i = edges.begin();
          for ( ; edge_i != edges.end(); ++edge_i)
          {
              DafEdge *de = *edge_i;
              DafNode *n = de->getSrcNode();
              if(n->isNew())
              {
                   buffer = "\"" + de->getSrcNode()->getName() + "\"" + "->" + "\"" + node->getName() + "\"" + "[label=\"" + de->getEdgeType()->getName() + "\"]";
                   stream << buffer << endl;
                   qDebug()<< buffer << endl;
                   stream.flush();
                   n->incOpenCnt();
                   stack.push(n);
              }
          }
      }
  }
      buffer = "\}";
    stream << buffer;
    stream.flush();
    file.close();
*/
    return 0;
}


int DafGraph::obhod_deep(int rootNodeId)
{
/*
    if (nodes.isEmpty())
        return -1;

    DafNode *rootNode = nodes.value(rootNodeId);
    if(rootNode == NULL)
        rootNode = nodes.first();
    qDebug() << "Deep" << endl;
    qDebug() << "root= " << rootNode->getName();



    QStack<DafNode*> stack;
    stack.push(rootNode);




    while (!stack.isEmpty())
    {
        DafNode *node = stack.pop();
        node->incOpenCnt();


        qDebug() << node->getName();

        ;
        QList <DafEdge*>::iterator edge_i;

        QList<DafEdge*> edges = node->getOutEdgeList();
        edge_i = edges.begin();
        for ( ; edge_i != edges.end(); ++edge_i)
        {
            DafEdge *de = *edge_i;
            DafNode *n = de->getDstNode();
            if(n->isNew())
            {
                n->incOpenCnt();
                stack.push(n);
            }
        }

        edges = node->getInEdgeList();
        edge_i = edges.begin();
        for ( ; edge_i != edges.end(); ++edge_i)
        {
            DafEdge *de = *edge_i;
            DafNode *n = de->getSrcNode();
            if(n->isNew())
            {
                 n->incOpenCnt();
                 stack.push(n);
            }
        }
    }


    */
    return 0;
}

DafNodeType* DafGraph::getNodeTypeById(int id)
{
    foreach(DafNodeType* nt,m_nodeTypes)
    {
        if(nt->getId()==id) return nt;
    }
}

DafEdgeType* DafGraph::getEdgeTypeById(int id)
{
    foreach(DafEdgeType* et,m_edgeTypes)
    {
        if(et->getId()==id) return et;
    }
}
