#ifndef RULE_H
#define RULE_H

#include <QString>

struct Rule
{
    int id;
    QString name;
    QString author;
    QString rem;
};

struct RuleNode
{
    int id;
    int type;
    bool is_create;
    QString name;
};

struct RuleEdge
{
    int id;
    int type;
    int node_src;
    int node_dst;
    bool is_create;
    QString quant_src;
    QString quant_dst;
};

#endif // RULE_H
