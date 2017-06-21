#include "gostblock.h"

GostBlock::GostBlock(QObject *parent) :
    QObject(parent)
{
}

GostBlock::GostBlock(QString name, bool isOr, QObject *parent):
       QObject(parent)
{
    m_name = name;
    m_isOr = isOr;
}

GostBlock::GostBlock(const GostBlock &gostBlock, QObject *parent)
    : QObject(parent)
{
    this->m_isOr = gostBlock.m_isOr;
    this->m_name = gostBlock.m_name;
    this->m_subBlocks = gostBlock.m_subBlocks;
}

void GostBlock::operator=(const GostBlock &gostBlock)
{
    this->m_isOr = gostBlock.m_isOr;
    this->m_name = gostBlock.m_name;
    this->m_subBlocks = gostBlock.m_subBlocks;
}

QString GostBlock::name()
{
    return m_name;
}

void GostBlock::setName(QString name)
{
    m_name = name;
}

void GostBlock::setIsOr()
{
    m_isOr = true;
}

void GostBlock::setIsAnd()
{
    m_isOr = false;
}

bool GostBlock::isOr()
{
    return m_isOr;
}

bool GostBlock::isAnd()
{
    return !m_isOr;
}

QStringList *GostBlock::getSubBlocks()
{
    return &m_subBlocks;
}

void GostBlock::addSubBlock(QString subBlock)
{
    m_subBlocks.append(subBlock);
}
