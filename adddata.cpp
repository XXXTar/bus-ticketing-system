#include "adddata.h"
#include "ui_adddata.h"
#include <QDebug>
#include <viewwindow.h>
#include <QPainter>
#include <QMessageBox>
adddata::adddata(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::adddata)
{
    ui->setupUi(this);
     this->setWindowTitle("车次添加");
    //ui->lineEdit->setText("默认值");
    ui->lineEdit->setText("00000000");
    ui->lineEdit_2->setText("天津");
    ui->lineEdit_3->setText("石家庄");
    ui->lineEdit_4->setText("2024-01-16");
    ui->lineEdit_5->setText("13:30");
    ui->lineEdit_6->setText("14:30");
    ui->lineEdit_7->setText("10");
    ui->lineEdit_8->setText("50.0");
    ui->lineEdit_9->setText("40.0");
    ui->lineEdit_10->setText("0.0");
}

adddata::~adddata()
{
    delete ui;
}

void adddata::on_pushButton_2_clicked()
{
    this->close();
}


void adddata::on_pushButton_clicked()
{
    QString str1 = ui->lineEdit->text();
    QString str2 = ui->lineEdit_2->text();
    QString str3 = ui->lineEdit_3->text();
    QString str4 = ui->lineEdit_4->text();
    QString str5 = ui->lineEdit_5->text();
    QString str6 = ui->lineEdit_6->text();
    QString str7 = ui->lineEdit_7->text();
    QString str8 = ui->lineEdit_8->text();
    QString str9 = ui->lineEdit_9->text();
    QString str10 = ui->lineEdit_10->text();


    // Add the condition check here
    if(str8.toFloat() <= str9.toFloat()){
        QMessageBox::warning(this, "输入错误", "可退票价格需要高于不可退票价格");
        return;
    }
        // 获取输入的信息
    db.m_db.close();
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db.m_db = QSqlDatabase::database("qt_sql_default_connection");
        qDebug() << "contains";
    }
    else
    {
        db.m_db = QSqlDatabase::addDatabase(
            "QSQLITE");
        db.m_db.setDatabaseName("user.db");
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认", "是否要添加该行程？",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    if(db.m_db.open())
    {
        {
           QSqlQuery query(db.m_db);
            query.prepare("insert into busticket (行程号,起点,终点,日期,发车时间,到达时间,剩余座位,可退票价,不退票价,优惠幅度)"
                      "values(?,?,?,?,?,?,?,?,?,?)");
            query.addBindValue(str1);
            query.addBindValue(str2);
            query.addBindValue(str3);
            query.addBindValue(str4);
            query.addBindValue(str5);
            query.addBindValue(str6);
            query.addBindValue(str7);
            query.addBindValue(str8);
            query.addBindValue(str9);
            query.addBindValue(str10);
            query.exec();
            qDebug() << "Insert executed";
            QMessageBox::about(this,"提示","成功增加行程");
        }
        db.m_db.close();
    }
    else{
        qDebug() << "db.m_db not open";
    }
    this->close();
}

void adddata::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/bus.jpg"));
}
