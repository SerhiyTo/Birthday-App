#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H
#include <QWidget>
#include <QGridLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QDate>
#include <memory>
#include "stylehelper.h"

class WidgetFactory
{
public:
    WidgetFactory();
    static QWidget* getNewEventWidget(const QString& nameUser, const QString& dateUser, std::function<void()> deleteBtnActions);
};

#endif // WIDGETFACTORY_H
