#include "preferential.h"
#include "ui_preferential.h"

preferential::preferential(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::preferential)
{
    ui->setupUi(this);
     this->setWindowTitle("优惠调整");
 // 添加所有的选项到每个comboBox中
    ui->comboBoxSeason->addItems({"春", "夏", "秋", "冬"});
    ui->comboBoxWeekend->addItems({"周末", "非周末"});
    ui->comboBoxDepartureTime->addItems({"早上", "下午", "晚上"});
    ui->comboBoxTicketQuantity->addItems({"1", "2", "3", "4", "5"});
    ui->comboBoxAdvancePurchase->addItems({"1天", "2天", "3天", "4天", "5天"});
    ui->comboBoxChild->addItems({"是", "否"});
    ui->comboBoxElderly->addItems({"是", "否"});
}

preferential::~preferential()
{
    delete ui;
}



void preferential::on_btn_quit_clicked()
{
    this->close();
}


void preferential::on_btn_yes_clicked()
{
    // 获取每个comboBox和spinBox的值
    QString season = ui->comboBoxSeason->currentText();
    int discountSeason = ui->spinBoxSeason->value();

    QString weekend = ui->comboBoxWeekend->currentText();
    int discountWeekend = ui->spinBoxWeekend->value();

    QString departureTime = ui->comboBoxDepartureTime->currentText();
    int discountDepartureTime = ui->spinBoxDepartureTime->value();

    // 打开数据库
    db.m_db.open();
    if(db.m_db.open())
    {
        QSqlQuery query(db.m_db);

        // 更新所有符合条件的记录
        QString sqlUpdate = QString("UPDATE busticket SET 优惠幅度 = CASE "
                                    "WHEN strftime('%m', 日期) BETWEEN '03' AND '05' AND '%1' = '春' THEN %2 "
                                    "WHEN strftime('%w', 日期) IN ('6', '0') AND '%3' = '周末' THEN %4 "
                                    "WHEN CAST(strftime('%H', 发车时间) AS INTEGER)*60 + CAST(strftime('%M', 发车时间) AS INTEGER) BETWEEN 360 AND 719 AND '%5' = '早上' THEN %6 "
                                    "WHEN strftime('%m', 日期) BETWEEN '06' AND '08' AND '%1' = '夏' THEN %2 "
                                    "WHEN strftime('%w', 日期) NOT IN ('6', '0') AND '%3' = '非周末' THEN %4 "
                                    "WHEN CAST(strftime('%H', 发车时间) AS INTEGER)*60 + CAST(strftime('%M', 发车时间) AS INTEGER) BETWEEN 720 AND 1079 AND '%5' = '下午' THEN %6 "
                                    "WHEN strftime('%m', 日期) BETWEEN '09' AND '11' AND '%1' = '秋' THEN %2 "
                                    "WHEN (CAST(strftime('%H', 发车时间) AS INTEGER)*60 + CAST(strftime('%M', 发车时间) AS INTEGER) BETWEEN 1080 AND 1439 OR CAST(strftime('%H', 发车时间) AS INTEGER)*60 + CAST(strftime('%M', 发车时间) AS INTEGER) BETWEEN 0 AND 359) AND '%5' = '晚上' THEN %6 "
                                    "WHEN strftime('%m', 日期) NOT BETWEEN '03' AND '11' AND '%1' = '冬' THEN %2 "
                                    "ELSE 优惠幅度 END")
                                .arg(season, QString::number(discountSeason),
                                     weekend, QString::number(discountWeekend),
                                     departureTime, QString::number(discountDepartureTime));


        // 执行更新语句
        if(!query.exec(sqlUpdate))
            qDebug() << "Failed to execute update: " << query.lastError();

        QMessageBox::information(this, "优惠幅度设置", "优惠幅度已成功设置");

        db.m_db.close();
    }
}

