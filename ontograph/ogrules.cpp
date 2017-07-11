#include "ogrules.h"
#include <QSqlQuery>
#include <QVariant>
#include "datamodel.h"
//#include <oggraph.h>
#include <QDebug>
#include <QSqlError>

VzRule::VzRule():
    m_edgeBDId(-1), m_edgeDCId(-1), m_edgeADId(-1), m_edgeACId(-1), m_D_typeId(-1),
    m_edgeBCId(-1), m_edgeABId(-1), m_A_typeId(-1), m_B_typeId(-1), m_C_typeId(-1)
{
    m_D_typeId = -1;
    return;
}

VzRule::~VzRule()
{

}

void VzRule::setId(int id)
{
    m_id=id;
}

QString VzRule::getRuleDescription()
{
    QString descr;
    // если
    descr = "IF (" + m_A_typeName;

    if (m_edgeABdirect)
        descr += " --" + m_edgeABName + "-> ";
    else
        descr += " <-" + m_edgeABName + "-- ";

    descr += m_B_typeName + "  AND " + m_B_typeName;

    if(m_edgeBCdirect)
        descr += " --" + m_edgeBCName + "-> ";
    else
        descr += " <-" + m_edgeBCName + "-- ";

    descr += m_C_typeName + ") THEN ";

    // то
    if( m_ruleType == rtTranz)
    {
        descr += m_A_typeName;
        if(m_edgeACdirect)
            descr += " --" + m_edgeACName + "-> ";
        else
            descr += " <-" + m_edgeACName + "-- ";
        descr += m_C_typeName;
    }

    if( m_ruleType == rtNewConcept)
    {
        descr += m_A_typeName;
        if(m_edgeADdirect)
            descr += " --" + m_edgeADName + "-> ";
        else
            descr += " <-" + m_edgeADName + "-- ";
        descr += " AND ";

        descr += m_D_typeName;
        if(m_edgeDCdirect)
            descr += " --" + m_edgeDCName + "-> ";
        else
            descr += " <-" + m_edgeDCName + "-- ";

    }

    return descr;
}

void VzRule::setName(QString name)
{
    m_name=name;
}

void VzRule::setRem(QString rem)
{
    m_rem=rem;
}

void VzRule::setAuthor(QString author)
{
    m_author=author;
}

void VzRule::setTemplate(QString templatee)
{
    m_template=templatee;
}

int VzRule::getId()
{
    return m_id;
}

QString VzRule::getName()
{
    return m_name;
}

QString VzRule::getAuthor()
{
    return m_author;
}

QString VzRule::getRem()
{
    return m_rem;
}

QString VzRule::getTemplate()
{
    return m_template;
}

void VzRule::setM_A(QString name, int id)
{
    m_A_typeName=name;
    m_A_typeId=id;
}

void VzRule::setM_B(QString name, int id)
{
    m_B_typeName=name;
    m_B_typeId=id;
}

void VzRule::setM_C(QString name, int id)
{
    m_C_typeName=name;
    m_C_typeId=id;
}

void VzRule::setM_D(QString name, int id)
{
    m_D_typeName=name;
    m_D_typeId=id;
}

QString VzRule::getM_A_name()
{
    return m_A_typeName;
}

QString VzRule::getM_B_name()
{
    return m_B_typeName;
}

QString VzRule::getM_C_name()
{
    return m_C_typeName;
}

QString VzRule::getM_D_name()
{
    return m_D_typeName;
}

QString VzRule::getM_edgeABName()
{
    return m_edgeABName;
}

QString VzRule::getM_edgeACName()
{
    return m_edgeACName;
}

QString VzRule::getM_edgeBCName()
{
    return m_edgeBCName;
}

QString VzRule::getM_edgeADName()
{
    return m_edgeADName;
}

QString VzRule::getM_edgeDCName()
{
    return m_edgeDCName;
}
QString VzRule::getM_edgeBDName()
{
    return m_edgeBDName;
}
void VzRule::setM_edgeAB(QString name,int id,bool direct)
{
    m_edgeABdirect=direct;
    m_edgeABId=id;
    m_edgeABName=name;
}

void VzRule::setM_edgeBC(QString name,int id,bool direct)
{
    m_edgeBCdirect=direct;
    m_edgeBCId=id;
    m_edgeBCName=name;
}

void VzRule::setM_edgeAC(QString name,int id,bool direct)
{
    m_edgeACdirect=direct;
    m_edgeACId=id;
    m_edgeACName=name;
}

void VzRule::setM_edgeAD(QString name,int id,bool direct)
{
    m_edgeADdirect=direct;
    m_edgeADId=id;
    m_edgeADName=name;
}

void VzRule::setM_edgeDC(QString name,int id,bool direct)
{
    m_edgeDCdirect=direct;
    m_edgeDCId=id;
    m_edgeDCName=name;
}
void VzRule::setM_edgeBD(QString name,int id,bool direct)
{
    m_edgeBDdirect=direct;
    m_edgeBDId=id;
    m_edgeBDName=name;
}
void VzRule::setM_edgeABQuant(int quant)
{
    m_edgeABQuant=quant;
}

void VzRule::setM_edgeBCQuant(int quant)
{
    m_edgeBCQuant=quant;
}
void VzRule::setM_edgeACQuant(int quant)
{
    m_edgeACQuant=quant;
}
void VzRule::setM_edgeADQuant(int quant)
{
    m_edgeADQuant=quant;
}
void VzRule::setM_edgeDCQuant(int quant)
{
    m_edgeDCQuant=quant;
}
void VzRule::setM_edgeBDQuant(int quant)
{
    m_edgeBDQuant=quant;
}
