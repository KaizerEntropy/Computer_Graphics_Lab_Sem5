#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QPixmap>
#include <QColorDialog>
#include <QElapsedTimer>
#include <QDebug>
#include <QSet>
#include <algorithm>
#include <cmath>

static const double SHAPE_PI = 3.14159265358979323846;

// Draws faint grid lines every `gridSize` pixels and highlights the
// row/column through the origin.
static void drawAxesAndGrid(QPainter &painter, int width, int height, int gridSize)
{
    if (gridSize <= 0) gridSize = 1;

    int originX = (width / 2 / gridSize) * gridSize;
    int originY = (height / 2 / gridSize) * gridSize;

    painter.setPen(QPen(Qt::white, 1));
    for (int x = gridSize; x < width; x += gridSize) painter.drawLine(x, 0, x, height - 1);
    for (int y = gridSize; y < height; y += gridSize) painter.drawLine(0, y, width - 1, y);

    painter.setPen(QPen(Qt::white, 1));
    painter.setBrush(QBrush(Qt::white));
    for (int x = 0; x < width; x += gridSize) painter.drawRect(x, originY, gridSize, gridSize);
    for (int y = 0; y < height; y += gridSize) painter.drawRect(originX, y, gridSize, gridSize);

    painter.setBrush(QBrush(Qt::blue));
    painter.drawRect(originX, originY, gridSize, gridSize);
}

// Screen pixel -> cartesian grid index, floor-based (not truncating '/')
// so it behaves consistently on both sides of the origin.
int MainWindow::screenToCartesian(int screenCoord, int originCoord, int gridSize)
{
    return static_cast<int>(std::floor(static_cast<double>(screenCoord - originCoord) / gridSize));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Grid Drawing - Line / Circle / Ellipse + Boundary Fill / Flood Fill");

    lastPoint1 = QPoint(-1, -1);
    lastPoint2 = QPoint(-1, -1);

    ui->algoCombo->addItems({
        "Boundary Fill (4-connected)",
        "Boundary Fill (8-connected)",
        "Flood Fill (4-connected)",
        "Flood Fill (8-connected)",
        "Scanline Fill"
    });

    ui->boundaryColorButton->setStyleSheet("background-color: white;");
    ui->fillColorButton->setStyleSheet(QString("background-color: %1;").arg(fillColor.name()));

    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &MainWindow::animateStep);

    connect(ui->frame, SIGNAL(Mouse_Pos()), this, SLOT(Mouse_Pressed()));
    connect(ui->frame, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showMousePosition(QPoint&)));

    on_spinBox_valueChanged(ui->spinBox->value());
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ---------------------------------------------------------------------
// Rendering: rebuild the whole frame from scratch every time - grid lines,
// then every non-empty cell from cellColors, then (while not in fill mode)
// a highlight box around the most recently picked point.
// ---------------------------------------------------------------------
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

    painter.setPen(Qt::NoPen);
    for (auto it = cellColors.constBegin(); it != cellColors.constEnd(); ++it) {
        const QPoint &cart = it.key();
        int drawX = originX + cart.x() * gridSize;
        int drawY = originY - cart.y() * gridSize;
        if (drawX + gridSize < 0 || drawX >= pm.width() || drawY + gridSize < 0 || drawY >= pm.height())
            continue;   // off the visible canvas, skip
        painter.fillRect(drawX, drawY, gridSize, gridSize, it.value());
    }

    if (!ui->fillModeCheckBox->isChecked() && lastPoint2 != QPoint(-1, -1)) {
        int hx = (lastPoint2.x() / gridSize) * gridSize;
        int hy = (lastPoint2.y() / gridSize) * gridSize;
        painter.setPen(QPen(Qt::yellow, 2));
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(hx, hy, gridSize, gridSize);
    }

    painter.end();
    ui->frame->setPixmap(pm);
}

