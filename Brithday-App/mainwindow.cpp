#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stylehelper.h"
#include "work_with_json/jsonfilemanager.h"
#include "widgetfactory.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    traySysIcon = new QSystemTrayIcon(this);
    traySysIcon->setIcon(QIcon(":/graphics/birthday-cake.ico"));
    traySysIcon->setVisible(true);

    // Get styles from our styles.css
    this->setStyleSheet(StyleHelper::mainStyles());
    ui->laForData->setAlignment(Qt::AlignTop);

    myEvent = new MyEvent();
    myEventConfigurationForm = new MyEventConfigurationForm(this, myEvent);

    // Connect all signals with slots
    connect(traySysIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayActivated);
    connect(ui->btnAddPeople, &QPushButton::clicked, this, &MainWindow::onAddClicked);

    formLoad();
}


MainWindow::~MainWindow()
{
    delete ui;
    delete traySysIcon;
}

void MainWindow::formLoad()
{
    checkDate();
    generateBirthdayWidgets();
    QString eventsToday = checkBirthdayFriends(QDate::currentDate());
    if(!eventsToday.isEmpty()) sendMessageBox("Todays events", eventsToday);
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
    JSONFileManager jsonManager;
    QJsonArray jArr = jsonManager.readFromJsonArray();
    QJsonObject obj;
    QString dateString;
    QDate dateFromJson;
    QString peopleName;

    static int counter = 0;
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
    else if (counter == 1) peopleName.push_front("Wish your friend:\n");
    else peopleName.push_front("Wish your friends:\n");

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

void MainWindow::sendMessageBox(const QString &title, const QString &message)
{
    QMessageBox::information(this, title, message);
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

    JSONFileManager jsonManager;
    QJsonArray jArr = jsonManager.readFromJsonArray();
    QString nameUser, dateUserStr;  // Creating variable for using its in loop
    QDate dateUser;

    for (const QJsonValue &value : jArr)
    {
        if (ui->laForData->count() >= 6) return;
        QJsonObject jsonObj = value.toObject();
        nameUser = jsonObj.value("Name").toString();  // Get data from JSON with key parametr "Name"
        dateUserStr = jsonObj.value("Date").toString();
        dateUser = QDate::fromString(dateUserStr, "yyyy-MM-dd");  // Get data from JSON with key parametr "Date"

        //display label
        QWidget* lableWidget = WidgetFactory::getNewEventWidget(nameUser, dateUserStr,
            //deleteBtn actions
            [this, dateUserStr, nameUser]() {
                jsonWork.deleteFromJson(nameUser, dateUserStr);
                generateBirthdayWidgets();
            },
            //editBtn actions
            [this, dateUser, dateUserStr, nameUser](){
                myEvent->setName(nameUser);
                myEvent->setDate(dateUser);
                myEventConfigurationForm->updateInputInfo();
                jsonWork.deleteFromJson(nameUser, dateUserStr);
                onAddClicked();
            }
        );

        //display widget
        if(lableWidget){
            ui->laForData->addWidget(lableWidget);
        }
    }
}


void MainWindow::onAddClicked()
{
    if(myEventConfigurationForm->exec() == QDialog::Accepted)
    {
        jsonWork.writeToJson(myEvent->getName(), myEvent->getDate());

        generateBirthdayWidgets();

        QMessageBox::information(this, "People was added!", "Adding people to database was sucessed");
    }
}
