#include "importform.h"

#include "ui_importform.h"

importform::importform(DataModel *sqlmodel,QSettings *SettingsNodes, QSettings *SettingsEdges,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::importform)
{
    ui->setupUi(this);
    ui->edgestable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->nodestable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->quant_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    Sqlmodel = sqlmodel;
    settingsEdges = SettingsEdges;
    settingsNodes = SettingsNodes;
}

importform::~importform()
{
    delete ui;
}

void importform::on_buttonBox_accepted()
{
    QString fName = QFileDialog::getOpenFileName(0, "Open file","","DOT files (*.dot)");
    dotimport *dotImport = new dotimport(Sqlmodel,this);
    dotImport->ImportFromDot(fName,ui->edgestable,ui->nodestable,ui->quant_widget);

    delete dotImport;
}

void importform::refresh_tables()
{

    ui->edgestable->setRowCount(0);
    ui->quant_widget->setRowCount(0);

    ui->nodestable->setRowCount(0);

    QStringList keyslist;
    QStringList keyslistEdges;
    QStringList keyslistQuants;
    settingsNodes->beginGroup("uzel");
    keyslist = settingsNodes->childKeys();
    settingsNodes->endGroup();
    settingsEdges->beginGroup("rebr_type_str");
    keyslistQuants = settingsEdges->childKeys();
    settingsEdges->endGroup();

    for(int i = 0; i<keyslist.size();i++)
    {
       QTableWidgetItem *node = new QTableWidgetItem;
       QTableWidgetItem *nodefigure = new QTableWidgetItem;
       QTableWidgetItem *nodebg = new QTableWidgetItem;
       QTableWidgetItem *nodeborder = new QTableWidgetItem;



       node->setText(keyslist.at(i));
       nodebg->setText(settingsNodes->value("colorbg/"+keyslist.at(i)).toString());
       nodeborder->setText(settingsNodes->value("uzel/"+keyslist.at(i)).toString());
       nodefigure->setText(settingsNodes->value("uzelfig/"+keyslist.at(i)).toString());


       ui->nodestable->insertRow(i);
       ui->nodestable->setItem(i,0,node);
       ui->nodestable->setItem(i,1,nodefigure);
       ui->nodestable->setItem(i,3,nodebg);
       ui->nodestable->setItem(i,2,nodeborder);
    }

    settingsEdges->beginGroup("rebr");
    keyslistEdges = settingsEdges->childKeys();
    settingsEdges->endGroup();

    for(int i = 0; i<keyslistEdges.size();i++)
    {
        QTableWidgetItem *edge = new QTableWidgetItem;
        QTableWidgetItem *edgename = new QTableWidgetItem;
        QTableWidgetItem *edgecolor = new QTableWidgetItem;
        QTableWidgetItem *edgetype = new QTableWidgetItem;

        edge->setText(keyslistEdges.at(i));
        edgename->setText(settingsEdges->value("rebr_names/"+keyslistEdges.at(i)).toString());
        edgecolor->setText(settingsEdges->value("rebr/"+keyslistEdges.at(i)).toString());
        edgetype->setText(settingsEdges->value("rebr_type/"+keyslistEdges.at(i)).toString());

        ui->edgestable->insertRow(i);
        ui->edgestable->setItem(i,0,edge);
        ui->edgestable->setItem(i,1,edgename);
        ui->edgestable->setItem(i,2,edgecolor);
        ui->edgestable->setItem(i,3,edgetype);
    }
    for(int i = 0; i < keyslistQuants.size();i++)
    {
        QTableWidgetItem *quant = new QTableWidgetItem;
        QTableWidgetItem *quant_str = new QTableWidgetItem;
        quant->setText(keyslistQuants.at(i));
        quant_str->setText(settingsEdges->value("rebr_type_str/"+keyslistQuants.at(i)).toString());

        ui->quant_widget->insertRow(i);
        ui->quant_widget->setItem(i,0,quant);
        ui->quant_widget->setItem(i,1,quant_str);
    }
}
