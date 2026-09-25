#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QElapsedTimer>
#include <cmath>
#include <algorithm>

// Number of times each algorithm's pixel-generation is repeated back to
// back purely for timing purposes. A single short line finishes in a
// few hundred nanoseconds, which is too small/noisy to compare
// meaningfully, so we time TIMING_REPEATS runs and report the average
// time per run. This does NOT affect what gets drawn - rendering
// happens exactly once, after the timing loop.
static constexpr int TIMING_REPEATS = 2000;

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
    if (x < 0 || y < 0 || x >= ui->frame->width() || y >= ui->frame->height())
        return;

    // NOTE: QLabel::pixmap() has TWO overloads in Qt 5.15+: a deprecated
    // one returning "const QPixmap*" and a by-value one that takes
    // Qt::ReturnByValue. Calling pixmap() with no arguments resolves to
    // the deprecated pointer overload and fails to compile against a
    // QPixmap variable, so we ask for the by-value overload explicitly.
    QPixmap pm = ui->frame->pixmap(Qt::ReturnByValue);
    if (pm.isNull()) return;
    QImage img = pm.toImage();

    int r = 0, g = 0, b = 0;
    switch (c) {
    case 0: r = g = b = 255; break; // white
    case 1: r = 255; break;         // red   - clicked endpoints
    case 2: g = 255; break;         // green - DDA line
    case 3: b = 255; break;         // blue  - Bresenham line
    }
    const QRgb colour = qRgb(r, g, b);

    // Paint a small 5x5 marker (not just a single pixel) around the
    // clicked point. A lone pixel would get silently overwritten the
    // moment a line is drawn through it (both algorithms legitimately
    // plot the exact endpoint pixel); the surrounding ring keeps the
    // clicked endpoint visibly marked in red afterwards.
    const int radius = 2;
    const int w = img.width(), h = img.height();
    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            const int px = x + dx, py = y + dy;
            if (px >= 0 && py >= 0 && px < w && py < h)
                img.setPixel(px, py, colour);
        }
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::renderPoints(const QVector<QPoint> &points, int c)
{
    QPixmap pm = ui->frame->pixmap(Qt::ReturnByValue);
    if (pm.isNull()) return;
    QImage img = pm.toImage();

    int r = 0, g = 0, b = 0;
    switch (c) {
    case 0: r = g = b = 255; break;
    case 1: r = 255; break;
    case 2: g = 255; break;
    case 3: b = 255; break;
    }
    const QRgb colour = qRgb(r, g, b);

    const int w = img.width();
    const int h = img.height();
    for (const QPoint &p : points) {
        if (p.x() >= 0 && p.y() >= 0 && p.x() < w && p.y() < h)
            img.setPixel(p.x(), p.y(), colour);
    }

    ui->frame->setPixmap(QPixmap::fromImage(img));
}

// ---------------------------------------------------------------------
// a) DDA (Digital Differential Analyzer)
//
// Uses the real-valued slope to step along the axis with the larger
// span (steps = max(|dx|,|dy|)), incrementing both x and y by a
// fractional amount each step and rounding to the nearest pixel.
// Because xInc/yInc keep their natural sign (dx/steps, dy/steps), the
// line is produced correctly no matter which of the 4 quadrants
// (p2.x</> p1.x combined with p2.y</> p1.y) the second point falls in,
// as well as the horizontal/vertical/45-degree edge cases.
// ---------------------------------------------------------------------
QVector<QPoint> MainWindow::computeDDA(const QPoint &p1, const QPoint &p2) const
{
    QVector<QPoint> points;

    const int x1 = p1.x(), y1 = p1.y();
    const int x2 = p2.x(), y2 = p2.y();
    const int dx = x2 - x1;
    const int dy = y2 - y1;

    const int steps = std::max(std::abs(dx), std::abs(dy));
    if (steps == 0) {
        points.append(QPoint(x1, y1));
        return points;
    }

    const double xInc = static_cast<double>(dx) / steps;
    const double yInc = static_cast<double>(dy) / steps;

    double x = x1;
    double y = y1;

    points.reserve(steps + 1);
    for (int i = 0; i <= steps; ++i) {
        points.append(QPoint(qRound(x), qRound(y)));
        x += xInc;
        y += yInc;
    }
    return points;
}

