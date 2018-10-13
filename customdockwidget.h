#ifndef CUSTOMDOCKWIDGET_H
#define CUSTOMDOCKWIDGET_H

#include <QtWidgets>

class CustomDockWidget : public QDockWidget
{
public:
    CustomDockWidget(const QString &title, QWidget *parent = nullptr);
};

#endif // CUSTOMDOCKWIDGET_H
