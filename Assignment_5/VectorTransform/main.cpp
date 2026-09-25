#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("VectorTransform");
    a.setApplicationDisplayName("2D Vector Transformations Studio (Homogeneous Coordinates 3x3)");

    MainWindow w;
    w.show();
    return a.exec();
}
