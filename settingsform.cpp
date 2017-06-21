#include "settingsform.h"
#include "ui_settingsform.h"
#include "QMessageBox"
#include <QTreeWidget>
#include <QFileDialog>
#include <QDebug>
#include <QTreeWidgetItem>
#include <QPixmap>
#include <QPainter>
#include <QTextCodec>
#include <datamodel.h>
#include <QMessageBox>

void SettingsForm::refresh_list()
{
    QStringList keyslist,quant_list;
    settings_edges->beginGroup("rebr_type_str");
    quant_list = settings_edges->childKeys();
    settings_edges->endGroup();
    keyslist = settings->childKeys();
    ui->uzeltree->clear();
    ui->quant_list->clear();
    ui->edgestree->clear();
    settings->beginGroup("uzel");
    for(int i = 0; i<settings->childKeys().size();i++)
    {
        QTreeWidgetItem *items;
        items = new QTreeWidgetItem;
        items->setText(0,settings->childKeys().at(i));
        ui->uzeltree->addTopLevelItem(items);
    }
    settings->endGroup();
    settings_edges->beginGroup("rebr");
    for(int i = 0; i<settings_edges->childKeys().size();i++)
    {
        QTreeWidgetItem *items;
        items = new QTreeWidgetItem;
        items->setText(0,settings_edges->childKeys().at(i));
        ui->edgestree->addTopLevelItem(items);
    }
   settings_edges->endGroup();

   for(int i = 0; i<quant_list.size();i++)
   {

       QListWidgetItem *items;
       items = new QListWidgetItem;
       items->setText(quant_list.at(i));
       ui->quant_list->addItem(items);
   }
  settings_edges->endGroup();



   /// Бакиров
   connect(ui->listWidget,SIGNAL(currentRowChanged(int)),SLOT(on_listWidget_currentRowChanged(int)));


}


SettingsForm::SettingsForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsForm)
{

    itm = new QTreeWidgetItem;
    ui->setupUi(this);
    ui->font_color->setAutoFillBackground(true);
    ui->textcont->setValue(14);
    ui->textcont_2->setValue(14);
    ui->uzeltree->setContentsMargins(0,0,0,0);
    ui->uzeltree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->edgestree->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->quant_list->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tabWidget->setTabText(0,"Nodes");
    ui->tabWidget->setTabText(1,"Edges");
    ui->listWidget->setItemSelected(ui->listWidget->item(0),true);
    ui->font_color->setStyleSheet("QPushButton { background-color: #000000; }");
    ui->edge_color->setStyleSheet("QPushButton { background-color: #000000; }");
    ui->line_color->setStyleSheet("QPushButton { background-color: #000000; }");
    ui->back_color->setStyleSheet("QPushButton { background-color: #000000; }");

    connect(ui->edgestree,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(custom_menu_2(QPoint)));
    connect(ui->uzeltree,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(custom_menu_1(QPoint)));
    connect(ui->quant_list,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(custom_menu_quant(QPoint)));
    settings = new QSettings(QCoreApplication::applicationDirPath()+"/nodesettings.ini",QSettings::IniFormat);
    settings_edges = new QSettings(QCoreApplication::applicationDirPath()+"/edgesettings.ini",QSettings::IniFormat);
    settings_edges->setIniCodec("Windows-1251");
    settings->setIniCodec("Windows-1251");
    refresh_list();
    //ui->treeWidget->setAllColumnsShowFocus(1);





}

SettingsForm::~SettingsForm()
{
    delete ui;
}

void SettingsForm::custom_menu_1(const QPoint &a)
{
    point = a;
    QMenu *an = new QMenu;
    if(ui->uzeltree->itemAt(a)&&ui->uzeltree->itemAt(a)->isSelected())
    {

    an->addAction(QString("Удалить параметр"), this, SLOT(deleteedge()));
    an->exec(QCursor::pos());
    }
}
void SettingsForm::custom_menu_quant(const QPoint &a)
{
    point = a;
    QMenu *an = new QMenu;
    if(ui->quant_list->itemAt(a)&&ui->quant_list->itemAt(a)->isSelected())
    {

    an->addAction(QString("Удалить параметр"), this, SLOT(delete_quant()));
    an->exec(QCursor::pos());
    }
}