// ---------------------------------------------------------------------
// b) Bresenham's line drawing algorithm
//
// Classic integer-only, decision-parameter formulation, generalised to
// all 8 octants (hence all 4 quadrants):
//   - sx, sy hold the step DIRECTION (+1/-1) along x and y.
//   - dx, dy hold the (unsigned) magnitudes.
//   - whichever of dx/dy is larger decides the "driving" axis that is
//     stepped every iteration; the other axis is stepped only when the
//     accumulated error crosses zero.
// This single routine correctly reproduces all example cases from the
// assignment: shallow/steep slopes, all 4 quadrant directions, and the
// horizontal / vertical / diagonal special cases.
// ---------------------------------------------------------------------
QVector<QPoint> MainWindow::computeBresenham(const QPoint &p1, const QPoint &p2) const
{
    QVector<QPoint> points;

    int x = p1.x(), y = p1.y();
    const int x2 = p2.x(), y2 = p2.y();

    const int dx = std::abs(x2 - x);
    const int dy = std::abs(y2 - y);
    const int sx = (x < x2) ? 1 : -1;
    const int sy = (y < y2) ? 1 : -1;

    if (dx >= dy) {
        int p = 2 * dy - dx;
        const int twoDy = 2 * dy;
        const int twoDyMinusDx = 2 * (dy - dx);
        points.reserve(dx + 1);
        for (int i = 0; i <= dx; ++i) {
            points.append(QPoint(x, y));
            if (p < 0) {
                p += twoDy;
            } else {
                y += sy;
                p += twoDyMinusDx;
            }
            x += sx;
        }
    } else {
        int p = 2 * dx - dy;
        const int twoDx = 2 * dx;
        const int twoDxMinusDy = 2 * (dx - dy);
        points.reserve(dy + 1);
        for (int i = 0; i <= dy; ++i) {
            points.append(QPoint(x, y));
            if (p < 0) {
                p += twoDx;
            } else {
                x += sx;
                p += twoDxMinusDy;
            }
            y += sy;
        }
    }
    return points;
}

void MainWindow::on_clear_clicked()
{
    lastPoint1 = lastPoint2 = QPoint(-1, -1);
    QPixmap pix(ui->frame->width(), ui->frame->height());
    pix.fill(Qt::black);
    ui->frame->setPixmap(pix);
    ui->dda_time_label->setText("DDA: -");
    ui->bresenham_time_label->setText("Bresenham: -");
}

void MainWindow::on_draw_dda_clicked()
{
    if (lastPoint1 == QPoint(-1, -1) || lastPoint2 == QPoint(-1, -1)) return;

    QElapsedTimer timer;
    timer.start();
    QVector<QPoint> points;
    for (int i = 0; i < TIMING_REPEATS; ++i)
        points = computeDDA(lastPoint1, lastPoint2);
    const double elapsedMs = timer.nsecsElapsed() / 1e6 / TIMING_REPEATS;

    renderPoints(points, 2); // green
    ui->dda_time_label->setText(
        QString("DDA: %1").arg(elapsedMs, 0, 'f', 4));
}

void MainWindow::on_draw_bresenham_clicked()
{
    if (lastPoint1 == QPoint(-1, -1) || lastPoint2 == QPoint(-1, -1)) return;

    QElapsedTimer timer;
    timer.start();
    QVector<QPoint> points;
    for (int i = 0; i < TIMING_REPEATS; ++i)
        points = computeBresenham(lastPoint1, lastPoint2);
    const double elapsedMs = timer.nsecsElapsed() / 1e6 / TIMING_REPEATS;

    renderPoints(points, 3); // blue
    ui->bresenham_time_label->setText(
        QString("Bresenham: %1").arg(elapsedMs, 0, 'f', 4));
}
