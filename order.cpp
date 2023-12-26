#include "adwidget.h"
#include "order.h"
#include "ui_order.h"

#include <QMessageBox>

order::order(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::order)
{
    ui->setupUi(this);
     this->setWindowTitle("所有订单");
    showAllOrders();
}

order::~order()
{
    delete ui;
}

void order::showAllOrders()
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
        if(query.exec(QString("SELECT u.username, b.行程号, b.起点, b.终点, b.日期, b.可退票价-b.优惠幅度, b.不退票价-b.优惠幅度, u.状态 "
                               "FROM user_busticket u "
                               "JOIN busticket b ON u.行程号 = b.行程号 ")))
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


void order::on_btn_back_clicked()
{
    adwidget* w = new adwidget();
    w->show();
    this->hide();
}


void order::on_btn_print_clicked()
{

    int row = ui->tableWidget->currentRow();
    if(row != -1)  // 如果有选中的行
    {
        // 直接从表格中获取行程号和其他信息
        QString username = ui->tableWidget->item(row, 0)->text();
        QString tripNumber = ui->tableWidget->item(row, 1)->text();
        QString startPoint = ui->tableWidget->item(row, 2)->text();
        QString endPoint = ui->tableWidget->item(row, 3)->text();
        QString date = ui->tableWidget->item(row, 4)->text();
        QString refundablePrice = ui->tableWidget->item(row, 5)->text();
        QString nonRefundablePrice = ui->tableWidget->item(row, 6)->text();
        QString orderStatus = ui->tableWidget->item(row, 7)->text();

        // 弹出消息框询问用户是否要打印车票
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "打印车票", "是否打印当前车票？",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // 用户选择是，弹出一个消息框显示选中的车票信息，并提示打印成功
            QString ticketInfo = QString("用户名：%1\n行程号：%2\n起点：%3\n终点：%4\n日期：%5\n可退票价：%6\n不退票价：%7\n订单状态：%8")
                                 .arg(username, tripNumber, startPoint, endPoint, date, refundablePrice, nonRefundablePrice, orderStatus);
            QMessageBox::information(this, "车票信息", ticketInfo);
            QMessageBox::information(this, "打印成功", "车票已成功打印！");
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请选中行程");
        qDebug() << "No row selected.";
    }
}


void order::on_btn_month_clicked()
{
    // 获取当前的年份和月份
    QDate currentDate = QDate::currentDate();
    int year = currentDate.year();
    int month = currentDate.month();

    // 打开数据库
    db.m_db.open();
    if(db.m_db.open())
    {
        QSqlQuery query(db.m_db);

        // 执行SQL查询以获取本月售出的所有车票
        QString sqlQuery = QString("SELECT u.username, b.行程号, b.起点, b.终点, b.日期, b.可退票价-b.优惠幅度, b.不退票价-b.优惠幅度, u.状态 "
                                   "FROM user_busticket u "
                                   "JOIN busticket b ON u.行程号 = b.行程号 "
                                   "WHERE strftime('%Y', b.日期) = '%1' AND strftime('%m', b.日期) = '%2'")
                               .arg(QString::number(year), QString::number(month));
        if(query.exec(sqlQuery))
        {
            // 清空表格
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);

            double totalAmount = 0;

            // 遍历查询结果
            while(query.next())
            {
                // 根据订单状态计算总金额
                QString orderStatus = query.value(7).toString();
                if(orderStatus != "未支付") {
                double price = (orderStatus == "不可退票") ? query.value(6).toDouble() : query.value(5).toDouble();
                                                                                                 totalAmount += price;
                }
                // 将车票信息添加到表格中
                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowCount);
                for(int i = 0; i < 8; i++)
                {
                    QTableWidgetItem *columnItem = new QTableWidgetItem(query.value(i).toString());
                    ui->tableWidget->setItem(rowCount, i, columnItem);
                }
            }

            // 显示总金额
            QMessageBox::information(this, "月度统计", QString("本月售出的总金额：%1").arg(totalAmount));
        }
        else
        {
            qDebug() << "Failed to execute query: " << query.lastError();
        }

        db.m_db.close();
    }
}


