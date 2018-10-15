#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <QObject>

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
private:
    QGraphicsItem* itemAt(const QPointF&);

    void handleMousePressed(QGraphicsSceneMouseEvent* me);
    void handleMouseMoved(QGraphicsSceneMouseEvent* me);
    void handleMouseReleased(QGraphicsSceneMouseEvent* me);

private:
    MainWindow* _mainWindow;
    QGraphicsScene* _scene;
    NodeConnection* _currentConnection;
};

#endif // NODEEDITOR_H
