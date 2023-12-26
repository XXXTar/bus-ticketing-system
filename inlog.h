#ifndef INLOG_H
#define INLOG_H

#include <QWidget>
#include <QSqlDatabase>
#include <databases.h>
namespace Ui {
class inlog;
}

class inlog : public QWidget
{
    Q_OBJECT

public:
    explicit inlog(QWidget *parent = nullptr);
    ~inlog();
    void paintEvent(QPaintEvent *event);
private slots:
    void on_btn_inlog_clicked();

    void on_btn_reg_clicked();

    void on_btn_quit_clicked();

    void on_checkBox_clicked(bool checked);

private:
    Ui::inlog *ui;
    databases db;
};

#endif // INLOG_H
