#ifndef ADWIDGET_H
#define ADWIDGET_H

#include <QWidget>

namespace Ui {
class adwidget;
}

class adwidget : public QWidget
{
    Q_OBJECT

public:
    explicit adwidget(QWidget *parent = nullptr);
    ~adwidget();
    void paintEvent(QPaintEvent *event);
private slots:
    void on_btn_bus_clicked();

    void on_btn_back_clicked();

    void on_btn_order_clicked();

    void on_btn_preferential_clicked();

private:
    Ui::adwidget *ui;
};

#endif // ADWIDGET_H
