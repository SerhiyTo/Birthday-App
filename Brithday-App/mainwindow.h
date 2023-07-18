#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QLabel>
#include <QFrame>
#include <QVector>
#include <QPixmap>
#include <QSystemTrayIcon>
#include <QTimer>

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
    void generate_label(const QString& dateUser, const QString& nameUser);
    void generate_birthday_widgets();

    void check_date();
    QString check_birthday_friends(const QDate& dateNow);
    void send_notification(const QString& message);

private slots:
    void on_btnAddPeople_clicked();
    void on_btnCancel_clicked();
    void on_btnOk_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon* traySysIcon;
    JSON_work jsonWork;

};
#endif // MAINWINDOW_H