// ---------------------------------------------------------------------
// Mouse handling
// ---------------------------------------------------------------------
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

    int cx = screenToCartesian(sc_x, originX, gridSize);
    int cy = -screenToCartesian(sc_y, originY, gridSize);

    ui->mouse_movement->setText("X : " + QString::number(cx) + ", Y : " + QString::number(cy));
}

void MainWindow::Mouse_Pressed()
{
    org_x = sc_x;
    org_y = sc_y;

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    int width = ui->frame->width();
    int height = ui->frame->height();
    int originX = (width / 2 / gridSize) * gridSize;
    int originY = (height / 2 / gridSize) * gridSize;
    int cx = screenToCartesian(org_x, originX, gridSize);
    int cy = -screenToCartesian(org_y, originY, gridSize);

    if (ui->fillModeCheckBox->isChecked()) {
        // In fill mode a click is a seed point, not a line endpoint / centre.
        runFill(QPoint(cx, cy));
        return;
    }

    lastPoint1 = lastPoint2;
    lastPoint2 = QPoint(org_x, org_y);

    ui->center_label->setText(QString("Point picked: (%1, %2).\nUse it as a line endpoint, or as a circle/ellipse centre.")
                                   .arg(cx).arg(cy));
    redrawCanvas();
}

// Cartesian coords of the last clicked point - used as the centre for
// circle/ellipse drawing. Returns false if nothing's been clicked yet.
bool MainWindow::getPickedCenter(int &cx, int &cy) const
{
    if (lastPoint2 == QPoint(-1, -1)) return false;

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    int width = ui->frame->width();
    int height = ui->frame->height();
    int originX = (width / 2 / gridSize) * gridSize;
    int originY = (height / 2 / gridSize) * gridSize;

    cx = screenToCartesian(lastPoint2.x(), originX, gridSize);
    cy = -screenToCartesian(lastPoint2.y(), originY, gridSize);
    return true;
}

// ---------------------------------------------------------------------
// Bresenham's line algorithm over cartesian grid-cell coordinates.
// ---------------------------------------------------------------------
std::vector<QPoint> MainWindow::bresenhamLine(int x0, int y0, int x1, int y1) const
{
    std::vector<QPoint> points;

    int dx = std::abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;

    int x = x0, y = y0;
    while (true) {
        points.push_back(QPoint(x, y));
        if (x == x1 && y == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x += sx; }
        if (e2 <= dx) { err += dx; y += sy; }
    }
    return points;
}

void MainWindow::on_draw_line_clicked()
{
    if (lastPoint1 == QPoint(-1, -1) || lastPoint2 == QPoint(-1, -1)) {
        ui->center_label->setText("Pick two points on the grid first (click twice), then press \"Draw Line\".");
        return;
    }

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    int width = ui->frame->width();
    int height = ui->frame->height();
    int originX = (width / 2 / gridSize) * gridSize;
    int originY = (height / 2 / gridSize) * gridSize;

    int x0 = screenToCartesian(lastPoint1.x(), originX, gridSize);
    int y0 = -screenToCartesian(lastPoint1.y(), originY, gridSize);
    int x1 = screenToCartesian(lastPoint2.x(), originX, gridSize);
    int y1 = -screenToCartesian(lastPoint2.y(), originY, gridSize);

    for (const QPoint &p : bresenhamLine(x0, y0, x1, y1))
        cellColors[p] = boundaryColor;

    // Require a fresh pair of clicks for the next edge.
    lastPoint1 = QPoint(-1, -1);
    lastPoint2 = QPoint(-1, -1);

    ui->center_label->setText(QString("Line drawn: (%1,%2) -> (%3,%4).\nPick two new points for the next edge.")
                                   .arg(x0).arg(y0).arg(x1).arg(y1));
    redrawCanvas();
}

