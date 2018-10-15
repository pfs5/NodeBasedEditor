#include "inspectorwidget.h"
#include "ui_inspectorwidget.h"
#include "graphnode.h"
#include "nodeconnection.h"
#include "mainwindow.h"

#include <QDebug>   // #TODO remove this

InspectorWidget::InspectorWidget(MainWindow* mainWindow, QWidget *parent) :
    QWidget(parent),
    _ui(new Ui::InspectorWidget()),
    _mainWindow{mainWindow},
    _currentNode{nullptr}
{
    _ui->setupUi(this);

    deselect();
}

InspectorWidget::~InspectorWidget()
{
    delete _ui;
}

void InspectorWidget::inspectGraphNode(GraphNode *node)
{
    deselect();

    _currentNode = node;
    _ui->ObjectName->setText(node->getProperty(tr("name")));
    _ui->ObjectName->setReadOnly(false);
    _ui->NodeIsLooping->setCheckState(node->getProperty("islooping") == "true" ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    _ui->NodeSpeed->setValue(node->getProperty("speed").toDouble());

    _ui->NodeContentFrame->show();
    _ui->NodeLogo->show();
}

void InspectorWidget::inspectTransition(NodeConnection *con)
{
    deselect();

    _currentTransition = con;

    _ui->ObjectName->setText(tr(""));
    _ui->ObjectName->setReadOnly(true);
    _ui->TransitionTrigger->setText(con->getProperty("trigger"));
    _ui->TransitionFrom->setText(con->getStartNode()->getProperty("name"));
    _ui->TransitionTo->setText(con->getEndNode()->getProperty("name"));

    _ui->TransitionContentFrame->show();
    _ui->TransitionLogo->show();
}

void InspectorWidget::deselect()
{
    _currentNode = nullptr;
    _currentTransition = nullptr;

    _ui->ObjectName->setText(tr(""));
    _ui->ObjectName->setReadOnly(true);

    _ui->NodeContentFrame->hide();
    _ui->NodeLogo->hide();

    _ui->TransitionContentFrame->hide();
    _ui->TransitionLogo->hide();
}

void InspectorWidget::on_ObjectName_editingFinished()
{
    if (_currentNode != nullptr)
    {
        _currentNode->setProperty("name", _ui->ObjectName->text());
    }

    _mainWindow->updateUI();
}

void InspectorWidget::on_ObjectName_textChanged(const QString &arg1)
{
    if (_currentNode != nullptr)
    {
        _currentNode->setProperty("name", arg1);
    }

    _mainWindow->updateUI();
}

void InspectorWidget::on_TransitionTrigger_textChanged(const QString &arg1)
{
    if (_currentTransition != nullptr)
    {
        _currentTransition->setProperty("trigger", arg1);
    }
}

void InspectorWidget::on_NodeIsLooping_stateChanged(int arg1)
{
    QString value = arg1 > 0 ? "true" : "false";

    if (_currentNode != nullptr)
    {
        _currentNode->setProperty("islooping", value);
    }
}

void InspectorWidget::on_NodeSpeed_valueChanged(const QString &arg1)
{
    if (_currentNode != nullptr)
    {
        _currentNode->setProperty("speed", arg1);
    }
}
