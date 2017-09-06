#ifndef MYWEITU_H
#define MYWEITU_H
#include <QStyledItemDelegate>

class NoFocusDelegate :public QStyledItemDelegate
{
    void paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const;
};



#endif // MYWEITU_H
