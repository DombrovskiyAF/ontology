#include "dafnodetype.h"

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
