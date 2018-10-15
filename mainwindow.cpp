#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphnode.h"
#include "customdockwidget.h"
#include "nodeeditor.h"
#include "inspectorwidget.h"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _scene = new QGraphicsScene();

    // Editor
    CustomDockWidget* dock = new CustomDockWidget(tr("Editor"), this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    dock->setStyleSheet(QString{"background: url(:/images/bg-image.png);"});
    dock->setMinimumSize(QSize{800, 500});

    _view = new QGraphicsView(dock);
    _view->setScene(_scene);
    _view->setRenderHint(QPainter::Antialiasing, true);

    dock->setWidget(_view);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    // Inspector
    CustomDockWidget* inspectorDock = new CustomDockWidget(tr("Inspector"), this);
    inspectorDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    inspectorDock->setStyleSheet(QString{"background: url(:/images/bg-image.png);"});
    inspectorDock->setMinimumSize(400, 400);
    _inspector = new InspectorWidget(this);
    inspectorDock->setWidget(_inspector);
    addDockWidget(Qt::RightDockWidgetArea, inspectorDock);

    _nodeEditor = new NodeEditor(this, this);
    _nodeEditor->install(_scene);

    // #TESTING
    GraphNode* node = new GraphNode();
    node->setPos(150, 150);
    node->setProperty("name", "node1");
    _nodeEditor->addNode(node);

    GraphNode* node2 = new GraphNode();
    node2->setPos(400, 150);
    node2->setProperty("name", "node2");
    _nodeEditor->addNode(node2);
}

MainWindow::~MainWindow()
{
    delete _ui;
}

void MainWindow::onNodeSelected(GraphNode *node)
{
    _inspector->inspectGraphNode(node);
}

void MainWindow::onTransitionSelected(NodeConnection *con)
{
    _inspector->inspectTransition(con);
}

void MainWindow::onDeselect()
{
    _inspector->deselect();
}

void MainWindow::updateUI()
{
    _scene->update();
}

void MainWindow::on_actionAdd_Node_triggered()
{
    GraphNode* node = new GraphNode();
    node->setPos(150, 150);

    _nodeEditor->addNode(node);
}

void MainWindow::on_actionLoad_triggered()
{
    loadAction();
}

void MainWindow::on_actionSave_triggered()
{
    saveAction();
}

void MainWindow::on_actionNew_triggered()
{
    newProject();
}

bool MainWindow::loadAction()
{
    // Load
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open animation controller"), "",
        tr("Animation Controller (*.animctrl)"));

    if (!fileName.isEmpty())
    {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                file.errorString());
            return false;
        }

        QByteArray rawData = file.readAll();

        QJsonDocument data {QJsonDocument::fromJson(rawData)};

        _nodeEditor->load(data.object());
        return true;
    }

    return false;
}

bool MainWindow::saveAction()
{
    // Save
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save animation controller"), "",
        tr("Animation Controller (*.animctrl)"));

    if (!fileName.isEmpty())
    {
        QFile file{fileName};
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return false;
        }

        QTextStream out(&file);

        // Write to file
        QJsonObject data;
        _nodeEditor->save(data);

        QJsonDocument jsonDoc{data};
        file.write(jsonDoc.toJson());

        QMessageBox::information(this, tr("Success!"), tr("Great success!"));

        return true;
    }

    return false;
}

bool MainWindow::newProject()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Create new graph",
        "All unsaved changes will be lost. Are you sure?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes)
    {
        _nodeEditor->clear();
        return true;
    }

    return false;
}
