#ifndef ADDDATA_H
#define ADDDATA_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <databases.h>
#include <QPainter>
namespace Ui {
class adddata;
}

class adddata : public QWidget
{
    Q_OBJECT

public:
    explicit adddata(QWidget *parent = nullptr);
    ~adddata();
    databases db;
    void paintEvent(QPaintEvent *event);

private slots:

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::adddata *ui;
};

#endif // ADDDATA_H
