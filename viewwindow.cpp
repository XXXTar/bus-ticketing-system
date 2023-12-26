#include "viewwindow.h"
#include "ui_viewwindow.h"
#include "widget.h"
#include "databases.h"
#include "adwidget.h"
#include <adddata.h>
#include <QMessageBox>
#include <QPainter>
extern bool isroot;
viewwindow::viewwindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::viewwindow)
{
    ui->setupUi(this);
     this->setWindowTitle("车次表");
    // 非管理员不可对数据进行操作
    if(!isroot){
        ui->btn_add->setEnabled(false);
        ui->btn_delete->setEnabled(false);
        ui->btn_modify->setEnabled(false);
    }
    updataTablewidgetData();
}


viewwindow::~viewwindow()
{
    delete ui;
}

void viewwindow::on_btn_quit_clicked()
{
    if(isroot){
    adwidget *w=new adwidget();
    w->show();
    this->hide();
    }
    else{
        Widget *w=new Widget();
        w->show();
        this->hide();
    }
}

void viewwindow::updataTablewidgetData(){
    db.m_db.close();
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db.m_db = QSqlDatabase::database("qt_sql_default_connection");

        qDebug() << "contains";
    }
    else
    {
        db.m_db = QSqlDatabase::addDatabase(
            "QSQLITE", "qt_sql_default_connection");

        db.m_db.setDatabaseName("user.db");
    }
    db.m_db.open();
    if(db.m_db.open())
    {
        QSqlQuery query(db.m_db);
        if(query.exec("select * from busticket"))
        {
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);

            while(query.next())
            {
                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowCount);

                QTableWidgetItem *columnItem0 = new QTableWidgetItem(
                    query.value(0).toString());
                QTableWidgetItem *columnItem1 = new QTableWidgetItem(
                    query.value(1).toString());
                QTableWidgetItem *columnItem2 = new QTableWidgetItem(
                    query.value(2).toString());
                QTableWidgetItem *columnItem3 = new QTableWidgetItem(
                    query.value(3).toString());
                QTableWidgetItem *columnItem4 = new QTableWidgetItem(
                    query.value(4).toString());
                QTableWidgetItem *columnItem5 = new QTableWidgetItem(
                    query.value(5).toString());
                QTableWidgetItem *columnItem6 = new QTableWidgetItem(
                    query.value(6).toString());
                QTableWidgetItem *columnItem7 = new QTableWidgetItem(
                    query.value(7).toString());
                QTableWidgetItem *columnItem8 = new QTableWidgetItem(
                    query.value(8).toString());
                QTableWidgetItem *columnItem9 = new QTableWidgetItem(
                    query.value(9).toString());

                ui->tableWidget->setItem(rowCount,0, columnItem0);
                ui->tableWidget->setItem(rowCount,1, columnItem1);
                ui->tableWidget->setItem(rowCount,2, columnItem2);
                ui->tableWidget->setItem(rowCount,3, columnItem3);
                ui->tableWidget->setItem(rowCount,4, columnItem4);
                ui->tableWidget->setItem(rowCount,5, columnItem5);
                ui->tableWidget->setItem(rowCount,6, columnItem6);
                ui->tableWidget->setItem(rowCount,7, columnItem7);
                ui->tableWidget->setItem(rowCount,8, columnItem8);
                ui->tableWidget->setItem(rowCount,9, columnItem9);
            }
        }
        db.m_db.close();
    }
}

void viewwindow::on_btn_refresh_clicked()
{
    updataTablewidgetData();
}


void viewwindow::on_btn_find_clicked()
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


