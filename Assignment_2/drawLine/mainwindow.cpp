#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lastPoint1 = QPoint(-1, -1);
    lastPoint2 = QPoint(-1, -1);
    QPixmap pix(ui->frame->width(), ui->frame->height());
    pix.fill(Qt::black);
    ui->frame->setPixmap(pix);

    connect(ui->frame, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_Pressed()));
    connect(ui->frame, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMousePosition(QPoint &pos)
{
    sc_x = pos.x();
    sc_y = pos.y();
    ui->mouse_movement->setText("X : " + QString::number(sc_x) + ", Y : " + QString::number(sc_y));
}

void MainWindow::Mouse_Pressed()
{
    org_x = sc_x;
    org_y = sc_y;
    ui->mouse_pressed->setText("X : " + QString::number(sc_x) + ", Y : " + QString::number(sc_y));

    lastPoint1 = lastPoint2;
    lastPoint2 = QPoint(org_x, org_y);
    addPoint(org_x, org_y, 1);
}

void MainWindow::addPoint(int x, int y, int c)
{
    QPixmap pm = ui->frame->pixmap();
    if (pm.isNull()) return;
    QImage img = pm.toImage();

    int r = 0, g = 0, b = 0;
    switch (c) {
    case 0: r = g = b = 255; break;
    case 1: r = 255; break;
    case 2: g = 255; break;
    case 3: b = 255; break;
    }

    img.setPixel(x, y, qRgb(r, g, b));
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_clear_clicked()
{
    lastPoint1 = lastPoint2 = QPoint(-1, -1);
    QPixmap pix(ui->frame->width(), ui->frame->height());
    pix.fill(Qt::black);
    ui->frame->setPixmap(pix);
}

void MainWindow::on_draw_line_clicked()
{
    if (lastPoint1 == QPoint(-1, -1) || lastPoint2 == QPoint(-1, -1)) return;

    QPixmap pm = ui->frame->pixmap();
    QPainter painter(&pm);
    painter.setPen(QPen(Qt::blue, 2));
    painter.drawLine(lastPoint1, lastPoint2);
    painter.end();
    ui->frame->setPixmap(pm);
}
