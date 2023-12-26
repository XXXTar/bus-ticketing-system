#include "regist.h"
#include "inlog.h"
#include "ui_regist.h"
#include <QMessageBox>
#include <QString.h>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QDebug>
#include <databases.h>
#include <QPainter>
regist::regist(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::regist)
{
    ui->setupUi(this);
    this->setWindowTitle("用户注册");
     ui->let_passwd1->setEchoMode(QLineEdit::Password);
     ui->let_passwd2->setEchoMode(QLineEdit::Password);
     ui->let_passwd1->setMaxLength(6);
     ui->let_passwd2->setMaxLength(6);
     ui->let_passwd1->setPlaceholderText("请输入六位密码");
     ui->let_passwd2->setPlaceholderText("请输入六位密码");
     ui->let_email->setPlaceholderText("请输入邮箱");
     ui->let_phone->setPlaceholderText("请输入十一位电话号");
     qDebug() << "regist object created";

}

regist::~regist()
{
    delete ui;
}

//跳转到登录界面
void regist::on_btn_reg_clicked()
{
    QString str1=ui->let_passwd1->text();
    QString str2=ui->let_passwd2->text();
    int y = QString::compare(str1,str2,Qt::CaseSensitive);
    if(y)
    {
        QMessageBox::about(this,"错误","两次输入不同");
        return;
    }
    if((str1.length()!=6)&&(str2.length()!=6))
    {
        QMessageBox::about(this,"错误","密码需要六个字符");
        return;
    }

    QString username = ui->let_name->text();
    if (username.isEmpty())
    {
        QMessageBox::about(this,"警告","用户名不能为空");
        return;
    }
    QString str3=ui->let_email->text();
    if (!str3.contains("@") || !str3.contains(".com"))
    {
        QMessageBox::about(this,"警告","邮箱必须包含@和.com");
        return;
    }
    QString str4=ui->let_phone->text();
    if(str4.length()!=11 || str4.toLongLong() == 0)
    {
        QMessageBox::about(this,"警告","手机号位数不符合");
        return;
    }
    //创建数据库
    int flag = 1 ;
    bool ok = db.m_db.open();
    if(ok)
    {
        QSqlQuery query("select username from userInfo",db.m_db);
        while(query.next())
        {
            if(ui->let_name->text() == query.value(0).toString())
            {
                flag = 0;
                break;
            }
        }
        if(flag)
        {
            QSqlQuery query;
            query.prepare("INSERT INTO userInfo(username,password,email,phone,role) VALUES (:username,:password,:email,:phone,:role)");
            query.bindValue(":username",ui->let_name->text());
            query.bindValue(":password",ui->let_passwd2->text());
            query.bindValue(":email",ui->let_email->text());
            query.bindValue(":phone",ui->let_phone->text());
            query.bindValue(":role", "user");  // 所有新用户默认都是普通用户
            query.exec();
            QMessageBox::about(this,"提示","注册用户成功!");
            this->hide();
            inlog *w=new inlog();
            w->show();
        }
        else{
            QMessageBox::about(this,"提示","用户名已被使用");
        }
    }
    else
    {
        qDebug()<<"数据库连接失败!";
        exit(-1);
    }
}

//跳转到登录界面
void regist::on_btn_quit_clicked()
{
    inlog *w=new inlog();
    w->show();
    this->hide();
}


void regist::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/bus.jpeg"));
}
