#include "adwidget.h"
#include "inlog.h"
#include "order.h"
#include "preferential.h"
#include "ui_adwidget.h"
#include "viewwindow.h"
adwidget::adwidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::adwidget)
{
    ui->setupUi(this);
     this->setWindowTitle("管理员界面");
}

adwidget::~adwidget()
{
    delete ui;
}

void adwidget::on_btn_bus_clicked()
{
    viewwindow *w=new viewwindow();
    w->show();
    this->hide();
}


void adwidget::on_btn_back_clicked()
{
    inlog *w=new inlog();
    w->show();
    this->hide();
}


void adwidget::on_btn_order_clicked()
{
    order *w = new order();
    w->show();
    this->hide();
}


void adwidget::on_btn_preferential_clicked()
{
    preferential * add = new preferential();
    add->setWindowTitle("数据填写");
    add->show(); // 使用 exec 函数，而不是 show 函数
}

