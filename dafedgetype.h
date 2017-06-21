#ifndef DAFEDGETYPE_H
#define DAFEDGETYPE_H

#include "dafgraphelement.h"
#include "dafnodetype.h"

class DafEdgeType : public DafGraphElement
{
    Q_OBJECT

public:
    DafEdgeType();
    ~DafEdgeType();
    DafEdgeType(int id,
                QString name,
                DafNodeType *srcNodeType,
                DafNodeType *dstNodeType,
                QString mult);
    DafNodeType* getSrcNodeType(){ return m_srcNodeType; }
    DafNodeType* getDstNodeType(){ return m_dstNodeType; }
    QString      getMult(){ return m_mult; }
    virtual QString getShortName();

private:
    DafNodeType *m_srcNodeType;
    DafNodeType *m_dstNodeType;
    QString      m_mult;

};

#endif // DAFEDGETYPE_H
