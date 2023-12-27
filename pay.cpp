#include "pay.h"
#include "ui_pay.h"
#include <QMessageBox>
#include <QPainter>
#include "databases.h"
pay::pay(QString tripNumber,QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::pay)
    , m_tripNumber(tripNumber)
{
    ui->setupUi(this);
     this->setWindowTitle("支付界面");
}

pay::~pay()
{
    delete ui;
}

void pay::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,414,562,QPixmap(":/weixin.jpg"));
    painter.drawPixmap(414,0,414,562,QPixmap(":/zhifubao.jpg"));
}

void pay::on_pushButton_clicked()
{
    // 打开数据库
    //db.m_db.open();
    //if(db.m_db.isOpen())
    //{
    //    QSqlQuery query(db.m_db);
    //    QString username = g_username; // 这里添加你的用户名
    //    QString tripNumber = m_tripNumber; // 这里添加你的行程号

        // 更新对应的订单状态
    //    if(query.exec(QString("UPDATE user_busticket SET 状态 = '已支付' WHERE username = '%1' AND 行程号 = '%2'")
    //                       .arg(username, tripNumber)))
    //    {
    //        db.m_db.commit();  // 提交更改
    //        qDebug() << "Order updated successfully.";
    //    }
    //    else
    //    {
    //        qDebug() << "Failed to update order: " << query.lastError();
    //   }

     //   db.m_db.close();
    //}
    QMessageBox::about(this,"提示","成功支付");
    this->close();
}

