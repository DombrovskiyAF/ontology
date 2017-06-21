#include "selectdbwindow.h"
#include "ui_selectdbwindow.h"
#include "datamodel.h"
#include <QFileDialog>

selectdbwindow::selectdbwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::selectdbwindow)
{
    ui->setupUi(this);
}

selectdbwindow::~selectdbwindow()
{
    delete ui;
}

void selectdbwindow::on_buttonBox_accepted()
{
    if (ui->rb_mysql->isChecked()){

    }
    else if (ui->rb_odbc->isChecked()) {
        QFileDialog fd;
        fd.setDefaultSuffix("db");
        fd.setWindowTitle("РћС‚РєСЂС‹С‚СЊ Р±Р°Р·Сѓ РґР°РЅРЅС‹С…");
        fd.setNameFilter("Р‘Р°Р·С‹ РґР°РЅРЅС‹С… Access (*.mdb)");
        if (fd.exec() == QDialog::Accepted) {
            if (fd.selectedFiles().count() > 0) {
                dataModel->m_dbName = fd.selectedFiles().at(0);
                dataModel->connectDB(dataModel->m_dbName);
                dataModel->createModel();
                dataModel->setupView();
            }
        }
    }
    else if (ui->rb_postgree->isChecked()) {

    }
    hide();
}

void selectdbwindow::on_buttonBox_rejected()
{
    hide();
}
