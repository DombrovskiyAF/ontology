#include "ognodetype.h"

DafNodeType::DafNodeType()
{
}

DafNodeType::DafNodeType( QString name, int id )
{
    setId(id);
    setName(name);
}

DafNodeType::~DafNodeType()
{
    return;
}
