#ifndef QUPDATEABLEVIEW_H
#define QUPDATEABLEVIEW_H

#include <QTableView>

class QUpdateableView : public QTableView
{
    Q_OBJECT
public:
    explicit QUpdateableView(QWidget *parent = 0);
    void submit();
signals:

};

#endif // QUPDATEABLEVIEW_H
