#include "pushnotification.h"

PushNotification::PushNotification(QObject *parent)
    : QObject(parent)
{
    trayIcon = new QSystemTrayIcon(QIcon(":/bird.png"), parent);

    connect(trayIcon, &QSystemTrayIcon::activated, this, &PushNotification::tray_icon_activated);
}

PushNotification::~PushNotification()
{
    delete trayIcon;
}

void PushNotification::show_info(const QString &message)
{
    trayIcon->showMessage("Button Clicked", message, QSystemTrayIcon::Information, 5000);
}

void PushNotification::tray_icon_activated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Context)
    {
        qInfo() << "Context: " << reason;
    }
    else if (reason == QSystemTrayIcon::Trigger)
    {
        qInfo() << "Triggered: " << reason;
    }
}
