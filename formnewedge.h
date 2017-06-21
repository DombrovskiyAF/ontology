#ifndef FORMNEWEDGE_H
#define FORMNEWEDGE_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QTreeWidgetItem>

#include <tree.h>

namespace Ui {
class FormNewEdge;
}

class FormNewEdge : public QDialog
{
    Q_OBJECT

public:
    explicit FormNewEdge(QWidget *parent = 0);
    ~FormNewEdge();

    QDataWidgetMapper *mapper_src;
    QDataWidgetMapper *mapper_src_priv;
    QDataWidgetMapper *mapper_type;
    QDataWidgetMapper *mapper_type_priv;
    QDataWidgetMapper *mapper_dst;
    QDataWidgetMapper *mapper_dst_priv;
    QDataWidgetMapper *mapper_quant;
    QDataWidgetMapper *mapper_quant_priv;
    QDataWidgetMapper *mapper_src_type;
    QDataWidgetMapper *mapper_dst_type;

    Tree* tree = new Tree(this);
    int who_call;

public slots:
    void setmappers();
    void from_child(QString str);
    //void tree_type;

private slots:
    void currentIndexChanged(int i);
    void open_tree();
    void for_cb_src();
    void for_cb_dst();

    void on_kkk_clicked();

    void on_cancel_clicked();

    void checking();

private:
    Ui::FormNewEdge *ui;
};

#endif // FORMNEWEDGE_H
