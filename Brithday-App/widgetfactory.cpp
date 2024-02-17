#include "widgetfactory.h"

WidgetFactory::WidgetFactory()
{

}

QWidget *WidgetFactory::getNewEventWidget(const QString &nameUser, const QString &dateUser, std::function<void()> deleteBtnActions, std::function<void()> editBtnActions)
{
    std::unique_ptr<QGridLayout> layOneUser = std::make_unique<QGridLayout>();
    std::unique_ptr<QFrame> frLayWithData = std::make_unique<QFrame>();

    QDate currentDay = QDate::currentDate();
    QDate dateFromString = QDate::fromString(dateUser, "yyyy-MM-dd");

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
    deleteButton->setMinimumSize(80, 20);
    deleteButton->setMaximumSize(81, 20);
    deleteButton->setStyleSheet(StyleHelper::listStyles());

    QObject::connect(deleteButton.get(), &QPushButton::clicked, [deleteBtnActions]() {
        deleteBtnActions();
    });

    //Edit button
    std::unique_ptr<QPushButton> editButton = std::make_unique<QPushButton>();
    editButton->setText("Edit");
    editButton->setMinimumSize(80, 20);
    editButton->setMaximumSize(81, 20);
    editButton->setStyleSheet(StyleHelper::listStyles());

    QObject::connect(editButton.get(), &QPushButton::clicked, [editBtnActions]() {
        editBtnActions();
    });

    // Add to layout
    layOneUser->addWidget(lblUserName.release(), 0, 0);
    layOneUser->addWidget(deleteButton.release(), 0, 1);
    layOneUser->addWidget(lblUserDate.release(), 1, 0);
    layOneUser->addWidget(editButton.release(), 1, 1);

    frLayWithData->setLayout(layOneUser.release());

    return frLayWithData.release();
}
