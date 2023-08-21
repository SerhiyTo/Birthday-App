#ifndef JSONFILEMANAGER_H
#define JSONFILEMANAGER_H

#include <QObject>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

class JSONFileManager : public QObject
{
    Q_OBJECT
public:
    explicit JSONFileManager(const QIODevice::OpenMode& mode = QIODevice::ReadWrite,
                            const QString& fileName = "./EventList.json");
    ~JSONFileManager();
    void writeJsonArray(const QJsonArray& jsonArray);
    QJsonArray readFromJsonArray();

private:
    QFile jsonFile;

signals:

};

#endif // JSONFILEMANAGER_H
