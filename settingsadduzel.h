#ifndef SETTINGSADDUZEL_H
#define SETTINGSADDUZEL_H

#include <QDialog>

namespace Ui {
class settingsadduzel;
}

class settingsadduzel : public QDialog
{
    Q_OBJECT

public:
    explicit settingsadduzel(QWidget *parent = 0);
    ~settingsadduzel();

private:
    Ui::settingsadduzel *ui;
};

#endif // SETTINGSADDUZEL_H
