#include "json_work.h"
#include "jsonfilemanager.h"


JSONWork::JSONWork(QObject *parent)
    : QObject(parent)
{

}

JSONWork::~JSONWork()
{

}


void JSONWork::writeToJson(const QString& event_name, const QDate& event_date)
{
    JSONFileManager jsonManager(QIODevice::ReadWrite | QIODevice::Text);
    QJsonArray jArrToSort = jsonManager.readFromJsonArray();

    // Create new JSON object
    QJsonObject recordObject;
    recordObject.insert("Name", QJsonValue::fromVariant(event_name));
    recordObject.insert("Date", QJsonValue::fromVariant(event_date));

    jArrToSort.append(recordObject);
    sortJsonData(jArrToSort);

    jsonManager.writeJsonArray(jArrToSort);
}

inline void swap(QJsonValueRef v1, QJsonValueRef v2)
{
    QJsonValue temp(v1);
    v1 = QJsonValue(v2);
    v2 = temp;
}

void JSONWork::sortJsonData(QJsonArray &jarrToSort)
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
            dateFromJson = dateFromJson.addYears(
                QDate::currentDate().year() - dateFromJson.year() +
                (QDate::currentDate().month() > dateFromJson.month()
            ));
        }
        datesVec.append(dateFromJson);
        recordObject.insert("Date", QJsonValue::fromVariant(dateFromJson));
        jarrToSort.replace(i, recordObject);
    }

    std::sort(jarrToSort.begin(), jarrToSort.end(), [](const QJsonValue& v1, const QJsonValue& v2){
        return v1.toObject()["Date"].toString() < v2.toObject()["Date"].toString();
    });
}

void JSONWork::deleteFromJson(const QString& nameUser, const QString& dateUser)
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
