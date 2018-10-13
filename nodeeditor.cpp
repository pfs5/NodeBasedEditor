#include "nodeeditor.h"
#include "nodeeditorshared.h"
#include "nodeconnection.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

#include <QDebug>   // #TODO REMOVE THIS ON RELEASE

NodeEditor::NodeEditor(QObject* parent):
    QObject{parent},
    _currentConnection{nullptr}
{

}

void NodeEditor::install(QGraphicsScene* scene)
{
    scene->installEventFilter(this);
    _scene = scene;
}

bool NodeEditor::eventFilter(QObject *o, QEvent *e)
{
    QGraphicsSceneMouseEvent *me = static_cast<QGraphicsSceneMouseEvent*>(e);

    switch (e->type())
    {
        case QEvent::GraphicsSceneMousePress:
        {
            handleMousePressed(me);
            break;
        }

        case QEvent::GraphicsSceneMouseMove:
        {
            handleMouseMoved(me);
            break;
        }

        case QEvent::GraphicsSceneMouseRelease:
        {
            handleMouseReleased(me);
            break;
        }
    }

    return QObject::eventFilter(o, e);
}

QGraphicsItem* NodeEditor::itemAt(const QPointF& pos)
{
    QList<QGraphicsItem*> items = _scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

    foreach(QGraphicsItem* item, items)
        if (item->type() > QGraphicsItem::UserType)
            return item;

    return nullptr;
}

void NodeEditor::handleMousePressed(QGraphicsSceneMouseEvent* me)
{
    if (me->button() == Qt::RightButton)
    {
        QGraphicsItem* targetItem = itemAt(me->scenePos());

        if(targetItem != nullptr && targetItem->type() == static_cast<int>(ItemType::Node))
        {
            _currentConnection = new NodeConnection{};
            _currentConnection->setStartPos(targetItem->scenePos());
            _currentConnection->setEndPos(me->scenePos());
            _currentConnection->updatePath();

            _scene->addItem(_currentConnection);
        }
    }
}

void NodeEditor::handleMouseMoved(QGraphicsSceneMouseEvent* me)
{
    if (_currentConnection != nullptr)
    {
        _currentConnection->setEndPos(me->scenePos());
        _currentConnection->updatePath();
    }
}

void NodeEditor::handleMouseReleased(QGraphicsSceneMouseEvent* me)
{
    if (me->button() == Qt::RightButton)
    {
        // #TODO handle connection to node
        _currentConnection = nullptr;
    }
}
