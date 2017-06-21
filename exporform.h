#ifndef EXPORFORM_H
#define EXPORFORM_H

#include <QDialog>

namespace Ui {
class ImportGostForm;
}

class ImportGostForm : public QDialog
{
    Q_OBJECT

public:
    explicit ImportGostForm(QWidget *parent = 0);
    ~ImportGostForm();

private slots:
    void on_pbOpenFile_clicked();
    void on_pbStartParsing_clicked();

private:
    Ui::ImportGostForm *ui;
};

#endif // EXPORFORM_H
