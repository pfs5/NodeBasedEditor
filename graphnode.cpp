#include "graphnode.h"
#include "nodeeditorshared.h"
#include "nodeconnection.h"
#include <QPainter>
#include <QPen>
#include <QString>
#include <QDebug>       // #TODO REMOVE THIS ON RELEASE

GraphNode::GraphNode(qreal width, qreal height, QGraphicsItem *parent):
    QGraphicsPathItem {parent},
    _width{width},
    _height{height}
{
    QPainterPath p;
    p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);
    setPath(p);

    QFont font("Arial", TEXT_SIZE);
    _textPath.addText(-width/2 + 30, -height/2 + 30, font, "<missing name>");

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptDrops(true);

    _connectionPoints.append(QPointF{width/2, 0});
    _connectionPoints.append(QPointF{0, height/2});
    _connectionPoints.append(QPointF{-width/2, 0});
    _connectionPoints.append(QPointF{0, -height/2});
}

void GraphNode::setProperty(const QString &prop, const QString &value)
{
    _properties[prop] = value;

    if (prop == "name")
    {
        QPainterPath p;
        QFont font("Arial", TEXT_SIZE);
        p.addText(-_width/2 + 10, -_height/2 + 30, font, value);
        _textPath = p;
    }
}

QString GraphNode::getProperty(const QString &prop) const
{
    if (_properties.contains(prop))
    {
        return _properties[prop];
    }

    return QString{};
}

void GraphNode::addOutputConnection(NodeConnection* con)
{
    _outputConnections.append(con);
    con->setStartNode(this);

    updateConnections();

    if (GraphNode* endNode = con->getEndNode())
    {
        endNode->updateConnections();
    }
}

void GraphNode::addInputConnection(NodeConnection* con)
{
    _inputConnections.append(con);
    con->setEndNode(this);

    updateConnections();

    if (GraphNode* startNode = con->getStartNode())
    {
        startNode->updateConnections();
    }
}

void GraphNode::removeConnection(NodeConnection* con)
{
    int outputIndex = _outputConnections.indexOf(con);
    if (outputIndex != -1)
    {
        _outputConnections.remove(outputIndex);
    }

    int inputIndex = _inputConnections.indexOf(con);
    if (inputIndex != -1)
    {
        _inputConnections.remove(inputIndex);
    }
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

    painter->setPen(Qt::NoPen);
    painter->setBrush(TEXT_COLOR);
    painter->drawPath(_textPath);
}

int GraphNode::type() const
{
    return static_cast<int>(ItemType::Node);
}

void GraphNode::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsPathItem::mouseMoveEvent(event);

    updateConnections();
    for (GraphNode* node : getConnectedNodes())
    {
        node->updateConnections();
    }
}

void GraphNode::updateConnections()
{
    for (NodeConnection* con : _outputConnections)
    {
        con->setStartPos(getClosestConnectionPoint(con->getEndPos()));
        con->updatePath();
    }

    for (NodeConnection* con : _inputConnections)
    {
        con->setEndPos(getClosestConnectionPoint(con->getStartPos()));
        con->updatePath();
    }
}

QPointF GraphNode::getClosestConnectionPoint(const QPointF &p)
{
    qreal minDist = std::numeric_limits<qreal>::max();
    QPointF closestPoint;

    for (const QPointF& connPt : _connectionPoints) {
        QPointF connPtWorld = scenePos() + connPt;
        QPointF diff = connPtWorld - p;
        qreal distance = QPointF::dotProduct(diff, diff);

        if (distance < minDist)
        {
            minDist = distance;
            closestPoint = connPtWorld;
        }
    }

    return closestPoint;
}

QSet<GraphNode*> GraphNode::getConnectedNodes()
{
    QSet<GraphNode*> nodes;

    for (NodeConnection* con : _outputConnections)
    {
        if (GraphNode* endNode = con->getEndNode())
        {
            nodes.insert(endNode);
        }
    }

    for (NodeConnection* con : _inputConnections)
    {
        if (GraphNode* startNode = con->getStartNode())
        {
            nodes.insert(startNode);
        }
    }

    return nodes;
}
