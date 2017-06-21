#include "userulesform.h"
#include "ui_userulesform.h"
#include "qdebug.h"
#include "QStandardItemModel"
#include "QSqlQuery"
#include "QStandardItem"
#include "datamodel.h"
#include "QSqlError"

UseRulesForm::UseRulesForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UseRulesForm)
{
    ui->setupUi(this);
    connect( ui->pbLoadRules, SIGNAL(clicked()), this, SLOT(loadRules()) );
    connect( ui->pbUseAllRules, SIGNAL(clicked()), this, SLOT(useRules()) );
}

UseRulesForm::~UseRulesForm()
{
    delete ui;
}

void UseRulesForm::loadRules()
{
    qDebug() << "loadRules";
    dataModel->LoadRulesList();
}

void UseRulesForm::useRules()
{
    qDebug() << "useRules";

    QList<VzEdge> allEdges;
    // и загрузить в список все ребра
    QSqlQuery query;
    QString str="SELECT * FROM edges";
    if(query.exec(str))
        while(query.next())
        {
            VzEdge e(query.value("id").toInt(),
                     query.value("id_node_src").toInt(),
                     query.value("id_edge_type").toInt(),
                     query.value("id_node_dst").toInt(),
                     query.value("quant_src").toString(),
                     query.value("quant_dst").toString(),
                     query.value("capacity").toInt(),
                     query.value("status").toInt());
            allEdges.append(e);
        }
    else
        qDebug()<<"ERROR loading all edges"<<query.lastError().text();

    QList<VzEdge> newEdges;

    QList<VzEdge> *inEdges = &allEdges;
    QList<VzEdge> *outEdges = &newEdges;
    QList<VzEdge> *tmpEdges;

    while(inEdges->size() > 0)
    {
        dataModel->UseRules(inEdges, outEdges);
        inEdges->clear();
        tmpEdges = inEdges;
        inEdges = outEdges;
        outEdges = tmpEdges;
    }
}
