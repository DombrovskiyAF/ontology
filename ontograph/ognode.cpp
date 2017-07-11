#include "ognode.h"

DafNode::DafNode()
{
    m_id = -1;
    m_name = "";//setName("");
    m_nodeType = NULL;
    m_brightness = -1;
    m_status = -1;
    m_openCnt = 0;
}

DafNode::~DafNode()
{
    return;
}

DafNode::DafNode(int id, QString name, DafNodeType *nodeType, int brightness, int status, int openCnt)
{
    m_id = id;//setId(id);
    m_name = name;//setName(name);
    m_nodeType = nodeType;
    m_brightness = brightness;
    m_status = status;
    m_openCnt = openCnt;
}

void DafNode::getEdgeList(QList<DafEdge*> &list)
{
    list.append(outEdges.values());
    list.append(inEdges.values());
    return;
}

DafNodeType *DafNode::getNodeType()
{
    return m_nodeType;
}

void DafNode::setNodeType(DafNodeType *nodeType)
{
    m_nodeType = nodeType;
}

int DafNode::addOutEdge(DafEdge* edge)
{
    outEdges.insert(edge->getName(), edge);
    return 0;
}

int DafNode::addInEdge(DafEdge* edge)
{
    inEdges.insert(edge->getName(), edge);
    return 0;
}
