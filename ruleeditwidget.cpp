#include "ruleeditwidget.h"
#include "ui_ruleeditwidget.h"

#include <QDebug>
#include "formrules.h"
RuleEditWidget::RuleEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleEditWidget)
{
    ui->setupUi(this);

}

RuleEditWidget::~RuleEditWidget()
{
    delete ui;
}
void RuleEditWidget::slotgetfield(int row)
{
    dataModel->LoadRulesList();
    QList<VzRule*> *rules = dataModel->getRulesList();
    QList<VzRule*>::iterator i;

    for (i = rules->begin(); i != rules->end(); ++i)
    {
        VzRule* r = *i;
        if(r->getId()==row)
        {
            ui->le_author->setText(r->getAuthor());
            ui->le_rule_name->setText(r->getName());
            ui->le_template->setText(r->getTemplate());
            ui->te_ruleRem->setText(r->getRem());
        }

    }
}

void RuleEditWidget::on_pushButton_clicked()
{
    emit(signaleditrule(ui->le_rule_name->text(),ui->le_author->text(),
                        ui->te_ruleRem->toPlainText(),ui->le_template->text()));
    ui->le_author->clear();
    ui->le_rule_name->clear();
    ui->le_template->clear();
    ui->te_ruleRem->clear();
}
