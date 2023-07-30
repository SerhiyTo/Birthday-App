#include "json_work.h"


JSON_work::JSON_work(QObject *parent)
    : QObject(parent)
{

}

JSON_work::~JSON_work()
{

}


void JSON_work::write_to_json(const QString& event_name, const QDate& event_date)
{
    // Read existing JSON file
    QFile json_file(file_name);
    QJsonDocument doc = QJsonDocument::fromJson("[]");

    if (json_file.exists() && json_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray jsonData = json_file.readAll();
        doc = QJsonDocument::fromJson(jsonData);
        json_file.close();
    }
    else
    {
        json_file.close();
        json_file.open(QIODevice::WriteOnly | QIODevice::Text);
        json_file.close();
    }


    // Create new JSON object
    QJsonObject record_object;
    record_object.insert("Name", QJsonValue::fromVariant(event_name));
    record_object.insert("Date", QJsonValue::fromVariant(event_date));

    // Sort array
    QJsonArray jarrToSort =  doc.array();
    jarrToSort.append(record_object);

    sort_json_data(jarrToSort);

    // Write the updated JSON array to the file
    json_file.open(QIODevice::WriteOnly | QIODevice::Text);
    json_file.write(QJsonDocument(jarrToSort).toJson());
    json_file.close();
}


void JSON_work::sort_json_data(QJsonArray &jarrToSort)
{
    QVector<QDate> datesVec;
    QJsonValue temp;

    QString dateString;
    QDate dateFromJson;

    QJsonObject record_object;

    // Сreate vector for dates
    for (int i = 0; i < jarrToSort.size(); ++i)
    {
        record_object = jarrToSort[i].toObject();
        dateString = jarrToSort[i].toObject().value("Date").toString();
        dateFromJson = QDate::fromString(dateString, "yyyy-MM-dd");

        if (QDate::currentDate().daysTo(dateFromJson) < 0)
        {
            dateFromJson = dateFromJson.addYears(QDate::currentDate().year() - dateFromJson.year() + (QDate::currentDate().month() > dateFromJson.month()));
        }
        datesVec.append(dateFromJson);
        record_object.insert("Date", QJsonValue::fromVariant(dateFromJson));
        jarrToSort.replace(i, record_object);
    }

    // Bubble sort for dates
    for (int i = 0; i < datesVec.size() - 1; ++i)
    {
        if (datesVec[i] > datesVec[i + 1])
        {
            temp = jarrToSort[i];
            jarrToSort[i] = jarrToSort[i + 1];
            jarrToSort[i + 1] = temp;
            std::swap(datesVec[i],datesVec[i + 1]);
            i -= i == 0 ? 1 : 2;
            continue;
        }
    }
}


QJsonArray JSON_work::get_json_array()
{
    QFile jsonFileToRead(file_name);
    jsonFileToRead.open(QIODevice::ReadOnly | QIODevice::Text);  // Opening JSON file for reading data from it
    if (!jsonFileToRead.isOpen()) return QJsonArray();

    QString readInfo = jsonFileToRead.readAll();  // Get all data from JSON file
    jsonFileToRead.close();  // Closing file

    QJsonDocument doc = QJsonDocument::fromJson(readInfo.toUtf8());  // Convert data to UTF-8
    QJsonArray jArr = doc.array();  // Casting our data from json to array
    return jArr;
}


void JSON_work::delete_from_json(const QString& nameUser, const QString& dateUser)
{
    QJsonArray jArr = get_json_array();

    for (int i = 0; i < jArr.size(); ++i)
    {
        if (jArr[i].toObject().value("Name").toString() == nameUser &&
            jArr[i].toObject().value("Date").toString() == dateUser)
        {
            jArr.removeAt(i);
            break;
        }
    }

    QFile json_file(file_name);
    json_file.open(QIODevice::WriteOnly);
    json_file.write(QJsonDocument(jArr).toJson());
    json_file.close();
}
