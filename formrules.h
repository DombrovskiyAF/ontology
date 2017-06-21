#ifndef FORMRULES_H
#define FORMRULES_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <QDataWidgetMapper>
#include <QString>

#include "ruleaddwidget.h"
#include "ruleeditwidget.h"

namespace Ui {
class FormRules;
}

class FormRules : public QDialog
{
    Q_OBJECT

public:

    explicit FormRules(QWidget *parent = 0);
    ~FormRules();
    void setRulesModel(QSqlRelationalTableModel *model);
    QVector<int> setEdgeTypeCB(QSqlRelationalTableModel *model);
    QSqlRelationalTableModel *tmp;
public slots:

    void slotaddrule(QString name, QString author, QString remark, QString tmplt);
    void sloteditrule(QString name, QString author, QString remark, QString tmplt);

signals:
    int signalsendfields(int row);
private:

    QString replaceNumbersEdges(int i,QString string);
    QString replaceNumbersNodes(int i,QString string);
    void selected1();
    void selected2();

private slots:

    void on_tvRules_clicked(const QModelIndex &index);
    void on_pb_confirm_clicked();
    void on_pushButton_2_clicked();
    void on_pb_add2_clicked();
    void on_l_done1_linkHovered(const QString &link);
    void on_l_done2_linkHovered(const QString &link);
    void on_pushButton_4_clicked();
    void on_pb_edit2_clicked();
    void on_cb_A_currentIndexChanged(const QString &arg1);
    void on_cb_B_currentIndexChanged(const QString &arg1);
    void on_cb_C_currentIndexChanged(const QString &arg1);
    void on_cb_A_2_currentIndexChanged(const QString &arg1);
    void on_cb_B_2_currentIndexChanged(const QString &arg1);
    void on_cb_C_2_currentIndexChanged(const QString &arg1);
    void on_cb_D_currentIndexChanged(const QString &arg1);
    void on_pb_addedit_clicked();
    void on_pushButton_clicked();

private:
    Ui::FormRules *ui;
    QDataWidgetMapper *m_mapper;
    RuleAddWidget *ruleaddwiget = new RuleAddWidget();
    RuleEditWidget *ruleeditwiget = new RuleEditWidget();
};

#endif // FORMRULES_H
