#include "qupdateableview.h"
#include <QPainter>
#include <QPaintEvent>

QUpdateableView::QUpdateableView(QWidget *parent) :
    QTableView(parent)
{
}

void QUpdateableView::submit()
{
    // ��������, ��� ���������� ������� ���������, �� �� �������� ������������ Qt
    // http://developer.qt.nokia.com/forums/viewthread/4531/#27159
    QModelIndex index = currentIndex();
    currentChanged( index, index );
}

