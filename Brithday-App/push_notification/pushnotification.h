#ifndef PUSHNOTIFICATION_H
#define PUSHNOTIFICATION_H

#include <QObject>

class PushNotification : public QObject
{
    Q_OBJECT
public:
    explicit PushNotification(QObject *parent = nullptr);

signals:

};

#endif // PUSHNOTIFICATION_H
