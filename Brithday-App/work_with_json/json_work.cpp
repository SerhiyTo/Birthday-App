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
    JSONFileManager jsonManager(QIODevice::ReadWrite | QIODevice::Text);
    QJsonArray jArrToSort = jsonManager.readFromJsonArray();

    // Create new JSON object
    QJsonObject recordObject;
    recordObject.insert("Name", QJsonValue::fromVariant(event_name));
    recordObject.insert("Date", QJsonValue::fromVariant(event_date));

    jArrToSort.append(recordObject);
    sort_json_data(jArrToSort);

    jsonManager.writeJsonArray(jArrToSort);
}


void JSON_work::sort_json_data(QJsonArray &jarrToSort)
{
    QVector<QDate> datesVec;
    QJsonValue temp;

    QString dateString;
    QDate dateFromJson;

    QJsonObject recordObject;

    // Сreate vector for dates
    for (int i = 0; i < jarrToSort.size(); ++i)
    {
        recordObject = jarrToSort[i].toObject();
        dateString = jarrToSort[i].toObject().value("Date").toString();
        dateFromJson = QDate::fromString(dateString, "yyyy-MM-dd");

        if (QDate::currentDate().daysTo(dateFromJson) < 0)
        {
            dateFromJson = dateFromJson.addYears(QDate::currentDate().year() - dateFromJson.year() + (QDate::currentDate().month() > dateFromJson.month()));
        }
        datesVec.append(dateFromJson);
        recordObject.insert("Date", QJsonValue::fromVariant(dateFromJson));
        jarrToSort.replace(i, recordObject);
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

void JSON_work::delete_from_json(const QString& nameUser, const QString& dateUser)
{
    JSONFileManager jsonManager(QIODevice::ReadWrite | QIODevice::Text);
    QJsonArray jArr = jsonManager.readFromJsonArray();

    for (int i = 0; i < jArr.size(); ++i)
    {
        QJsonObject recordObject = jArr[i].toObject();
        QString name = recordObject.value("Name").toString();
        QString date = recordObject.value("Date").toString();
        if (name == nameUser && date == dateUser)
        {
            jArr.removeAt(i);
            break;
        }
    }

    jsonManager.writeJsonArray(jArr);
}
