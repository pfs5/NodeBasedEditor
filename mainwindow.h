#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtWidgets>

class NodeEditor;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow* _ui;
    QGraphicsScene* _scene;
    QGraphicsView* _view;
    NodeEditor* _nodeEditor;
};

#endif // MAINWINDOW_H
