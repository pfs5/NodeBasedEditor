#ifndef NODEEDITOR_H
#define NODEEDITOR_H

#include <QObject>

class QGraphicsScene;
class QGraphicsItem;
class QGraphicsSceneMouseEvent;
class NodeConnection;

class NodeEditor : public QObject
{
    Q_OBJECT
public:
    explicit NodeEditor(QObject* parent = nullptr);

    void install(QGraphicsScene* scene);
    bool eventFilter(QObject *, QEvent *);

private:
    QGraphicsItem* itemAt(const QPointF&);

    void handleMousePressed(QGraphicsSceneMouseEvent* me);
    void handleMouseMoved(QGraphicsSceneMouseEvent* me);
    void handleMouseReleased(QGraphicsSceneMouseEvent* me);

private:
    QGraphicsScene* _scene;
    NodeConnection* _currentConnection;
};

#endif // NODEEDITOR_H
