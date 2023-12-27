#include "widget.h"
#include "inlog.h"
#include "ui_widget.h"
#include "viewwindow.h"
#include "myorder.h"
#include <QPainter>
#include "hisorder.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("用户界面");
}

Widget::~Widget()
{
    delete ui;
}

//跳转到车次查询界面
void Widget::on_btn_bus_clicked()
{
    viewwindow *w=new viewwindow();
    w->show();
    this->hide();
}

//跳转到我的订单界面
void Widget::on_btn_order_clicked()
{
    MyOrder *w=new MyOrder();
    w->show();
    this->hide();
}

//跳转到历史购票界面
void Widget::on_btn_his_clicked()
{
    hisorder *w=new hisorder();
    w->show();
    this->hide();
}

//跳转到登录界面
void Widget::on_btn_quit_clicked()
{
    inlog *w=new inlog();
    w->show();
    this->hide();
}


void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/bus.jpg"));
}