// ---------------------------------------------------------------------
// Circle algorithms (ported from Assignment 2)
// ---------------------------------------------------------------------
std::array<QPoint, 8> MainWindow::eightWaySymmetry(int x, int y) const
{
    return {
        QPoint( x,  y), QPoint( y,  x),
        QPoint(-x,  y), QPoint(-y,  x),
        QPoint( x, -y), QPoint( y, -x),
        QPoint(-x, -y), QPoint(-y, -x)
    };
}

// (a) Polar / trigonometric: step theta from 0 to 45 degrees and evaluate
// x = r*cos(theta), y = r*sin(theta) directly.
std::vector<QPoint> MainWindow::computePolarCircle(int radius, qint64 &elapsedNanos) const
{
    std::vector<QPoint> points;
    if (radius <= 0) { elapsedNanos = 0; return points; }

    QElapsedTimer timer;
    timer.start();

    // One angular step per unit of arc length keeps the octant free of gaps.
    double increment = 1.0 / radius;
    for (double theta = 0.0; theta <= SHAPE_PI / 4.0 + 1e-9; theta += increment) {
        int x = static_cast<int>(std::round(radius * std::cos(theta)));
        int y = static_cast<int>(std::round(radius * std::sin(theta)));
        auto oct = eightWaySymmetry(x, y);
        points.insert(points.end(), oct.begin(), oct.end());
    }

    elapsedNanos = timer.nsecsElapsed();
    return points;
}

// (b) Bresenham's midpoint circle algorithm: pure integer arithmetic, no
// trig or square roots.
std::vector<QPoint> MainWindow::computeMidpointCircle(int radius, qint64 &elapsedNanos) const
{
    std::vector<QPoint> points;
    if (radius <= 0) { elapsedNanos = 0; return points; }

    QElapsedTimer timer;
    timer.start();

    int x = 0;
    int y = radius;
    int p = 1 - radius;   // initial decision parameter

    auto oct = eightWaySymmetry(x, y);
    points.insert(points.end(), oct.begin(), oct.end());

    while (x < y) {
        x++;
        if (p < 0) {
            p += 2 * x + 1;
        } else {
            y--;
            p += 2 * (x - y) + 1;
        }
        oct = eightWaySymmetry(x, y);
        points.insert(points.end(), oct.begin(), oct.end());
    }

    elapsedNanos = timer.nsecsElapsed();
    return points;
}

// ---------------------------------------------------------------------
// Ellipse algorithms (ported from Assignment 3)
// ---------------------------------------------------------------------
std::array<QPoint, 4> MainWindow::fourWaySymmetry(int x, int y) const
{
    return {
        QPoint( x,  y),
        QPoint(-x,  y),
        QPoint( x, -y),
        QPoint(-x, -y)
    };
}

// (a) Polar / trigonometric: step theta from 0 to 90 degrees (fixed
// resolution) and evaluate x = rx*cos(theta), y = ry*sin(theta) directly.
std::vector<QPoint> MainWindow::computePolarEllipse(int rx, int ry, qint64 &elapsedNanos) const
{
    std::vector<QPoint> points;
    if (rx <= 0 || ry <= 0) { elapsedNanos = 0; return points; }

    QElapsedTimer timer;
    timer.start();

    const double angleStepDegrees = 2.0;   // fixed resolution, independent of rx/ry
    double increment = angleStepDegrees * SHAPE_PI / 180.0;
    for (double theta = 0.0; theta <= SHAPE_PI / 2.0 + 1e-9; theta += increment) {
        int x = static_cast<int>(std::round(rx * std::cos(theta)));
        int y = static_cast<int>(std::round(ry * std::sin(theta)));
        auto quad = fourWaySymmetry(x, y);
        points.insert(points.end(), quad.begin(), quad.end());
    }

    elapsedNanos = timer.nsecsElapsed();
    return points;
}

