#include "inlog.h"
#include "widget.h"
#include "regist.h"
#include "ui_inlog.h"
#include "adwidget.h"
#include <QString.h>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <databases.h>
#include <QPainter>
extern QString g_username;
bool isroot = false;        // 默认不是管理员
inlog::inlog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::inlog)
{
    ui->setupUi(this);
    this->setWindowTitle("用户登录");
    ui->let_passwd->setEchoMode(QLineEdit::Password);
    ui->let_passwd->setMaxLength(6);
    ui->let_passwd->setPlaceholderText("请输入六位密码");
//创建数据库
}

inlog::~inlog()
{
    delete ui;
}
//跳转到主界面
void inlog::on_btn_inlog_clicked()
{

    QString username = ui->let_name->text();
    if (username.isEmpty())
    {
        QMessageBox::about(this,"提示","用户名不能为空");
        return;
    }
    QString str=ui->let_passwd->text();
    if(str.length()<6)
    {
        QMessageBox::about(this,"提示","请输入六位密码");
        return;
    }
    int open_falg=1;
    bool ok = db.m_db.open();
    if(ok)
    {
        QSqlQuery query("select username, password, role from userInfo",db.m_db);
        while(query.next())
        {
            QString acc=query.value(0).toString();
            QString pass=query.value(1).toString();
            QString role=query.value(2).toString();
            if(ui->let_name->text()==acc&&ui->let_passwd->text()==pass)
            {
                g_username = acc;  // 将用户名保存到全局变量
                if (role == "admin") {
                    isroot = true;
                    adwidget *w=new adwidget();
                    w->show();
                } else {
                    isroot = false;
                    Widget *w=new Widget();
                    w->show();
                }
                this->hide();
                db.m_db.close();
                open_falg = 0;
                break;
            }
        }
        if(open_falg)QMessageBox::about(this,"提示","账号或密码错误");
    }
    else
    {
        qDebug()<<"数据库连接失败!";
    }
}

//跳转到注册界面
void inlog::on_btn_reg_clicked()
{
    regist *w=new regist();
    w->show();
    this->hide();
}

//退出
void inlog::on_btn_quit_clicked()
{
    this->close();
}



//显示密码
void inlog::on_checkBox_clicked(bool checked)
{
    if(checked)
    {
        //显示Normal, NoEcho, Password, PasswordEchoOnEdit
        ui->let_passwd->setEchoMode(QLineEdit::Normal);
    }
    else
    {
        //密文
        ui->let_passwd->setEchoMode(QLineEdit::Password);
    }
}


void inlog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/bus.jpeg"));
}
