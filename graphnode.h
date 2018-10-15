#ifndef GRAPHNODE_H
#define GRAPHNODE_H

#include <QGraphicsPathItem>
#include <QColor>
#include <QPen>
#include <QVector>
#include <QMap>
#include <QSet>

class NodeConnection;

class GraphNode : public QGraphicsPathItem
{
public:
    GraphNode(qreal width = 200, qreal heigth = 50, QGraphicsItem *parent = nullptr);

    void setProperty(const QString &prop, const QString &value);
    QString getProperty(const QString &prop) const;

    void addOutputConnection(NodeConnection* con);
    void addInputConnection(NodeConnection* con);

    void removeConnection(NodeConnection* con);

    const QVector<NodeConnection*>& getInputConnections() const;
    const QVector<NodeConnection*>& getOutputConnections() const;

// QGraphicsItem
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual int type() const override;

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    const QColor BG_COLOR_UNSELECTED = QColor{24, 26, 24, 200};
    const QColor BG_COLOR_SELECTED = QColor{24, 26, 24, 220};
    const QColor EDGE_COLOR_UNSELECTED = QColor{16, 17, 16};
    const QColor EDGE_COLOR_SELECTED = QColor{242, 178, 0};

    const QColor TEXT_COLOR = QColor{255, 255, 255};
    const int TEXT_SIZE = 10;

    const qreal EDGE_WIDTH = 5;

    qreal _width;
    qreal _height;

    QPainterPath _textPath;

    QMap<QString, QString> _properties;

    QVector<NodeConnection*> _outputConnections;
    QVector<NodeConnection*> _inputConnections;

    QVector<QPointF> _connectionPoints;

private:
    void updateConnections();
    QPointF getClosestConnectionPoint(const QPointF &p);
    QSet<GraphNode*> getConnectedNodes();
};

#endif // GRAPHNODE_H
