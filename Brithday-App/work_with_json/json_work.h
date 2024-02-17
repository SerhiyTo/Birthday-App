#ifndef JSON_WORK_H
#define JSON_WORK_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

class JSONWork : public QObject
{
    Q_OBJECT
public:
    explicit JSONWork(QObject *parent = nullptr);
    ~JSONWork();
    static void writeToJson(const QString& event_name, const QDate& event_date);
    static void sortJsonData(QJsonArray& jarrToSort);
    static void deleteFromJson(const QString& nameUser, const QString& dateUser);

signals:

};


#endif // JSON_WORK_H
