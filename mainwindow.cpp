#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphnode.h"
#include "customdockwidget.h"
#include "nodeeditor.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _scene = new QGraphicsScene();

    CustomDockWidget* dock = new CustomDockWidget(tr("Editor"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setStyleSheet(QString{"background: url(:/images/bg-image.png);"});
    dock->setMinimumSize(QSize{500, 500});

    _view = new QGraphicsView(dock);
    _view->setScene(_scene);
    _view->setRenderHint(QPainter::Antialiasing, true);

    dock->setWidget(_view);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    _nodeEditor = new NodeEditor(this);
    _nodeEditor->install(_scene);

    // #TESTING
    GraphNode* node = new GraphNode();
    node->setPos(150, 150);
    _scene->addItem(node);

}

MainWindow::~MainWindow()
{
    delete _ui;
}
