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
    _textPath.addText(-width/2 + 30, -height/2 + 30, font, "");

    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptDrops(true);

    _connectionPoints.append(QPointF{0, -height/2});    // Top
    _connectionPoints.append(QPointF{width/2, 0});      // Right
    _connectionPoints.append(QPointF{0, height/2});     // Bottom
    _connectionPoints.append(QPointF{-width/2, 0});     // Left

    // Set default properties
    setProperty("name", "");
    setProperty("speed", "1.0");
    setProperty("islooping", "false");
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

const QVector<NodeConnection *> &GraphNode::getInputConnections() const
{
    return _inputConnections;
}

const QVector<NodeConnection *> &GraphNode::getOutputConnections() const
{
    return _outputConnections;
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
    std::vector<NodeConnection*> connectionsPerPosition [4][2];

    for (NodeConnection* con : _outputConnections)
    {
        int conPos = getClosesConnectionPosition(con->getEndPos());
        //con->setStartPos(_connectionPoints[(int) conPos]);
        connectionsPerPosition[conPos][0].push_back(con);
        con->updatePath();
    }

    for (NodeConnection* con : _inputConnections)
    {
        int conPos = getClosesConnectionPosition(con->getStartPos());
        //con->setEndPos(_connectionPoints[(int) conPos]);;
        connectionsPerPosition[conPos][1].push_back(con);

        con->updatePath();
    }

    float dist = 20.f;

    // Set node positions
    for (int i = 0; i < 4; ++i)
    {
        unsigned int pointsPerConnectionPoint = connectionsPerPosition[i][0].size() + connectionsPerPosition[i][1].size();
        float globalOffset = - dist * (pointsPerConnectionPoint - 1) / 2.f;
        float currentOffset = 0.f;

        for (NodeConnection* con : connectionsPerPosition[i][0])
        {
            float offset = currentOffset + globalOffset;
            currentOffset += dist;

            QPointF position = _connectionPoints[i] + scenePos();

            if (i % 2 == 0)
            {
                position.setX(position.x() + offset);
            }
            else
            {
                position.setY(position.y() + offset);
            }


            con->setStartPos(position);
        }

        for (NodeConnection* con : connectionsPerPosition[i][1])
        {
            float offset = currentOffset + globalOffset;
            currentOffset += dist;

            QPointF position = _connectionPoints[i] + scenePos();
            if (i % 2 == 0)
            {
                position.setX(position.x() + offset);
            }
            else
            {
                position.setY(position.y() + offset);
            }

            con->setEndPos(position);
        }
    }
}

int GraphNode::getClosesConnectionPosition(const QPointF &p)
{
    qreal minDist = std::numeric_limits<qreal>::max();
    int connectionPositionIndex = 0;

    for (int i = 0; i < 4; ++i) {
        const QPointF& connPt = _connectionPoints[i];

        QPointF connPtWorld = scenePos() + connPt;
        QPointF diff = connPtWorld - p;
        qreal distance = QPointF::dotProduct(diff, diff);

        if (distance < minDist)
        {
            minDist = distance;
            connectionPositionIndex = i;
        }
    }

    return connectionPositionIndex;
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
