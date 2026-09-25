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

static const double CIRCLE_PI = 3.14159265358979323846;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Circle Drawing Algorithms - Polar / Bresenham (Midpoint) / Cartesian");

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

    // Changed grid/axes base lines to white (was blue)
    painter.setPen(QPen(Qt::white, 1));
    for (int x = gridSize; x < width; x += gridSize) {
        painter.drawLine(x, 0, x, height - 1);
    }
    for (int y = gridSize; y < height; y += gridSize) {
        painter.drawLine(0, y, width - 1, y);
    }

    painter.setPen(QPen(Qt::white, 1));
    // Changed X and Y axis boxes to white (was QColor(50, 50, 50))
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
    ui->center_label->setText("Centre selected: (" + QString::number(cx) + ", " + QString::number(cy) + ")");

    pm.fill(Qt::black);

    QPainter painter(&pm);
    int width = pm.width();
    int height = pm.height();

    drawAxesAndGrid(painter, width, height, gridSize);

    // Changed box selection color to blue (was red)
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

    for (const DrawnCircle &c : drawnCircles) {
        qint64 elapsed; // unused here, we're just re-rendering, not re-timing
        std::vector<std::array<QPoint, 8>> octets;

        if (c.algoName == "Polar")
            octets = computePolarCircle(c.radius, elapsed);
        else if (c.algoName == "Bresenham (Midpoint)")
            octets = computeMidpointCircle(c.radius, elapsed);
        else
            octets = computeCartesianCircle(c.radius, elapsed);

        painter.setBrush(QBrush(c.color));
        painter.setPen(QPen(Qt::white, 1));
        for (const auto &octet : octets) {
            for (const QPoint &pt : octet) {
                int drawX = originX + (c.centerX + pt.x()) * gridSize;
                int drawY = originY - (c.centerY + pt.y()) * gridSize;
                painter.drawRect(drawX, drawY, gridSize, gridSize);
            }
        }
    }

    painter.end();
    ui->frame->setPixmap(pm);
}

void MainWindow::on_clear_clicked()
{
    drawnCircles.clear();   // <-- NEW: also forget the drawing history, not just the pixels
    animTimer->stop();
    ui->time_label->clear();
    ui->center_label->setText("Centre: not selected");
    lastPoint1 = lastPoint2 = QPoint(-1, -1);
    int gridSize = ui->spinBox->value();

    QPixmap pix(ui->frame->width(), ui->frame->height());
    pix.fill(Qt::black);

    QPainter painter(&pix);
    drawAxesAndGrid(painter, pix.width(), pix.height(), gridSize);
    painter.end();

    ui->frame->setPixmap(pix);
}

/*void MainWindow::on_draw_dda_clicked()
{
    if (lastPoint1 == QPoint(-1, -1) || lastPoint2 == QPoint(-1, -1)) return;

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;

    QPixmap pm = ui->frame->pixmap();
    QPainter painter(&pm);

    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;

    int x0_cart = (lastPoint1.x() - originX) / gridSize;
    int y0_cart = -(lastPoint1.y() - originY) / gridSize;
    int x1_cart = (lastPoint2.x() - originX) / gridSize;
    int y1_cart = -(lastPoint2.y() - originY) / gridSize;

    int dx = x1_cart - x0_cart;
    int dy = y1_cart - y0_cart;

    int steps = std::max(std::abs(dx), std::abs(dy));
    if (steps == 0) steps = 1;

    float xInc = dx / static_cast<float>(steps);
    float yInc = dy / static_cast<float>(steps);

    float x = x0_cart;
    float y = y0_cart;

    std::vector<QPoint> calculatedPoints;
    calculatedPoints.reserve(steps + 1);

    // --- TIMING ONLY THE MATH ---
    QElapsedTimer timer;
    timer.start();

    for (int i = 0; i <= steps; i++) {
        calculatedPoints.push_back(QPoint(std::round(x), std::round(y)));
        x += xInc;
        y += yInc;
    }

    qint64 elapsedNano = timer.nsecsElapsed();
    qDebug() << "DDA Algorithm execution time:" << elapsedNano << "nanoseconds";
    // ----------------------------

    // Draw the calculated points
    // Changed DDA points color to orange (was green)
    painter.setBrush(QBrush(QColor(255, 165, 0)));
    painter.setPen(QPen(Qt::white, 1));
    for (const QPoint& pt : calculatedPoints) {
        int drawX = originX + pt.x() * gridSize;
        int drawY = originY - pt.y() * gridSize;

        painter.drawRect(drawX, drawY, gridSize, gridSize);

        if (gridSize >= 25) {
            painter.setPen(Qt::black);
            painter.drawText(drawX, drawY, gridSize, gridSize, Qt::AlignCenter,
                             QString("%1,%2").arg(pt.x()).arg(pt.y()));
            painter.setPen(QPen(Qt::white, 1)); // Reset pen for next rectangle
        }
    }

    painter.end();
    ui->frame->setPixmap(pm);
}*/