void SettingsForm::on_uzeltree_itemClicked()
{
    ui->boldfont->setChecked(0);ui->italicfont->setChecked(0);
    ui->lineEdit->setText(ui->uzeltree->currentItem()->text(0));
    ui->textcont->cleanText();
    ui->textcont->setValue(14);
    line_color = settings->value("uzel/"+ui->uzeltree->currentItem()->text(0)).toString();
    back_color = settings->value("colorbg/"+ui->uzeltree->currentItem()->text(0)).toString();
    nodetext_color = settings->value("colorfont/"+ui->uzeltree->currentItem()->text(0)).toString();
    ui->back_color->setStyleSheet("QPushButton { background-color: "+back_color+"; }");
    ui->nodetext_color->setStyleSheet("QPushButton { background-color: "+nodetext_color+"; }");
    ui->line_color->setStyleSheet("QPushButton { background-color: "+line_color+"; }");


            if(settings->value("uzelfontbold/"+ui->uzeltree->currentItem()->text(0)).toString()=="1")
            {
                ui->boldfont->setChecked(1);
            }
            if(settings->value("uzelfontitalic/"+ui->uzeltree->currentItem()->text(0)).toString()=="1")
            {
                ui->italicfont->setChecked(1);
            }
         ui->textcont->setValue(settings->value("uzelfontcount/"+ui->uzeltree->currentItem()->text(0)).toString().toInt());
             QString fig = settings->value("uzelfig/"+ui->uzeltree->currentItem()->text(0)).toString();
            for(int b = 0;0<ui->figurebox->maxCount();b++)
            {
                ui->figurebox->setCurrentIndex(b);
                if(ui->figurebox->currentText()==fig)
                {
                    break;
                }
            }

}


void SettingsForm::on_SaveButton_clicked()
{
QString name = ui->uzeltree->currentItem()->text(0);
settings->setValue("uzelfig/"+name,ui->figurebox->currentText());
settings->setValue("uzel/"+name,line_color);
if(ui->boldfont->isChecked()==1)
{settings->setValue("uzelfontbold/"+name,"1");}
else
{settings->setValue("uzelfontbold/"+name,"0");}
if(ui->italicfont->isChecked()==1)
{ settings->setValue("uzelfontitalic/"+name,"1");}
else
{settings->setValue("uzelfontitalic/"+name,"0");}



if(ui->textcont->text()=="0")
{settings->setValue("uzelfontcount/"+name,"14");}
else
{settings->setValue("uzelfontcount/"+name,ui->textcont->text());}
settings->setValue("colorbg/"+name,back_color);
settings->setValue("colorfont/"+name,nodetext_color);
if(ui->lineEdit->text()!=ui->uzeltree->currentItem()->text(0)&&ui->lineEdit->text().isEmpty()==false)
{
    settings->beginGroup("uzel");
    for(int k = 0; k<settings->childKeys().size();k++)
    {
        if(ui->lineEdit->text()==settings->childKeys().at(k))
        {
            QMessageBox msgBox;

            msgBox.setText("Такой узел уже существует, выберите другое название");

            msgBox.exec();
            settings->endGroup();

            break;
        }
        if(k==settings->childKeys().size()-1)
        {
            settings->endGroup();
        changeNameedge();

        }
    }
    settings->endGroup();


}


}


void SettingsForm::on_pushButton_clicked()
{
    QString a;
    a.setNum(settings->allKeys().size()+1);
    settings->setValue("uzel/New_Edge_"+a,"black");
    settings->setValue("colorfont/New_Edge_"+a,"black");
    settings->setValue("uzelfontcount/New_Edge_"+a,"14");
    settings->setValue("colorbg/New_Edge_"+a,"white");
    settings->setValue("uzelfontitalic/New_Edge_"+a,"0");
    settings->setValue("uzelfig/New_Edge_"+a,"polygon");
    settings->setValue("uzelfontbold/New_Edge_"+a,"0");
    refresh_list();
}

