#include "graphnode.h"
#include "nodeeditorshared.h"
#include <QPainter>
#include <QPen>

GraphNode::GraphNode(QGraphicsItem *parent):
    QGraphicsPathItem {parent}
{
    QPainterPath p;
    p.addRoundedRect(-50, -50, 100, 100, 5, 5);
    setPath(p);

    setPen(QPen{EDGE_COLOR_UNSELECTED, EDGE_WIDTH});
    setBrush(BG_COLOR_UNSELECTED);

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (isSelected()) {
        painter->setPen(QPen{EDGE_COLOR_SELECTED, EDGE_WIDTH});
        painter->setBrush(BG_COLOR_SELECTED);
    } else {
        painter->setPen(QPen{EDGE_COLOR_UNSELECTED, EDGE_WIDTH});
        painter->setBrush(BG_COLOR_UNSELECTED);
    }

    painter->drawPath(path());
}

int GraphNode::type() const
{
    return static_cast<int>(ItemType::Node);
}
