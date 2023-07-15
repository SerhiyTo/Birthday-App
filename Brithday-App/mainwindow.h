#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>

#include <QMessageBox>
#include <QLabel>
#include <QFrame>
#include <QVector>
#include <QPixmap>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    const QString file_name = "./EventList.json";

private:
    void write_to_json(const QString& file_name_to_write);
    void read_from_json(const QString& file_name_to_read);
    void generate_label(const QString& dateUser, const QString& nameUser);

private slots:
    void on_btnAddPeople_clicked();

    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
