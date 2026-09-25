#include "mainwindow.h"
#include <QThread>
#include <cmath>
#include <algorithm>

// ===========================================================================
// GridWidget
// ===========================================================================
GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(600, 600);
    setMouseTracking(false);
    resetCanvas();
}

void GridWidget::resetCanvas()
{
    canvas = QPixmap(size());
    canvas.fill(Qt::white);
    // full grid + axis rendering happens once the widget has a real size,
    // in resizeEvent() -> drawAxes()
}

QPoint GridWidget::gridToScreen(const QPoint &gridPt) const
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    // top-left corner of the cell representing raster pixel (X, Y)
    int screenX = centerX + gridPt.x() * gridOffset;
    int screenY = centerY - (gridPt.y() + 1) * gridOffset;
    return QPoint(screenX, screenY);
}

QPoint GridWidget::screenToGrid(const QPoint &screenPt) const
{
    int centerX = width() / 2;
    int centerY = height() / 2;
    int X = (int)std::floor((screenPt.x() - centerX) * 1.0 / gridOffset);
    int Y = (int)std::floor((centerY - screenPt.y()) * 1.0 / gridOffset);
    return QPoint(X, Y);
}

void GridWidget::colorPoint(const QPoint &gridPt, const QColor &color)
{
    QPoint topLeft = gridToScreen(gridPt);
    QPainter p(&canvas);
    p.fillRect(topLeft.x(), topLeft.y(), gridOffset, gridOffset, color);
    p.end();
    update();
}

void GridWidget::drawGridAndAxes()
{
    // 1) faint reference grid lines (thin, screen-pixel lines -- just a visual
    //    aid so you can count cells; not part of the raster "content")
    {
        QPainter p(&canvas);
        QPen thin(QColor(225, 225, 225));
        thin.setWidth(1);
        p.setPen(thin);

        int centerX = width() / 2;
        int centerY = height() / 2;

        for (int x = centerX % gridOffset; x < width(); x += gridOffset)
            p.drawLine(x, 0, x, height());
        for (int y = centerY % gridOffset; y < height(); y += gridOffset)
            p.drawLine(0, y, width(), y);
    }

    // 2) the axes themselves: coloured in RED, one whole raster pixel at a
    //    time via colorPoint() -- exactly the same mechanism used to plot
    //    DDA/Bresenham points -- so the axis is "pixelated" like everything
    //    else on the grid, not a thin painter stroke.
    QColor axisColor(220, 0, 0);

    QPoint leftEdgeGrid  = screenToGrid(QPoint(0, height() / 2));
    QPoint rightEdgeGrid = screenToGrid(QPoint(width(), height() / 2));
    QPoint topEdgeGrid   = screenToGrid(QPoint(width() / 2, 0));
    QPoint bottomEdgeGrid= screenToGrid(QPoint(width() / 2, height()));

    for (int x = leftEdgeGrid.x() - 1; x <= rightEdgeGrid.x() + 1; x++)
        colorPoint(QPoint(x, 0), axisColor);      // X axis, pixel by pixel
    for (int y = bottomEdgeGrid.y() - 1; y <= topEdgeGrid.y() + 1; y++)
        colorPoint(QPoint(0, y), axisColor);      // Y axis, pixel by pixel

    update();
}

void GridWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, canvas);
}

void GridWidget::resizeEvent(QResizeEvent *)
{
    canvas = QPixmap(size());
    canvas.fill(Qt::white);
    drawGridAndAxes();
}

void GridWidget::mousePressEvent(QMouseEvent *event)
{
    QPoint gridPt = screenToGrid(event->pos());
    emit gridPointClicked(gridPt);
}

// ===========================================================================
// MainWindow::myTimer
// ===========================================================================
MainWindow::myTimer::myTimer(QString algoName, MainWindow *mainWindow)
{
    this->algoName = algoName;
    this->mainWindow = mainWindow;
    timer.start();
}

