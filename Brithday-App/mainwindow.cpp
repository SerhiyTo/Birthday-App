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

    // Get styles from our styles.css
    QFile file(":/styles/main-styles.css");
    file.open(QFile::ReadOnly);
    this->setStyleSheet(file.readAll());
    ui->laForData->setAlignment(Qt::AlignTop);

    ui->datInput->setDate(QDate::currentDate());
    ui->frBackgroundMessage->hide();

    checkDate();
    generateBirthdayWidgets();

    // Connect all signals with slots
    connect(traySysIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayActivated);
    connect(ui->btnAddPeople, &QPushButton::clicked, this, &MainWindow::onAddClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &MainWindow::onCancelClicked);
    connect(ui->btnOk, &QPushButton::clicked, this, &MainWindow::onOkClicked);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete traySysIcon;
}


void MainWindow::checkDate()
{
    int delay = 360000;
    QDate lastSavedDate = QDate::currentDate();
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this, &lastSavedDate](){
        if (lastSavedDate.daysTo(QDate::currentDate()) != 0) {
            lastSavedDate = QDate::currentDate();
            generateBirthdayWidgets();
            sendNotification(checkBirthdayFriends(lastSavedDate));
        }
    });
    timer->start(delay);
}

QString MainWindow::checkBirthdayFriends(const QDate& dateNow)
{
    QJsonArray jArr = jsonWork.get_json_array();
    QJsonObject obj;
    QString dateString;
    QDate dateFromJson;
    QString peopleName;

    int counter = 0;
    for (const QJsonValue& value : jArr)
    {
        obj = value.toObject();
        dateString = obj.value("Date").toString();
        dateFromJson = QDate::fromString(dateString, "yyyy-MM-dd");

        if (dateFromJson.isValid() && dateFromJson == dateNow)
        {
            peopleName += obj.value("Name").toString() + '\n';
            ++counter;
        }
    }

    if (counter == 0) peopleName = "";
    else if (counter == 1) peopleName.push_front("Wish your friend - ");
    else peopleName.push_front("Wish your friends - ");

    return peopleName;
}

void MainWindow::sendNotification(const QString &message)
{
    if (!message.isEmpty())
    {
        traySysIcon->showMessage("Wish your friends a happy birthday!",
                                 message,
                                 QSystemTrayIcon::Information,
                                 5000);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (isVisible())
    {
        event->ignore();  // Ignore closing the app
        hide();  // Hide our main widget
        traySysIcon->show();
    }
    else event->accept();  // Close the app if window is not visiable
}

void MainWindow::trayActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::Context)  // If our request was right click (request == calling context menu)
    {
        QMenu* traySysMenu = new QMenu(this);  // We create new menu
        QAction* exitAction = traySysMenu->addAction("Exit");  // Create new action for closing the app
        connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);  // Connect it
        traySysMenu->popup(traySysIcon->geometry().center());
    }
    else if (reason == QSystemTrayIcon::Trigger) // If our request was left clicked (request == system tray entry)
    {
        show();  // Show our application (Main Window)
    }
}


void MainWindow::generateBirthdayWidgets()
{
    // Delete existing tabs
    QLayoutItem* wItem;
    while ((wItem = ui->laForData->takeAt(0)) != 0) wItem->widget()->deleteLater();

    QJsonArray jArr = jsonWork.get_json_array();
    QString dateUser, nameUser;  // Creating variable for using its in loop

    for (const QJsonValue &value : jArr)
    {
        QJsonObject jsonObj = value.toObject();
        dateUser = jsonObj.value("Date").toString();  // Get data from JSON with key parametr "Date"
        nameUser = jsonObj.value("Name").toString();  // Get data from JSON with key parametr "Name"
        generateLabel(dateUser, nameUser);  // Calling function "generate label" for display current data in label
    }
}


void MainWindow::generateLabel(const QString& dateUser, const QString& nameUser)
{
    if (ui->laForData->count() >= 6) return; // Don't add new tabs, if count >=x

    std::unique_ptr<QGridLayout> layOneUser = std::make_unique<QGridLayout>();
    std::unique_ptr<QFrame> frLayWithData = std::make_unique<QFrame>();

    QDate currentDay = QDate::currentDate();
    QDate dateFromString = QDate::fromString(dateUser, "yyyy-MM-dd");
    if (currentDay.daysTo(dateFromString) < 0) return; // Don't add past events

    QString formattedDate = dateFromString.toString("dd.MM");  // Format date
    QString daysToBirthday = " (Days to Birthday: " + QString::number(currentDay.daysTo(dateFromString)) + ")";

    std::unique_ptr<QLabel> lblUserName = std::make_unique<QLabel>(nameUser);  // Creating new Label with User Name
    std::unique_ptr<QLabel> lblUserDate = std::make_unique<QLabel>(formattedDate + daysToBirthday);  // Creating new Label with our formatted date
                                                                                                    // and counted days to birthday

    // Label with user name formating
    QFont userNameFont = lblUserName->font();
    userNameFont.setBold(true);
    lblUserName->setFont(userNameFont);

    QFile file(":/styles/list-styles.css");
    file.open(QFile::ReadOnly);

    // Delete button
    std::unique_ptr<QPushButton> deleteButton = std::make_unique<QPushButton>();
    deleteButton->setText("Delete");
    deleteButton->setStyleSheet(file.readAll());

    connect(deleteButton.get(), &QPushButton::clicked, this, [this, dateUser, nameUser]() {
        jsonWork.delete_from_json(nameUser, dateUser);
        generateBirthdayWidgets();
    });

    // Add to form
    layOneUser->addWidget(lblUserName.release(), 0, 0);
    layOneUser->addWidget(deleteButton.release(), 0, 1);
    layOneUser->addWidget(lblUserDate.release(), 1, 0);

    frLayWithData->setLayout(layOneUser.release());
    ui->laForData->addWidget(frLayWithData.release());
}


void MainWindow::onAddClicked()
{
    QFile file(":/styles/input-styles.css");
    file.open(QFile::ReadOnly);
    ui->frBackgroundMessage->show();  // Showing message box
    ui->frBackgroundMessage->setStyleSheet(file.readAll());
}


void MainWindow::onCancelClicked()
{
    // Clear text boxes and close tab
    ui->lnNameInput->clear();
    ui->datInput->setDate(QDate::currentDate());
    ui->frBackgroundMessage->hide();
}


void MainWindow::onOkClicked()
{
    // Get info
    QString event_name = ui->lnNameInput->text();
    QDate event_date = ui->datInput->date();
    jsonWork.write_to_json(event_name, event_date);

    QMessageBox::information(this, "People was added!", "Adding people to database was sucessed");

    // Return to default
    onCancelClicked();

    generateBirthdayWidgets();
}
