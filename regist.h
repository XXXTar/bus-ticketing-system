#ifndef REGIST_H
#define REGIST_H

#include <QWidget>
#include <QSqlDatabase>
#include <databases.h>
namespace Ui {
class regist;
}

class regist : public QWidget
{
    Q_OBJECT

public:
    explicit regist(QWidget *parent = nullptr);
    ~regist();
    void paintEvent(QPaintEvent *event);
private slots:
    void on_btn_reg_clicked();

    void on_btn_quit_clicked();

private:
    Ui::regist *ui;
    databases db;
};

#endif // REGIST_H
