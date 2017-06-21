#ifndef SIMILARITY_H
#define SIMILARITY_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QSet>
#include <QVector>
//#include <stdlib.h>

namespace Ui {
class Similarity;
}

class Similarity : public QWidget
{
    Q_OBJECT

public:
    explicit Similarity(QWidget *parent = 0);
    ~Similarity();

    int children, parents;

    QList <QHBoxLayout*> list_of_lays;
//    QList <QLineEdit*> list_of_lineedit; //массив с лайнэдитами
//    QVector <QVector*> set_of_parents;            //массив с предками
    QVector<QVector<QLineEdit*> > vec_of_parents;


private slots:
    void on_push_similarity_clicked();
    int if_have(QString src);

private:
    Ui::Similarity *ui;
};

#endif // SIMILARITY_H
