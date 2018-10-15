#ifndef NODECONNECTION_H
#define NODECONNECTION_H

#include <QGraphicsPathItem>
#include <QColor>

class GraphNode;

class NodeConnection : public QGraphicsPathItem
{
public:
    NodeConnection(QGraphicsItem* parent = nullptr);
    ~NodeConnection() override;

    void setProperty(const QString &prop, const QString &value);
    QString getProperty(const QString &prop) const;

    void setStartPos(const QPointF &p);
    void setEndPos(const QPointF &p);

    const QPointF &getStartPos() const;
    const QPointF &getEndPos() const;

    void setStartNode(GraphNode* node);
    void setEndNode(GraphNode* node);

    GraphNode* getStartNode() const;
    GraphNode* getEndNode() const;

    void updatePath();

// QGraphicsItem
public:
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    virtual int type() const override;

private:
    const QColor CONNECTION_COLOR_UNSELECTED = QColor{43, 154, 157};
    const QColor CONNECTION_COLOR_SELECTED = QColor{50, 161, 164};
    const qreal CONNECTION_THICKNESS_UNSELECTED = 3;
    const qreal CONNECTION_THICKNESS_SELECTED = 5;

    QMap<QString, QString> _properties;

    QPointF _startPos;
    QPointF _endPos;

    GraphNode* _startNode;
    GraphNode* _endNode;
};

#endif // NODECONNECTION_H
