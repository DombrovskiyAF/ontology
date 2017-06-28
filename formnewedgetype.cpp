#include "formnewedgetype.h"
#include "ui_formnewedgetype.h"
#include <QMessageBox>
#include <datamodel.h>
#include <QSqlQuery>
#include <QSqlError>


FormNewEdgeType::FormNewEdgeType(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormNewEdgeType)
{
    ui->setupUi(this);

    ui->cb_dst_private->hide();
    ui->cb_src_private->hide();

    connect(ui->cb_src, SIGNAL(currentIndexChanged(int)),ui->cb_src_private,SLOT(setCurrentIndex(int)));
    connect(ui->cb_dst, SIGNAL(currentIndexChanged(int)),ui->cb_dst_private,SLOT(setCurrentIndex(int)));
}

FormNewEdgeType::~FormNewEdgeType()
{
    delete ui;
}

void FormNewEdgeType::on_push_add_clicked()
{
    if (!dataModel->db.isOpen())
    {
        QMessageBox::information(0, "SQL INSERT:", "Database isn't open!");
    }
    else
    {
        dataModel->insertEdgeType(ui->cb_src_private->currentText(),
                                  ui->line_edge_name->text(),
                                  ui->cb_dst_private->currentText());
        ui->line_edge_name->clear();
        hide();
    }

}

void FormNewEdgeType::on_push_cancel_clicked()
{
    ui->line_edge_name->clear();
    hide();
}

void FormNewEdgeType::on_setup_view()
{
    mapper->setModel(dataModel->m_edgeTypes);
    ui->cb_src->setModel(dataModel->m_nodeTypes);
    ui->cb_src->setModelColumn(1);

    mapper->addMapping(ui->cb_src, 1);
    ui->cb_dst->setModel(dataModel->m_nodeTypes);
    ui->cb_dst->setModelColumn(1);
    mapper->addMapping(ui->cb_dst,1);

    mapper2->setModel(dataModel->m_edgeTypes);

    ui->cb_src_private->setModel(dataModel->m_nodeTypes);
    mapper2->addMapping(ui->cb_src_private, 1);

    ui->cb_dst_private->setModel(dataModel->m_nodeTypes);
    mapper2->addMapping(ui->cb_dst_private,1);
}
