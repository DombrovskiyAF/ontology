#ifndef FORMNEWEDGETYPE_H
#define FORMNEWEDGETYPE_H

#include <QDialog>
#include <QDataWidgetMapper>

namespace Ui {
class FormNewEdgeType;
}

class FormNewEdgeType : public QDialog
{
    Q_OBJECT
    QDataWidgetMapper *mapper = new QDataWidgetMapper;
    QDataWidgetMapper *mapper2 = new QDataWidgetMapper;

public:
    explicit FormNewEdgeType(QWidget *parent = 0);
    ~FormNewEdgeType();

    void on_setup_view();


private slots:
    void on_push_add_clicked();
    void on_push_cancel_clicked();

private:
    Ui::FormNewEdgeType *ui;
};

#endif // FORMNEWEDGETYPE_H