void viewwindow::on_btn_modify_clicked()
{
    QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();
    int count=items.count();
    int m_row;
    for(int i=0;i<count;i++)
    {
        m_row=ui->tableWidget->row(items.at(i));//获取选中的行
        qDebug() << m_row;
    }
    db.m_db.close();
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db.m_db = QSqlDatabase::database("qt_sql_default_connection");
        qDebug() << "contains";
    }
    else
    {
        db.m_db = QSqlDatabase::addDatabase( "QSQLITE", "qt_sql_default_connection");
        db.m_db.setDatabaseName("user.db");
    }
    db.m_db.open();
    if(db.m_db.open())
    {
        QString s0 = ui->tableWidget->item(m_row,0)->text();
        QString s1 = ui->tableWidget->item(m_row, 1)->text();
        QString s2 = ui->tableWidget->item(m_row, 2)->text();
        QString s3 = ui->tableWidget->item(m_row, 3)->text();
        QString s4 = ui->tableWidget->item(m_row, 4)->text();
        QString s5 = ui->tableWidget->item(m_row, 5)->text();
        QString s6 = ui->tableWidget->item(m_row, 6)->text();
        QString s7 = ui->tableWidget->item(m_row, 7)->text();
        QString s8 = ui->tableWidget->item(m_row, 8)->text();
        QString s9 = ui->tableWidget->item(m_row, 9)->text();

            //   获取表单中所选行内容
        qDebug() << s0<<" "<<s1<<" "<<s2<<" "<<s3<<" "<<s4;
        QSqlQuery query(db.m_db);
        query.prepare("update busticket set 起点=?,终点=?,日期=?,发车时间=?,到达时间=?,剩余座位=?,可退票价=?,不退票价=?,优惠幅度=? where 行程号=?");
        query.addBindValue(s1);
        query.addBindValue(s2);
        query.addBindValue(s3);
        query.addBindValue(s4);
        query.addBindValue(s5);
        query.addBindValue(s6);
        query.addBindValue(s7);
        query.addBindValue(s8);
        query.addBindValue(s9);
        query.addBindValue(s0);
        query.exec();
            // 数据写入数据库
        db.m_db.close();
    }

    updataTablewidgetData();
}


void viewwindow::on_btn_delete_clicked()
{
    QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();
    int count=items.count();
    int m_row;
    for(int i=0;i<count;i++)
    {
        m_row=ui->tableWidget->row(items.at(i));
    }
    //获取选中的行
    db.m_db.close();
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db.m_db = QSqlDatabase::database("qt_sql_default_connection");
        qDebug() << "contains";
    }
    else
    {
        db.m_db = QSqlDatabase::addDatabase( "QSQLITE", "qt_sql_default_connection");
        db.m_db.setDatabaseName("user.db");
    }
    //连接数据库
    db.m_db.open();
    if(db.m_db.open())
    {
        QString busid = ui->tableWidget->item(m_row,0)->text();
        QSqlQuery query(db.m_db);
        query.prepare("delete from busticket where 行程号=?");
        query.addBindValue(busid);
        query.exec();
            //在数据库中删除相应内容
        db.m_db.close();
    }
    updataTablewidgetData();
        //对数据库内容刷新显示
}


void viewwindow::on_btn_add_clicked()
{
    adddata * add = new adddata();
    add->setWindowTitle("数据填写");
    add->show(); // 使用 exec 函数，而不是 show 函数
    updataTablewidgetData();
}


void viewwindow::on_btn_book_clicked()
{
    QList<QTableWidgetItem*>items=ui->tableWidget->selectedItems();
    int count=items.count();
    int m_row;
    for(int i=0;i<count;i++)
    {
        m_row=ui->tableWidget->row(items.at(i));
    }
    //获取选中的行
    db.m_db.close();
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db.m_db = QSqlDatabase::database("qt_sql_default_connection");
        qDebug() << "contains";
    }
    else
    {
        db.m_db = QSqlDatabase::addDatabase( "QSQLITE", "qt_sql_default_connection");
        db.m_db.setDatabaseName("user.db");
    }

    // 检查是否已经购买过这个行程的票
    //连接数据库
    db.m_db.open();
    if(db.m_db.open()){
        QSqlQuery query(db.m_db);
        QString busid = ui->tableWidget->item(m_row,0)->text();
        QString username = g_username; // 这里添加你的用户名
        query.prepare("SELECT * FROM user_busticket WHERE username = ? AND 行程号 = ?");
        query.addBindValue(username);
        query.addBindValue(busid);
        if (!query.exec()) {
            qDebug() << "Failed to execute select query: " << query.lastError();
            return;
        }
        if (query.next()) {
            // 如果查询结果不为空，那么说明已经购买过这个行程的票
            QMessageBox::warning(this, "提示", "你已经购买过这个行程的票，不能重复购买");
            return;
        }

        // 添加选中行到user_busticket表中
        query.prepare("INSERT INTO user_busticket(username,行程号,状态) VALUES (?, ?, ?)");
        query.addBindValue(username);
        query.addBindValue(busid);
        query.addBindValue("未支付");
        if (!query.exec()) {
            qDebug() << "Failed to insert into table: " << query.lastError();
        }
        db.m_db.close();
    }
}


void viewwindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/bus.jpeg"));
}
