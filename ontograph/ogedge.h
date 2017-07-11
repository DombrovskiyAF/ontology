#ifndef DAFEDGE_H
#define DAFEDGE_H

#include "oggraphelement.h"

class DafNode;
class DafEdgeType;

class DafEdge : public DafGraphElement
{
        Q_OBJECT
public:
    DafEdge();
    DafEdge(int id, DafEdgeType *edgeType, DafNode *nodeSrc, DafNode *nodeDst);
    ~DafEdge();

    DafEdgeType *getEdgeType();
    void setNType(DafEdgeType *edgeType);

    DafNode *getSrcNode() { return m_srcNode; };
    DafNode *getDstNode() { return m_dstNode;};


private:

    DafEdgeType *m_edgeType;
    DafNode *m_srcNode;
    DafNode *m_dstNode;

};

#endif // DAFEDGE_H
