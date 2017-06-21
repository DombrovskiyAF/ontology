#ifndef DAFNODE_H
#define DAFNODE_H

#include "dafgraphelement.h"

#include "dafnodetype.h"
#include "dafedge.h"

#include <QMap>

class DafNode : public DafGraphElement
{
    Q_OBJECT

public:
    DafNode();
    DafNode(int id, QString name, DafNodeType *nodeType, int brightness, int status, int openCnt = 0);
    ~DafNode();

    DafNodeType *getNodeType();
    void setNodeType(DafNodeType *nodeType);

    int addOutEdge(DafEdge* edge);
    int addInEdge(DafEdge* edge);


    QList <DafEdge*> getOutEdgeList(){ return outEdges.values(); };
    QList <DafEdge*> getInEdgeList(){ return inEdges.values(); };

    QMap <QString, DafEdge*> *getOutEdgesMap(){ return &outEdges; };
    QMap <QString, DafEdge*> *getInEdgesMap(){ return &inEdges; };


    void getEdgeList(QList<DafEdge*> &list);

private:
    DafNodeType *m_nodeType;
    int m_brightness;
    int m_status;

    QMap <QString, DafEdge*> outEdges;
    QMap <QString, DafEdge*> inEdges;
};

#endif // DAFNODE_H
