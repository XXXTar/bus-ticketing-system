#include "databases.h"
#include <QMessageBox>
#include <QDebug>

QString g_username;
databases::databases()
{
    m_db.close();
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        m_db = QSqlDatabase::database("qt_sql_default_connection");
        qDebug() << "contains";
    }
    else
    {
        m_db = QSqlDatabase::addDatabase(
            "QSQLITE");
        m_db.setDatabaseName("user.db");
    }

    if(m_db.open())
    {
        qDebug() << "数据库已连接";
    }
    else
    {
        QMessageBox::warning(NULL,"error","数据库连接失败");
    }
    initializeDatabase();
}

void databases::initializeDatabase()
{
    QSqlQuery query(m_db);
    QString sql = "select * from userInfo";
    if(!query.exec(sql))
    {
        qDebug()<<"userInfo不存在";
        sql="create table userInfo(username varchar(20) primary key,password varchar(20),email varchar(20),phone varchar(20),role varchar(20))";
        query.exec(sql);
        sql="INSERT INTO userInfo(username,password,email,phone,role) VALUES ('xxy','123456','1144029309@qq.com','15202359899','admin')";
        query.exec(sql);//设置初始账号密码,执行sql
    }else
    {
        qDebug()<<"用户表存在";
    }

    if(!m_db.tables().contains("busticket"))
    {
        bool ret = query.exec(QString("create table busticket(行程号 char(8) primary key,起点 varchar(15),终点 varchar(15),日期 datetime,发车时间 char(8),到达时间 char(8),剩余座位 int(4),可退票价 float(8),不退票价 float(8),优惠幅度 float(8))"));
        if (!ret) {
            qDebug() << "Failed to create table: " << query.lastError();
        }
        ret = query.exec(QString("INSERT INTO busticket(行程号,起点,终点,日期,发车时间,到达时间,剩余座位,可退票价,不退票价,优惠幅度) VALUES ('00000001','天津','北京','2024-01-15','16:30','18:30','10','50.0','40.0','0.0')"));
        if (!ret) {
            qDebug() << "Failed to Insert table: " << query.lastError();
        }
        ret = query.exec(QString("INSERT INTO busticket(行程号,起点,终点,日期,发车时间,到达时间,剩余座位,可退票价,不退票价,优惠幅度) VALUES ('00000002','天津','石家庄','2024-01-16','13:30','14:30','5','50.0','40.0','0.0')"));
        if (!ret) {
            qDebug() << "Failed to Insert table: " << query.lastError();
        }
    }
    else
    {
        qDebug() << "长途巴士表存在";
    }

    if(!m_db.tables().contains("user_busticket"))
    {
        bool ret = query.exec(QString("create table user_busticket(username varchar(20),行程号 char(8),状态 varchar(20))"));
        if (!ret) {
            qDebug() << "Failed to create table: " << query.lastError();
        }
        //添加行程号和
        //ret = query.exec(QString("INSERT INTO user_busticket(username,行程号，状态) VALUES ('xxy','00000001','未支付')"));
        //if (!ret) {
        //    qDebug() << "Failed to Insert table: " << query.lastError();
        //}
    }
    else
    {
        qDebug() << "用户行程表存在";
    }

    // 检查 history 表是否已经存在
    if(!m_db.tables().contains("history"))
    {
        // 如果 history 表不存在，则创建
        if(query.exec(QString("CREATE TABLE history (行程号 CHAR(8),起点 VARCHAR(15),终点 VARCHAR(15),日期 DATETIME,发车时间 CHAR(8),到达时间 CHAR(8),状态 VARCHAR(20),username varchar(20))"))
            )
        {
            qDebug() << "Created history table successfully.";
        }
        else
        {
            qDebug() << "Failed to create history table: " << query.lastError();
        }
    }
    else
    {
        qDebug() << "历史订单表存在";
    }

    m_db.close();

}
