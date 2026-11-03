#include "canvas.h"
#include <QPainter>
#include <QPen>

Canvas::Canvas(QWidget *parent) : QWidget(parent),
    gridSize(10), showGrid(true), showAxes(true), continuousC1(true), curveCalculated(false)
{
    setMouseTracking(true); // To track mouse movement without pressing
    setMinimumSize(400, 400);
}

void Canvas::setGridSize(int size)
{
    if (size <= 0) size = 1;
    gridSize = size;
    redraw();
}

void Canvas::setShowGrid(bool show)
{
    showGrid = show;
    redraw();
}

void Canvas::setShowAxes(bool show)
{
    showAxes = show;
    redraw();
}

void Canvas::setContinuousC1(bool c1)
{
    continuousC1 = c1;
    curveCalculated = false; // Need to recalculate curve when mode changes
    redraw();
}

void Canvas::addControlPoint(QPoint cartesianPoint)
{
    controlPoints.append(cartesianPoint);
    emit pointAdded(cartesianPoint);
    
    // Automatically recompute curve if possible
    int n = controlPoints.size();
    if (n >= 4) {
        if (continuousC1) {
            if ((n - 4) % 2 == 0) computeBezierCurve();
        } else {
            if (n % 4 == 0) computeBezierCurve();
        }
    }
    redraw();
}

void Canvas::removeLastControlPoint()
{
    if (!controlPoints.isEmpty()) {
        controlPoints.removeLast();
        curveCalculated = false; // Reset calculation, will recompute on next redraw if valid
        
        // Recompute if still valid
        int n = controlPoints.size();
        if (n >= 4) {
            if (continuousC1 && (n - 4) % 2 == 0) computeBezierCurve();
            else if (!continuousC1 && n % 4 == 0) computeBezierCurve();
        }
        
        redraw();
    }
}

void Canvas::clearCanvas()
{
    controlPoints.clear();
    curvePoints.clear();
    curveCalculated = false;
    redraw();
}

QPointF Canvas::evalCubicBezier(const QPointF &P0, const QPointF &P1,
                               const QPointF &P2, const QPointF &P3,
                               double t)
{
    double u = 1.0 - t;
    double u2 = u * u;
    double u3 = u2 * u;
    double t2 = t * t;
    double t3 = t2 * t;

    double x = u3 * P0.x() + 3.0 * u2 * t * P1.x() + 3.0 * u * t2 * P2.x() + t3 * P3.x();
    double y = u3 * P0.y() + 3.0 * u2 * t * P1.y() + 3.0 * u * t2 * P2.y() + t3 * P3.y();
    return QPointF(x, y);
}

void Canvas::computeBezierCurve()
{
    curvePoints.clear();
    int n = controlPoints.size();
    if (n < 4) return;
    
    const int numSteps = 200;
    
    if (continuousC1) {
        // C1 continuous mode (2 extra points per new segment after first 4)
        int numSegments = (n - 4) / 2 + 1;
        
        for (int seg = 0; seg < numSegments; ++seg) {
            QPointF P0, P1, P2, P3;
            
            if (seg == 0) {
                P0 = controlPoints[0];
                P1 = controlPoints[1];
                P2 = controlPoints[2];
                P3 = controlPoints[3];
            } else {
                int base = seg * 2;
                P0 = controlPoints[base + 1];
                P1 = QPointF(2.0 * P0.x() - controlPoints[base].x(),
                             2.0 * P0.y() - controlPoints[base].y());
                P2 = controlPoints[base + 2];
                P3 = controlPoints[base + 3];
            }
            
            for (int i = 0; i <= numSteps; ++i) {
                double t = static_cast<double>(i) / numSteps;
                curvePoints.append(evalCubicBezier(P0, P1, P2, P3, t));
            }
        }
    } else {
        // C0 independent mode (4 points per new segment)
        int numSegments = n / 4;
        
        for (int seg = 0; seg < numSegments; ++seg) {
            int base = seg * 4;
            QPointF P0 = controlPoints[base];
            QPointF P1 = controlPoints[base + 1];
            QPointF P2 = controlPoints[base + 2];
            QPointF P3 = controlPoints[base + 3];
            
            for (int i = 0; i <= numSteps; ++i) {
                double t = static_cast<double>(i) / numSteps;
                curvePoints.append(evalCubicBezier(P0, P1, P2, P3, t));
            }
        }
    }
    
    curveCalculated = true;
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint cartesianPoint = screenToCartesian(event->pos());
        addControlPoint(cartesianPoint);
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    QPoint cartesianPoint = screenToCartesian(event->pos());
    emit mouseMoved(cartesianPoint);
}

void Canvas::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    redraw();
}

QPoint Canvas::screenToCartesian(const QPoint &screenPos) const
{
    int originX = (width() / 2 / gridSize) * gridSize;
    int originY = (height() / 2 / gridSize) * gridSize;
    
    int cx = std::floor(static_cast<double>(screenPos.x() - originX) / gridSize);
    int cy = -std::floor(static_cast<double>(screenPos.y() - originY) / gridSize);
    
    return QPoint(cx, cy);
}

