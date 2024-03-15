#include "TimeTrackingApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //a.setStyleSheet("* { border: 1px solid rgba(0, 255, 0, 35); }");
    TimeTrackingApp w;
    w.show();
    return a.exec();
}
