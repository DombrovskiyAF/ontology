#ifndef DOTIMPORT_H
#define DOTIMPORT_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTableWidget>
#include <QDebug>
#include <QRegExp>
#include <QStringList>
#include <QObject>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
#include "datamodel.h"

class DataModel;

class dotimport : public QObject

{
    Q_OBJECT
public:
    dotimport(DataModel *sqlmodel, QWidget *parent = 0);
    QTableWidget *Edgestable;
    QTableWidget *Nodesstable;
    DataModel *model;
public slots:
    bool ImportFromDot(QString FName, QTableWidget *edgestable, QTableWidget *nodesstable, QTableWidget *quanttable);
};

#endif // DOTIMPORT_H
