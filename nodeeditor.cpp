#include "nodeeditor.h"
#include "nodeeditorshared.h"
#include "nodeconnection.h"
#include "graphnode.h"
#include "mainwindow.h"

#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>

#include <QDebug>   // #TODO REMOVE THIS ON RELEASE

NodeEditor::NodeEditor(MainWindow* mainWindow, QObject* parent):
    QObject{parent},
    _mainWindow{mainWindow},
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

void NodeEditor::addNode(GraphNode *node)
{
    _scene->addItem(node);
    _nodes.append(node);
}

void NodeEditor::removeNode(GraphNode *node)
{
    int index = _nodes.indexOf(node);
    if (index != -1)
    {
        _nodes.removeAt(index);
    }

    delete node;
}

void NodeEditor::save(QJsonObject &data) const
{
    QJsonArray nodes;
    QJsonArray transitions;

    for (const GraphNode* node : _nodes)
    {
        QJsonObject nodeData;
        nodeData["name"] = node->getProperty("name");
        nodeData["speed"] = node->getProperty("speed");
        nodeData["islooping"] = node->getProperty("islooping");
        nodeData["_posx"] = node->pos().x();
        nodeData["_posy"] = node->pos().y();
        nodes.append(nodeData);

        for (const NodeConnection* con : node->getOutputConnections())
        {
            QJsonObject transData;
            transData["trigger"] = con->getProperty("trigger");
            transData["from"] = con->getStartNode()->getProperty("name");
            transData["to"] = con->getEndNode()->getProperty("name");

            transitions.append(transData);
        }
    }

    data["animations"] = nodes;
    data["transitions"] = transitions;
}

void NodeEditor::load(const QJsonObject &data)
{
    clear();

    // Load new data
    for (const auto nodeData : data["animations"].toArray())
    {
        GraphNode * node = new GraphNode();
        node->setPos(nodeData.toObject()["_posx"].toDouble(), nodeData.toObject()["_posy"].toDouble());
        node->setProperty("name", nodeData.toObject()["name"].toString());
        node->setProperty("speed", nodeData.toObject()["speed"].toString());
        node->setProperty("islooping", nodeData.toObject()["islooping"].toString());

        addNode(node);
    }

    for (const auto conData : data["transitions"].toArray())
    {
        NodeConnection* con = new NodeConnection();
        con->setProperty("trigger", conData.toObject()["trigger"].toString());

        for (GraphNode* node : _nodes)
        {
            if (node->getProperty("name") == conData.toObject()["from"].toString())
            {
                node->addOutputConnection(con);
            }

            if (node->getProperty("name") == conData.toObject()["to"].toString())
            {
                node->addInputConnection(con);
            }
        }

        _scene->addItem(con);
    }

    _mainWindow->updateUI();
}

void NodeEditor::clear()
{
    for (GraphNode* node : _nodes)
    {
        delete node;
    }

    _nodes.clear();
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

           if (GraphNode* node = dynamic_cast<GraphNode*>(targetItem))
           {
               node->addOutputConnection(_currentConnection);
               return;
           }
        }
    }
    else if (me->button() == Qt::LeftButton)
    {
        QGraphicsItem* targetItem = itemAt(me->scenePos());
        if (GraphNode* node = dynamic_cast<GraphNode*>(targetItem))
        {
            _mainWindow->onNodeSelected(node);
            return;
        }
        else if(NodeConnection* connection = dynamic_cast<NodeConnection*>(targetItem))
        {
            _mainWindow->onTransitionSelected(connection);
            return;
        }
    }

    _mainWindow->onDeselect();
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
        if (_currentConnection)
        {
            QGraphicsItem* targetItem = itemAt(me->scenePos());

            if(targetItem != nullptr && targetItem->type() == static_cast<int>(ItemType::Node) && targetItem != _currentConnection->getStartNode())
            {
                if (GraphNode* node = dynamic_cast<GraphNode*>(targetItem))
                {
                    node->addInputConnection(_currentConnection);
                }
            } else
            {
                delete _currentConnection;
            }

            _currentConnection = nullptr;
        }
    }
}
