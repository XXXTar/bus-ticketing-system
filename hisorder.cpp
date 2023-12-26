#include "hisorder.h"
#include "ui_hisorder.h"
#include "widget.h"
#include <QMessageBox>
extern QString g_username;

hisorder::hisorder(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::hisorder)
{
    ui->setupUi(this);
     this->setWindowTitle("历史订单");
    loadOrderHistory();
}

hisorder::~hisorder()
{
    delete ui;
}

void hisorder::loadOrderHistory()
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
        if(query.exec(QString("SELECT * FROM history WHERE username = '%1'").arg(username)))
        {
            ui->tableWidget->clearContents();
            ui->tableWidget->setRowCount(0);

            while(query.next())
            {
                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->insertRow(rowCount);
                for(int i = 0; i < 7; i++)
                {
                    QTableWidgetItem *columnItem = new QTableWidgetItem(query.value(i).toString());
                    ui->tableWidget->setItem(rowCount, i, columnItem);
                }
            }

        }
        db.m_db.close();
    }
}

void hisorder::on_btn_back_clicked()
{
    Widget *w=new Widget();
    w->show();
    this->hide();
}


void hisorder::on_btn_delete_clicked()
{
    int row = ui->tableWidget->currentRow();
    if(row != -1)  // 如果有选中的行
    {
        // 直接从表格中获取行程号
        QString tripNumber = ui->tableWidget->item(row, 0)->text();

        // 确认是否删除
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "删除历史订单", "确定要删除这个历史订单吗？",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            // 打开数据库
            db.m_db.open();
            if(db.m_db.open())
            {
                QSqlQuery query(db.m_db);
                QString username = g_username; // 这里添加你的用户名

                // 删除对应的历史订单
                if(query.exec(QString("DELETE FROM history WHERE username = '%1' AND 行程号 = '%2'")
                                   .arg(username, tripNumber)))
                {
                    db.m_db.commit();  // 提交更改
                    qDebug() << "HisOrder deleted successfully.";
                    QMessageBox::information(this, "删除成功", "历史订单已删除");
                }
                else
                {
                    qDebug() << "Failed to delete order: " << query.lastError();
                }

                db.m_db.close();
            }

            // 更新表格数据
            loadOrderHistory();
        }
    }
    else
    {
        QMessageBox::warning(this, "提示", "请选中历史订单");
        qDebug() << "No row selected.";
    }
}

