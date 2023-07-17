#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    traySysIcon = new QSystemTrayIcon(this);
    traySysIcon->setIcon(QIcon(":/graphics/birthday-cake.ico"));
    traySysIcon->setVisible(true);

    check_date();

    // Set linear gradient for background
    this->setStyleSheet("background: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(240, 240, 240, 0.79), stop:1 rgba(240, 240, 240, 1));");
    ui->laForData->setAlignment(Qt::AlignTop);

    ui->datInput->setDate(QDate::currentDate());
    ui->frMessageOpen->hide();

    read_from_json(file_name);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete traySysIcon;
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

    sort_json_data(jarrToSort);

    // Write the updated JSON array to the file
    json_file.open(QIODevice::WriteOnly | QIODevice::Text);
    json_file.write(QJsonDocument(jarrToSort).toJson());
    json_file.close();

    read_from_json(file_name_to_write);
}


void MainWindow::sort_json_data(QJsonArray &jarrToSort)
{
    QVector<QDate> datesVec;
    QJsonValue temp;

    // Сreate vector for dates
    for (int i = 0; i < jarrToSort.size(); i++)
    {
        datesVec.append(QDate::fromString(jarrToSort[i].toObject()["Date"].toString(), "yyyy-MM-dd"));
    }

    // Bubble sort for dates
    for (int i = 0; i < datesVec.size() - 1; ++i)
    {
        if(datesVec[i] > datesVec[i + 1])
        {
            temp = jarrToSort[i];
            jarrToSort[i] = jarrToSort[i + 1];
            jarrToSort[i + 1] = temp;
            std::swap(datesVec[i],datesVec[i + 1]);
            i -= i == 0 ? 1 : 2;
            continue;
        }
    }
}

void MainWindow::check_date()
{
    int delay = 360000;
    QDate* lastSavedDate = new QDate(QDate::currentDate());
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, lastSavedDate, &delay](){
        if (lastSavedDate->daysTo(QDate::currentDate()) != 0) {
            *lastSavedDate = QDate::currentDate();
            if (delay == 3600000)
            {
                delay = 8640000;
            }
            read_from_json(file_name);
            send_notification(check_birthday_friends(*lastSavedDate));
            //qInfo() << "LastSavedDay: " << lastSavedDate->toString();
        }
    });
    timer->start(1000);
}

QString MainWindow::check_birthday_friends(const QDate& dateNow)
{
    QFile file(file_name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString readInfo = file.readAll();  // Get all data from JSON file
    file.close();  // Closing file

    QJsonArray jArr = QJsonDocument::fromJson(readInfo.toUtf8()).array();  // Convert data to UTF-8

    QJsonObject obj;
    QString dateString;
    QDate dateFromJson;
    QString peopleName;

    int counter = 0;
    for (int i = 0; i < jArr.size(); ++i)
    {
        obj = jArr[i].toObject();
        dateString = obj.value("Date").toString();
        dateFromJson = QDate::fromString(dateString, "yyyy-MM-dd");
        //qInfo() << jArr[i].toObject().value("Date").toString() << "\t" << dateNow.toString();
        if (dateFromJson.isValid() && dateFromJson == dateNow)
        {
            peopleName += obj.value("Name").toString() + "\n";
            ++counter;
        }
    }

    if (counter == 1) peopleName += " - wish your friend!";
    else peopleName += " - wish your friends!";

    return peopleName;
}

void MainWindow::send_notification(const QString &message)
{
    if (!message.isEmpty())
        traySysIcon->showMessage("Wish your friends a happy birthday!",
                                 message,
                                 QSystemTrayIcon::Information,
                                 5000);
}


void MainWindow::read_from_json(const QString& file_name_to_read)
{
    QLayoutItem* wItem;
    while ((wItem = ui->laForData->takeAt(0)) != 0) wItem->widget()->deleteLater();

    QFile jsonFileToRead(file_name_to_read);
    jsonFileToRead.open(QIODevice::ReadOnly | QIODevice::Text);  // Opening JSON file for reading data from it
    if (!jsonFileToRead.isOpen()) return;

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
    if (ui->laForData->count() >= 6) return;

    std::unique_ptr<QGridLayout> layOneUser = std::make_unique<QGridLayout>();
    std::unique_ptr<QFrame> frLayWithData = std::make_unique<QFrame>();

    QDate currentDay = QDate::currentDate();
    QDate dateFromString = QDate::fromString(dateUser, "yyyy-MM-dd");
    if (currentDay.daysTo(dateFromString) < 0) return;

    QString formattedDate = dateFromString.toString("dd.MM");  // Format date
    QString daysToBirthday = " (Days to Birthday: " + QString::number(currentDay.daysTo(dateFromString)) + ")";

    std::unique_ptr<QLabel> lblUserName = std::make_unique<QLabel>(nameUser);  // Creating new Label with User Name
    std::unique_ptr<QLabel> lblUserDate = std::make_unique<QLabel>(formattedDate + daysToBirthday);  // Creating new Label with our formatted date
                                                                                                    // and counted days to birthday

    // Label with user name formating
    QFont userNameFont = lblUserName->font();
    userNameFont.setBold(true);
    lblUserName->setFont(userNameFont);

    // Delete button
    std::unique_ptr<QPushButton> deleteButton = std::make_unique<QPushButton>();
    deleteButton->setText("Delete");
    deleteButton->setStyleSheet("background-color: red;");

    connect(deleteButton.get(), &QPushButton::clicked, this, [this, dateUser, nameUser]() { delete_from_json(dateUser, nameUser); });

    // Add to form
    layOneUser->addWidget(lblUserName.release(), 0, 0);
    layOneUser->addWidget(deleteButton.release(), 0, 1);
    layOneUser->addWidget(lblUserDate.release(), 1, 0);

    frLayWithData->setLayout(layOneUser.release());
    ui->laForData->addWidget(frLayWithData.release());
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


void MainWindow::delete_from_json(const QString& dateUser, const QString& nameUser)
{
    QFile file(file_name);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen()) return;

    QString readInfo = file.readAll();  // Get all data from JSON file
    file.close();  // Closing file

    QJsonArray jArr = QJsonDocument::fromJson(readInfo.toUtf8()).array();  // Convert data to UTF-8

    for (int i = 0; i < jArr.size(); ++i)
    {
        if (jArr[i].toObject().value("Date").toString() == dateUser &&
            jArr[i].toObject().value("Name").toString() == nameUser)
        {
            jArr.removeAt(i);
            break;
        }
    }

    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(jArr).toJson());
    file.close();

    read_from_json(file_name);
}
