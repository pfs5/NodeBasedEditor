#ifndef NODEEDITORSHARED_H
#define NODEEDITORSHARED_H

#include <QGraphicsItem>

enum class ItemType : int
{
    Node = QGraphicsItem::UserType + 1,
    Connection = QGraphicsItem::UserType + 2
};

#endif // NODEEDITORSHARED_H