/*void MainWindow::on_draw_bresenham_clicked()
{
    if (lastPoint1 == QPoint(-1, -1) || lastPoint2 == QPoint(-1, -1)) return;

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;

    QPixmap pm = ui->frame->pixmap();
    QPainter painter(&pm);

    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;

    int x0_cart = (lastPoint1.x() - originX) / gridSize;
    int y0_cart = -(lastPoint1.y() - originY) / gridSize;
    int x1_cart = (lastPoint2.x() - originX) / gridSize;
    int y1_cart = -(lastPoint2.y() - originY) / gridSize;

    int dx = std::abs(x1_cart - x0_cart);
    int dy = std::abs(y1_cart - y0_cart);
    int sx = (x0_cart < x1_cart) ? 1 : -1;
    int sy = (y0_cart < y1_cart) ? 1 : -1;
    int err = dx - dy;

    int x_curr = x0_cart;
    int y_curr = y0_cart;

    std::vector<QPoint> calculatedPoints;

    // FIX: Allocate memory BEFORE starting the timer, just like DDA!
    // The number of pixels is always max(dx, dy) + 1
    calculatedPoints.reserve(std::max(dx, dy) + 1);

    // --- TIMING ONLY THE MATH ---
    QElapsedTimer timer;
    timer.start();

    while (true) {
        calculatedPoints.push_back(QPoint(x_curr, y_curr));

        if (x_curr == x1_cart && y_curr == y1_cart) break;

        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x_curr += sx;
        }
        if (e2 < dx) {
            err += dx;
            y_curr += sy;
        }
    }

    qint64 elapsedNano = timer.nsecsElapsed();
    qDebug() << "Bresenham Algorithm execution time:" << elapsedNano << "nanoseconds";
    // ----------------------------

    // Draw the calculated points
    // Changed Bresenham points to yellow (was cyan)
    painter.setBrush(QBrush(Qt::yellow));
    painter.setPen(QPen(Qt::white, 1));
    for (const QPoint& pt : calculatedPoints) {
        int drawX = originX + pt.x() * gridSize;
        int drawY = originY - pt.y() * gridSize;

        painter.drawRect(drawX, drawY, gridSize, gridSize);

        if (gridSize >= 25) {
            painter.setPen(Qt::black);
            painter.drawText(drawX, drawY, gridSize, gridSize, Qt::AlignCenter,
                             QString("%1,%2").arg(pt.x()).arg(pt.y()));
            painter.setPen(QPen(Qt::white, 1)); // Reset pen for next rectangle
        }
    }

    painter.end();
    ui->frame->setPixmap(pm);
}*/


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

std::array<QPoint, 8> MainWindow::eightWaySymmetry(int x, int y) const
{
    return {
        QPoint( x,  y), QPoint( y,  x),
        QPoint(-x,  y), QPoint(-y,  x),
        QPoint( x, -y), QPoint( y, -x),
        QPoint(-x, -y), QPoint(-y, -x)
    };
}

// (a) Polar / trigonometric algorithm: step theta from 0 to 45 degrees and
// evaluate x = r cos(theta), y = r sin(theta) directly.
std::vector<std::array<QPoint, 8>> MainWindow::computePolarCircle(int radius, qint64 &elapsedNanos) const
{
    std::vector<std::array<QPoint, 8>> octets;
    if (radius <= 0) { elapsedNanos = 0; return octets; }

    QElapsedTimer timer;
    timer.start();

    // One angular step per unit of arc length keeps the octant free of gaps.
    double increment = 1.0 / radius;
    for (double theta = 0.0; theta <= CIRCLE_PI / 4.0 + 1e-9; theta += increment) {
        int x = static_cast<int>(std::round(radius * std::cos(theta)));
        int y = static_cast<int>(std::round(radius * std::sin(theta)));
        octets.push_back(eightWaySymmetry(x, y));
    }

    elapsedNanos = timer.nsecsElapsed();
    return octets;
}

// (b) Bresenham's midpoint circle algorithm: pure integer arithmetic, no
// trig or square roots. Classic decision-parameter formulation.
std::vector<std::array<QPoint, 8>> MainWindow::computeMidpointCircle(int radius, qint64 &elapsedNanos) const
{
    std::vector<std::array<QPoint, 8>> octets;
    if (radius <= 0) { elapsedNanos = 0; return octets; }

    QElapsedTimer timer;
    timer.start();

    int x = 0;
    int y = radius;
    int p = 1 - radius;   // initial decision parameter

    octets.push_back(eightWaySymmetry(x, y));
    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        octets.push_back(eightWaySymmetry(x, y));
    }

    elapsedNanos = timer.nsecsElapsed();
    return octets;
}

