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

    generate_birthday_widgets();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete traySysIcon;
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
            //read_from_json(file_name);
            generate_birthday_widgets();
            send_notification(check_birthday_friends(*lastSavedDate));
            //qInfo() << "LastSavedDay: " << lastSavedDate->toString();
        }
    });
    timer->start(1000);
}

QString MainWindow::check_birthday_friends(const QDate& dateNow)
{
    QJsonArray jArr = jsonWork.get_json_array();
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


void MainWindow::generate_birthday_widgets()
{
    QLayoutItem* wItem;
    while ((wItem = ui->laForData->takeAt(0)) != 0) wItem->widget()->deleteLater();

    QJsonArray jArr = jsonWork.get_json_array();

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

    connect(deleteButton.get(), &QPushButton::clicked, this, [this, dateUser, nameUser]() { jsonWork.delete_from_json(nameUser, dateUser); });

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
    // Get info
    QString event_name = ui->lnNameInput->text();
    QDate event_date = ui->datInput->date();
    jsonWork.write_to_json(event_name, event_date);

    QMessageBox::information(this, "People was added!", "Adding people to databasw was sucessed");

    ui->lnNameInput->clear();  // Clear the form
    ui->datInput->setDate(QDate::currentDate());  // Set current date by deffault in our QDate
    ui->frMessageOpen->hide();  // Hide the form

    // generate_birthday_widgets();
}
