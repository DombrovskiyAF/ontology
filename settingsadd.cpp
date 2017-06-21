#include "settingsadd.h"
#include "settingsform.h"
#include "ui_settingsform.h"
#include "ui_settingsadd.h"
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPen>
//#include "mainwindow.cpp"

settingsadd::settingsadd(SettingsForm *form,QString info) :
    ui(new Ui::settingsadd)
{
    ui->setupUi(this);
    inform = info;
    items = new QTreeWidgetItem;
    form1 = form;
    int size = ui->comboBox->style()->pixelMetric(QStyle::PM_SmallIconSize);
    QPixmap pixmap(size,size-5);
    pixmap.fill(QColor(Qt::black));

           QRect rBorder(0,0,size-1,size-6);
           QPainter p(&pixmap);
           QPen pen(Qt::lightGray, 1, Qt::SolidLine);
           p.setPen(pen);
           p.drawRect(rBorder);

           ui->comboBox->setItemData(0, pixmap, Qt::DecorationRole);
           pixmap.fill(QColor(Qt::red));
           ui->comboBox->setItemData(1, pixmap, Qt::DecorationRole);
           pixmap.fill(QColor(Qt::green));
           ui->comboBox->setItemData(2, pixmap, Qt::DecorationRole);
           pixmap.fill(QColor(Qt::blue));
           ui->comboBox->setItemData(3, pixmap, Qt::DecorationRole);
}

settingsadd::~settingsadd()
{
    delete ui;
}

void settingsadd::on_buttonBox_accepted()
{
 SettingsForm settingsfm;
 uzel = ui->name->text();
 QString color;
 if( (ui->comboBox->currentText())=="BLACK") {

     color = "black";

 }
 if( (ui->comboBox->currentText())=="RED") {

     color = "red";

 }
 if( (ui->comboBox->currentText())=="BLUE") {

     color = "blue";

 }
 if( (ui->comboBox->currentText())=="GREEN") {

     color = "green";

 }

 if(inform=="uzel")
 {
settingsfm.settings->setValue(ui->name->text(),color);
 }
 else
 {
     if(inform=="rebr")
     {
         settingsfm.settings->setValue("rebr_"+ui->name->text(),color);
     }

 }

form1->refresh_list();
}