// (b) Bresenham's midpoint ellipse algorithm: pure integer/decision-parameter
// arithmetic, no trig. Classic two-region formulation (Foley/van Dam).
std::vector<QPoint> MainWindow::computeMidpointEllipse(int rx, int ry, qint64 &elapsedNanos) const
{
    std::vector<QPoint> points;
    if (rx <= 0 || ry <= 0) { elapsedNanos = 0; return points; }

    QElapsedTimer timer;
    timer.start();

    long long rx2 = static_cast<long long>(rx) * rx;
    long long ry2 = static_cast<long long>(ry) * ry;
    long long twoRx2 = 2 * rx2;
    long long twoRy2 = 2 * ry2;

    int x = 0;
    int y = ry;
    long long px = 0;
    long long py = twoRx2 * y;

    auto quad = fourWaySymmetry(x, y);
    points.insert(points.end(), quad.begin(), quad.end());

    // --- Region 1: slope magnitude < 1 (step in x) ---
    double p1 = ry2 - rx2 * ry + 0.25 * rx2;
    while (px < py) {
        x++;
        px += twoRy2;
        if (p1 < 0) {
            p1 += ry2 + px;
        } else {
            y--;
            py -= twoRx2;
            p1 += ry2 + px - py;
        }
        quad = fourWaySymmetry(x, y);
        points.insert(points.end(), quad.begin(), quad.end());
    }

    // --- Region 2: slope magnitude >= 1 (step in y) ---
    double p2 = ry2 * (x + 0.5) * (x + 0.5) + rx2 * (y - 1) * (y - 1) - rx2 * ry2;
    while (y > 0) {
        y--;
        py -= twoRx2;
        if (p2 > 0) {
            p2 += rx2 - py;
        } else {
            x++;
            px += twoRy2;
            p2 += rx2 - py + px;
        }
        quad = fourWaySymmetry(x, y);
        points.insert(points.end(), quad.begin(), quad.end());
    }

    elapsedNanos = timer.nsecsElapsed();
    return points;
}

// ---------------------------------------------------------------------
// Shared plumbing for circle/ellipse: stamp the outline into cellColors
// (in the boundary colour) so it becomes fillable exactly like any other
// shape drawn on this grid, report timing, then redraw.
// ---------------------------------------------------------------------
void MainWindow::stampOutline(const std::vector<QPoint> &points, int cx, int cy,
                               const QString &shapeName, const QString &algoName,
                               qint64 elapsedNanos)
{
    if (points.empty()) return;

    animTimer->stop();

    for (const QPoint &p : points)
        cellColors[QPoint(cx + p.x(), cy + p.y())] = boundaryColor;

    qDebug() << shapeName << algoName << "- points stamped:" << points.size()
             << ", time:" << elapsedNanos << "ns";

    ui->center_label->setText(QString("%1 (%2)\nCentre: (%3, %4)\nOutline points: %5\nTime: %6 ns\n\nTick \"Fill mode\" and click inside to fill it.")
                                   .arg(shapeName, algoName)
                                   .arg(cx).arg(cy)
                                   .arg(points.size())
                                   .arg(elapsedNanos));

    redrawCanvas();
}

void MainWindow::on_drawCirclePolar_clicked()
{
    int cx, cy;
    if (!getPickedCenter(cx, cy)) {
        ui->center_label->setText("Click on the grid to pick a centre first!");
        return;
    }
    int radius = ui->circleRadiusBox->value();
    qint64 elapsed;
    auto pts = computePolarCircle(radius, elapsed);
    stampOutline(pts, cx, cy, "Circle", "Polar", elapsed);
}

void MainWindow::on_drawCircleBresenham_clicked()
{
    int cx, cy;
    if (!getPickedCenter(cx, cy)) {
        ui->center_label->setText("Click on the grid to pick a centre first!");
        return;
    }
    int radius = ui->circleRadiusBox->value();
    qint64 elapsed;
    auto pts = computeMidpointCircle(radius, elapsed);
    stampOutline(pts, cx, cy, "Circle", "Bresenham (Midpoint)", elapsed);
}