void MainWindow::myTimer::measureTime()
{
    qint64 elapsedNs = timer.nsecsElapsed();
    double elapsedMs = elapsedNs / 1000000.0;
    mainWindow->timeMessage->setText(QString("By ") + algoName + QString(" drawing algorithm"));
    mainWindow->timeTaken->display(elapsedMs);
    mainWindow->lastElapsedMs = elapsedMs;   // stashed so callers (e.g. "Both") can read it
}

// ===========================================================================
// MainWindow
// ===========================================================================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupUi();
    setupColorPalette();
    setWindowTitle("Raster Line Drawing — DDA vs Bresenham");
    resize(900, 700);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUi()
{
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    workArea = new GridWidget(central);
    connect(workArea, &GridWidget::gridPointClicked, this, &MainWindow::on_gridPointClicked);
    mainLayout->addWidget(workArea, 1);

    // ---- manual coordinate entry row: (X1,Y1) -> (X2,Y2) ----
    QHBoxLayout *coordLayout = new QHBoxLayout();
    const int coordRange = 500; // grid coords can go well beyond the visible area

    x1Spin = new QSpinBox(central);
    y1Spin = new QSpinBox(central);
    x2Spin = new QSpinBox(central);
    y2Spin = new QSpinBox(central);
    for (QSpinBox *sb : {x1Spin, y1Spin, x2Spin, y2Spin})
        sb->setRange(-coordRange, coordRange);
    x2Spin->setValue(5);
    y2Spin->setValue(5);

    setPointsButton = new QPushButton("Set Points", central);
    connect(setPointsButton, &QPushButton::clicked, this, &MainWindow::on_SetPoints_clicked);

    coordLayout->addWidget(new QLabel("X1:", central));
    coordLayout->addWidget(x1Spin);
    coordLayout->addWidget(new QLabel("Y1:", central));
    coordLayout->addWidget(y1Spin);
    coordLayout->addSpacing(12);
    coordLayout->addWidget(new QLabel("X2:", central));
    coordLayout->addWidget(x2Spin);
    coordLayout->addWidget(new QLabel("Y2:", central));
    coordLayout->addWidget(y2Spin);
    coordLayout->addWidget(setPointsButton);
    coordLayout->addStretch(1);
    mainLayout->addLayout(coordLayout);

    QHBoxLayout *infoLayout = new QHBoxLayout();
    pointsLabel = new QLabel("Click two points on the grid, or type coordinates above and press Set Points.", central);
    timeMessage = new QLabel("No algorithm run yet.", central);
    timeTaken = new QLCDNumber(central);
    timeTaken->setDigitCount(8);
    timeTaken->setSegmentStyle(QLCDNumber::Flat);
    timeTaken->display(0.0);
    QLabel *msLabel = new QLabel("ms", central);

    infoLayout->addWidget(pointsLabel, 2);
    infoLayout->addWidget(timeMessage, 1);
    infoLayout->addWidget(timeTaken, 1);
    infoLayout->addWidget(msLabel);
    mainLayout->addLayout(infoLayout);

    comparisonLabel = new QLabel("", central);
    comparisonLabel->setStyleSheet("font-weight: bold;");
    mainLayout->addWidget(comparisonLabel);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    ddaButton = new QPushButton("1) Draw DDA (pink)", central);
    bresenhamButton = new QPushButton("2) Draw Bresenham (green)", central);
    bothButton = new QPushButton("3) Draw Both (compare)", central);
    clearButton = new QPushButton("Clear Grid", central);
    connect(ddaButton, &QPushButton::clicked, this, &MainWindow::on_DDADraw_clicked);
    connect(bresenhamButton, &QPushButton::clicked, this, &MainWindow::on_BresenhamLineDraw_clicked);
    connect(bothButton, &QPushButton::clicked, this, &MainWindow::on_BothDraw_clicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::on_ClearGrid_clicked);
    buttonLayout->addWidget(ddaButton);
    buttonLayout->addWidget(bresenhamButton);
    buttonLayout->addWidget(bothButton);
    buttonLayout->addWidget(clearButton);
    mainLayout->addLayout(buttonLayout);

    setCentralWidget(central);
}

