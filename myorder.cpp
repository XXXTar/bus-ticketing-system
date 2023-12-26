#include "myorder.h"
#include "ui_myorder.h"
#include "databases.h"
#include "widget.h"
#include <QMessageBox>
#include "pay.h"
#include <QPainter>

extern QString g_username;

MyOrder::MyOrder(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MyOrder)
{
    ui->setupUi(this);
     this->setWindowTitle("我的订单");
    updatawidgetData();
}

MyOrder::~MyOrder()
{
    delete ui;
}

void MyOrder::updatawidgetData()
{
    db.m_db.close();
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db.m_db = QSqlDatabase::database("qt_sql_default_connection");
        qDebug() << "contains";
    }
    else
    {
        db.m_db = QSqlDatabase::addDatabase("QSQLITE", "qt_sql_default_connection");
        db.m_db.setDatabaseName("user.db");
    }
    db.m_db.open();
    if(db.m_db.open())
    {
        QSqlQuery query(db.m_db);
        QString username = g_username; // 这里添加你的用户名
        if(query.exec(QString("SELECT b.行程号,b.起点, b.终点, b.日期, b.发车时间, b.到达时间, b.可退票价-b.优惠幅度,b.不退票价-b.优惠幅度, u.状态 "
                               "FROM user_busticket u "
                               "JOIN busticket b ON u.行程号 = b.行程号 "
                               "WHERE u.username = '%1'").arg(username)))
        {
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);

            while(query.next())
            {
                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowCount);
                for(int i = 0; i < 10; i++)
                {
                    QTableWidgetItem *columnItem = new QTableWidgetItem(query.value(i).toString());
                    ui->tableWidget->setItem(rowCount, i, columnItem);
                }
            }

        }
        db.m_db.close();
    }
}


void MyOrder::on_btn_shuaxin_clicked()
{
    updatawidgetData();
}

//返回主界面
void MyOrder::on_btn_fanhui_clicked()
{
    Widget *w=new Widget();
    w->show();
    this->hide();
}

void MyOrder::on_btn_quxiao_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row != -1)  // 如果有选中的行
    {
        // 直接从表格中获取行程号
        QString tripNumber = ui->tableWidget->item(row, 0)->text();

        // 打开数据库
        db.m_db.open();
        if(db.m_db.open())
        {
            QSqlQuery query(db.m_db);
            QString username = g_username; // 这里添加你的用户名

            // 首先检查订单状态
            query.prepare("SELECT 状态 FROM user_busticket WHERE username = ? AND 行程号 = ?");
            query.addBindValue(username);
            query.addBindValue(tripNumber);
            if (query.exec() && query.next()) {
                QString status = query.value(0).toString();
                if (status != "未支付") {
                    QMessageBox::warning(this, "提示", "无法取消订单，请进行退票处理");
                    return;
                }
            } else {
                qDebug() << "Failed to check order status: " << query.lastError();
            }

            // 添加记录到history表中
            query.prepare("INSERT INTO history(行程号, 起点, 终点, 日期, 发车时间, 到达时间, 状态, username) "
                          "SELECT b.行程号, b.起点, b.终点, b.日期, b.发车时间, b.到达时间, '已取消', u.username "
                          "FROM busticket AS b "
                          "JOIN user_busticket AS u ON b.行程号 = u.行程号 "
                          "WHERE b.行程号 = ? AND u.username = ?");
            query.addBindValue(tripNumber);
            query.addBindValue(username);
            if (!query.exec()) {
                qDebug() << "Failed to insert into table: " << query.lastError();
            }

            // 删除对应的订单
            query.prepare("DELETE FROM user_busticket WHERE username = ? AND 行程号 = ?");
            query.addBindValue(username);
            query.addBindValue(tripNumber);
            if (!query.exec()) {
                qDebug() << "Failed to delete order: " << query.lastError();
            }

            db.m_db.close();
        }

        // 从表格中删除选中的行
        ui->tableWidget->removeRow(row);
    }
    else
    {
        QMessageBox::warning(this, "提示", "请选中行程");
        qDebug() << "No row selected.";
    }
    // 更新表格数据
    updatawidgetData();
}


void MyOrder::on_btn_zhifu_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row != -1)  // 如果有选中的行
    {
        // 直接从表格中获取行程号
        QString tripNumber = ui->tableWidget->item(row, 0)->text();

        // 弹出消息框询问用户是否要支付不可退票价格
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "支付不可退票价格", "是否支付不可退票价格？",
                                                QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // 用户选择是，订单状态变为 "不可退票"
            updateOrderStatus(tripNumber, "不可退票");
        } else {
            // 用户选择否，订单状态变为 "已支付"
            updateOrderStatus(tripNumber, "已支付");
        }

        pay * p = new pay(tripNumber);
        p->setWindowTitle("支付");
        p->show();
    }
    else
    {
        QMessageBox::warning(this, "提示", "请选中行程");
        qDebug() << "No row selected.";
    }
}


