#include "QtSGS.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtSGS* w = new QtSGS();
    w->show();
    return a.exec();
}