void SettingsForm::changeNameedge()
{
QString name(ui->uzeltree->currentItem()->text(0));
settings->beginGroup("uzel");

for(int i=0;i<settings->childKeys().size();i++)
{
    if( settings->childKeys().at(i) == name)

       {
        settings->endGroup();
        settings->setValue("uzel/"+ui->lineEdit->text(),settings->value("uzel/"+name));
        settings->setValue("colorbg/"+ui->lineEdit->text(),settings->value("colorbg/"+name));
        settings->setValue("colorfont/"+ui->lineEdit->text(),settings->value("colorfont/"+name));
        settings->setValue("uzelfontcount/"+ui->lineEdit->text(),settings->value("uzelfontcount/"+name));
        settings->setValue("uzelfontitalic/"+ui->lineEdit->text(),settings->value("uzelfontitalic/"+name));
        settings->setValue("uzelfontbold/"+ui->lineEdit->text(),settings->value("uzelfontbold/"+name));
        settings->setValue("uzelfig/"+ui->lineEdit->text(),settings->value("uzelfig/"+name));
        deleteedge(ui->uzeltree->currentItem()->text(0));
        refresh_list();
        break;
    }
}
settings->endGroup();
}

void SettingsForm::deleteedge(QString edge)
{
    settings->remove("colorbg/"+edge);        settings->remove("uzelfig/"+edge);
    settings->remove("colorfont/"+edge);    settings->remove("uzelfontitalic/"+edge);    settings->remove("uzel/"+edge);
    settings->remove("uzelfontcount/"+edge);    settings->remove("uzelfontbold/"+edge);
}

void SettingsForm::deleteedge()
{
    QString edge;
    edge = ui->uzeltree->itemAt(point)->text(0);

    if(ui->lineEdit->text() == ui->uzeltree->itemAt(point)->text(0))
    {
       ui->boldfont->setChecked(false);
       ui->figurebox->setCurrentIndex(ui->figurebox->maxCount()-1);
       ui->lineEdit->clear();
       ui->italicfont->setChecked(false);
    }


    settings->remove("colorbg/"+edge);       settings->remove("uzelfig/"+edge);
    settings->remove("colorfont/"+edge);    settings->remove("uzelfontitalic/"+edge);    settings->remove("uzel/"+edge);
    settings->remove("uzelfontcount/"+edge);    settings->remove("uzelfontbold/"+edge);

    refresh_list();
}

void SettingsForm::custom_menu_2(const QPoint &a)
{
    point = a;
    QMenu *an = new QMenu;
    if(ui->edgestree->itemAt(a)&&ui->edgestree->itemAt(a)->isSelected())
    {

    an->addAction(QString("Удалить параметр"), this, SLOT(delete_rebr()));
    an->exec(QCursor::pos());
    }
}

void SettingsForm::delete_rebr()
{

    if(ui->edgename->text() == ui->edgestree->itemAt(point)->text(0))
    {
       ui->boldfont_2->setChecked(false);
       ui->edgetype->setCurrentIndex(ui->edgetype->maxCount()-1);
       ui->comboBox->setCurrentIndex(ui->comboBox->maxCount()-1);


          ui->edgetext->clear();
          ui->edgename->clear();
          ui->italicfont_2->setChecked(false);
          ui->textcont_2->setValue(14);
    }

    QString rebro(ui->edgestree->currentItem()->text(0));
    settings_edges->remove("rebr/"+rebro);       settings_edges->remove("rebr_font_bold/"+rebro);
    settings_edges->remove("rebr_font_colors/"+rebro);    settings_edges->remove("rebr_font_italic/"+rebro);    settings_edges->remove("rebr_type/"+rebro);
    settings_edges->remove("rebr_names/"+rebro);    settings_edges->remove("rebr_font_size/"+rebro);            //settings_edges->remove("rebr_type_str/"+rebro);

    refresh_list();
}

void SettingsForm::delete_rebr(QString rebro)
{
    settings_edges->remove("rebr/"+rebro);       settings_edges->remove("rebr_font_bold/"+rebro);
    settings_edges->remove("rebr_font_colors/"+rebro);    settings_edges->remove("rebr_font_italic/"+rebro);    settings_edges->remove("rebr_type/"+rebro);
    settings_edges->remove("rebr_names/"+rebro);    settings_edges->remove("rebr_font_size/"+rebro);            //settings_edges->remove("rebr_type_str/"+rebro);

    refresh_list();
}




