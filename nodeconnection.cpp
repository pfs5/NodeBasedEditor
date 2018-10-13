#include "nodeconnection.h"

#include <QPen>

NodeConnection::NodeConnection(QGraphicsItem* parent)
{
    setPen(QPen{CONNECTION_COLOR, CONNECTION_THICKNESS});
    setBrush(Qt::NoBrush);
    setZValue(-1);
}

NodeConnection::~NodeConnection()
{

}

void NodeConnection::setStartPos(const QPointF &p)
{
    _startPos = p;
}

void NodeConnection::setEndPos(const QPointF &p)
{
    _endPos = p;
}

void NodeConnection::updatePath()
{
   QPainterPath p;

   p.moveTo(_startPos);

   qreal dx = _endPos.x() - _startPos.x();
   qreal dy = _endPos.y() - _startPos.y();

   QPointF ctr1{_startPos.x() + dx * 0.25, _startPos.y() + dy * 0.1};
   QPointF ctr2{_startPos.x() + dx * 0.75, _startPos.y() + dy * 0.9};

   p.cubicTo(ctr1, ctr2, _endPos);

   setPath(p);
}
