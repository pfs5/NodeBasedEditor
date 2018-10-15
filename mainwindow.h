#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>

class NodeEditor;
class GraphNode;
class InspectorWidget;
class NodeConnection;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void onNodeSelected(GraphNode* node);
    void onTransitionSelected(NodeConnection* con);
    void onDeselect();

    void updateUI();

private slots:
    void on_actionAdd_Node_triggered();

    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_actionNew_triggered();

private:
    bool loadAction();
    bool saveAction();
    bool newProject();

private:
    Ui::MainWindow* _ui;
    QGraphicsScene* _scene;
    QGraphicsView* _view;
    NodeEditor* _nodeEditor;
    InspectorWidget* _inspector;
};

#endif // MAINWINDOW_H
