#ifndef DAFGRAPH_H
#define DAFGRAPH_H

#include "oggraphelement.h"

#include <QMap>

class DafNodeType;
class DafEdgeType;
class DafNode;
class DafEdge;

class VzRule;

class DafGraph : public DafGraphElement
{
    Q_OBJECT

public:
    DafGraph();
    ~DafGraph();
    void clearGraphData();
    void clearGraphTypes();

    int UseRule(VzRule *r);

    // возвращает идентификатор типа узла с именем nodeTypeName
    // или -1, если такого типа узла нет
    int getNodeTypeId(QString nodeTypeName);

    // возвращает идентификатор типа узла с именем nodeTypeName
    //
    int getEdgeTypeId(QString nodeSrcTypeName, QString edgeTypeName, QString nodeDstTypeName);



    int addNodeType( QString name, int id = -1 );
    int addEdgeType( QString name, QString srcNodeType, QString dstNodeType, QString mult, int id = -1 );
    int addNode( QString name, QString nodeType, int brigh, int status, int id = -1 );
    int addEdge( QString edgeType, QString nodeSrc, QString nodeDst, int id = -1 );

    QMap<QString, DafNodeType*> *getNodeTypes(){ return &m_nodeTypes;}
    QMap<QString, DafEdgeType*> *getEdgeTypes(){ return &m_edgeTypes;}
    QMap<QString, DafNode*> *getNodes(){ return &m_nodes;}
    QMap<QString, DafEdge*> *getEdges(){ return &m_edges;}


    DafNodeType* getNodeTypeById(int id);
    DafEdgeType* getEdgeTypeById(int id);

    /*! Добавляет узел в граф, не внося его в модель
     *
     */
    //DafNode* addProduceNode(int type_id, QString name, int brigh, int status);

    /*! Добавляет ребро в граф, не внося его в модель
     *
     */
    //DafEdge* addProduceNodeEdge(int type_id, DafNode* nodeSrc, DafNode* nodeDst);


    int obhod(int rootNodeId);
    int obhod_deep(int rootNodeId);
    int toFile(int rootNodeId);

private:

    /*!
     * \brief m_nodes
     */
    QMap<QString, DafNode*> m_nodes;

    /*!
     * \brief m_edges
     */
    QMap<QString, DafEdge*> m_edges;

    /*!
     * \brief nodeTypes
     */
    QMap<QString, DafNodeType*> m_nodeTypes;

    /*!
     * \brief edgeTypes
     */
    QMap<QString, DafEdgeType*> m_edgeTypes;

};

#endif // DAFGRAPH_H