QPoint Canvas::cartesianToScreen(const QPointF &cartesianPos) const
{
    int originX = (width() / 2 / gridSize) * gridSize;
    int originY = (height() / 2 / gridSize) * gridSize;
    
    int sx = originX + static_cast<int>(std::round(cartesianPos.x() * gridSize));
    int sy = originY - static_cast<int>(std::round(cartesianPos.y() * gridSize));
    
    return QPoint(sx, sy);
}

void Canvas::fillRectPixels(QImage &img, int x, int y, int w, int h, const QColor &color)
{
    for (int yy = y; yy < y + h; ++yy) {
        for (int xx = x; xx < x + w; ++xx) {
            if (xx >= 0 && xx < img.width() && yy >= 0 && yy < img.height()) {
                img.setPixel(xx, yy, color.rgb());
            }
        }
    }
}

void Canvas::drawBresenhamLine(QImage &img, int x0, int y0, int x1, int y1, const QColor &color)
{
    int dx = std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx + dy;
    
    while (true) {
        fillRectPixels(img, x0 * gridSize, y0 * gridSize, gridSize, gridSize, color);
        
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void Canvas::drawGridAndAxes(QImage &img)
{
    int w = img.width();
    int h = img.height();
    int originX = (w / 2 / gridSize) * gridSize;
    int originY = (h / 2 / gridSize) * gridSize;
    
    // Draw Grid
    if (showGrid) {
        for (int x = gridSize; x < w; x += gridSize) {
            for (int y = 0; y < h; ++y) {
                if (x >= 0 && x < w && y >= 0 && y < h)
                    img.setPixel(x, y, QColor(40, 40, 40).rgb());
            }
        }
        for (int y = gridSize; y < h; y += gridSize) {
            for (int x = 0; x < w; ++x) {
                if (x >= 0 && x < w && y >= 0 && y < h)
                    img.setPixel(x, y, QColor(40, 40, 40).rgb());
            }
        }
    }
    
    // Draw Axes
    if (showAxes) {
        QColor axisColor(100, 100, 100);
        for (int x = 0; x < w; x += gridSize)
            fillRectPixels(img, x, originY, gridSize, gridSize, axisColor);
        for (int y = 0; y < h; y += gridSize)
            fillRectPixels(img, originX, y, gridSize, gridSize, axisColor);
            
        // Highlight Origin
        fillRectPixels(img, originX, originY, gridSize, gridSize, Qt::white);
    }
}

void Canvas::redraw()
{
    if (width() <= 0 || height() <= 0) return;
    
    canvasImage = QImage(width(), height(), QImage::Format_RGB32);
    canvasImage.fill(Qt::black);
    
    drawGridAndAxes(canvasImage);
    
    // Draw curve
    if (curveCalculated && curvePoints.size() >= 2) {
        for (int i = 0; i < curvePoints.size() - 1; ++i) {
            QPoint c1 = cartesianToScreen(curvePoints[i]);
            QPoint c2 = cartesianToScreen(curvePoints[i+1]);
            int cx1 = std::floor(static_cast<double>(c1.x()) / gridSize);
            int cy1 = std::floor(static_cast<double>(c1.y()) / gridSize);
            int cx2 = std::floor(static_cast<double>(c2.x()) / gridSize);
            int cy2 = std::floor(static_cast<double>(c2.y()) / gridSize);
            drawBresenhamLine(canvasImage, cx1, cy1, cx2, cy2, Qt::red);
        }
    }
    
    // Create painter to draw text/labels for points on top of the image
    QPainter painter(&canvasImage);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    
    // Draw control points
    for (int i = 0; i < controlPoints.size(); ++i) {
        QPoint screenPos = cartesianToScreen(controlPoints[i]);
        fillRectPixels(canvasImage, screenPos.x(), screenPos.y(), gridSize, gridSize, Qt::blue);
        
        painter.setPen(Qt::white);
        painter.drawText(screenPos.x() + gridSize + 2, screenPos.y() + gridSize / 2 + 4, QString("P%1").arg(i));
    }
    
    // Draw reflected points if C1 continuity
    if (continuousC1) {
        int n = controlPoints.size();
        int numSegments = (n >= 4) ? ((n - 4) / 2 + 1) : 0;
        
        for (int seg = 1; seg < numSegments; ++seg) {
            int base = seg * 2;
            QPointF endpoint = controlPoints[base + 1];
            QPointF reflected(2.0 * endpoint.x() - controlPoints[base].x(),
                              2.0 * endpoint.y() - controlPoints[base].y());
            
            QPoint screenPos = cartesianToScreen(reflected);
            fillRectPixels(canvasImage, screenPos.x(), screenPos.y(), gridSize, gridSize, Qt::yellow);
            
            painter.setPen(Qt::yellow);
            painter.drawText(screenPos.x() + gridSize + 2, screenPos.y() + gridSize / 2 + 4, QString("R%1").arg(seg));
        }
    }
    
    update(); // Trigger paintEvent
}

void Canvas::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    if (!canvasImage.isNull()) {
        painter.drawImage(0, 0, canvasImage);
    }
}
