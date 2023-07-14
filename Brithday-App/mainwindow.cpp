#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set linear gradient for background
    this->setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(240, 240, 240, 0.79), stop:1 rgba(240, 240, 240, 1));");

    ui->datInput->setDate(QDate::currentDate());
    ui->frMessageOpen->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnAddPeople_clicked()
{
    ui->frMessageOpen->show();
}


void MainWindow::on_btnCancel_clicked()
{
    ui->lnNameInput->clear();
    ui->frMessageOpen->hide();
}


void MainWindow::on_btnOk_clicked()
{

}

