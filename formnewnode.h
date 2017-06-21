#ifndef FORMNEWNODE_H
#define FORMNEWNODE_H

#include <QDialog>
#include <QDataWidgetMapper>

namespace Ui {
class FormNewNode;
}

class FormNewNode : public QDialog
{
    Q_OBJECT

public:
    explicit FormNewNode(QWidget *parent = 0);
    ~FormNewNode();

    void on_setup_view();
    QDataWidgetMapper* m_mapper = new QDataWidgetMapper;
    QDataWidgetMapper* m_mapper2 = new QDataWidgetMapper;

private slots:
    void on_push_add_clicked();

    void on_push_cancel_clicked();


private:
    Ui::FormNewNode *ui;
};

#endif // FORMNEWNODE_H
