#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QGraphicsPathItem>
#include <QColor>
#include <QPen>

class GraphNode : public QGraphicsPathItem
{
public:
    GraphNode(QGraphicsItem *parent = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual int type() const override;

private:
    const QColor BG_COLOR_UNSELECTED = QColor{24, 26, 24, 200};
    const QColor BG_COLOR_SELECTED = QColor{24, 26, 24, 220};
    const QColor EDGE_COLOR_UNSELECTED = QColor{16, 17, 16};
    const QColor EDGE_COLOR_SELECTED = QColor{242, 178, 0};

    const qreal EDGE_WIDTH = 5;
};

#endif // GRAPHNODE_H