// (c) Cartesian algorithm: evaluate y = sqrt(r^2 - x^2) directly for each
// integer x. Simple, but a sqrt() per point makes it the reference case for
// "why we bother with Bresenham".
std::vector<std::array<QPoint, 8>> MainWindow::computeCartesianCircle(int radius, qint64 &elapsedNanos) const
{
    std::vector<std::array<QPoint, 8>> octets;
    if (radius <= 0) { elapsedNanos = 0; return octets; }

    QElapsedTimer timer;
    timer.start();

    int limit = static_cast<int>(std::round(radius / std::sqrt(2.0)));
    for (int x = 0; x <= limit; x++) {
        double ySq = static_cast<double>(radius) * radius - static_cast<double>(x) * x;
        int y = static_cast<int>(std::round(std::sqrt(std::max(0.0, ySq))));
        octets.push_back(eightWaySymmetry(x, y));
    }

    elapsedNanos = timer.nsecsElapsed();
    return octets;
}

// Shared draw/animate/report logic used by all three algorithm slots below.
// NOTE: now takes `radius` too, purely so a finished (non-animated) circle
// can be recorded into drawnCircles for redrawCanvas() to replay later.
void MainWindow::runCircleAlgorithm(const std::vector<std::array<QPoint, 8>> &octets,
                                    qint64 elapsedNanos,
                                    const QColor &color,
                                    const QString &algoName,
                                    int radius)
{
    if (lastPoint2 == QPoint(-1, -1)) {
        ui->center_label->setText("Click on the grid to pick a centre first!");
        return;
    }
    if (octets.empty()) return;

    animTimer->stop();

    int pixelCount = static_cast<int>(octets.size()) * 8;

    qDebug() << algoName << "circle algorithm execution time:" << elapsedNanos
             << "nanoseconds, pixels drawn:" << pixelCount;

    ui->time_label->setText(ui->time_label->text() +
                            algoName + ": " + QString::number(elapsedNanos) + " ns, " +
                            QString::number(pixelCount) + " px\n");

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;

    QPixmap pm = ui->frame->pixmap();
    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;

    int cx = (lastPoint2.x() - originX) / gridSize;
    int cy = -(lastPoint2.y() - originY) / gridSize;

    // Shift every octant offset so the circle is centred on the chosen point.
    std::vector<std::array<QPoint, 8>> centered;
    centered.reserve(octets.size());
    for (const auto &octet : octets) {
        std::array<QPoint, 8> shifted;
        for (int i = 0; i < 8; i++) {
            shifted[i] = QPoint(cx + octet[i].x(), cy + octet[i].y());
        }
        centered.push_back(shifted);
    }

    if (ui->animate_checkBox->isChecked()) {
        // Reveal one octant's 8 mirrored points at a time so the symmetry is visible.
        animOctets = centered;
        animIndex = 0;
        animColor = color;
        int interval = std::max(15, 400 / static_cast<int>(centered.size()));
        animTimer->start(interval);
    } else {
        QPainter painter(&pm);
        painter.setBrush(QBrush(color));
        painter.setPen(QPen(Qt::white, 1));
        for (const auto &octet : centered) {
            for (const QPoint &pt : octet) {
                int drawX = originX + pt.x() * gridSize;
                int drawY = originY - pt.y() * gridSize;
                painter.drawRect(drawX, drawY, gridSize, gridSize);
            }
        }
        painter.end();
        ui->frame->setPixmap(pm);

        // <-- NEW: remember this circle so it survives a grid-size change
        drawnCircles.push_back({cx, cy, radius, color, algoName});
    }
}

void MainWindow::animateStep()
{
    if (animIndex >= static_cast<int>(animOctets.size())) {
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
    for (const QPoint &pt : animOctets[animIndex]) {
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
    int radius = ui->radiusBox->value();
    qint64 elapsed;
    auto octets = computePolarCircle(radius, elapsed);
    runCircleAlgorithm(octets, elapsed, QColor(255, 165, 0), "Polar", radius);
}

void MainWindow::on_draw_bresenham_circle_clicked()
{
    int radius = ui->radiusBox->value();
    qint64 elapsed;
    auto octets = computeMidpointCircle(radius, elapsed);
    runCircleAlgorithm(octets, elapsed, Qt::yellow, "Bresenham (Midpoint)", radius);
}

void MainWindow::on_draw_cartesian_clicked()
{
    int radius = ui->radiusBox->value();
    qint64 elapsed;
    auto octets = computeCartesianCircle(radius, elapsed);
    runCircleAlgorithm(octets, elapsed, QColor(0, 255, 255), "Cartesian", radius);
}