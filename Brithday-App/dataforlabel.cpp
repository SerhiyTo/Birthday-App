#include "dataforlabel.h"

DataForLabel::DataForLabel(QObject *parent)
    : QObject{parent}, nameUser{" "}, dateUser{QDate::currentDate()}
{

}

DataForLabel::DataForLabel(QObject *parent, QString nameUser, QDate dateUser)
    : QObject{parent}, nameUser{nameUser}, dateUser{dateUser}
{

}

