#ifndef FORMNODES_H
#define FORMNODES_H

#include <QDialog>
#include <QAbstractItemModel>
#include <QDataWidgetMapper>
#include <QPalette>
#include <formnewnode.h>

namespace Ui {
class FormNodes;
}

class FormNodes : public QDialog
{
    Q_OBJECT

public:
    explicit FormNodes(QWidget *parent = 0);
    ~FormNodes();

    void setNodesModel(QAbstractItemModel *model);
    QDataWidgetMapper *m_mapper;
    QDataWidgetMapper *m_mapper2;

    bool edit_flag;
    bool add_flag;

    QPalette *pal_lineEdit;
    FormNewNode* formnewnode = new FormNewNode;



public slots:
    void on_pbTest_clicked();
    void on_select(QModelIndex current);
    void on_save();
    void on_revert();
    void on_delete();
    void on_edge_prev_select(QModelIndex current3);
    void on_edge_next_select(QModelIndex current2);

    void on_push_filter();
    void on_push_find();


    void activate();
    void deactivate();

    QAbstractItemModel * getmodel();


    void on_pbAdd_clicked();

private slots:
    void on_tvNodes_activated(const QModelIndex &index);



private:
    Ui::FormNodes *ui;

};

#endif // FORMNODES_H
