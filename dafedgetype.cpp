#include "dafedgetype.h"

DafEdgeType::DafEdgeType()
{
    return;
}

DafEdgeType::~DafEdgeType()
{
    return;
}

DafEdgeType::DafEdgeType(int id,
                         QString name,
                         DafNodeType *srcNodeType,
                         DafNodeType *dstNodeType,
                         QString mult)
{
    m_mult = mult;
    setId(id);
    QString longName = srcNodeType->getName() + "->" +
                       name + "->" +
                       dstNodeType->getName();
    setName(longName);
    m_srcNodeType = srcNodeType;
    m_dstNodeType = dstNodeType;
}

QString DafEdgeType::getShortName()
{
    // todo: выделить имя роли из полного имени
    return m_name;
}
