#ifndef IMPORTFORM_H
#define IMPORTFORM_H

#include <QDialog>
#include <QSettings>
#include <QFileDialog>
#include "dotimport.h"
#include "datamodel.h"


namespace Ui {
class importform;
}
class DataModel;
class importform : public QDialog
{
    Q_OBJECT

public:
     importform(DataModel *sqlmodel,QSettings *SettingsNodes, QSettings *SettingsEdges,QWidget *parent = 0);
    ~importform();
    QSettings *settingsNodes;
    QSettings *settingsEdges;
    DataModel *Sqlmodel;


private slots:
    void on_buttonBox_accepted();
public slots:
    void refresh_tables();

private:
    Ui::importform *ui;
};

#endif // IMPORTFORM_H
