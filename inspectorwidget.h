#ifndef INSPECTORWIDGET_H
#define INSPECTORWIDGET_H

#include <QWidget>

class GraphNode;
class MainWindow;
class NodeConnection;

namespace Ui {
class InspectorWidget;
}

class InspectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit InspectorWidget(MainWindow* mainWindow, QWidget *parent = nullptr);
    ~InspectorWidget();

    void inspectGraphNode(GraphNode* node);
    void inspectTransition(NodeConnection* con);

    void deselect();
private slots:
    void on_ObjectName_editingFinished();

    void on_ObjectName_textChanged(const QString &arg1);

    void on_TransitionTrigger_textChanged(const QString &arg1);

    void on_NodeIsLooping_stateChanged(int arg1);

    void on_NodeSpeed_valueChanged(const QString &arg1);

private:
    Ui::InspectorWidget* _ui;
    MainWindow* _mainWindow;

    GraphNode* _currentNode;
    NodeConnection* _currentTransition;
};

#endif // INSPECTORWIDGET_H
