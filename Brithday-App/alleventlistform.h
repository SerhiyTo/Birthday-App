#ifndef ALLEVENTLISTFORM_H
#define ALLEVENTLISTFORM_H

#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>

namespace Ui {
class AllEventListForm;
}

class AllEventListForm : public QDialog
{
    Q_OBJECT

public:
    explicit AllEventListForm(QWidget *parent = nullptr);
    ~AllEventListForm();
    QLayout* getEventContainerLayout();

private:
    Ui::AllEventListForm *ui;
    QVBoxLayout *EventListContainerLayout;
};

#endif // ALLEVENTLISTFORM_H
