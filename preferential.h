#ifndef PREFERENTIAL_H
#define PREFERENTIAL_H

#include <QWidget>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include "databases.h"
namespace Ui {
class preferential;
}

class preferential : public QWidget
{
    Q_OBJECT

public:
    explicit preferential(QWidget *parent = nullptr);
    ~preferential();
    databases db;
private slots:

    void on_btn_quit_clicked();

    void on_btn_yes_clicked();

private:
    Ui::preferential *ui;
};

#endif // PREFERENTIAL_H
