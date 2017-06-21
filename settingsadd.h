#ifndef SETTINGSADD_H
#define SETTINGSADD_H
#include <QString>
#include <QDialog>
#include <QTreeWidgetItem>
#include <settingsform.h>
class SettingsForm;
namespace Ui {
class settingsadd;
}

class settingsadd : public QDialog
{
    Q_OBJECT

public:
    settingsadd(SettingsForm *form, QString info);
    ~settingsadd();
    QString uzel;
    QString color;
    QTreeWidgetItem *items;
    SettingsForm *form1;
    QString inform;

private slots:
    void on_buttonBox_accepted();


private:
    Ui::settingsadd *ui;
};

#endif // SETTINGSADD_H