void SettingsForm::on_pushButton_2_clicked()
{
    QString a;
    a.setNum(settings_edges->allKeys().size()+1);
    settings_edges->setValue("rebr/New_Edge_"+a,"black");
   settings_edges->setValue("rebr_font_colors/New_Edge_"+a,"black");
   settings_edges->setValue("rebr_names/New_Edge_"+a,"Unnamed");
   settings_edges->setValue("rebr_font_bold/New_Edge_"+a,"0");
   settings_edges->setValue("rebr_font_italic/New_Edge_"+a,"0");
   settings_edges->setValue("rebr_font_size/New_Edge_"+a,"14");
   settings_edges->setValue("rebr_type/New_Edge_"+a,"solid");
//   settings_edges->setValue("rebr_type_str/New_Edge_"+a,"normal");



    refresh_list();

}

void SettingsForm::on_edgestree_itemClicked(QTreeWidgetItem *item, int column)
{
font_color = settings_edges->value("rebr_font_colors/"+(item->text(0))).toString();
edge_color = settings_edges->value("rebr/"+(item->text(0))).toString();
currentQuant.clear();
ui->edgename->setText(item->text(0));
ui->edgetext->setText(settings_edges->value("rebr_names/"+(item->text(0))).toString());
ui->font_color->setStyleSheet("QPushButton { background-color: "+font_color+"; }");
ui->edge_color->setStyleSheet("QPushButton { background-color: "+edge_color+"; }");

ui->edgetype->setCurrentText(settings_edges->value("rebr_type/"+(item->text(0))).toString());
//ui->comboBox->setCurrentText(settings_edges->value("rebr_type_str/"+(item->text(0))).toString());
ui->textcont_2->setValue(settings_edges->value("rebr_font_size/"+(item->text(0))).toInt());
if(settings_edges->value("rebr_font_bold/"+(item->text(0))).toInt()==1)
{
    ui->boldfont_2->setChecked(true);
}
else
{
    ui->boldfont_2->setChecked(false);
}
if(settings_edges->value("rebr_font_italic/"+(item->text(0))).toInt()==1)
{
    ui->italicfont_2->setChecked(true);
}
else
{
    ui->italicfont_2->setChecked(false);
}
}

bool SettingsForm::on_pushButton_3_clicked()
{

    if(currentQuant.isEmpty()==false)
    {
        settings_edges->setValue("rebr_type_str/"+ui->quant_name->text(),ui->comboBox->currentText()+"_"+ui->start_arrow_box->currentText());
        if(ui->quant_name->text()!=currentQuant)
        {
        delete_quant(currentQuant);
        }
        currentQuant = ui->quant_name->text();
        refresh_list();
        return 0;
    }
   QString a;
    if(ui->edgestree->currentItem()->text(0)!=ui->edgename->text())
    {
     a=ui->edgename->text();
     delete_rebr(ui->edgestree->currentItem()->text(0));

    }
    else
    {
        a=ui->edgestree->currentItem()->text(0);
    }
   settings_edges->setValue("rebr/"+a,edge_color);
   qDebug()<<font_color;
   settings_edges->setValue("rebr_font_colors/"+a,font_color);
   settings_edges->setValue("rebr_names/"+a,ui->edgetext->text());
   settings_edges->setValue("rebr_font_bold/"+a,ui->boldfont_2->isChecked());
   settings_edges->setValue("rebr_font_italic/"+a,ui->italicfont_2->isChecked());
   settings_edges->setValue("rebr_font_size/"+a,ui->textcont_2->value());
   settings_edges->setValue("rebr_type/"+a,ui->edgetype->currentText());
   //settings_edges->setValue("rebr_type_str/"+ui->quant_name->text(),ui->comboBox->currentText());
   refresh_list();
   return 0;

}

void SettingsForm::delete_quant(QString quant)
{
    settings_edges->remove("rebr_type_str/"+quant);
    refresh_list();
}
void SettingsForm::delete_quant()
{
    settings_edges->remove("rebr_type_str/"+currentQuant);
    refresh_list();
}


