#include "alleventlistform.h"
#include "ui_alleventlistform.h"

AllEventListForm::AllEventListForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AllEventListForm)
{
    ui->setupUi(this);
    this->EventListContainerLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
}

AllEventListForm::~AllEventListForm()
{
    delete ui;
}

QLayout *AllEventListForm::getEventContainerLayout()
{
    return ui->scrollAreaWidgetContents->layout();
}