void MainWindow::setupColorPalette()
{
    colorPalette["pink"]  = QColor(230, 30, 140);   // DDA
    colorPalette["green"] = QColor(20, 170, 60);    // Bresenham
    colorPalette["blue"]  = QColor(0, 0, 255);      // clicked endpoints
    colorPalette["red"]   = QColor(220, 0, 0);      // axes (pixelated)
}

void MainWindow::updatePointsLabel()
{
    if (points.isEmpty()) {
        pointsLabel->setText("Click two points on the grid, or type coordinates above and press Set Points.");
    } else if (points.size() == 1) {
        QPoint p = points.first();
        pointsLabel->setText(QString("Start point set at (%1, %2). Click the end point.")
                              .arg(p.x()).arg(p.y()));
    } else {
        pointsLabel->setText(QString("Line locked: (%1, %2) -> (%3, %4). "
                                      "Pick 1) DDA, 2) Bresenham, or 3) Both -- "
                                      "you can press any of them repeatedly on this pair. "
                                      "Click again to start a new line.")
                              .arg(start.x()).arg(start.y()).arg(end.x()).arg(end.y()));
    }
}

void MainWindow::on_gridPointClicked(QPoint gridPt)
{
    // A pair that's already complete (2 points) is "locked in" for drawing.
    // A further click always means "start a brand new line" -- it must NOT
    // silently reuse a stale point via points[n-2]/points[n-1] on an
    // ever-growing vector (that was the root cause of lines appearing to
    // start from (0,0)/garbage indices). So: discard the old pair first.
    if (points.size() >= 2) {
        points.clear();
        pairReady = false;
        comparisonLabel->setText("");
    }

    points.push_back(gridPt);
    workArea->colorPoint(gridPt, colorPalette["blue"]);

    if (points.size() == 1) {
        x1Spin->setValue(gridPt.x());
        y1Spin->setValue(gridPt.y());
    } else if (points.size() == 2) {
        start = points[0];
        end   = points[1];
        pairReady = true;
        x2Spin->setValue(gridPt.x());
        y2Spin->setValue(gridPt.y());
    }
    updatePointsLabel();
}

void MainWindow::on_SetPoints_clicked()
{
    QPoint p1(x1Spin->value(), y1Spin->value());
    QPoint p2(x2Spin->value(), y2Spin->value());

    // start a fresh pair, same bookkeeping as two grid clicks
    points.clear();
    comparisonLabel->setText("");

    points.push_back(p1);
    workArea->colorPoint(p1, colorPalette["blue"]);
    points.push_back(p2);
    workArea->colorPoint(p2, colorPalette["blue"]);

    start = p1;
    end = p2;
    pairReady = true;
    updatePointsLabel();
}

void MainWindow::delay(int ms)
{
    // small animation delay that keeps the UI responsive
    QElapsedTimer t;
    t.start();
    while (t.elapsed() < ms) {
        QApplication::processEvents(QEventLoop::AllEvents, ms);
    }
}

void MainWindow::color_pts(const QVector<QPoint> &linePts, const QColor &color)
{
    for (const QPoint &currentPt : linePts) {
        workArea->colorPoint(currentPt, color);
        delay(1);
    }
}

// ---------------------------------------------------------------------------
// DDA — works for all 4 quadrants because xinc/yinc carry the sign of dx/dy.
// ---------------------------------------------------------------------------
QVector<QPoint> MainWindow::generate_dda_linePts(QPoint startPt, QPoint endPt)
{
    int startX = startPt.x(), startY = startPt.y();
    int endX = endPt.x(), endY = endPt.y();

    int dx = endX - startX, dy = endY - startY;

    if (dx == 0 && dy == 0) {
        QVector<QPoint> single;
        single.push_back(startPt);
        return single;
    }

    int steps = std::max(std::abs(dx), std::abs(dy));

    float xinc = (dx * 1.0f) / steps;
    float yinc = (dy * 1.0f) / steps;
    float curX = startX, curY = startY;

    myTimer t("DDA Line", this);
    QVector<QPoint> linePts;
    linePts.push_back(startPt);

    int curXi = startX, curYi = startY;
    while (curXi != endX || curYi != endY) {
        curX = curX + xinc;
        curY = curY + yinc;
        curXi = (int)std::round(curX);
        curYi = (int)std::round(curY);
        linePts.push_back(QPoint(curXi, curYi));
    }
    t.measureTime();
    return linePts;
}

