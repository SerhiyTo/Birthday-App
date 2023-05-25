#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(240, 240, 240, 0.79), stop:1 rgba(240, 240, 240, 1));");

    button_adding = ui->button_add_people;
    button_adding->setStyleSheet("background-color: #000000; color: #FFFFFF; border-radius: 30px");

    text_upper_button = ui->text_upper_button;
    text_upper_button->setTextFormat(Qt::RichText);
    text_upper_button->setText("<span style='font-family: \"Segoe UI Emoji\";'>✍️</span> "
                               "<span style='font-family: \"Inter\"; font-weight: 500;'>Click the button below to add<br>"
                               "a new person to the database</span>");
}

MainWindow::~MainWindow()
{
    delete ui;
}

