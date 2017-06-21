#ifndef EXPORTONTOFORM_H
#define EXPORTONTOFORM_H

#include <QDialog>

namespace Ui {
class ExportOntoForm;
}

class ExportOntoForm : public QDialog
{
    Q_OBJECT

public:
    explicit ExportOntoForm(QWidget *parent = 0);
    ~ExportOntoForm();

private:
    Ui::ExportOntoForm *ui;
};

#endif // EXPORTONTOFORM_H
