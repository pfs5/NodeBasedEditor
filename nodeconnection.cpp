#include "nodeconnection.h"
#include "nodeeditorshared.h"
#include "graphnode.h"

#include <QPen>
#include <QPainter>

NodeConnection::NodeConnection(QGraphicsItem* parent) :
    QGraphicsPathItem{parent},
    _startNode{nullptr},
    _endNode{nullptr}
{
    setPen(QPen{CONNECTION_COLOR_UNSELECTED, CONNECTION_THICKNESS_UNSELECTED});
    setBrush(Qt::NoBrush);
    setZValue(-1);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

NodeConnection::~NodeConnection()
{
    if (_startNode != nullptr) _startNode->removeConnection(this);
    if (_endNode != nullptr) _endNode->removeConnection(this);
}

void NodeConnection::setProperty(const QString &prop, const QString &value)
{
    _properties[prop] = value;
}

QString NodeConnection::getProperty(const QString &prop) const
{
    if (_properties.contains(prop))
    {
        return _properties[prop];
    }

    return QString{};
}

void NodeConnection::setStartPos(const QPointF &p)
{
    _startPos = p;
}

void NodeConnection::setEndPos(const QPointF &p)
{
    _endPos = p;
}

const QPointF &NodeConnection::getStartPos() const
{
    return _startPos;
}

const QPointF &NodeConnection::getEndPos() const
{
    return _endPos;
}

void NodeConnection::setStartNode(GraphNode* node)
{
    _startNode = node;
}

void NodeConnection::setEndNode(GraphNode* node)
{
    _endNode = node;
}

GraphNode *NodeConnection::getStartNode() const
{
    return _startNode;
}

GraphNode *NodeConnection::getEndNode() const
{
    return _endNode;
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
   p.addEllipse(_startPos, 5, 5);

   setPath(p);
}

void NodeConnection::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (isSelected())
    {
        painter->setPen(QPen{CONNECTION_COLOR_SELECTED, CONNECTION_THICKNESS_SELECTED});

    } else
    {
        painter->setPen(QPen{CONNECTION_COLOR_UNSELECTED, CONNECTION_THICKNESS_UNSELECTED});
    }

    painter->setBrush(Qt::NoBrush);

    painter->drawPath(path());
}

int NodeConnection::type() const
{
    return static_cast<int>(ItemType::Connection);
}
