#include "myevent.h"

MyEvent::MyEvent() {}

MyEvent::MyEvent(QString name, QDate date) : name(name), date(date) {}

QString MyEvent::getName() const {
    return name;
}

void MyEvent::setName(const QString &newName) {
    name = newName;
}

QDate MyEvent::getDate() const {
    return date;
}

void MyEvent::setDate(const QDate &newDate) {
    date = newDate;
}
