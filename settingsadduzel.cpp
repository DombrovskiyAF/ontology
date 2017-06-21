#include "settingsadduzel.h"
#include "ui_settingsadduzel.h"

settingsadduzel::settingsadduzel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsadduzel)
{
    ui->setupUi(this);
    hide();
}

settingsadduzel::~settingsadduzel()
{
    delete ui;
}
