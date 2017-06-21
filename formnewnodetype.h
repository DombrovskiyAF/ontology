#ifndef FORMNEWNODETYPE_H
#define FORMNEWNODETYPE_H

#include <QDialog>

namespace Ui {
class FormNewNodeType;
}

class FormNewNodeType : public QDialog
{
    Q_OBJECT

public:
    explicit FormNewNodeType(QWidget *parent = 0);
    ~FormNewNodeType();

private slots:
    void on_push_add_clicked();

    void on_push_cancel_clicked();

private:
    Ui::FormNewNodeType *ui;
};

#endif // FORMNEWNODETYPE_H
