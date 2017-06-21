#include "exporform.h"
#include "ui_exporform.h"
#include "datamodel.h"

#include "vz_settings.h"

#include <QFileDialog>
#include <QDebug>
#include <QList>


enum TreeNodeType
{
    tntTerm, //
    tntAnd,  //
    tntOr    //
};

class TreeNode: public QObject
{
    Q_OBJECT
  public:
    TreeNodeType nodeType;
    QString name;
    QList <TreeNode> childs;
};


ImportGostForm::ImportGostForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImportGostForm)
{
    ui->setupUi(this);
}

ImportGostForm::~ImportGostForm()
{
    delete ui;
}

void ImportGostForm::on_pbOpenFile_clicked()
{
    QString fName = QFileDialog::getOpenFileName(0, "Open file");
    ui->leFileName->setText(fName);
}

void ImportGostForm::on_pbStartParsing_clicked()
{
    vzsetNumber = ui->leNumber->text();
    vzsetStringId = ui->leStringId->text();
    vzsetHierarhRole = ui->leHierarhRole->text();
    vzsetDependRole = ui->leDependRole->text();

    dataModel->exportFromGost(ui->leFileName->text());

    return;
}
