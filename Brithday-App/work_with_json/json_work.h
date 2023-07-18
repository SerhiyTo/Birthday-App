#ifndef JSON_WORK_H
#define JSON_WORK_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>


class JSON_work : public QObject
{
    Q_OBJECT
public:
    explicit JSON_work(QObject *parent = nullptr);
    ~JSON_work();
    void write_to_json(const QString& event_name, const QDate& event_date);
    void sort_json_data(QJsonArray& jarrToSort);
    QJsonArray get_json_array();
    void delete_from_json(const QString& nameUser, const QString& dateUser);

    const QString file_name;

signals:

};


#endif // JSON_WORK_H
