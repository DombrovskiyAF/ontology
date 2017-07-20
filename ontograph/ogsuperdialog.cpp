#include "ogsuperdialog.h"
#include "ui_ogsuperdialog.h"

#include "datamodel.h"

#include "ognodetype.h"
#include "ognode.h"
#include "ogedgetype.h"
#include "ogedge.h"
#include "oggraph.h"
#include <QDebug>

//QList<VzRule*> rules;

DafSuperDialog::DafSuperDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DafSuperDialog)
{
    ui->setupUi(this);
    connect(ui->pbStart, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->pbObhod, SIGNAL(clicked()), this, SLOT(obhod()));
}

DafSuperDialog::~DafSuperDialog()
{
    delete ui;
}

/*void DafSuperDialog::obhod()
{
    //int rootNodeId = ui->spinBox->value();
    //graph->obhod(rootNodeId);
}
/*
void DafSuperDialog::start()
{

    //graph->loadData();
    //graph->loadRules();
    qDebug() << "start..";
    //
    dataModel->createDafGraph();
    qDebug() << "end..";
    DafGraph* graph = dataModel->getGraph();


    ui->textEdit->append("========================================================");
    ui->textEdit->append("node_types");
    QList <DafNodeType*> dafNodeTypeList = graph->getNodeTypes()->values();
    QList <DafNodeType*>::iterator i = dafNodeTypeList.begin();
    for ( ; i != dafNodeTypeList.end(); ++i)
    {
        DafNodeType *dnt = *i;
        QString txt = QString::number(dnt->getId()) + "   " + dnt->getName();
        ui->textEdit->append(txt);
    }

    ui->textEdit->append("========================================================");
    ui->textEdit->append("edge_types");
    QList<DafEdgeType*> dafEdgeTypeList = graph->getEdgeTypes()->values();
    QList<DafEdgeType*>::iterator j = dafEdgeTypeList.begin();
    for ( ; j != dafEdgeTypeList.end(); ++j)
    {
        DafEdgeType *det = *j;
        QString txt = QString::number(det->getId()) + "   "
                + det->getSrcNodeType()->getName() + "   "
                + det->getName() + "   "
                + det->getDstNodeType()->getName() + "   "
                + det->getMult();
        ui->textEdit->append(txt);
    }

    ui->textEdit->append("========================================================");
    ui->textEdit->append("nodes");
    QList <DafNode*> dafNodeList = graph->getNodes()->values();
    QList <DafNode*>::iterator n = dafNodeList.begin();
    for ( ; n != dafNodeList.end(); ++n)
    {
        DafNode *dn = *n;
        QString txt = QString::number(dn->getId()) + "   " + dn->getName()
                + "   " + dn->getNodeType()->getName();
        ui->textEdit->append(txt);
    }

    ui->textEdit->append("========================================================");
    ui->textEdit->append("edges");
    QList <DafEdge*> dafEdgeList = graph->getEdges()->values();
    QList <DafEdge*>::iterator e = dafEdgeList.begin();
    for ( ; e != dafEdgeList.end(); ++e)
    {

        DafEdge *de = *e;
               QString txt = QString::number(de->getId()) + "   "+  de->getSrcNode()->getName() + "      " + de->getName() + "    " +  de->getDstNode()->getName();
            ui->textEdit->append(txt);
    }

}
*/
//void DafSuperDialog::on_pushButton_clicked()
//{
//    //graph->toFile(NULL);
//}

void DafSuperDialog::on_pushButton_2_clicked()
{
    ui->textEdit->clear();
    //start();
    //int rootNodeId = ui->spinBox->value();
    //graph->obhod_deep(rootNodeId);
}

void DafSuperDialog::on_pbAddNode_clicked()
{
    //dataModel->addNodeToModel(ui->lineEditNodeName->text(), ui->lineEditNodeType->text(), 1, 1, "автор", "remark");
}

void DafSuperDialog::on_pbViewGraph_clicked()
{
    DafGraph* graph = dataModel->getGraph();
    ui->textEdit->append("========================================================");
    ui->textEdit->append("node_types");
    QList <DafNodeType*> dafNodeTypeList = graph->getNodeTypes()->values();
    QList <DafNodeType*>::iterator i = dafNodeTypeList.begin();
    for ( ; i != dafNodeTypeList.end(); ++i)
    {
        DafNodeType *dnt = *i;
        QString txt = QString::number(dnt->getId()) + "   " + dnt->getName();
        ui->textEdit->append(txt);
    }

    ui->textEdit->append("========================================================");
    ui->textEdit->append("edge_types");
    QList<DafEdgeType*> dafEdgeTypeList = graph->getEdgeTypes()->values();
    QList<DafEdgeType*>::iterator j = dafEdgeTypeList.begin();
    for ( ; j != dafEdgeTypeList.end(); ++j)
    {
        DafEdgeType *det = *j;
        QString txt = QString::number(det->getId()) + "   "
                + det->getSrcNodeType()->getName() + "   "
                + det->getName() + "   "
                + det->getDstNodeType()->getName() + "   "
                + det->getMult();
        ui->textEdit->append(txt);
    }

    ui->textEdit->append("========================================================");
    ui->textEdit->append("nodes");
    QList <DafNode*> dafNodeList = graph->getNodes()->values();
    QList <DafNode*>::iterator n = dafNodeList.begin();
    for ( ; n != dafNodeList.end(); ++n)
    {
        DafNode *dn = *n;
        QString txt = QString::number(dn->getId()) + "   " + dn->getName()
                + "   " + dn->getNodeType()->getName();
        ui->textEdit->append(txt);
    }

    ui->textEdit->append("========================================================");
    ui->textEdit->append("edges");
    QList <DafEdge*> dafEdgeList = graph->getEdges()->values();
    QList <DafEdge*>::iterator e = dafEdgeList.begin();
    for ( ; e != dafEdgeList.end(); ++e)
    {

        DafEdge *de = *e;
               QString txt = QString::number(de->getId()) + "   "+  de->getSrcNode()->getName() + "      " + de->getName() + "    " +  de->getDstNode()->getName();
            ui->textEdit->append(txt);
    }

}

void DafSuperDialog::on_pbSaveNewToDB_clicked()
{

}
