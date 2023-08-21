#include "jsonfilemanager.h"

JSONFileManager::JSONFileManager(const QIODevice::OpenMode& mode, const QString& fileName)
    : jsonFile(fileName)
{
    jsonFile.open(mode);
}

JSONFileManager::~JSONFileManager()
{
    if (jsonFile.isOpen())
    {
        jsonFile.close();
    }
}

void JSONFileManager::writeJsonArray(const QJsonArray& jsonArray)
{
    if (jsonFile.isOpen())
    {
        jsonFile.resize(0);
        jsonFile.write(QJsonDocument(jsonArray).toJson());
    }
}

QJsonArray JSONFileManager::readFromJsonArray()
{
    if (!jsonFile.isOpen())
    {
        return QJsonArray();
    }

    QByteArray jsonData = jsonFile.readAll();
    return QJsonDocument::fromJson(jsonData).array();
}
