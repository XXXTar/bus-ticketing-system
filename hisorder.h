#ifndef HISORDER_H
#define HISORDER_H

#include <QWidget>
#include <databases.h>

namespace Ui {
class hisorder;
}

class hisorder : public QWidget
{
    Q_OBJECT

public:
    explicit hisorder(QWidget *parent = nullptr);
    ~hisorder();
    void loadOrderHistory();
    databases db;
private slots:
    void on_btn_back_clicked();

    void on_btn_delete_clicked();

private:
    Ui::hisorder *ui;
};

#endif // HISORDER_H
