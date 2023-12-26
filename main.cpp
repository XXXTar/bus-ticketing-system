#include "widget.h"
#include "inlog.h"
#include "regist.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    inlog w;
    w.show();
    return a.exec();
}
