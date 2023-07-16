#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
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

    // Read existing JSON file
    QFile json_file(file_name_to_write);
    QJsonDocument doc = QJsonDocument::fromJson("[]");

    if (json_file.exists() && json_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QByteArray jsonData = json_file.readAll();
        doc = QJsonDocument::fromJson(jsonData);
        json_file.close();
    }

    // Create new JSON object
    QJsonObject record_object;
    record_object.insert("Name", QJsonValue::fromVariant(event_name));
    record_object.insert("Date", QJsonValue::fromVariant(event_date));


    // Sort array
    QJsonArray jarrToSort =  doc.array();
    jarrToSort.append(record_object);
    QVector<QDate> datesVec;
    QJsonValue temp;
    //create vector for dates
    for(int i=0;i<jarrToSort.size();i++)
    {
        datesVec.append(QDate::fromString(jarrToSort[i].toObject()["Date"].toString(), "yyyy-MM-dd"));
    }
    //buuble sort for dates
    for(int i=0;i<datesVec.size()-1;i++)
    {
        if(datesVec[i]>datesVec[i+1])
        {
            temp = jarrToSort[i];
            jarrToSort[i] = jarrToSort[i+1];
            jarrToSort[i+1] = temp;
            std::swap(datesVec[i],datesVec[i+1]);
            i-=i==0?1:2;
            continue;
        }
    }

    // Write the updated JSON array to the file
    json_file.open(QIODevice::WriteOnly | QIODevice::Text);
    json_file.write(QJsonDocument(jarrToSort).toJson());
    json_file.close();
}

void MainWindow::read_from_json(const QString& file_name_to_read)
{
    QFile jsonFileToRead(file_name_to_read);
    jsonFileToRead.open(QIODevice::ReadOnly | QIODevice::Text);  // Opening JSON file for reading data from it
    if(!jsonFileToRead.isOpen()) return;

    QString readInfo = jsonFileToRead.readAll();  // Get all data from JSON file
    jsonFileToRead.close();  // Closing file

    QJsonDocument doc = QJsonDocument::fromJson(readInfo.toUtf8());  // Convert data to UTF-8
    QJsonArray jArr = doc.array();  // Casting our data from json to array

    QString dateUser, nameUser;  // Creating variable for using its in loop
    for (auto jsonObj : jArr)
    {
        dateUser = jsonObj.toObject().value("Date").toString();  // Get data from JSON with key parametr "Date"
        nameUser = jsonObj.toObject().value("Name").toString();  // Get data from JSON with key parametr "Name"
        generate_label(dateUser, nameUser);  // Calling function "generate label" for display current data in label
    }
}


void MainWindow::generate_label(const QString& dateUser, const QString& nameUser)
{
    if(ui->laForData->count() >= 6) return;

    QVBoxLayout* layOneUser = new QVBoxLayout;
    QFrame* frLayWithData = new QFrame;

    QDate currentDay = QDate::currentDate();
    QDate dateFromString = QDate::fromString(dateUser, "yyyy-MM-dd");
    if(currentDay.daysTo(dateFromString) < 0)return;

    QString formattedDate = dateFromString.toString("dd.MM");  // Format date
    QString daysToBirthday = " (Days to Birthday: " + QString::number(currentDay.daysTo(dateFromString)) + ")";

    QLabel* lblUserName = new QLabel(nameUser);  // Creating new Label with User Name
    QLabel* lblUserDate = new QLabel(formattedDate + daysToBirthday);  // Creating new Label with our formatted date
                                                                       // and counted days to birthday
    //label with user name formating
    QFont userNameFont = lblUserName->font();
    userNameFont.setBold(true);
    lblUserName->setFont(userNameFont);

    //add to form
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
    ui->datInput->setDate(QDate::currentDate());  // Set current date by deffault in our QDate
    ui->frMessageOpen->hide();  // Hide our message
}


void MainWindow::on_btnOk_clicked()
{
    write_to_json(file_name);

    QMessageBox::information(this, "People was added!", "Adding people to databasw was sucessed");

    ui->lnNameInput->clear();  // Clear the form
    ui->datInput->setDate(QDate::currentDate());  // Set current date by deffault in our QDate
    ui->frMessageOpen->hide();  // Hide the form
}

