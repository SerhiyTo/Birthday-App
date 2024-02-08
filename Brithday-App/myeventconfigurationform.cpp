#include "myeventconfigurationform.h"
#include "ui_myeventconfigurationform.h"

MyEventConfigurationForm::MyEventConfigurationForm(QWidget *parent, MyEvent *myEvent) :
    QDialog(parent),
    ui(new Ui::MyEventConfigurationForm),
    myEvent(myEvent)
{
    ui->setupUi(this);

    ui->lnNameInput->setText(myEvent->getName());
    ui->datInput->setDate(myEvent->getDate());
}

MyEventConfigurationForm::~MyEventConfigurationForm()
{
    delete ui;
}

void MyEventConfigurationForm::on_btnOk_clicked()
{
    myEvent->setName(ui->lnNameInput->text());
    myEvent->setDate(ui->datInput->date());

    accept();
}


void MyEventConfigurationForm::on_btnCancel_clicked()
{
    reject();
}

