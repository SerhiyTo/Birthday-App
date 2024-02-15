#ifndef MYEVENT_H
#define MYEVENT_H
#include"QDate"

class MyEvent
{
public:
    MyEvent(); // Default constructor
    MyEvent(QString name, QDate date); // Constructor with parameters

    // Getters and setters for member variables
    QString getName() const;
    void setName(const QString &newName);
    QDate getDate() const;
    void setDate(const QDate &newDate);

private:
    QString name;
    QDate date;
};

#endif // MYEVENT_H
