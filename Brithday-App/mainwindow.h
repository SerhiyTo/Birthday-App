#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddPeople_clicked();

    void on_btnCancel_clicked();

    void on_btnOk_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
