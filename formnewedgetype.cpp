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
        QSqlQuery query;
        QString str;

        QString strF ="INSERT INTO edge_types "
                "(id_node_type_src,edge_name,id_node_type_dst) VALUES "
                "(%1, '%2', %3);";
        str = strF.arg(ui->cb_src_private->currentText())
                  .arg(ui->line_edge_name->text())
                  .arg(ui->cb_dst_private->currentText());

        if (!query.exec(str))
        {
            QMessageBox::information(0, "SQL INSERT:", query.lastError().text());
        }
        else
        {
//            QMessageBox::information(0, "SQL INSERT:", "Operation successfully!");
            dataModel->m_edgeTypes->select();
            ui->line_edge_name->clear();
            hide();
        }
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
