#ifndef PAY_H
#define PAY_H

#include <QWidget>
#include <QPainter>
#include "databases.h"
namespace Ui {
class pay;
}

class pay : public QWidget
{
    Q_OBJECT

public:
    explicit pay(QString tripNumber, QWidget *parent = nullptr);
    ~pay();
    void paintEvent(QPaintEvent *event);
private slots:
    void on_pushButton_clicked();

private:
    Ui::pay *ui;
    databases db;
    QString m_tripNumber;  // 声明用来存储行程号的成员变量
};

#endif // PAY_H