void MyOrder::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/bus.jpeg"));
}

void MyOrder::on_btn_tuipiao_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row != -1)  // 如果有选中的行
    {
        // 直接从表格中获取行程号
        QString tripNumber = ui->tableWidget->item(row, 0)->text();
        QString orderStatus = ui->tableWidget->item(row, 8)->text();

        // 打开数据库
        db.m_db.open();
        if(db.m_db.open())
        {
            QSqlQuery query(db.m_db);
            QString username = g_username; // 这里添加你的用户名

            if(orderStatus == "已支付")
            {

                // 添加记录到history表中
                query.prepare("INSERT INTO history(行程号, 起点, 终点, 日期, 发车时间, 到达时间, 状态, username) "
                              "SELECT b.行程号, b.起点, b.终点, b.日期, b.发车时间, b.到达时间, '已退款', u.username "
                              "FROM busticket AS b "
                              "JOIN user_busticket AS u ON b.行程号 = u.行程号 "
                              "WHERE b.行程号 = ? AND u.username = ?");
                query.addBindValue(tripNumber);
                query.addBindValue(username);
                if (!query.exec()) {
                    qDebug() << "Failed to insert into table: " << query.lastError();
                }
                // 删除对应的订单
                if(query.exec(QString("DELETE FROM user_busticket WHERE username = '%1' AND 行程号 = '%2'")
                                   .arg(username, tripNumber)))
                {
                    db.m_db.commit();  // 提交更改
                    qDebug() << "Order deleted successfully.";
                    QMessageBox::information(this, "退票成功", "已退还金额至支付宝/微信");
                }
                else
                {
                    qDebug() << "Failed to delete order: " << query.lastError();
                }
            }
            else if(orderStatus == "未支付")
            {
                // 取消订单
                on_btn_quxiao_clicked();
                QMessageBox::warning(this, "退票失败", "订单未支付，已为您取消订单");
            }
            else if(orderStatus == "不可退票")
            {
                QMessageBox::warning(this, "退票失败", "订单状态为不可退票，无法退票");
            }

            db.m_db.close();
        }

        // 更新表格数据
        updatawidgetData();
    }
    else
    {
        QMessageBox::warning(this, "提示", "请选中行程");
        qDebug() << "No row selected.";
    }
}

void MyOrder::updateOrderStatus(const QString& tripNumber, const QString& newStatus)
{
    // 打开数据库
    db.m_db.open();
    if(db.m_db.open())
    {
        QSqlQuery query(db.m_db);
        QString username = g_username; // 这里添加你的用户名

        // 更新订单状态
        if(query.exec(QString("UPDATE user_busticket SET 状态 = '%1' WHERE username = '%2' AND 行程号 = '%3'")
                           .arg(newStatus, username, tripNumber)))
        {
            db.m_db.commit();  // 提交更改
            qDebug() << "Order status updated successfully.";
        }
        else
        {
            qDebug() << "Failed to update order status: " << query.lastError();
        }

        db.m_db.close();
    }

    // 更新表格数据
    updatawidgetData();
}

void MyOrder::on_btn_print_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row != -1)  // 如果有选中的行
    {
        QString orderStatus = ui->tableWidget->item(row, 8)->text();

        // 打开数据库
        db.m_db.open();
        if(db.m_db.open())
        {
            QSqlQuery query(db.m_db);
            QString username = g_username; // 这里添加你的用户名

            if(orderStatus == "已支付" || orderStatus == "不可退票")
            {
                // 弹出消息框询问用户是选择个人打印还是电子车票
                QMessageBox::StandardButton reply;
                reply = QMessageBox::question(this, "打印选项", "请选择打印方式：\nYes - 个人打印\nNo - 电子车票",
                                              QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::Yes) {
                    // 用户选择个人打印
                    // 从数据库中获取用户的邮箱
                    query.exec(QString("SELECT email FROM userinfo WHERE username = '%1'").arg(username));
                    query.next();
                    QString email = query.value(0).toString();

                    QMessageBox::information(this, "个人打印", "车票已发送到邮箱：" + email);
                } else {
                    // 用户选择电子车票
                    // 从数据库中获取用户的手机号
                    query.exec(QString("SELECT phone FROM userinfo WHERE username = '%1'").arg(username));
                    query.next();
                    QString phone = query.value(0).toString();

                    QMessageBox::information(this, "电子车票", "电子车票已发送到手机：" + phone);
                }
            }
            else if(orderStatus == "未支付")
            {
                // 订单未支付
                QMessageBox::warning(this, "打印失败", "订单未支付，请先支付订单");
            }

            db.m_db.close();
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请选中行程");
        qDebug() << "No row selected.";
    }
}

