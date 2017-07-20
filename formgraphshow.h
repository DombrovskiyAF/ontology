#ifndef FORMGRAPHSHOW_H
#define FORMGRAPHSHOW_H

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QGraphicsView>
namespace Ui {
class formGraphShow;
}

class formGraphShow : public QWidget
{
    Q_OBJECT

public:
    QString GVDIR;
    explicit formGraphShow(QWidget *parent = 0);
    void exec(QString GV_DIR);
    ~formGraphShow();

private:

    Ui::formGraphShow *ui;


};

#endif // FORMGRAPHSHOW_H
