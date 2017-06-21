#ifndef VZEDGE_H
#define VZEDGE_H

#include <QString>

class VzEdge
{
public:
    VzEdge();
    VzEdge(int id,int nsrc,int et,int ndst,QString qs,QString qd,int c,int s);

    int m_Id;
    int m_IdNodeSrc;
    int m_IdEdgeType;
    int m_NodeDst;
    QString m_QuantSrc;
    QString m_QuantDst;
    int m_Capacity;
    int m_Status;
};

#endif // VZEDGE_H
