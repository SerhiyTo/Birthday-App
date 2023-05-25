#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set linear gradient for background
    this->setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(240, 240, 240, 0.79), stop:1 rgba(240, 240, 240, 1));");

    button_adding = ui->buttonAdd;  // Add button for calling form for input user's info
    button_adding->setText("Add people");  // Add text for our button
    button_adding->setStyleSheet("QPushButton {"
                                 "    background-color: #000000;"
                                 "    color: #FFFFFF;"
                                 "    min-width: 180x;"
                                 "    min-height: 50px;"
                                 "    border-radius: 30px;"
                                 "}");  // Style for our button

    text_upper_button = ui->text_upper_button;  // Text above the button
    text_upper_button->setTextFormat(Qt::RichText);
    text_upper_button->setText("<span style='font-family: \"Segoe UI Emoji\";'>✍️</span> "  // Set Segoe UI Emoji for emoji
                               "<span style='font-family: \"Inter\"; font-weight: 500;'>Click the button below to add<br>"  // Set Inter as our main font
                               "a new person to the database</span>");
}

MainWindow::~MainWindow()
{
    delete ui;
}

