#include "../headers/TimeTrackingApp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TimeTrackingApp w;
    w.show();
    return a.exec();
}
