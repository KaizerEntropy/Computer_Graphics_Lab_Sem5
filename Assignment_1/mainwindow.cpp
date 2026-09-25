#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    lastPoint1 = QPoint(-1, -1);
    lastPoint2 = QPoint(-1, -1);

    // Initialize canvas with black background
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

    // Calculate logical Cartesian coordinates relative to the center
    int cx = ui->frame->width() / 2;
    int cy = ui->frame->height() / 2;
    int log_x = std::floor(static_cast<double>(sc_x - cx) / gridSize);
    int log_y = std::floor(static_cast<double>(cy - sc_y) / gridSize);

    ui->mouse_movement->setText("X : " + QString::number(log_x) + ", Y : " + QString::number(log_y));
}

void MainWindow::Mouse_Pressed()
{
    org_x = sc_x;
    org_y = sc_y;

    int cx = ui->frame->width() / 2;
    int cy = ui->frame->height() / 2;

    // Snapping logic aligned to the center of the frame
    int cell_x = std::floor(static_cast<double>(org_x - cx) / gridSize) * gridSize + cx;
    int cell_y = std::floor(static_cast<double>(org_y - cy) / gridSize) * gridSize + cy;

    // Logical coordinates for the UI label
    int log_x = std::floor(static_cast<double>(org_x - cx) / gridSize);
    int log_y = std::floor(static_cast<double>(cy - org_y) / gridSize);

    ui->mouse_pressed->setText("Origin X : " + QString::number(log_x) + ", Y : " + QString::number(log_y));

    lastPoint1 = lastPoint2;
    lastPoint2 = QPoint(cell_x, cell_y);

    addPoint(org_x, org_y, 1);
}

void MainWindow::addPoint(int x, int y, int c)
{
    QPixmap pm = ui->frame->pixmap();
    if (pm.isNull()) return;

    int cx = ui->frame->width() / 2;
    int cy = ui->frame->height() / 2;

    // Snapping logic aligned to the center of the frame
    int cell_x = std::floor(static_cast<double>(x - cx) / gridSize) * gridSize + cx;
    int cell_y = std::floor(static_cast<double>(y - cy) / gridSize) * gridSize + cy;

    QPainter painter(&pm);
    QColor color;

    switch (c) {
    case 0: color = Qt::white; break;
    case 1: color = Qt::red; break;
    case 2: color = Qt::green; break;
    case 3: color = Qt::blue; break;
    default: color = Qt::white; break;
    }

    // Fill the exact grid cell that was clicked
    painter.fillRect(cell_x, cell_y, gridSize, gridSize, color);
    painter.end();

    ui->frame->setPixmap(pm);
}

void MainWindow::on_clear_clicked()
{
    lastPoint1 = lastPoint2 = QPoint(-1, -1);

    // Clear the canvas to black
    QPixmap pix(ui->frame->width(), ui->frame->height());
    pix.fill(Qt::black);
    ui->frame->setPixmap(pix);

    // Redraw the grid if a grid size is set
    if (gridSize > 0) {
        on_pushButton_2_clicked();
    }
}





