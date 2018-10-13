#ifndef NODECONNECTION_H
#define NODECONNECTION_H

#include <QGraphicsPathItem>
#include <QColor>

class NodeConnection : public QGraphicsPathItem
{
public:
    NodeConnection(QGraphicsItem* parent = nullptr);
    ~NodeConnection();

    void setStartPos(const QPointF &p);
    void setEndPos(const QPointF &p);

    void updatePath();

private:
    const QColor CONNECTION_COLOR = QColor{43, 154, 157};
    const qreal CONNECTION_THICKNESS = 3;

    QPointF _startPos;
    QPointF _endPos;
};

#endif // NODECONNECTION_H
