#ifndef DAFNODETYPE_H
#define DAFNODETYPE_H

#include "dafgraphelement.h"

class DafNodeType : public DafGraphElement
{

    Q_OBJECT

public:

    DafNodeType();
    DafNodeType( QString name, int id = -1 );
    ~DafNodeType();

};

#endif // DAFNODETYPE_H