void order::on_btn_season_clicked()
{
    // 获取当前的年份和季度
    QDate currentDate = QDate::currentDate();
    int year = currentDate.year();
    int month = currentDate.month();
    int quarter = (month - 1) / 3 + 1;  // 计算季度

    // 计算季度的开始和结束月份
    int startMonth = (quarter - 1) * 3 + 1;
    int endMonth = startMonth + 2;

    // 打开数据库
    db.m_db.open();
    if(db.m_db.open())
    {
        QSqlQuery query(db.m_db);

        // 执行SQL查询以获取本季度售出的所有车票
        QString sqlQuery = QString("SELECT u.username, b.行程号, b.起点, b.终点, b.日期, b.可退票价-b.优惠幅度, b.不退票价-b.优惠幅度, u.状态 "
                                   "FROM user_busticket u "
                                   "JOIN busticket b ON u.行程号 = b.行程号 "
                                   "WHERE strftime('%Y', b.日期) = '%1' AND strftime('%m', b.日期) >= '%2' AND strftime('%m', b.日期) <= '%3'")
                               .arg(QString::number(year), QString::number(startMonth), QString::number(endMonth));
        if(query.exec(sqlQuery))
        {
            // 清空表格
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);

            double totalAmount = 0;

            // 遍历查询结果
            while(query.next())
            {
                // 根据订单状态计算总金额
                QString orderStatus = query.value(7).toString();
                if(orderStatus != "未支付") {
                    double price = (orderStatus == "不可退票") ? query.value(6).toDouble() : query.value(5).toDouble();
                                                                                                     totalAmount += price;
                }
                // 将车票信息添加到表格中
                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowCount);
                for(int i = 0; i < 8; i++)
                {
                    QTableWidgetItem *columnItem = new QTableWidgetItem(query.value(i).toString());
                    ui->tableWidget->setItem(rowCount, i, columnItem);
                }
            }

            // 显示总金额
            QMessageBox::information(this, "季度统计", QString("本季度售出的总金额：%1").arg(totalAmount));
        }
        else
        {
            qDebug() << "Failed to execute query: " << query.lastError();
        }

        db.m_db.close();
    }
}


void order::on_btn_year_clicked()
{
    // 获取当前的年份
    QDate currentDate = QDate::currentDate();
    int year = currentDate.year();

    // 打开数据库
    db.m_db.open();
    if(db.m_db.open())
    {
        QSqlQuery query(db.m_db);

        // 执行SQL查询以获取本年度售出的所有车票
        QString sqlQuery = QString("SELECT u.username, b.行程号, b.起点, b.终点, b.日期, b.可退票价-b.优惠幅度, b.不退票价-b.优惠幅度, u.状态 "
                                   "FROM user_busticket u "
                                   "JOIN busticket b ON u.行程号 = b.行程号 "
                                   "WHERE strftime('%Y', b.日期) = '%1'")
                               .arg(QString::number(year));
        if(query.exec(sqlQuery))
        {
            // 清空表格
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);

            double totalAmount = 0;

            // 遍历查询结果
            while(query.next())
            {
                // 根据订单状态计算总金额
                QString orderStatus = query.value(7).toString();
                                if(orderStatus != "未支付") {
                double price = (orderStatus == "不可退票") ? query.value(6).toDouble() : query.value(5).toDouble();
                                                                                                 totalAmount += price;
                }
                // 将车票信息添加到表格中
                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowCount);
                for(int i = 0; i < 8; i++)
                {
                    QTableWidgetItem *columnItem = new QTableWidgetItem(query.value(i).toString());
                    ui->tableWidget->setItem(rowCount, i, columnItem);
                }
            }

            // 显示总金额
            QMessageBox::information(this, "年度统计", QString("本年度售出的总金额：%1").arg(totalAmount));
        }
        else
        {
            qDebug() << "Failed to execute query: " << query.lastError();
        }

        db.m_db.close();
    }
}


void order::on_btn_find_clicked()
{
    QString text = ui->lineEdit->text();        // 获取输入框内容
    QList<QTableWidgetItem*> findItems = ui->tableWidget->findItems(text,Qt::MatchContains);

    int nRow = ui->tableWidget->rowCount();
    bool bRowHidden = true;
    for(int i=0;i<nRow; i++)
    {
        bRowHidden = true;
        foreach(QTableWidgetItem* item,findItems)
        {
            if(NULL == item) continue;
            if(ui->tableWidget->row(item) == i)
            {
                bRowHidden = false;                         // 对含输入内容的行进行设置不隐藏
                break;
            }
        }
        ui->tableWidget->setRowHidden(i,bRowHidden);       // 对行进行隐藏
    }
}


void order::on_btn_shuaxin_clicked()
{
    showAllOrders();
}

