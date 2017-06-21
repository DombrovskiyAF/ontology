#include "dafgraphelement.h"

#include "datamodel.h"

DafGraphElement::DafGraphElement(QObject *parent) :
    QObject(parent)
{
    m_id = -1;
    m_isProduce = false;
}

int DafGraphElement::getId()
{
    return m_id;
}
void DafGraphElement::setId(int id)
{
    m_id = id;
}

QString DafGraphElement::getName()
{
    return m_name;
}

void DafGraphElement::setName(QString name)
{
    m_name = name;
}

bool DafGraphElement::isNew()
{
    if ( m_openCnt == 0 )
        return true;
    return false;
}

bool DafGraphElement::isProduce()
{
    return m_isProduce;
}
