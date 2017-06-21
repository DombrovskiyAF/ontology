#ifndef USERULESFORM_H
#define USERULESFORM_H

#include <QDialog>
#include <qsqlrelationaltablemodel.h>
namespace Ui {
class UseRulesForm;
}

class UseRulesForm : public QDialog
{
    Q_OBJECT

public:
    explicit UseRulesForm(QWidget *parent = 0);
    ~UseRulesForm();


public slots:

    void loadRules();
    void useRules();

private:
    Ui::UseRulesForm *ui;
};

#endif // USERULESFORM_H
