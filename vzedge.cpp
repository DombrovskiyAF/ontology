#include "vzedge.h"

VzEdge::VzEdge()
{

}

VzEdge::VzEdge(int id,int nsrc,int et,int ndst,QString qs,QString qd,int c,int s):
m_Id(id), m_IdNodeSrc(nsrc), m_IdEdgeType(et), m_NodeDst(ndst),
m_QuantSrc(qs), m_QuantDst(qd), m_Capacity(c), m_Status(s)
{

}