void SettingsForm::on_Add_quant_clicked()
{
    QString as;
    as = as.setNum(ui->quant_list->count());
   settings_edges->setValue("rebr_type_str/New_Quant"+as,ui->comboBox->currentText()+"_"+ui->start_arrow_box->currentText());
   refresh_list();
}


void SettingsForm::on_listWidget_currentRowChanged(int menu)
{
    switch (menu) {
    case 0:
        ///main
        ui->tabWidget->show();
        ///other
        ui->tab_open_db->hide();
        break;
    case 1:
        ///main
        ui->tab_open_db->show();
        ///other
        ui->tabWidget->hide();
        break;
    default:
        break;
    }
}

void SettingsForm::on_push_save_setting_clicked()
{
    if (ui->check_file->isChecked()){
        dataModel->getSettings()->setValue("settings/autoload", true);
        dataModel->getSettings()->setValue("settings/file", ui->line_file_name->text());
    }else{
        dataModel->getSettings()->setValue("settings/autoload", false);
    }
    QMessageBox::information(this,"Setting autorun","Succesfully saved");
}

void SettingsForm::on_check_file_clicked(bool checked)
{
    if (checked) {
        ui->push_file->setEnabled(1);
        ui->line_file_name->setEnabled(1);
    }else{
        ui->push_file->setEnabled(0);
        ui->line_file_name->setEnabled(0);
    }
}

void SettingsForm::on_push_file_clicked(){
    ui->line_file_name->setText(QFileDialog::getOpenFileName(this, tr("Remember file"), "", tr("MS Accsess Data Base (*.mdb)")));
}


void SettingsForm::on_quant_list_itemClicked(QListWidgetItem *item)
{
    ui->quant_name->setText(item->text());
       currentQuant = item->text();
       QString arrow = settings_edges->value("rebr_type_str/"+(currentQuant)).toString();
       qDebug()<<arrow;
       qDebug()<<arrow.left(arrow.indexOf("_"));
       qDebug()<<arrow.right(arrow.size()-arrow.indexOf("_")-1);
    ui->comboBox->setCurrentText(arrow.left(arrow.indexOf("_")));
    ui->start_arrow_box->setCurrentText(arrow.right(arrow.size()-arrow.indexOf("_")-1));

}



void SettingsForm::on_quant_list_itemPressed(QListWidgetItem *item)
{
    ui->quant_name->setText(item->text());
       currentQuant = item->text();
    ui->comboBox->setCurrentText(settings_edges->value("rebr_type_str/"+(currentQuant)).toString());
}

void SettingsForm::on_font_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue);
    if(!color.isValid())
    {
        ui->font_color->setStyleSheet("QPushButton { background-color: "+font_color+"; }");
    }
    else
    {
        ui->font_color->setStyleSheet("QPushButton { background-color: "+color.name()+"; }");
        font_color = color.name();
    }
}

void SettingsForm::on_edge_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue);
    if(!color.isValid())
    {
        ui->edge_color->setStyleSheet("QPushButton { background-color: "+edge_color+"; }");
    }
    else
    {
        ui->edge_color->setStyleSheet("QPushButton { background-color: "+color.name()+"; }");
        edge_color = color.name();
    }
}

void SettingsForm::on_line_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue);
    if(!color.isValid())
    {
        ui->line_color->setStyleSheet("QPushButton { background-color: "+line_color+"; }");
    }
    else
    {
        ui->line_color->setStyleSheet("QPushButton { background-color: "+color.name()+"; }");
        line_color = color.name();
    }
}

void SettingsForm::on_back_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue);
    if(!color.isValid())
    {
        ui->back_color->setStyleSheet("QPushButton { background-color: "+back_color+"; }");
    }
    else
    {
        ui->back_color->setStyleSheet("QPushButton { background-color: "+color.name()+"; }");
        back_color = color.name();
    }
}

void SettingsForm::on_nodetext_color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue);
    if(!color.isValid())
    {
        ui->nodetext_color->setStyleSheet("QPushButton { background-color: "+nodetext_color+"; }");
    }
    else
    {
        ui->nodetext_color->setStyleSheet("QPushButton { background-color: "+color.name()+"; }");
        nodetext_color = color.name();
    }
}
