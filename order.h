#ifndef ORDER_H
#define ORDER_H

#include "databases.h"
#include <QWidget>

namespace Ui {
class order;
}

class order : public QWidget
{
    Q_OBJECT

public:
    explicit order(QWidget *parent = nullptr);
    ~order();
    void showAllOrders();
    databases db;
private slots:
    void on_btn_back_clicked();

    void on_btn_print_clicked();

    void on_btn_month_clicked();

    void on_btn_season_clicked();

    void on_btn_year_clicked();

    void on_btn_find_clicked();

    void on_btn_shuaxin_clicked();

private:
    Ui::order *ui;
};

#endif // ORDER_H
