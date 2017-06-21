#include "ruleaddwidget.h"
#include "ui_ruleaddwidget.h"
#include "formrules.h"
#include <qdebug.h>
RuleAddWidget::RuleAddWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RuleAddWidget)
{
    ui->setupUi(this);
}

RuleAddWidget::~RuleAddWidget()
{
    delete ui;
}

void RuleAddWidget::on_pushButton_clicked()
{

    emit(signaladdrule(ui->le_rule_name->text(),ui->le_author->text(),
                     ui->te_ruleRem->toPlainText(),ui->le_template->text()));
    ui->le_author->clear();
    ui->le_rule_name->clear();
    ui->le_template->clear();
    ui->te_ruleRem->clear();
}
