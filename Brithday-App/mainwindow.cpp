#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set linear gradient for background
    this->setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(240, 240, 240, 0.79), stop:1 rgba(240, 240, 240, 1));");

    ui->datInput->setDate(QDate::currentDate());
    ui->frMessageOpen->hide();

    read_from_json(file_name);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::write_to_json(const QString& file_name_to_write)
{
    // Get info
    QString event_name = ui->lnNameInput->text();
    QDate event_date = ui->datInput->date();

    // Create json
    QJsonObject record_object;
    record_object.insert("Name", QJsonValue::fromVariant(event_name));
    record_object.insert("Date", QJsonValue::fromVariant(event_date));

    QJsonDocument doc(record_object);

    // Write to file
    QFile json_file(file_name_to_write);
    json_file.open(QIODevice::WriteOnly | QIODevice::Append);
    json_file.write(doc.toJson());
    json_file.close();

    read_from_json(file_name_to_write);
}

void MainWindow::read_from_json(const QString& file_name_to_read)
{
    QFile jsonFileToRead(file_name_to_read);
    jsonFileToRead.open(QIODevice::ReadOnly | QIODevice::Text);  // Opening JSON file for reading data from it
    QString readInfo = jsonFileToRead.readAll();  // Get all data from JSON file
    jsonFileToRead.close();  // Closing file

    qInfo() << readInfo;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(readInfo.toUtf8(), &error);  // Convert data to UTF-8
    if (error.error)
    {
        qInfo() << "Error: " << error.errorString();
    }
    QJsonArray jArr = doc.array();  // Casting our data from json to array

    qInfo() << doc;

    QString dateUser, nameUser;  // Creating variable for using its in loop    
    for (auto jsonObj : jArr)
    {
        dateUser = jsonObj.toObject().value("Date").toString();  // Get data from JSON with key parametr "Date"
        nameUser = jsonObj.toObject().value("Name").toString();  // Get data from JSON with key parametr "Name"
        qInfo() << dateUser << typeid(dateUser).name() << "\n" << nameUser << typeid(nameUser).name();
        generate_label(dateUser, nameUser);  // Calling function "generate label" for display current data in label
    }
}

void MainWindow::generate_label(const QString& dateUser, const QString& nameUser)
{
    QVBoxLayout* layOneUser = new QVBoxLayout;
    QFrame* frLayWithData = new QFrame;

    QLabel* lblUserName = new QLabel(nameUser);
    QLabel* lblUserDate = new QLabel(dateUser);

    layOneUser->addWidget(lblUserName);
    layOneUser->addWidget(lblUserDate);

    frLayWithData->setLayout(layOneUser);

    ui->laForData->addWidget(frLayWithData);
}

void MainWindow::on_btnAddPeople_clicked()
{
    ui->frMessageOpen->show();  // Showing our form
}


void MainWindow::on_btnCancel_clicked()
{
    ui->lnNameInput->clear();  // Clear the form
    ui->datInput->setDate(QDate::currentDate());
    ui->frMessageOpen->hide();  // Hide our message
}


void MainWindow::on_btnOk_clicked()
{
    write_to_json(file_name);

    QMessageBox::information(this, "People was added!", "Adding people to databasw was sucessed");

    ui->lnNameInput->clear();  // Clear the form
    ui->datInput->setDate(QDate::currentDate());
    ui->frMessageOpen->hide();
}

