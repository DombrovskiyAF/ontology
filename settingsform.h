#ifndef SETTINGSFORM_H
#define SETTINGSFORM_H

#include <QDialog>
#include <QComboBox>
#include <QListWidgetItem>
#include <QSettings>
#include <QFileDialog>
#include <QTimer>
#include <QMenu>
#include <QSignalMapper>
#include <QTreeWidgetItem>
#include <QColorDialog>
class settingsadd;
namespace Ui {
class SettingsForm;
}

class SettingsForm : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsForm(QWidget *parent = 0);
    ~SettingsForm();
    QFileDialog fd;
    QSettings *settings;
    settingsadd *uzeladd;
    QStringList keylist;
    QString currentQuant;
    QString font_color,edge_color,line_color,back_color,nodetext_color;
    QTreeWidgetItem *itm;
    QTimer *timer;
    settingsadd *rebroadd;
    QPoint point;
    QSignalMapper *signalmapper;
    QSettings *settings_edges;



public slots:
    void custom_menu_1(const QPoint &a);
     void refresh_list();
     void changeNameedge();
     void deleteedge(QString edge);
     void deleteedge();
     void custom_menu_2(const QPoint &a);
     void delete_rebr();
     void delete_rebr(QString rebro);
     void delete_quant();
     void delete_quant(QString quant);



private slots:
     void on_uzeltree_itemClicked();
     void on_SaveButton_clicked();
     void on_pushButton_clicked();
     void on_pushButton_2_clicked();
     void on_edgestree_itemClicked(QTreeWidgetItem *item, int column);
     bool on_pushButton_3_clicked();
     void custom_menu_quant(const QPoint &a);
     void on_Add_quant_clicked();

     /// �������
     void on_listWidget_currentRowChanged(int menu);

     void on_push_save_setting_clicked();

     void on_check_file_clicked(bool checked);

     void on_push_file_clicked();


     void on_quant_list_itemClicked(QListWidgetItem *item);


     void on_quant_list_itemPressed(QListWidgetItem *item);

     void on_font_color_clicked();

     void on_edge_color_clicked();

     void on_line_color_clicked();

     void on_back_color_clicked();

     void on_nodetext_color_clicked();

private:
    Ui::SettingsForm *ui;

};

#endif // SETTINGSFORM_H