// ---------------------------------------------------------------------------
// Bresenham — integer-only, generalized to all 4 quadrants via stepX/stepY.
// ---------------------------------------------------------------------------
QVector<QPoint> MainWindow::generate_bresenham_linePts(QPoint startPt, QPoint endPt)
{
    int startX = startPt.x(), startY = startPt.y();
    int endX = endPt.x(), endY = endPt.y();

    int dx = endX - startX;
    int dy = endY - startY;

    int stepX = (dx >= 0) ? 1 : -1;
    int stepY = (dy >= 0) ? 1 : -1;

    dx = std::abs(dx);
    dy = std::abs(dy);

    myTimer t("Bresenham Line", this);
    QVector<QPoint> linePts;
    linePts.push_back(startPt);

    int x = startX, y = startY;

    if (dx == 0 && dy == 0) {
        t.measureTime();
        return linePts;
    }

    if (dx >= dy) {
        // shallow slope (|m| <= 1): step along x, decide y
        int p = 2 * dy - dx;
        int twoDy = 2 * dy;
        int twoDyMinusDx = 2 * (dy - dx);

        for (int i = 0; i < dx; i++) {
            x += stepX;
            if (p < 0) {
                p += twoDy;
            } else {
                y += stepY;
                p += twoDyMinusDx;
            }
            linePts.push_back(QPoint(x, y));
        }
    } else {
        // steep slope (|m| > 1): step along y, decide x
        int p = 2 * dx - dy;
        int twoDx = 2 * dx;
        int twoDxMinusDy = 2 * (dx - dy);

        for (int i = 0; i < dy; i++) {
            y += stepY;
            if (p < 0) {
                p += twoDx;
            } else {
                x += stepX;
                p += twoDxMinusDy;
            }
            linePts.push_back(QPoint(x, y));
        }
    }

    t.measureTime();
    return linePts;
}

// ---------------------------------------------------------------------------
// Slots
// ---------------------------------------------------------------------------
void MainWindow::on_DDADraw_clicked()
{
    if (!pairReady) {
        pointsLabel->setText("Select two points on the grid first (click twice).");
        return;
    }
    QVector<QPoint> linePts = generate_dda_linePts(start, end);
    line = linePts;
    color_pts(linePts, colorPalette["pink"]);
}

void MainWindow::on_BresenhamLineDraw_clicked()
{
    if (!pairReady) {
        pointsLabel->setText("Select two points on the grid first (click twice).");
        return;
    }
    QVector<QPoint> linePts = generate_bresenham_linePts(start, end);
    line = linePts;
    color_pts(linePts, colorPalette["green"]);
}

void MainWindow::on_BothDraw_clicked()
{
    if (!pairReady) {
        pointsLabel->setText("Select two points on the grid first (click twice).");
        return;
    }

    QVector<QPoint> ddaPts = generate_dda_linePts(start, end);
    double ddaMs = lastElapsedMs;
    color_pts(ddaPts, colorPalette["pink"]);

    QVector<QPoint> bresPts = generate_bresenham_linePts(start, end);
    double bresMs = lastElapsedMs;
    color_pts(bresPts, colorPalette["green"]);

    line = bresPts;
    comparisonLabel->setText(
        QString("DDA: %1 ms (pink)   |   Bresenham: %2 ms (green)")
            .arg(ddaMs, 0, 'f', 4)
            .arg(bresMs, 0, 'f', 4));
}

void MainWindow::on_ClearGrid_clicked()
{
    points.clear();
    line.clear();
    pairReady = false;
    workArea->canvas.fill(Qt::white);
    workArea->drawGridAndAxes();
    timeMessage->setText("No algorithm run yet.");
    timeTaken->display(0.0);
    comparisonLabel->setText("");
    updatePointsLabel();
}
