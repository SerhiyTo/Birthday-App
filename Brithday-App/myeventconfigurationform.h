#ifndef MYEVENTCONFIGURATIONFORM_H
#define MYEVENTCONFIGURATIONFORM_H

#include <QDialog>
#include "myevent.h"

namespace Ui {
class MyEventConfigurationForm;
}

class MyEventConfigurationForm : public QDialog
{
    Q_OBJECT

public:
    explicit MyEventConfigurationForm(QWidget *parent = nullptr, MyEvent *myEvent = nullptr);
    ~MyEventConfigurationForm();

private slots:
    void on_btnOk_clicked();

    void on_btnCancel_clicked();

private:
    Ui::MyEventConfigurationForm *ui;
    MyEvent *myEvent;
};

#endif // MYEVENTCONFIGURATIONFORM_H
