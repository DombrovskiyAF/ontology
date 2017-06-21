#include "qupdateableview.h"
#include <QPainter>
#include <QPaintEvent>

QUpdateableView::QUpdateableView(QWidget *parent) :
    QTableView(parent)
{
}

void QUpdateableView::submit()
{
    // Возможно, эта реализация кажется неуклюжей, но ее советуют разработчики Qt
    // http://developer.qt.nokia.com/forums/viewthread/4531/#27159
    QModelIndex index = currentIndex();
    currentChanged( index, index );
}

