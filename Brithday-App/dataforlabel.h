#ifndef DATAFORLABEL_H
#define DATAFORLABEL_H

#include <QObject>
#include <QPixmap>
#include <QDate>

class DataForLabel : public QObject
{
    Q_OBJECT
public:
    explicit DataForLabel(QObject *parent = nullptr);
    explicit DataForLabel(QObject *parent = nullptr, QString nameUser = " ", QDate dateUser = QDate::currentDate());

private:
    QPixmap photoUser;
    QString nameUser;
    QDate dateUser;

signals:

};

#endif // DATAFORLABEL_H
