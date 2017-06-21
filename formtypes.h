#ifndef FORMTYPES_H
#define FORMTYPES_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QDataWidgetMapper>

#include "formnewquant.h"
#include <formnewnodetype.h>
#include <formnewedgetype.h>

namespace Ui {
class FormTypes;
}

class FormTypes : public QDialog
{
    Q_OBJECT

public:
    explicit FormTypes(QWidget *parent = 0);
    ~FormTypes();

    void setNodeTypesModel(QAbstractItemModel *model);
    void setEdgeTypesModel(QAbstractItemModel *model);
    void setQuantTypesModel(QAbstractItemModel *model);

    QDataWidgetMapper *mapper;
    QDataWidgetMapper *mapper2;

    formNewQuant* formnewquant = new formNewQuant;
    FormNewEdgeType* formnewedgetype = new FormNewEdgeType;
    FormNewNodeType* formnewnodetype  = new FormNewNodeType;


public slots:
    void on_db_open();

private slots:  
    void on_n_select(QModelIndex current);
    void on_push_n_add_clicked();
    void on_push_n_revert();
    void on_push_n_save();
    void on_push_n_del();
    void on_n_changed();

    void on_q_select(QModelIndex current);
    void on_push_q_add();
    void on_push_q_revert();
    void on_push_q_save();
    void on_push_q_del();
    void on_q_changed();

    void on_e_select(QModelIndex current);
    void on_push_e_add_clicked();
    void on_push_e_revert();
    void on_push_e_save();
    void on_push_e_del();
    void on_e_changed();

    void del_edgetypes_for_del_nodetype();

    void update_only();





private:
    Ui::FormTypes *ui;

};

#endif // FORMTYPES_H
