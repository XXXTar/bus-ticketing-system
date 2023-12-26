#ifndef MYORDER_H
#define MYORDER_H

#include <QWidget>
#include <databases.h>
namespace Ui {
class MyOrder;
}

class MyOrder : public QWidget
{
    Q_OBJECT

public:
    explicit MyOrder(QWidget *parent = nullptr);
    ~MyOrder();
    databases db;
    void updatawidgetData();
    void paintEvent(QPaintEvent *event);
    void updateOrderStatus(const QString& tripNumber, const QString& newStatus);
private slots:
    void on_btn_shuaxin_clicked();

    void on_btn_fanhui_clicked();

    void on_btn_quxiao_clicked();

    void on_btn_zhifu_clicked();

    void on_btn_tuipiao_clicked();
    void on_btn_print_clicked();

private:
    Ui::MyOrder *ui;
};

#endif // MYORDER_H
