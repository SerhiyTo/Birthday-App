#include "jsonfilemanager.h"

JSONFileManager::JSONFileManager(const QIODevice::OpenMode& mode, const QString& fileName)
    : jsonFile(fileName)
{
    jsonFile.open(mode);  // Open the file with passed mode in param
}

JSONFileManager::~JSONFileManager()
{
    if (jsonFile.isOpen())
    {
        jsonFile.close();  // Close the file
    }
}

void JSONFileManager::writeJsonArray(const QJsonArray& jsonArray)
{
    if (jsonFile.isOpen())
    {
        jsonFile.resize(0);  // Get jsonFile empty
        jsonFile.write(QJsonDocument(jsonArray).toJson());  // Write new json array to our json file
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
