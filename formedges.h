#ifndef FORMEDGES_H
#define FORMEDGES_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QTreeWidgetItem>

#include <formnewedge.h>

namespace Ui {
class FormEdges;
}

class FormEdges : public QDialog
{
    Q_OBJECT

public:
    explicit FormEdges(QWidget *parent = 0);
    ~FormEdges();

    QDataWidgetMapper *mapper_src;
    QDataWidgetMapper *mapper_src_priv;
    QDataWidgetMapper *mapper_type;
    QDataWidgetMapper *mapper_type_priv;
    QDataWidgetMapper *mapper_dst;
    QDataWidgetMapper *mapper_dst_priv;
    QDataWidgetMapper *mapper_quant;
    QDataWidgetMapper *mapper_quant_priv;

    void setEdgesModel(QAbstractItemModel *model);

    bool add_flag;
    bool edit_flag;

    QString node_for_delete;

    FormNewEdge *formnewedge;// = new FormNewEdge;

public slots:
    void on_select(QModelIndex current);
    void on_add();
    void on_save();
    void on_del();
    void on_revert();
    void on_node_input();
    void on_node_output();
    void open_add();
    void open_src();
    void open_dst();
    //void create_update_tree();

    void data_canged();

    void src_tree_clicked(QModelIndex clicked);

    void on_right_click(QPoint child);

    void on_right_delete();
    //void on_right_delete_all(QString node);

    QAbstractItemModel* getmodel();

private:
    Ui::FormEdges *ui;
};

#endif // FORMEDGES_H
