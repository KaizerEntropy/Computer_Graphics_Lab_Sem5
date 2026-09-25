#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent), gridSize(20), 
    m_showArbLine(false), m_arbLineM(1), m_arbLineC(0),
    m_showArbPoint(false), m_arbPointX(0), m_arbPointY(0)
{
    setMouseTracking(true);
}

void Canvas::clearCanvas() {
    polygon.clear();
    transformedPolygon.clear();
    m_showArbLine = false;
    m_showArbPoint = false;
    update();
}

void Canvas::applyTransformation(const TransformMatrix &matrix) {
    if (polygon.isEmpty()) return;
    
    // We apply transformations to the current transformedPolygon state
    if (transformedPolygon.isEmpty()) {
        transformedPolygon = polygon;
    }
    transformedPolygon = matrix.transform(transformedPolygon);
    update();
}

void Canvas::setShowArbitraryLine(bool show, double m, double c) {
    m_showArbLine = show;
    m_arbLineM = m;
    m_arbLineC = c;
    update();
}

void Canvas::setShowArbitraryPoint(bool show, double px, double py) {
    m_showArbPoint = show;
    m_arbPointX = px;
    m_arbPointY = py;
    update();
}

QPointF Canvas::toScreen(const QPointF &logical) const {
    double cx = width() / 2.0;
    double cy = height() / 2.0;
    return QPointF(cx + logical.x() * gridSize, cy - logical.y() * gridSize);
}

QPointF Canvas::toLogical(const QPointF &screen) const {
    double cx = width() / 2.0;
    double cy = height() / 2.0;
    return QPointF(std::round((screen.x() - cx) / gridSize), std::round((cy - screen.y()) / gridSize));
}

void Canvas::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    painter.fillRect(rect(), Qt::white);

    int cx = width() / 2;
    int cy = height() / 2;

    // Draw grid
    painter.setPen(QPen(QColor(220, 220, 220), 1));
    for (int x = cx % gridSize; x < width(); x += gridSize)
        painter.drawLine(x, 0, x, height());
    for (int y = cy % gridSize; y < height(); y += gridSize)
        painter.drawLine(0, y, width(), y);

    // Draw Axes
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(0, cy, width(), cy); // X axis
    painter.drawLine(cx, 0, cx, height()); // Y axis

    // Draw Arbitrary Line if set
    if (m_showArbLine) {
        painter.setPen(QPen(Qt::blue, 2, Qt::DashLine));
        double x1_log = -cx / gridSize - 1;
        double x2_log = cx / gridSize + 1;
        double y1_log = m_arbLineM * x1_log + m_arbLineC;
        double y2_log = m_arbLineM * x2_log + m_arbLineC;
        QPointF p1 = toScreen(QPointF(x1_log, y1_log));
        QPointF p2 = toScreen(QPointF(x2_log, y2_log));
        painter.drawLine(p1, p2);
    }

    // Draw Arbitrary Point if set
    if (m_showArbPoint) {
        painter.setPen(QPen(Qt::magenta, 5));
        painter.setBrush(Qt::magenta);
        QPointF pt = toScreen(QPointF(m_arbPointX, m_arbPointY));
        painter.drawEllipse(pt, 4, 4);
    }

    // Draw Original Polygon (ghostly)
    if (!polygon.isEmpty()) {
        painter.setPen(QPen(QColor(150, 150, 150), 2, Qt::DashLine));
        painter.setBrush(QColor(200, 200, 200, 100));
        QPolygonF drawPoly;
        for (const auto& p : polygon) {
            drawPoly << toScreen(p);
        }
        painter.drawPolygon(drawPoly);
        
        // Draw points
        painter.setPen(QPen(Qt::gray, 5));
        for (const auto& p : polygon) {
            painter.drawPoint(toScreen(p));
        }
    }

    // Draw Transformed Polygon
    const QVector<QPointF>& currentPoly = transformedPolygon.isEmpty() ? polygon : transformedPolygon;
    if (!currentPoly.isEmpty()) {
        painter.setPen(QPen(Qt::red, 2));
        painter.setBrush(QColor(255, 0, 0, 100));
        QPolygonF drawPoly;
        for (const auto& p : currentPoly) {
            drawPoly << toScreen(p);
        }
        painter.drawPolygon(drawPoly);

        painter.setPen(QPen(Qt::darkRed, 5));
        for (const auto& p : currentPoly) {
            painter.drawPoint(toScreen(p));
        }
    }

    // Draw mouse hover point
    QPointF snapped = toLogical(mousePos);
    QPointF scrSnapped = toScreen(snapped);
    painter.setPen(QPen(Qt::blue, 6));
    painter.drawPoint(scrSnapped);
    painter.setPen(Qt::black);
    painter.drawText(scrSnapped + QPointF(5, -5), QString("(%1, %2)").arg(snapped.x()).arg(snapped.y()));
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPointF logicalPt = toLogical(event->pos());
        polygon.append(logicalPt);
        transformedPolygon.clear(); // Reset transformation on new point
        m_showArbLine = false;
        m_showArbPoint = false;
        update();
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    mousePos = event->pos();
    update();
}
