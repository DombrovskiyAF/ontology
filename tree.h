#ifndef TREE_H
#define TREE_H

#include <QDialog>
#include <QTreeWidgetItem>
#include <QDataWidgetMapper>

namespace Ui {
class Tree;
}

class Tree : public QDialog
{
    Q_OBJECT

public:
    explicit Tree(QWidget *parent = 0);
    ~Tree();

    QList <QString> ListOfNodesForTree;
    QList <QString> ListOfSrcEdgesForTree;
    QList <QString> ListOfDstEdgesForTree;

    //QDataWidgetMapper *mapper_type = new QDataWidgetMapper;
    //QDataWidgetMapper *mapper_src = new QDataWidgetMapper;
    //QDataWidgetMapper *mapper_dst = new QDataWidgetMapper;

    QDataWidgetMapper *mapper_type;
    QDataWidgetMapper *mapper_src;
    QDataWidgetMapper *mapper_dst;

    int whoiscall;

private:
    Ui::Tree *ui;

public slots:
    void create_update_tree(int who, QString src, QString type, QString dst);
    void itemClicked(QTreeWidgetItem* current,int cur_i);
    void currentIndexChanged(int i);
    void on_open(int i);

private slots:
    void on_kk_clicked();
    void on_no_clicked();

};

#endif // TREE_H
