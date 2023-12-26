#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QWidget>
#include "databases.h"
#include <adddata.h>

namespace Ui {
class viewwindow;
}

class viewwindow : public QWidget
{
    Q_OBJECT

public:
    explicit viewwindow(QWidget *parent = nullptr);
    ~viewwindow();
    databases db;
    void updataTablewidgetData();
    void paintEvent(QPaintEvent *event);
private slots:
    void on_btn_quit_clicked();

    void on_btn_refresh_clicked();

    void on_btn_find_clicked();

    void on_btn_modify_clicked();

    void on_btn_delete_clicked();

    void on_btn_add_clicked();

    void on_btn_book_clicked();

private:
    Ui::viewwindow *ui;
};

#endif // VIEWWINDOW_H
