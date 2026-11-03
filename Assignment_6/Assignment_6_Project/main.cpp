#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Clipping");
    a.setApplicationDisplayName("2D Clipping Studio — Cohen-Sutherland & Sutherland-Hodgman");

    MainWindow w;
    w.show();
    return a.exec();
}
