#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

#include <QMessageBox>
#include <QLabel>
#include <QFrame>
#include <QVector>
#include <QPixmap>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QMenu>

#include "work_with_json/json_work.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void generateLabel(const QString& dateUser, const QString& nameUser);
    void generateBirthdayWidgets();

    void checkDate();
    QString checkBirthdayFriends(const QDate& dateNow);
    void sendNotification(const QString& message);

    void closeEvent(QCloseEvent* event);  // overload function for closing app
    void trayActivated(QSystemTrayIcon::ActivationReason reason);  // function for activated tray system

    void updateOkButtonState(const QString& nameInput);

private slots:
    void onAddClicked();
    void onCancelClicked();
    void onOkClicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *traySysIcon;
    JSONWork jsonWork;

};
#endif // MAINWINDOW_H
