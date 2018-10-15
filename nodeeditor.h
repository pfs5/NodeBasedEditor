#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <QObject>
#include <QVector>

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class NodeConnection;
class GraphNode;
class MainWindow;

class NodeEditor : public QObject
{
    Q_OBJECT
public:
    explicit NodeEditor(MainWindow* mainWindow, QObject* parent = nullptr);

    void install(QGraphicsScene* scene);
    bool eventFilter(QObject *, QEvent *);

    void addNode(GraphNode* node);
    void removeNode(GraphNode* node);

    void save(QJsonObject& data) const;
    void load(const QJsonObject& data);
    void clear();
private:
    QGraphicsItem* itemAt(const QPointF&);

    void handleMousePressed(QGraphicsSceneMouseEvent* me);
    void handleMouseMoved(QGraphicsSceneMouseEvent* me);
    void handleMouseReleased(QGraphicsSceneMouseEvent* me);

private:
    MainWindow* _mainWindow;
    QGraphicsScene* _scene;
    NodeConnection* _currentConnection;

    QVector<GraphNode*> _nodes;
};

#endif // NODEEDITOR_H
