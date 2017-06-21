#ifndef NODESEDGES_H
#define NODESEDGES_H

#include <QDialog>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSqlRelationalTableModel>

namespace Ui {
class NodesEdges;
}

class NodesEdges : public QDialog
{
    Q_OBJECT
    
public:
    explicit NodesEdges(QWidget *parent = 0);
    ~NodesEdges();
    QSqlRelationalTableModel *model_nodes;
    QSqlRelationalTableModel *model_edges;
    virtual void keyPressEvent(QKeyEvent *eventPress);
private:
    Ui::NodesEdges *ui;
public slots:
    void re();
    void check_edges(bool a);
    void check_nodes(bool a);
    void add_edge();
    void del_edge();
    void add_node();
    void del_node();
};

#endif // NODESEDGES_H
