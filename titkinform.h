#ifndef TITKINFORM_H
#define TITKINFORM_H

#include <QDialog>
#include <QString>

#include "ontograph\ogrules.h"

namespace Ui {
class TitkinForm;
}

class TitkinForm : public QDialog
{
    Q_OBJECT

public:
    explicit TitkinForm(QWidget *parent = 0);
    ~TitkinForm();

private slots:

    void useRules(QList<int> *inEdges, QList<int> *outEdges);
    void useRuleType1(VzRule *r, QList<int> *inEdges, QList<int> *outEdges);
    void useRuleType2(VzRule *r, QList<int> *inEdges, QList<int> *outEdges);
    int insert_edge(QString a, QString ac, QString c, VzRule *r);
    QStringList insert_node(QString type, QString name, QString a, QString ad, QString c, QString dc, QString author,VzRule *r);
    //QStringList nsert_node(QString type, QString name, QString a, QString ad, QString c, QString dc, QString author,VzRule *r);



    void on_pbStart_clicked();

    void on_pbLoadRules_clicked();

    void on_pb_startNew_clicked();

private:
    void logStrIf(int id_AB, int id_BC);
    Ui::TitkinForm *ui;

};





#endif // TITKINFORM_H