void MainWindow::on_drawEllipsePolar_clicked()
{
    int cx, cy;
    if (!getPickedCenter(cx, cy)) {
        ui->center_label->setText("Click on the grid to pick a centre first!");
        return;
    }
    int rx = ui->ellipseRxBox->value();
    int ry = ui->ellipseRyBox->value();
    qint64 elapsed;
    auto pts = computePolarEllipse(rx, ry, elapsed);
    stampOutline(pts, cx, cy, "Ellipse", "Polar", elapsed);
}

void MainWindow::on_drawEllipseBresenham_clicked()
{
    int cx, cy;
    if (!getPickedCenter(cx, cy)) {
        ui->center_label->setText("Click on the grid to pick a centre first!");
        return;
    }
    int rx = ui->ellipseRxBox->value();
    int ry = ui->ellipseRyBox->value();
    qint64 elapsed;
    auto pts = computeMidpointEllipse(rx, ry, elapsed);
    stampOutline(pts, cx, cy, "Ellipse", "Bresenham (Midpoint)", elapsed);
}

// ---------------------------------------------------------------------
void MainWindow::on_clear_clicked()
{
    animTimer->stop();
    cellColors.clear();
    lastPoint1 = lastPoint2 = QPoint(-1, -1);
    ui->center_label->setText("Canvas cleared.");
    redrawCanvas();
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

void MainWindow::on_boundaryColorButton_clicked()
{
    QColor c = QColorDialog::getColor(boundaryColor, this, "Select Boundary Colour");
    if (c.isValid()) {
        boundaryColor = c;
        ui->boundaryColorButton->setStyleSheet(QString("background-color: %1;").arg(c.name()));
    }
}

void MainWindow::on_fillColorButton_clicked()
{
    QColor c = QColorDialog::getColor(fillColor, this, "Select Fill Colour");
    if (c.isValid()) {
        fillColor = c;
        ui->fillColorButton->setStyleSheet(QString("background-color: %1;").arg(c.name()));
    }
}

// ---------------------------------------------------------------------
// (a) Boundary fill: spread to 4/8 neighbours as long as a cell is
// neither the boundary colour nor already the fill colour.
// ---------------------------------------------------------------------
std::vector<QPoint> MainWindow::boundaryFill(int x, int y, const QColor &fillC, const QColor &boundaryC,
                                              bool eightConnected,
                                              int minX, int maxX, int minY, int maxY,
                                              qint64 &elapsedNanos)
{
    std::vector<QPoint> order;
    QElapsedTimer timer;
    timer.start();

    QPoint seed(x, y);
    if (x < minX || x > maxX || y < minY || y > maxY) { elapsedNanos = timer.nsecsElapsed(); return order; }

    QColor seedColor = cellColors.value(seed, Qt::black);
    if (seedColor == boundaryC || seedColor == fillC) {
        elapsedNanos = timer.nsecsElapsed();
        return order;   // seed sits on the boundary, or the region is already filled
    }

    static const int dx4[4] = { 1, -1, 0, 0 };
    static const int dy4[4] = { 0, 0, 1, -1 };
    static const int dx8[8] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    static const int dy8[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };
    const int neighborCount = eightConnected ? 8 : 4;

    QSet<QPoint> visited;
    std::vector<QPoint> stack;
    stack.push_back(seed);
    visited.insert(seed);

    while (!stack.empty()) {
        QPoint p = stack.back();
        stack.pop_back();

        QColor c = cellColors.value(p, Qt::black);
        if (c == boundaryC || c == fillC) continue;

        cellColors[p] = fillC;
        order.push_back(p);

        for (int i = 0; i < neighborCount; i++) {
            int nx = p.x() + (eightConnected ? dx8[i] : dx4[i]);
            int ny = p.y() + (eightConnected ? dy8[i] : dy4[i]);
            if (nx < minX || nx > maxX || ny < minY || ny > maxY) continue;
            QPoint np(nx, ny);
            if (visited.contains(np)) continue;
            visited.insert(np);
            stack.push_back(np);
        }
    }

    elapsedNanos = timer.nsecsElapsed();
    return order;
}

// ---------------------------------------------------------------------
// (b) Flood fill: spread to 4/8 neighbours as long as a cell still has the
// *original* colour that was under the seed - regardless of what that
// colour is.
// ---------------------------------------------------------------------
std::vector<QPoint> MainWindow::floodFill(int x, int y, const QColor &fillC,
                                           bool eightConnected,
                                           int minX, int maxX, int minY, int maxY,
                                           qint64 &elapsedNanos)
{
    std::vector<QPoint> order;
    QElapsedTimer timer;
    timer.start();

    QPoint seed(x, y);
    if (x < minX || x > maxX || y < minY || y > maxY) { elapsedNanos = timer.nsecsElapsed(); return order; }

    QColor oldColor = cellColors.value(seed, Qt::black);
    if (oldColor == fillC) {
        elapsedNanos = timer.nsecsElapsed();
        return order;   // already the fill colour, nothing to replace
    }

    static const int dx4[4] = { 1, -1, 0, 0 };
    static const int dy4[4] = { 0, 0, 1, -1 };
    static const int dx8[8] = { 1, -1, 0, 0, 1, 1, -1, -1 };
    static const int dy8[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };
    const int neighborCount = eightConnected ? 8 : 4;

    QSet<QPoint> visited;
    std::vector<QPoint> stack;
    stack.push_back(seed);
    visited.insert(seed);

    while (!stack.empty()) {
        QPoint p = stack.back();
        stack.pop_back();

        if (cellColors.value(p, Qt::black) != oldColor) continue;

        cellColors[p] = fillC;
        order.push_back(p);

        for (int i = 0; i < neighborCount; i++) {
            int nx = p.x() + (eightConnected ? dx8[i] : dx4[i]);
            int ny = p.y() + (eightConnected ? dy8[i] : dy4[i]);
            if (nx < minX || nx > maxX || ny < minY || ny > maxY) continue;
            QPoint np(nx, ny);
            if (visited.contains(np)) continue;
            visited.insert(np);
            stack.push_back(np);
        }
    }

    elapsedNanos = timer.nsecsElapsed();
    return order;
}

// ---------------------------------------------------------------------
// (c) Scanline fill: for each row y, walk left to right and record every
// x where the cell is boundary-coloured, collapsing consecutive boundary
// cells into a single "crossing" (so a horizontal edge or a shared vertex
// between two edges only counts once, the way the classic scanline
// algorithm treats an edge list). Crossings are then paired up left to
// right - even-odd rule - and every non-boundary cell strictly between a
// pair is filled. No seed/connectivity search is needed since a row's
// membership is decided purely by counting crossings, so a single pass
// over the bounded region fills the whole (closed) polygon in one go.
// ---------------------------------------------------------------------
std::vector<QPoint> MainWindow::scanlineFill(const QColor &fillC, const QColor &boundaryC,
                                              int minX, int maxX, int minY, int maxY,
                                              qint64 &elapsedNanos)
{
    std::vector<QPoint> order;
    QElapsedTimer timer;
    timer.start();

    for (int y = minY; y <= maxY; y++) {
        std::vector<int> crossings;
        bool inRun = false;

        for (int x = minX; x <= maxX; x++) {
            bool isBoundary = (cellColors.value(QPoint(x, y), Qt::black) == boundaryC);
            if (isBoundary && !inRun) {
                crossings.push_back(x);   // start of a new boundary run = one crossing
                inRun = true;
            } else if (!isBoundary) {
                inRun = false;
            }
        }

        for (size_t k = 0; k + 1 < crossings.size(); k += 2) {
            int xStart = crossings[k];
            int xEnd = crossings[k + 1];
            for (int x = xStart + 1; x < xEnd; x++) {
                QPoint p(x, y);
                if (cellColors.value(p, Qt::black) == boundaryC) continue;
                cellColors[p] = fillC;
                order.push_back(p);
            }
        }
    }

    elapsedNanos = timer.nsecsElapsed();
    return order;
}

// ---------------------------------------------------------------------
// Runs whichever algorithm is selected in algoCombo, from the clicked
// seed cell. Bounds the search to whatever's currently visible on the
// canvas so an unclosed shape can't spread forever.
// ---------------------------------------------------------------------
void MainWindow::runFill(QPoint seedCartesian)
{
    animTimer->stop();

    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    int width = ui->frame->width();
    int height = ui->frame->height();
    int originX = (width / 2 / gridSize) * gridSize;
    int originY = (height / 2 / gridSize) * gridSize;

    int minX = screenToCartesian(0, originX, gridSize);
    int maxX = screenToCartesian(width - 1, originX, gridSize);
    int minY = -screenToCartesian(height - 1, originY, gridSize);
    int maxY = -screenToCartesian(0, originY, gridSize);

    QHash<QPoint, QColor> before = cellColors;   // snapshot for animation replay

    qint64 elapsedNanos = 0;
    std::vector<QPoint> order;
    int algoIndex = ui->algoCombo->currentIndex();

    switch (algoIndex) {
    case 0: order = boundaryFill(seedCartesian.x(), seedCartesian.y(), fillColor, boundaryColor, false, minX, maxX, minY, maxY, elapsedNanos); break;
    case 1: order = boundaryFill(seedCartesian.x(), seedCartesian.y(), fillColor, boundaryColor, true,  minX, maxX, minY, maxY, elapsedNanos); break;
    case 2: order = floodFill(seedCartesian.x(), seedCartesian.y(), fillColor, false, minX, maxX, minY, maxY, elapsedNanos); break;
    case 3: order = floodFill(seedCartesian.x(), seedCartesian.y(), fillColor, true,  minX, maxX, minY, maxY, elapsedNanos); break;
    case 4: order = scanlineFill(fillColor, boundaryColor, minX, maxX, minY, maxY, elapsedNanos); break;
    default: break;
    }

    qDebug() << ui->algoCombo->currentText() << "- cells filled:" << order.size() << ", time:" << elapsedNanos << "ns";

    if (order.empty()) {
        ui->center_label->setText(ui->algoCombo->currentText() +
                                   QString("\nNo cells filled at seed (%1, %2) -\nit's on the boundary, out of range,\nor already the fill colour.")
                                       .arg(seedCartesian.x()).arg(seedCartesian.y()));
        return;
    }

    ui->center_label->setText(QString("%1\nSeed: (%2, %3)\nCells filled: %4\nTime: %5 ns")
                                   .arg(ui->algoCombo->currentText())
                                   .arg(seedCartesian.x()).arg(seedCartesian.y())
                                   .arg(order.size())
                                   .arg(elapsedNanos));

    if (ui->animateCheckBox->isChecked()) {
        cellColors = before;   // revert to pre-fill state, then replay the fill order
        redrawCanvas();
        animOrder = order;
        animIndex = 0;
        animTimer->start(8);
    } else {
        redrawCanvas();
    }
}

// One playback tick: colour in a batch of cells (not just one) so fills of
// thousands of cells still finish in a reasonable amount of screen time.
void MainWindow::animateStep()
{
    if (animIndex >= static_cast<int>(animOrder.size())) {
        animTimer->stop();
        return;
    }

    int batch = static_cast<int>(std::max<size_t>(1, animOrder.size() / 200));
    for (int i = 0; i < batch && animIndex < static_cast<int>(animOrder.size()); i++, animIndex++) {
        cellColors[animOrder[animIndex]] = fillColor;
    }

    redrawCanvas();
}