void MainWindow::on_pushButton_2_clicked()
{
    if (gridSize <= 0) return;

    QPixmap pm = ui->frame->pixmap();
    if (pm.isNull()) return;

    QPainter painter(&pm);

    int width = pm.width();
    int height = pm.height();
    int cx = width / 2;
    int cy = height / 2;

    // 1. Fill the main X and Y axes with a distinct background color
    painter.fillRect(0, cy - gridSize, width, gridSize, QColor(40, 40, 40)); // X axis
    painter.fillRect(cx, 0, gridSize, height, QColor(40, 40, 40)); // Y axis

    // Highlight the (0,0) origin cell slightly brighter
    painter.fillRect(cx, cy - gridSize, gridSize, gridSize, QColor(80, 80, 80));

    // 2. Draw the standard dashed grid radiating from the center
    painter.setPen(QPen(Qt::darkGray, 1, Qt::DashLine));

    // Vertical grid lines
    for (int x = cx; x <= width; x += gridSize) painter.drawLine(x, 0, x, height);
    for (int x = cx; x >= 0; x -= gridSize) painter.drawLine(x, 0, x, height);

    // Horizontal grid lines
    for (int y = cy; y <= height; y += gridSize) painter.drawLine(0, y, width, y);
    for (int y = cy; y >= 0; y -= gridSize) painter.drawLine(0, y, width, y);

    painter.end();
    ui->frame->setPixmap(pm);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if (arg1 > 0) {
        gridSize = arg1;

        // Clear the window completely (fill with black)
        lastPoint1 = lastPoint2 = QPoint(-1, -1);
        QPixmap pix(ui->frame->width(), ui->frame->height());
        pix.fill(Qt::black);
        ui->frame->setPixmap(pix);

        // Redraw the grid with the new size automatically
        on_pushButton_2_clicked();
    }
}
// DDA Line Drawing Algorithm (Orange)
void MainWindow::on_draw_line_clicked()
{
    if (lastPoint1 == QPoint(-1, -1) || lastPoint2 == QPoint(-1, -1)) return;

    QPixmap pm = ui->frame->pixmap();
    QPainter painter(&pm);

    int cx = pm.width() / 2;
    int cy = pm.height() / 2;

    int x1 = (lastPoint1.x() - cx) / gridSize;
    int y1 = (lastPoint1.y() - cy) / gridSize;
    int x2 = (lastPoint2.x() - cx) / gridSize;
    int y2 = (lastPoint2.y() - cy) / gridSize;

    // --- START TIMER ---
    QElapsedTimer timer;
    timer.start();

    // DDA Algorithm
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = std::max(std::abs(dx), std::abs(dy));

    float xInc = steps == 0 ? 0 : (float)dx / steps;
    float yInc = steps == 0 ? 0 : (float)dy / steps;

    float x = x1;
    float y = y1;

    QColor orange = QColor(255, 0, 0);

    for (int i = 0; i <= steps; i++) {
        int cell_x = std::round(x) * gridSize + cx;
        int cell_y = std::round(y) * gridSize + cy;

        painter.fillRect(cell_x, cell_y, gridSize, gridSize, orange);

        x += xInc;
        y += yInc;
    }

    // --- STOP TIMER ---
    qint64 elapsed_ns = timer.nsecsElapsed();
    qDebug() << "DDA Time:" << elapsed_ns << "nanoseconds";

    painter.end();
    ui->frame->setPixmap(pm);
}

// Bresenham's Line Drawing Algorithm (Cyan)
void MainWindow::on_pushButton_clicked()
{
    if (lastPoint1 == QPoint(-1, -1) || lastPoint2 == QPoint(-1, -1)) return;

    QPixmap pm = ui->frame->pixmap();
    QPainter painter(&pm);

    int cx = pm.width() / 2;
    int cy = pm.height() / 2;

    int x1 = (lastPoint1.x() - cx) / gridSize;
    int y1 = (lastPoint1.y() - cy) / gridSize;
    int x2 = (lastPoint2.x() - cx) / gridSize;
    int y2 = (lastPoint2.y() - cy) / gridSize;

    // --- START TIMER ---
    QElapsedTimer timer;
    timer.start();

    // Bresenham's Algorithm
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int x = x1;
    int y = y1;

    while (true) {
        int cell_x = x * gridSize + cx;
        int cell_y = y * gridSize + cy;

        painter.fillRect(cell_x, cell_y, gridSize, gridSize, Qt::red);

        if (x == x2 && y == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 < dx) { err += dx; y += sy; }
    }

    // --- STOP TIMER ---
    qint64 elapsed_ns = timer.nsecsElapsed();
    qDebug() << "Bresenham Time:" << elapsed_ns << "nanoseconds";
    // ui->Time_Label->setText("Execution Time: " + QString::number(elapsed_ns) + " ns");

    painter.end();
    ui->frame->setPixmap(pm);
}