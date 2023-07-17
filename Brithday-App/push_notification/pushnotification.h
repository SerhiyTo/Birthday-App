#ifndef PUSHNOTIFICATION_H
#define PUSHNOTIFICATION_H

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>

class PushNotification : public QObject
{
    Q_OBJECT
public:
    explicit PushNotification(QObject *parent = nullptr);
    ~PushNotification();

    void show_info(const QString& message);

private:
    QSystemTrayIcon* trayIcon;

private slots:
    void tray_icon_activated(QSystemTrayIcon::ActivationReason reason);
};

#endif // PUSHNOTIFICATION_H
