#ifndef JSON_WORK_H
#define JSON_WORK_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

#include "jsonfilemanager.h"


class JSONWork : public QObject
{
    Q_OBJECT
public:
    explicit JSONWork(QObject *parent = nullptr);
    ~JSONWork();
    void write_to_json(const QString& event_name, const QDate& event_date);
    void sort_json_data(QJsonArray& jarrToSort);
    void delete_from_json(const QString& nameUser, const QString& dateUser);

signals:

};


#endif // JSON_WORK_H
