#ifndef RULEEDITWIDGET_H
#define RULEEDITWIDGET_H

#include <QWidget>


#include "dafrules.h"
#include "datamodel.h"

namespace Ui {
class RuleEditWidget;
}

class RuleEditWidget : public QWidget
{
    Q_OBJECT

signals:
    void signaleditrule(QString name, QString author, QString remark, QString tmplt);
public:
    explicit RuleEditWidget(QWidget *parent = 0);
    ~RuleEditWidget();
public slots:
    void slotgetfield(int row);
private slots:
    void on_pushButton_clicked();

private:
    Ui::RuleEditWidget *ui;

};

#endif // RULEEDITWIDGET_H
