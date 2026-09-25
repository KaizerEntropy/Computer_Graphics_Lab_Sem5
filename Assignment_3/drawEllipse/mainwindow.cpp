#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QElapsedTimer>
#include <algorithm>
#include <cmath>
#include <vector>

static const double ELLIPSE_PI = 3.14159265358979323846;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Ellipse Drawing Algorithms - Polar / Midpoint / Cartesian");

    lastPoint1 = QPoint(-1, -1);
    lastPoint2 = QPoint(-1, -1);

    animIndex = 0;
    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &MainWindow::animateStep);
    on_spinBox_valueChanged(ui->spinBox->value());

    connect(ui->frame, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_Pressed()));
    connect(ui->frame, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void drawAxesAndGrid(QPainter &painter, int width, int height, int gridSize)
{
    if (gridSize <= 0) gridSize = 1;

    int originX = (width / 2 / gridSize) * gridSize;
    int originY = (height / 2 / gridSize) * gridSize;

    painter.setPen(QPen(Qt::white, 1));
    for (int x = gridSize; x < width; x += gridSize) {
        painter.drawLine(x, 0, x, height - 1);
    }
    for (int y = gridSize; y < height; y += gridSize) {
        painter.drawLine(0, y, width - 1, y);
    }

    painter.setPen(QPen(Qt::white, 1));
    painter.setBrush(QBrush(Qt::white));
    for (int x = 0; x < width; x += gridSize) {
        painter.drawRect(x, originY, gridSize, gridSize);
    }

    for (int y = 0; y < height; y += gridSize) {
        painter.drawRect(originX, y, gridSize, gridSize);
    }

    painter.setBrush(QBrush(Qt::blue));
    painter.drawRect(originX, originY, gridSize, gridSize);
}

void MainWindow::showMousePosition(QPoint &pos)
{
    sc_x = pos.x();
    sc_y = pos.y();
    int gridSize = ui->spinBox->value();

    if (gridSize <= 0) gridSize = 1;

    int width = ui->frame->width();
    int height = ui->frame->height();

    int originX = (width / 2 / gridSize) * gridSize;
    int originY = (height / 2 / gridSize) * gridSize;

    int cartesian_x = std::floor(static_cast<double>(sc_x - originX) / gridSize);
    int cartesian_y = -std::floor(static_cast<double>(sc_y - originY) / gridSize);

    ui->mouse_movement->setText("X : " + QString::number(cartesian_x) +
                                ", Y : " + QString::number(cartesian_y));
}

void MainWindow::Mouse_Pressed()
{
    org_x = sc_x;
    org_y = sc_y;
    lastPoint1 = lastPoint2;
    lastPoint2 = QPoint(org_x, org_y);

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;

    int box_x = (org_x / gridSize) * gridSize;
    int box_y = (org_y / gridSize) * gridSize;

    QPixmap pm = ui->frame->pixmap();
    if (pm.isNull()) return;

    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;
    int cx = (org_x - originX) / gridSize;
    int cy = -(org_y - originY) / gridSize;
    ui->center_label->setText("Center selected: (" + QString::number(cx) + ", " + QString::number(cy) + ")");

    pm.fill(Qt::black);

    QPainter painter(&pm);
    int width = pm.width();
    int height = pm.height();

    drawAxesAndGrid(painter, width, height, gridSize);

    painter.fillRect(box_x, box_y, gridSize, gridSize, Qt::blue);
    painter.setPen(QPen(Qt::white, 1));
    painter.drawRect(box_x, box_y, gridSize, gridSize);

    painter.end();
    ui->frame->setPixmap(pm);
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

void MainWindow::redrawCanvas()
{
    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;

    QPixmap pm(ui->frame->width(), ui->frame->height());
    pm.fill(Qt::black);

    QPainter painter(&pm);
    drawAxesAndGrid(painter, pm.width(), pm.height(), gridSize);

    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;

    for (const DrawnEllipse &e : drawnEllipses) {
        qint64 elapsed; 
        std::vector<std::array<QPoint, 4>> quadrants;

        if (e.algoName == "Polar Ellipse")
            quadrants = computePolarEllipse(e.rx, e.ry, elapsed);
        else if (e.algoName == "Midpoint Ellipse")
            quadrants = computeMidpointEllipse(e.rx, e.ry, elapsed);
        else
            quadrants = computeCartesianEllipse(e.rx, e.ry, elapsed);

        painter.setBrush(QBrush(e.color));
        painter.setPen(QPen(Qt::white, 1));
        for (const auto &quadrant : quadrants) {
            for (const QPoint &pt : quadrant) {
                int drawX = originX + (e.centerX + pt.x()) * gridSize;
                int drawY = originY - (e.centerY + pt.y()) * gridSize;
                painter.drawRect(drawX, drawY, gridSize, gridSize);
            }
        }
    }

    painter.end();
    ui->frame->setPixmap(pm);
}

void MainWindow::on_clear_clicked()
{
    drawnEllipses.clear();
    animTimer->stop();
    ui->time_label->setText("Stats will appear here...");
    ui->center_label->setText("Center: Not Selected");
    lastPoint1 = lastPoint2 = QPoint(-1, -1);
    int gridSize = ui->spinBox->value();

    QPixmap pix(ui->frame->width(), ui->frame->height());
    pix.fill(Qt::black);

    QPainter painter(&pix);
    drawAxesAndGrid(painter, pix.width(), pix.height(), gridSize);
    painter.end();

    ui->frame->setPixmap(pix);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->spinBox->setValue(10);
    on_spinBox_valueChanged(10);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if (arg1 <= 0) return;
    redrawCanvas();
}

std::array<QPoint, 4> MainWindow::fourWaySymmetry(int x, int y) const
{
    return {
        QPoint( x,  y), QPoint(-x,  y),
        QPoint( x, -y), QPoint(-x, -y)
    };
}

std::vector<std::array<QPoint, 4>> MainWindow::computePolarEllipse(int rx, int ry, qint64 &elapsedNanos) const
{
    std::vector<std::array<QPoint, 4>> quadrants;
    if (rx <= 0 || ry <= 0) { elapsedNanos = 0; return quadrants; }

    QElapsedTimer timer;
    timer.start();

    double increment = 1.0 / std::max(rx, ry);
    for (double theta = 0.0; theta <= ELLIPSE_PI / 2.0 + 1e-9; theta += increment) {
        int x = static_cast<int>(std::round(rx * std::cos(theta)));
        int y = static_cast<int>(std::round(ry * std::sin(theta)));
        quadrants.push_back(fourWaySymmetry(x, y));
    }

    elapsedNanos = timer.nsecsElapsed();
    return quadrants;
}

std::vector<std::array<QPoint, 4>> MainWindow::computeMidpointEllipse(int rx, int ry, qint64 &elapsedNanos) const
{
    std::vector<std::array<QPoint, 4>> quadrants;
    if (rx <= 0 || ry <= 0) { elapsedNanos = 0; return quadrants; }

    QElapsedTimer timer;
    timer.start();

    long long rxSq = (long long)rx * rx;
    long long rySq = (long long)ry * ry;
    long long x = 0, y = ry;
    long long px = 0, py = 2 * rxSq * y;

    // Region 1
    quadrants.push_back(fourWaySymmetry(x, y));
    long long p1 = rySq - (rxSq * ry) + (0.25 * rxSq);
    while (px < py) {
        x++;
        px += 2 * rySq;
        if (p1 < 0) {
            p1 += rySq + px;
        } else {
            y--;
            py -= 2 * rxSq;
            p1 += rySq + px - py;
        }
        quadrants.push_back(fourWaySymmetry(x, y));
    }

    // Region 2
    long long p2 = rySq * (x + 0.5) * (x + 0.5) + rxSq * (y - 1) * (y - 1) - rxSq * rySq;
    while (y > 0) {
        y--;
        py -= 2 * rxSq;
        if (p2 > 0) {
            p2 += rxSq - py;
        } else {
            x++;
            px += 2 * rySq;
            p2 += rxSq - py + px;
        }
        quadrants.push_back(fourWaySymmetry(x, y));
    }

    elapsedNanos = timer.nsecsElapsed();
    return quadrants;
}

std::vector<std::array<QPoint, 4>> MainWindow::computeCartesianEllipse(int rx, int ry, qint64 &elapsedNanos) const
{
    std::vector<std::array<QPoint, 4>> quadrants;
    if (rx <= 0 || ry <= 0) { elapsedNanos = 0; return quadrants; }

    QElapsedTimer timer;
    timer.start();

    double rxSq = (double)rx * rx;
    double rySq = (double)ry * ry;
    
    double thresholdX = rxSq / std::sqrt(rxSq + rySq);
    int limitX = static_cast<int>(std::round(thresholdX));

    for (int x = 0; x <= limitX; x++) {
        double ySq = rySq * (1.0 - (double)(x * x) / rxSq);
        int y = static_cast<int>(std::round(std::sqrt(std::max(0.0, ySq))));
        quadrants.push_back(fourWaySymmetry(x, y));
    }
    
    double thresholdY = rySq / std::sqrt(rxSq + rySq);
    int limitY = static_cast<int>(std::round(thresholdY));

    for (int y = limitY - 1; y >= 0; y--) {
        double xSq = rxSq * (1.0 - (double)(y * y) / rySq);
        int x = static_cast<int>(std::round(std::sqrt(std::max(0.0, xSq))));
        quadrants.push_back(fourWaySymmetry(x, y));
    }

    elapsedNanos = timer.nsecsElapsed();
    return quadrants;
}

void MainWindow::runEllipseAlgorithm(const std::vector<std::array<QPoint, 4>> &quadrants,
                                     qint64 elapsedNanos,
                                     const QColor &color,
                                     const QString &algoName,
                                     int rx, int ry)
{
    if (lastPoint2 == QPoint(-1, -1)) {
        ui->center_label->setText("Click on the grid to pick a centre first!");
        return;
    }
    if (quadrants.empty()) return;

    animTimer->stop();

    int pixelCount = static_cast<int>(quadrants.size()) * 4;

    qDebug() << algoName << "execution time:" << elapsedNanos
             << "nanoseconds, pixels drawn:" << pixelCount;

    QString newText = algoName + ": " + QString::number(elapsedNanos) + " ns, " +
                      QString::number(pixelCount) + " px";
                      
    if (ui->time_label->text() == "Stats will appear here...") {
        ui->time_label->setText(newText);
    } else {
        ui->time_label->setText(ui->time_label->text() + "\n" + newText);
    }

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;

    QPixmap pm = ui->frame->pixmap();
    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;

    int cx = (lastPoint2.x() - originX) / gridSize;
    int cy = -(lastPoint2.y() - originY) / gridSize;

    std::vector<std::array<QPoint, 4>> centered;
    centered.reserve(quadrants.size());
    for (const auto &quadrant : quadrants) {
        std::array<QPoint, 4> shifted;
        for (int i = 0; i < 4; i++) {
            shifted[i] = QPoint(cx + quadrant[i].x(), cy + quadrant[i].y());
        }
        centered.push_back(shifted);
    }

    if (ui->animate_checkBox->isChecked()) {
        animQuadrants = centered;
        animIndex = 0;
        animColor = color;
        int interval = std::max(15, 400 / static_cast<int>(centered.size()));
        animTimer->start(interval);
    } else {
        QPainter painter(&pm);
        painter.setBrush(QBrush(color));
        painter.setPen(QPen(Qt::white, 1));
        for (const auto &quadrant : centered) {
            for (const QPoint &pt : quadrant) {
                int drawX = originX + pt.x() * gridSize;
                int drawY = originY - pt.y() * gridSize;
                painter.drawRect(drawX, drawY, gridSize, gridSize);
            }
        }
        painter.end();
        ui->frame->setPixmap(pm);

        drawnEllipses.push_back({cx, cy, rx, ry, color, algoName});
    }
}

void MainWindow::animateStep()
{
    if (animIndex >= static_cast<int>(animQuadrants.size())) {
        animTimer->stop();
        return;
    }

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;

    QPixmap pm = ui->frame->pixmap();
    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;

    QPainter painter(&pm);
    painter.setBrush(QBrush(animColor));
    painter.setPen(QPen(Qt::white, 1));
    for (const QPoint &pt : animQuadrants[animIndex]) {
        int drawX = originX + pt.x() * gridSize;
        int drawY = originY - pt.y() * gridSize;
        painter.drawRect(drawX, drawY, gridSize, gridSize);
    }
    painter.end();
    ui->frame->setPixmap(pm);

    animIndex++;
}

void MainWindow::on_draw_polar_clicked()
{
    int rx = ui->rxBox->value();
    int ry = ui->ryBox->value();
    qint64 elapsed;
    auto quadrants = computePolarEllipse(rx, ry, elapsed);
    runEllipseAlgorithm(quadrants, elapsed, QColor(255, 0, 0), "Polar Ellipse", rx, ry);
}

void MainWindow::on_draw_midpoint_ellipse_clicked()
{
    int rx = ui->rxBox->value();
    int ry = ui->ryBox->value();
    qint64 elapsed;
    auto quadrants = computeMidpointEllipse(rx, ry, elapsed);
    runEllipseAlgorithm(quadrants, elapsed, QColor(0, 255, 0), "Midpoint Ellipse", rx, ry);
}

void MainWindow::on_draw_cartesian_clicked()
{
    int rx = ui->rxBox->value();
    int ry = ui->ryBox->value();
    qint64 elapsed;
    auto quadrants = computeCartesianEllipse(rx, ry, elapsed);
    runEllipseAlgorithm(quadrants, elapsed, QColor(0, 0, 255), "Cartesian Ellipse", rx, ry);
}