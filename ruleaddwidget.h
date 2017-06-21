#ifndef RULEADDWIDGET_H
#define RULEADDWIDGET_H

#include <QWidget>
#include "QSqlRelationalTableModel"
#include "QSqlQuery"
#include "QSqlError"
#include "QDataWidgetMapper"
namespace Ui {
class RuleAddWidget;
}

class RuleAddWidget : public QWidget
{
    Q_OBJECT
public slots:
public:
    explicit RuleAddWidget(QWidget *parent = 0);
    ~RuleAddWidget();
private slots:
    void on_pushButton_clicked();
signals:
    void signaladdrule(QString name, QString author, QString remark, QString tmplt);

private:
    Ui::RuleAddWidget *ui;
};

#endif // RULEADDWIDGET_H
