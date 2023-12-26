#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

extern bool isroot ;        // 管理员权限全局变量声明

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void paintEvent(QPaintEvent *event);
private slots:
    void on_btn_bus_clicked();

    void on_btn_order_clicked();

    void on_btn_his_clicked();

    void on_btn_quit_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
