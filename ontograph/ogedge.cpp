#include "ogedge.h"

#include "ogedgetype.h"
#include "ognode.h"

DafEdge::DafEdge()
{
    m_id = -1;
    m_name = "";//setName("");
    m_edgeType = NULL;
    m_dstNode = NULL;
    m_srcNode = NULL;
    m_openCnt = 0;
}

DafEdge::~DafEdge()
{
    return;
}

DafEdge::DafEdge(int id, DafEdgeType *edgeType, DafNode *nodeSrc, DafNode *nodeDst)
{
    setId(id);
    setName(edgeType->getName());
    m_edgeType = edgeType;
    m_srcNode = nodeSrc;
    m_dstNode = nodeDst;
}

DafEdgeType *DafEdge::getEdgeType()
{
    return m_edgeType;
}

void DafEdge::setNType(DafEdgeType *edgeType)
{
    m_edgeType = edgeType;
}
