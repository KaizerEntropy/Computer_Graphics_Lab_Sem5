#include "canvas.h"
#include <cmath>
#include <QPen>
#include <QFont>
#include <QWheelEvent>
#include <QResizeEvent>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent)
{
    setMouseTracking(true);
    setFocusPolicy(Qt::StrongFocus);
    m_compositeMatrix = Matrix3x3::identity();
}

Canvas::~Canvas() {
}

void Canvas::setFillMode(FillMode mode) {
    m_fillMode = mode;
    if (mode != ModeDrawCircle) {
        m_isPlacingCircleRadius = false;
    }
    if (mode != ModeDrawLine) {
        m_isPlacingLineEnd = false;
    }
    update();
}

void Canvas::setBoundaryColor(const QColor &color) {
    m_boundaryColor = color;
    rebuildAllBoundaryPixels();
    update();
}

void Canvas::setFillColor(const QColor &color) {
    m_fillColor = color;
    update();
}

void Canvas::setGridSize(int size) {
    m_gridSize = qBound(4, size, 60);
    update();
}

void Canvas::setZoomLevel(int gridSize) {
    setGridSize(gridSize);
}

void Canvas::setShowGrid(bool show) { m_showGrid = show; update(); }
void Canvas::setShowAxes(bool show) { m_showAxes = show; update(); }
void Canvas::setShowHover(bool show) { m_showHover = show; update(); }
void Canvas::setShowVertexLabels(bool show) { m_showVertexLabels = show; update(); }
void Canvas::setShowGhost(bool show) { m_showGhost = show; update(); }
void Canvas::setShowRotationPivot(bool show) { m_showRotationPivot = show; update(); }
void Canvas::setShowReflectLine(bool show) { m_showReflectLine = show; update(); }
void Canvas::setShowScanlineBeam(bool show) { m_showScanlineBeam = show; update(); }

void Canvas::setShowClipWindow(bool show) { m_showClipWindow = show; update(); }
void Canvas::setShowOutcodeLabels(bool show) { m_showOutcodeLabels = show; update(); }
void Canvas::setShowClipResult(bool show) { m_showClipResult = show; update(); }

// ----------------------------------------------------------------------------
// Selection & Scope Management
// ----------------------------------------------------------------------------
void Canvas::selectShape(int id) {
    m_selectedShapeId = id;
    QString desc = getSelectedShapeName();
    emit shapeSelectionChanged(m_selectedShapeId, desc);
    emit statusMessage(QString("Selection: %1").arg(desc));
    update();
}

void Canvas::deselectAll() {
    selectShape(-1); // -1 = Entire Space
}

QString Canvas::getSelectedShapeName() const {
    if (m_selectedShapeId == -1) {
        return QString("🌐 Entire Space / All Shapes (%1 Polygons, %2 Circles)")
            .arg(m_polygons.size()).arg(m_circles.size());
    }

    for (const auto &poly : m_polygons) {
        if (poly.id == m_selectedShapeId) {
            return QString("🎯 Polygon #%1 (%2 vertices)").arg(poly.id).arg(poly.vertices.size());
        }
    }

    for (const auto &c : m_circles) {
        if (c.id == m_selectedShapeId) {
            return QString("🎯 Circle #%1 (Center (%2,%3), r=%4)").arg(c.id).arg(c.center.x()).arg(c.center.y()).arg(c.radius);
        }
    }

    return "Entire Space";
}

QPoint Canvas::getSelectedShapeCentroid() const {
    if (m_selectedShapeId > 0) {
        for (const auto &poly : m_polygons) {
            if (poly.id == m_selectedShapeId && !poly.vertices.isEmpty()) {
                int sumX = 0, sumY = 0;
                for (const auto &v : poly.vertices) {
                    sumX += v.x();
                    sumY += v.y();
                }
                return QPoint(sumX / poly.vertices.size(), sumY / poly.vertices.size());
            }
        }
        for (const auto &c : m_circles) {
            if (c.id == m_selectedShapeId) {
                return c.center;
            }
        }
    }

    // Centroid of all shapes or origin
    if (!m_polygons.isEmpty()) {
        int sumX = 0, sumY = 0, count = 0;
        for (const auto &p : m_polygons) {
            for (const auto &v : p.vertices) {
                sumX += v.x();
                sumY += v.y();
                count++;
            }
        }
        if (count > 0) return QPoint(sumX / count, sumY / count);
    }
    return QPoint(0, 0);
}

int Canvas::findShapeAt(const QPoint &pt) const {
    // 1. Check Circles first
    for (const auto &c : m_circles) {
        int dx = pt.x() - c.center.x();
        int dy = pt.y() - c.center.y();
        double dist = std::sqrt(dx * dx + dy * dy);
        if (dist <= c.radius + 1.5) {
            return c.id;
        }
    }

    // 2. Check Polygons (Ray-casting point in polygon test & vertex proximity)
    for (const auto &poly : m_polygons) {
        const auto &v = poly.vertices;
        int n = v.size();
        if (n < 3) continue;

        // Check vertex proximity
        for (int i = 0; i < n; ++i) {
            if (std::abs(pt.x() - v[i].x()) <= 1 && std::abs(pt.y() - v[i].y()) <= 1) {
                return poly.id;
            }
        }

        // Ray casting
        bool inside = false;
        for (int i = 0, j = n - 1; i < n; j = i++) {
            if (((v[i].y() > pt.y()) != (v[j].y() > pt.y())) &&
                (pt.x() < (v[j].x() - v[i].x()) * (pt.y() - v[i].y()) / (v[j].y() - v[i].y()) + v[i].x())) {
                inside = !inside;
            }
        }
        if (inside) return poly.id;
    }

    return -1; // None found
}

// ----------------------------------------------------------------------------
// Transformation Execution via Homogeneous Matrices
// ----------------------------------------------------------------------------
void Canvas::applyTransformation(const Matrix3x3 &mat, const QString &name, const QString &formula, const QString &details) {
    // Save ghost copy before transforming
    m_ghostPolygons = m_polygons;
    m_ghostCircles = m_circles;

    if (m_selectedShapeId == -1) {
        // Transform ALL polygons
        for (auto &poly : m_polygons) {
            poly.vertices = mat.transformPoints(poly.vertices);
        }

        // Transform ALL circles
        for (auto &c : m_circles) {
            c.center = mat.transformPoint(c.center);
            double det = std::abs(mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0]);
            double scaleFactor = std::sqrt(det);
            if (scaleFactor > 0.01) {
                c.radius = std::max(1, static_cast<int>(std::round(c.radius * scaleFactor)));
            }
        }

        // Transform Seed Point
        if (m_hasSeedPoint) {
            m_seedPoint = mat.transformPoint(m_seedPoint);
        }

        // Transform persistent fill pixels
        QHash<QPoint, QColor> newFill;
        for (auto it = m_fillPixels.constBegin(); it != m_fillPixels.constEnd(); ++it) {
            QPoint newPt = mat.transformPoint(it.key());
            newFill[newPt] = it.value();
        }
        m_fillPixels = newFill;

    } else {
        // Transform ONLY selected polygon
        for (auto &poly : m_polygons) {
            if (poly.id == m_selectedShapeId) {
                poly.vertices = mat.transformPoints(poly.vertices);
                break;
            }
        }

        // Transform ONLY selected circle
        for (auto &c : m_circles) {
            if (c.id == m_selectedShapeId) {
                c.center = mat.transformPoint(c.center);
                double det = std::abs(mat.m[0][0] * mat.m[1][1] - mat.m[0][1] * mat.m[1][0]);
                double scaleFactor = std::sqrt(det);
                if (scaleFactor > 0.01) {
                    c.radius = std::max(1, static_cast<int>(std::round(c.radius * scaleFactor)));
                }
                break;
            }
        }
    }

    m_compositeMatrix = mat * m_compositeMatrix;

    Matrix3x3::TransformStep step;
    step.name = name;
    step.matrix = mat;
    step.formula = formula;
    step.details = details;
    m_transformHistory.append(step);

    rebuildAllBoundaryPixels();
    emit transformationApplied(step, m_compositeMatrix);
    emit statusMessage(QString("Applied: %1 | Composite Matrix updated.").arg(name));
    update();
}

void Canvas::resetTransformations() {
    m_compositeMatrix = Matrix3x3::identity();
    m_transformHistory.clear();
    m_ghostPolygons.clear();
    m_ghostCircles.clear();
    emit transformationsReset();
    emit statusMessage("Transformations reset to Identity Matrix.");
    update();
}

void Canvas::undoLastTransformation() {
    if (m_transformHistory.isEmpty()) {
        emit statusMessage("No transformations to undo.");
        return;
    }

    m_transformHistory.removeLast();

    // Recompute composite matrix from scratch
    m_compositeMatrix = Matrix3x3::identity();
    for (const auto &step : m_transformHistory) {
        m_compositeMatrix = step.matrix * m_compositeMatrix;
    }

    // Revert to ghost if available
    if (!m_ghostPolygons.isEmpty() || !m_ghostCircles.isEmpty()) {
        m_polygons = m_ghostPolygons;
        m_circles = m_ghostCircles;
        m_ghostPolygons.clear();
        m_ghostCircles.clear();
    }

    rebuildAllBoundaryPixels();
    emit transformationsReset();
    emit statusMessage(QString("Undo transform. Remaining steps: %1").arg(m_transformHistory.size()));
    update();
}

// ----------------------------------------------------------------------------
// Arbitrary Point & Line Parameters
// ----------------------------------------------------------------------------
void Canvas::setRotationPivot(const QPoint &pt) {
    m_rotationPivot = pt;
    emit pivotChanged(m_rotationPivot);
    emit statusMessage(QString("Rotation Pivot Point set to (%1, %2)").arg(pt.x()).arg(pt.y()));
    update();
}

void Canvas::setReflectLine(double slope, double intercept) {
    m_reflectLineSlope = slope;
    m_reflectLineIntercept = intercept;
    emit reflectLineChanged(m_reflectLineSlope, m_reflectLineIntercept);
    emit statusMessage(QString("Reflection Line set to y = %1x + %2").arg(slope, 0, 'f', 2).arg(intercept, 0, 'f', 2));
    update();
}

// ----------------------------------------------------------------------------
// Multi-Shape Management
// ----------------------------------------------------------------------------
void Canvas::addPolygon(const QVector<QPoint> &vertices, const QColor &color) {
    if (vertices.size() < 3) return;

    PolygonShape poly;
    poly.id = m_nextShapeId++;
    poly.vertices = vertices;
    poly.boundaryColor = color.isValid() ? color : m_boundaryColor;
    poly.isClosed = true;

    m_polygons.append(poly);
    selectShape(poly.id);

    auto scanRes = CGAlgorithms::scanlineFill(vertices);
    if (!scanRes.filledPixels.isEmpty()) {
        setSeedPoint(scanRes.filledPixels[scanRes.filledPixels.size() / 2]);
    }

    rebuildAllBoundaryPixels();
    emit shapesChanged(m_polygons.size(), m_circles.size());
    update();
}

void Canvas::addCircle(const QPoint &center, int radius, const QColor &color) {
    if (radius <= 0) return;

    CircleShape circle;
    circle.id = m_nextShapeId++;
    circle.center = center;
    circle.radius = radius;
    circle.boundaryColor = color.isValid() ? color : m_boundaryColor;

    m_circles.append(circle);
    selectShape(circle.id);
    setSeedPoint(center);

    rebuildAllBoundaryPixels();
    emit shapesChanged(m_polygons.size(), m_circles.size());
    emit statusMessage(QString("Circle #%1 added: Center (%2, %3), Radius %4.").arg(circle.id).arg(center.x()).arg(center.y()).arg(radius));
    update();
}

void Canvas::addPolygonVertex(const QPoint &pt) {
    m_currentPolygonVertices.append(pt);
    rebuildAllBoundaryPixels();
    emit statusMessage(QString("Drawing Polygon: Vertex V%1 added at (%2, %3). Double-click or Right-click to close.")
                           .arg(m_currentPolygonVertices.size() - 1).arg(pt.x()).arg(pt.y()));
    update();
}

void Canvas::closePolygon() {
    if (m_currentPolygonVertices.size() < 3) {
        emit statusMessage("A polygon requires at least 3 vertices to close.");
        return;
    }

    PolygonShape poly;
    poly.id = m_nextShapeId++;
    poly.vertices = m_currentPolygonVertices;
    poly.boundaryColor = m_boundaryColor;
    poly.isClosed = true;

    m_polygons.append(poly);
    m_currentPolygonVertices.clear();
    selectShape(poly.id);

    rebuildAllBoundaryPixels();
    emit shapesChanged(m_polygons.size(), m_circles.size());
    emit statusMessage(QString("Polygon #%1 closed with %2 vertices. Selected for transformation.")
                           .arg(poly.id).arg(poly.vertices.size()));
    update();
}

void Canvas::clearCurrentDrawing() {
    m_currentPolygonVertices.clear();
    m_isPlacingCircleRadius = false;
    m_isPlacingLineEnd = false;
    rebuildAllBoundaryPixels();
    update();
}

void Canvas::undoLastShape() {
    if (!m_currentPolygonVertices.isEmpty() || m_isPlacingCircleRadius || m_isPlacingLineEnd) {
        clearCurrentDrawing();
        emit statusMessage("Cancelled current drawing.");
        return;
    }
    
    if (!m_lines.isEmpty()) {
        m_lines.removeLast();
        update();
        return;
    }

    if (!m_circles.isEmpty() && (m_polygons.isEmpty() || m_circles.last().id > m_polygons.last().id)) {
        m_circles.removeLast();
    } else if (!m_polygons.isEmpty()) {
        m_polygons.removeLast();
    }

    deselectAll();
    rebuildAllBoundaryPixels();
    emit shapesChanged(m_polygons.size(), m_circles.size());
    emit statusMessage("Last shape removed.");
    update();
}

void Canvas::clearAllShapes() {
    m_polygons.clear();
    m_circles.clear();
    m_lines.clear();
    m_currentPolygonVertices.clear();
    m_isPlacingCircleRadius = false;
    m_isPlacingLineEnd = false;
    m_boundaryPixels.clear();
    m_ghostPolygons.clear();
    m_ghostCircles.clear();
    m_selectedShapeId = -1;
    m_csResults.clear();
    m_shResult = ClipAlgorithms::SHResult();
    emit shapesChanged(0, 0);
    emit statusMessage("All shapes cleared.");
    update();
}

QVector<QPoint> Canvas::activeOrLastPolygonVertices() const {
    if (m_selectedShapeId > 0) {
        for (const auto &p : m_polygons) {
            if (p.id == m_selectedShapeId) return p.vertices;
        }
    }
    if (!m_currentPolygonVertices.isEmpty()) return m_currentPolygonVertices;
    if (!m_polygons.isEmpty()) return m_polygons.last().vertices;
    return {};
}

// ----------------------------------------------------------------------------
// Presets
// ----------------------------------------------------------------------------
void Canvas::setPolygonPreset(const QVector<QPoint> &vertices) {
    clearAllShapes();
    clearFillOnly();
    addPolygon(vertices, m_boundaryColor);
    emit statusMessage(QString("Polygon Preset Loaded (%1 vertices). Ready for Transformations.").arg(vertices.size()));
}

void Canvas::setNestedPolygonPreset(const QVector<QPoint> &outerVerts, const QVector<QPoint> &innerVerts) {
    clearAllShapes();
    clearFillOnly();
    addPolygon(outerVerts, m_boundaryColor);
    addPolygon(innerVerts, m_boundaryColor);
    deselectAll();
    emit statusMessage("Nested Donut Preset Loaded (2 Polygons). Entire Space selected.");
}

void Canvas::setMultiShapeScenePreset() {
    clearAllShapes();
    clearFillOnly();

    QVector<QPoint> p1, p2;
    QPoint cCenter;
    int cRadius;
    CGAlgorithms::getMultiShapeScenePreset(p1, p2, cCenter, cRadius);

    addPolygon(p1, m_boundaryColor);
    addPolygon(p2, m_boundaryColor);
    addCircle(cCenter, cRadius, m_boundaryColor);
    deselectAll();

    emit statusMessage("Multi-Shape Scene Loaded: 2 Polygons + 1 Circle. Right-click any shape to transform it!");
}

void Canvas::rebuildAllBoundaryPixels() {
    m_boundaryPixels.clear();

    for (const auto &poly : m_polygons) {
        int n = poly.vertices.size();
        if (n < 2) continue;
        int limit = poly.isClosed ? n : (n - 1);
        for (int i = 0; i < limit; ++i) {
            QPoint p1 = poly.vertices[i];
            QPoint p2 = poly.vertices[(i + 1) % n];
            auto linePts = CGAlgorithms::bresenhamLine(p1.x(), p1.y(), p2.x(), p2.y());
            for (const auto &pt : linePts) {
                m_boundaryPixels[pt] = poly.boundaryColor;
            }
        }
    }

    if (!m_currentPolygonVertices.isEmpty()) {
        int n = m_currentPolygonVertices.size();
        for (int i = 0; i < n - 1; ++i) {
            QPoint p1 = m_currentPolygonVertices[i];
            QPoint p2 = m_currentPolygonVertices[i + 1];
            auto linePts = CGAlgorithms::bresenhamLine(p1.x(), p1.y(), p2.x(), p2.y());
            for (const auto &pt : linePts) {
                m_boundaryPixels[pt] = m_boundaryColor;
            }
        }
    }

    for (const auto &c : m_circles) {
        auto circlePts = CGAlgorithms::bresenhamCircle(c.center.x(), c.center.y(), c.radius);
        for (const auto &pt : circlePts) {
            m_boundaryPixels[pt] = c.boundaryColor;
        }
    }
}

// ----------------------------------------------------------------------------
// Fill Operations
// ----------------------------------------------------------------------------
void Canvas::setSeedPoint(const QPoint &pt) {
    m_seedPoint = pt;
    m_hasSeedPoint = true;
    emit seedPointSet(m_seedPoint);
    emit statusMessage(QString("Seed point set at (%1, %2)").arg(pt.x()).arg(pt.y()));
    update();
}

void Canvas::triggerFill() {
    if (!m_hasSeedPoint) {
        if (!m_polygons.isEmpty()) {
            auto scanRes = CGAlgorithms::scanlineFill(m_polygons.last().vertices);
            if (!scanRes.filledPixels.isEmpty()) {
                setSeedPoint(scanRes.filledPixels[scanRes.filledPixels.size() / 2]);
            }
        } else if (!m_circles.isEmpty()) {
            setSeedPoint(m_circles.last().center);
        }
    }
    triggerFillAt(m_seedPoint);
}

void Canvas::triggerFillAt(const QPoint &seed) {
    QHash<QPoint, QColor> currentGrid = m_fillPixels;
    for (auto it = m_boundaryPixels.constBegin(); it != m_boundaryPixels.constEnd(); ++it) {
        currentGrid[it.key()] = it.value();
    }

    if (m_fillMode == ModeScanlineFill) {
        if (m_polygons.isEmpty()) {
            emit statusMessage("Scanline fill requires at least one polygon.");
            return;
        }
        const PolygonShape *targetPoly = &m_polygons.last();
        if (m_selectedShapeId > 0) {
            for (const auto &p : m_polygons) {
                if (p.id == m_selectedShapeId) { targetPoly = &p; break; }
            }
        }
        m_fillResult = CGAlgorithms::scanlineFill(targetPoly->vertices);
    } else {
        // Boundary Fill 8-Connected
        m_fillResult = CGAlgorithms::boundaryFill8_BFS(currentGrid, seed, m_boundaryColor, m_fillColor);
    }

    if (!m_fillResult.filledPixels.isEmpty()) {
        ShapeFillRecord rec;
        rec.algorithmName = m_fillResult.algorithmName;
        rec.color = m_fillColor;
        rec.elapsedNanos = m_fillResult.elapsedNanos;
        rec.seed = seed;
        rec.points = m_fillResult.filledPixels;

        for (const auto &p : m_fillResult.filledPixels) {
            m_fillPixels[p] = m_fillColor;
        }
        m_fillHistory.append(rec);

        emit fillFinished(m_fillResult.algorithmName, m_fillResult.elapsedNanos, m_fillResult.pointCount);
        emit statusMessage(QString("%1 completed: %2 pixels filled in %3 ns")
                               .arg(m_fillResult.algorithmName).arg(m_fillResult.pointCount).arg(m_fillResult.elapsedNanos));
    }
    update();
}

void Canvas::undoLastFill() {
    if (!m_fillHistory.isEmpty()) {
        m_fillHistory.removeLast();
    }
    m_fillPixels.clear();
    for (const auto &rec : m_fillHistory) {
        for (const auto &pt : rec.points) {
            m_fillPixels[pt] = rec.color;
        }
    }
    emit statusMessage("Undo fill successful.");
    update();
}

void Canvas::clearFillOnly() {
    m_fillPixels.clear();
    m_fillHistory.clear();
    emit statusMessage("Fills cleared. Shapes preserved.");
    update();
}

void Canvas::clearAll() {
    clearFillOnly();
    clearAllShapes();
    resetTransformations();
    emit statusMessage("Canvas fully cleared.");
}

// ----------------------------------------------------------------------------
// Coordinate Conversions & Mouse Events
// ----------------------------------------------------------------------------
QPoint Canvas::screenToLogical(const QPoint &screenPos) const {
    int cx = width() / 2 + m_panOffset.x();
    int cy = height() / 2 + m_panOffset.y();
    int logX = static_cast<int>(std::floor(static_cast<double>(screenPos.x() - cx) / m_gridSize));
    int logY = static_cast<int>(std::floor(static_cast<double>(cy - screenPos.y()) / m_gridSize));
    return QPoint(logX, logY);
}

QPoint Canvas::logicalToScreen(const QPoint &logicalPos) const {
    int cx = width() / 2 + m_panOffset.x();
    int cy = height() / 2 + m_panOffset.y();
    return QPoint(cx + logicalPos.x() * m_gridSize + m_gridSize / 2,
                  cy - (logicalPos.y() + 1) * m_gridSize + m_gridSize / 2);
}

QRect Canvas::logicalToCellRect(const QPoint &logicalPos) const {
    int cx = width() / 2 + m_panOffset.x();
    int cy = height() / 2 + m_panOffset.y();
    int cellX = cx + logicalPos.x() * m_gridSize;
    int cellY = cy - (logicalPos.y() + 1) * m_gridSize;
    return QRect(cellX, cellY, m_gridSize, m_gridSize);
}

QPoint Canvas::logicalToScreenF(const QPointF &logicalPos) const {
    double cx = width() / 2.0 + m_panOffset.x();
    double cy = height() / 2.0 + m_panOffset.y();
    return QPoint(static_cast<int>(cx + logicalPos.x() * m_gridSize + m_gridSize / 2.0),
                  static_cast<int>(cy - (logicalPos.y() + 1.0) * m_gridSize + m_gridSize / 2.0));
}

QRect Canvas::getClipWindowScreenRect() const {
    int cx = width() / 2 + m_panOffset.x();
    int cy = height() / 2 + m_panOffset.y();
    int xmin = qRound(m_clipRect.left());
    int xmax = qRound(m_clipRect.right());
    int ymin = qRound(m_clipRect.top());
    int ymax = qRound(m_clipRect.bottom());

    int left = cx + xmin * m_gridSize;
    int right = cx + (xmax + 1) * m_gridSize;
    int top = cy - (ymax + 1) * m_gridSize;
    int bottom = cy - ymin * m_gridSize;

    return QRect(left, top, right - left, bottom - top);
}

Canvas::ClipHit Canvas::hitTestClipWindow(const QPoint &screenPos) const {
    if (!m_showClipWindow) return HitNone;

    QRect r = getClipWindowScreenRect();
    int mx = screenPos.x();
    int my = screenPos.y();
    const int tol = 8; // 8 screen pixels tolerance for easy grab

    bool nearL = std::abs(mx - r.left()) <= tol;
    bool nearR = std::abs(mx - r.right()) <= tol;
    bool nearT = std::abs(my - r.top()) <= tol;
    bool nearB = std::abs(my - r.bottom()) <= tol;

    bool withinX = (mx >= r.left() - tol && mx <= r.right() + tol);
    bool withinY = (my >= r.top() - tol && my <= r.bottom() + tol);

    if (nearL && nearT) return HitTopLeft;
    if (nearR && nearT) return HitTopRight;
    if (nearL && nearB) return HitBottomLeft;
    if (nearR && nearB) return HitBottomRight;

    if (nearL && withinY) return HitLeft;
    if (nearR && withinY) return HitRight;
    if (nearT && withinX) return HitTop;
    if (nearB && withinX) return HitBottom;

    if (r.contains(screenPos)) return HitBody;

    return HitNone;
}

Qt::CursorShape Canvas::cursorForHit(ClipHit hit) const {
    switch (hit) {
    case HitLeft:
    case HitRight:
        return Qt::SizeHorCursor;
    case HitTop:
    case HitBottom:
        return Qt::SizeVerCursor;
    case HitTopLeft:
    case HitBottomRight:
        return Qt::SizeFDiagCursor;
    case HitTopRight:
    case HitBottomLeft:
        return Qt::SizeBDiagCursor;
    case HitBody:
        return Qt::SizeAllCursor;
    default:
        return Qt::ArrowCursor;
    }
}

void Canvas::wheelEvent(QWheelEvent *event) {
    int delta = event->angleDelta().y();
    int oldGrid = m_gridSize;
    if (delta > 0) {
        m_gridSize = qMin(m_gridSize + 2, 60);
    } else {
        m_gridSize = qMax(m_gridSize - 2, 4);
    }
    
    if (m_gridSize != oldGrid) {
        // Anchor-based zoom: adjust pan offset so point under cursor stays put
        QPointF mousePos = event->position();
        double cx = width() / 2.0 + m_panOffset.x();
        double cy = height() / 2.0 + m_panOffset.y();
        double logX = (mousePos.x() - cx) / oldGrid;
        double logY = (cy - mousePos.y()) / oldGrid;
        double newCx = mousePos.x() - logX * m_gridSize;
        double newCy = mousePos.y() + logY * m_gridSize;
        m_panOffset.setX(static_cast<int>(newCx - width() / 2.0));
        m_panOffset.setY(static_cast<int>(newCy - height() / 2.0));
        
        emit viewChanged(m_gridSize);
        update();
    }
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    // Middle button: start panning
    if (event->button() == Qt::MiddleButton) {
        m_isPanning = true;
        m_panStartScreen = event->pos();
        m_panStartOffset = m_panOffset;
        setCursor(Qt::ClosedHandCursor);
        return;
    }

    QPoint clickedLog = screenToLogical(event->pos());
    emit pointClicked(clickedLog);

    if (event->button() == Qt::RightButton) {
        // Right-Click to select shape or deselect all (Entire space)
        if (m_fillMode == ModeDrawPolygon && !m_currentPolygonVertices.isEmpty()) {
            closePolygon();
        } else if (m_fillMode == ModeDrawCircle && m_isPlacingCircleRadius) {
            m_isPlacingCircleRadius = false;
            update();
        } else if (m_fillMode == ModeDrawLine && m_isPlacingLineEnd) {
            m_isPlacingLineEnd = false;
            update();
        } else {
            int shapeId = findShapeAt(clickedLog);
            selectShape(shapeId);
        }
        return;
    }

    if (event->button() == Qt::LeftButton) {
        // Direct click on the clipping window border or resize handles to expand/resize!
        ClipHit hit = hitTestClipWindow(event->pos());
        if (m_showClipWindow && hit != HitNone && (hit != HitBody || m_fillMode == ModeClipRegionDrag)) {
            m_clipHit = hit;
            m_isDraggingClip = true;
            m_clipDragStart = QPointF(clickedLog);
            m_clipRectAtDragStart = m_clipRect;
            setCursor(cursorForHit(hit));
            emit statusMessage(QString("Expanding clipping window via %1 border/handle...").arg(
                hit == HitLeft ? "Left" : hit == HitRight ? "Right" : hit == HitTop ? "Top" : hit == HitBottom ? "Bottom" : "Corner"));
            return;
        }

        switch (m_fillMode) {
        case ModeSelectShape: {
            int shapeId = findShapeAt(clickedLog);
            selectShape(shapeId);
            break;
        }
        case ModeSetPivot:
            setRotationPivot(clickedLog);
            break;
        case ModeSetReflectLine:
            // Set 2 points on reflect line
            m_reflectLinePt1 = m_reflectLinePt2;
            m_reflectLinePt2 = clickedLog;
            if (m_reflectLinePt2.x() != m_reflectLinePt1.x()) {
                double m = static_cast<double>(m_reflectLinePt2.y() - m_reflectLinePt1.y()) /
                           static_cast<double>(m_reflectLinePt2.x() - m_reflectLinePt1.x());
                double c = m_reflectLinePt1.y() - m * m_reflectLinePt1.x();
                setReflectLine(m, c);
            }
            break;
        case ModeDrawPolygon:
            addPolygonVertex(clickedLog);
            break;
        case ModeDrawCircle:
            if (!m_isPlacingCircleRadius) {
                m_circleCenterPoint = clickedLog;
                m_isPlacingCircleRadius = true;
                emit statusMessage(QString("Center set to (%1,%2). Click outer point for radius.").arg(clickedLog.x()).arg(clickedLog.y()));
                update();
            } else {
                int dx = clickedLog.x() - m_circleCenterPoint.x();
                int dy = clickedLog.y() - m_circleCenterPoint.y();
                int r = std::max(1, static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy))));
                addCircle(m_circleCenterPoint, r, m_boundaryColor);
                m_isPlacingCircleRadius = false;
            }
            break;
        case ModeDrawLine:
            if (!m_isPlacingLineEnd) {
                m_lineStartPoint = QPointF(clickedLog);
                m_isPlacingLineEnd = true;
                emit statusMessage(QString("Line start set to (%1,%2). Click for end point.").arg(clickedLog.x()).arg(clickedLog.y()));
                update();
            } else {
                addLine(m_lineStartPoint, QPointF(clickedLog));
                m_isPlacingLineEnd = false;
            }
            break;
        case ModeClipRegionDrag: {
            // Already handled by hitTestClipWindow above
            break;
        }
        case ModeScanlineFill:
        case ModeBoundaryFill8:
            setSeedPoint(clickedLog);
            triggerFillAt(clickedLog);
            break;
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    // 1. Handle panning
    if (m_isPanning) {
        QPoint delta = event->pos() - m_panStartScreen;
        m_panOffset = m_panStartOffset + delta;
        update();
        return;
    }

    // 2. Handle clip window drag/resize
    if (m_isDraggingClip) {
        double cx = width() / 2.0 + m_panOffset.x();
        double cy = height() / 2.0 + m_panOffset.y();
        double logX = (event->pos().x() - cx) / m_gridSize;
        double logY = (cy - event->pos().y()) / m_gridSize;
        QPointF currentLog(logX, logY);
        QPointF delta = currentLog - m_clipDragStart;

        double nl = m_clipRectAtDragStart.left();
        double nr = m_clipRectAtDragStart.right();
        double nb = m_clipRectAtDragStart.top();
        double nt = m_clipRectAtDragStart.bottom();

        if (m_clipHit == HitBody) {
            nl += delta.x(); nr += delta.x();
            nb += delta.y(); nt += delta.y();
        } else {
            if (m_clipHit == HitLeft || m_clipHit == HitTopLeft || m_clipHit == HitBottomLeft) nl += delta.x();
            if (m_clipHit == HitRight || m_clipHit == HitTopRight || m_clipHit == HitBottomRight) nr += delta.x();
            if (m_clipHit == HitBottom || m_clipHit == HitBottomLeft || m_clipHit == HitBottomRight) nb += delta.y();
            if (m_clipHit == HitTop || m_clipHit == HitTopLeft || m_clipHit == HitTopRight) nt += delta.y();
        }

        if (nr <= nl + 1) nr = nl + 1;
        if (nt <= nb + 1) nt = nb + 1;

        m_clipRect = QRectF(QPointF(nl, nb), QPointF(nr, nt));
        emit clipWindowChanged(m_clipRect.left(), m_clipRect.top(), m_clipRect.right(), m_clipRect.bottom());

        // Live clipping re-computation on every drag step!
        if (!m_lines.isEmpty()) runCohenSutherland();
        if (!m_polygons.isEmpty()) runSutherlandHodgman();

        update();
        return;
    }

    // 3. Dynamic cursor feedback when hovering over borders or handles
    if (m_showClipWindow) {
        ClipHit hoverHit = hitTestClipWindow(event->pos());
        if (hoverHit != HitNone && (hoverHit != HitBody || m_fillMode == ModeClipRegionDrag)) {
            setCursor(cursorForHit(hoverHit));
        } else {
            setCursor(Qt::ArrowCursor);
        }
    } else {
        setCursor(Qt::ArrowCursor);
    }

    m_isHovering = true;
    m_hoverLogical = screenToLogical(event->pos());
    emit mouseMoved(m_hoverLogical.x(), m_hoverLogical.y(), event->pos().x(), event->pos().y());
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton && m_isPanning) {
        m_isPanning = false;
        setCursor(Qt::ArrowCursor);
        return;
    }
    if (m_isDraggingClip) {
        m_isDraggingClip = false;
        m_clipHit = HitNone;
        setCursor(Qt::ArrowCursor);
        // Re-run clipping after drag
        runCohenSutherland();
        runSutherlandHodgman();
        emit statusMessage(QString("Clipping window set to [%1, %2] x [%3, %4]")
                               .arg(qRound(m_clipRect.left())).arg(qRound(m_clipRect.right()))
                               .arg(qRound(m_clipRect.top())).arg(qRound(m_clipRect.bottom())));
        update();
        return;
    }
    QWidget::mouseReleaseEvent(event);
}

void Canvas::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    m_isHovering = false;
    update();
}

void Canvas::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    update();
}

void Canvas::resetView() {
    m_panOffset = QPoint(0, 0);
    m_gridSize = 15;
    emit viewChanged(m_gridSize);
    update();
}

void Canvas::fitToView() {
    // Calculate bounding box of all shapes + clip window
    double minX = m_clipRect.left(), maxX = m_clipRect.right();
    double minY = m_clipRect.top(), maxY = m_clipRect.bottom();
    for (const auto &poly : m_polygons) {
        for (const auto &v : poly.vertices) {
            minX = std::min(minX, (double)v.x());
            maxX = std::max(maxX, (double)v.x());
            minY = std::min(minY, (double)v.y());
            maxY = std::max(maxY, (double)v.y());
        }
    }
    for (const auto &l : m_lines) {
        minX = std::min({minX, l.p1.x(), l.p2.x()});
        maxX = std::max({maxX, l.p1.x(), l.p2.x()});
        minY = std::min({minY, l.p1.y(), l.p2.y()});
        maxY = std::max({maxY, l.p1.y(), l.p2.y()});
    }
    double rangeX = maxX - minX + 4;
    double rangeY = maxY - minY + 4;
    int gridX = static_cast<int>(width() / rangeX);
    int gridY = static_cast<int>(height() / rangeY);
    m_gridSize = qBound(4, qMin(gridX, gridY), 60);
    double centerLogX = (minX + maxX) / 2.0;
    double centerLogY = (minY + maxY) / 2.0;
    m_panOffset.setX(static_cast<int>(-centerLogX * m_gridSize));
    m_panOffset.setY(static_cast<int>(centerLogY * m_gridSize));
    emit viewChanged(m_gridSize);
    update();
}

void Canvas::setClipRect(double xmin, double ymin, double xmax, double ymax) {
    m_clipRect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax)).normalized();
    // Auto re-clip
    if (!m_lines.isEmpty()) runCohenSutherland();
    if (!m_polygons.isEmpty()) runSutherlandHodgman();
    emit clipWindowChanged(m_clipRect.left(), m_clipRect.top(), m_clipRect.right(), m_clipRect.bottom());
    update();
}

void Canvas::runCohenSutherland() {
    m_csResults.clear();
    m_lbResults.clear();
    double xmin = m_clipRect.left(), ymin = m_clipRect.top();
    double xmax = m_clipRect.right(), ymax = m_clipRect.bottom();
    for (const auto &line : m_lines) {
        auto result = ClipAlgorithms::cohenSutherlandClip(line.p1, line.p2, xmin, ymin, xmax, ymax);
        m_csResults.append(result);
    }
    emit clippingFinished("Cohen-Sutherland", m_csResults.size());
    update();
}


void Canvas::runLiangBarsky() {
    m_lbResults.clear();
    m_csResults.clear();
    double xmin = m_clipRect.left(), ymin = m_clipRect.top();
    double xmax = m_clipRect.right(), ymax = m_clipRect.bottom();
    for (const auto &line : m_lines) {
        auto result = ClipAlgorithms::liangBarskyClip(line.p1, line.p2, xmin, ymin, xmax, ymax);
        m_lbResults.append(result);
    }
    emit clippingFinished("Liang-Barsky", m_lbResults.size());
    update();
}


void Canvas::runWeilerAtherton() {
    m_waResult.disjointPolygons.clear();
    m_shResult.stages.clear();
    m_shResult.clippedVertices.clear();
    QVector<QPointF> vertices;
    const PolygonShape *targetPoly = nullptr;
    if (m_selectedShapeId > 0) {
        for (const auto &p : m_polygons) {
            if (p.id == m_selectedShapeId) { targetPoly = &p; break; }
        }
    }
    if (!targetPoly && !m_polygons.isEmpty()) targetPoly = &m_polygons.last();
    if (!targetPoly) return;
    
    for (const auto &v : targetPoly->vertices) {
        vertices.append(QPointF(v));
    }
    
    double xmin = m_clipRect.left(), ymin = m_clipRect.top();
    double xmax = m_clipRect.right(), ymax = m_clipRect.bottom();
    
    m_waResult = ClipAlgorithms::weilerAthertonClip(vertices, xmin, ymin, xmax, ymax);
    
    emit clippingFinished("Weiler-Atherton", m_waResult.disjointPolygons.size());
    update();
}

void Canvas::runSutherlandHodgman() {
    m_waResult.disjointPolygons.clear();
    m_shResult.stages.clear();
    m_shResult.clippedVertices.clear();
    // Use the first polygon or the selected polygon
    QVector<QPointF> vertices;
    const PolygonShape *targetPoly = nullptr;
    if (m_selectedShapeId > 0) {
        for (const auto &p : m_polygons) {
            if (p.id == m_selectedShapeId) { targetPoly = &p; break; }
        }
    }
    if (!targetPoly && !m_polygons.isEmpty()) targetPoly = &m_polygons.last();
    if (!targetPoly) return;
    
    for (const auto &v : targetPoly->vertices) {
        vertices.append(QPointF(v));
    }
    
    double xmin = m_clipRect.left(), ymin = m_clipRect.top();
    double xmax = m_clipRect.right(), ymax = m_clipRect.bottom();
    m_shResult = ClipAlgorithms::sutherlandHodgmanClip(vertices, xmin, ymin, xmax, ymax);
    m_shStageIndex = 4;  // Show final result by default
    emit clippingFinished("Sutherland-Hodgman", 1);
    update();
}

void Canvas::setSHStage(int stage) {
    m_shStageIndex = qBound(0, stage, 4);
    update();
}

void Canvas::addLine(const QPointF &p1, const QPointF &p2, const QColor &color) {
    LineShape line;
    line.id = m_nextShapeId++;
    line.p1 = p1;
    line.p2 = p2;
    line.color = color.isValid() ? color : QColor(88, 166, 255);
    m_lines.append(line);
    emit statusMessage(QString("Line #%1 added: (%2,%3)→(%4,%5)").arg(line.id)
                       .arg(p1.x(),0,'f',1).arg(p1.y(),0,'f',1)
                       .arg(p2.x(),0,'f',1).arg(p2.y(),0,'f',1));
    update();
}

void Canvas::loadCSTestLines() {
    m_lines.clear();
    auto testLines = ClipAlgorithms::getCSTestLines();
    for (auto &tl : testLines) {
        LineShape ls;
        ls.id = m_nextShapeId++;
        ls.p1 = tl.p1;
        ls.p2 = tl.p2;
        ls.color = tl.color;
        m_lines.append(ls);
    }
    runCohenSutherland();
    emit statusMessage(QString("Loaded %1 test lines for Cohen-Sutherland.").arg(m_lines.size()));
    update();
}

void Canvas::loadSHTestPreset(const QString &name) {
    clearAllShapes();
    QVector<QPointF> verts;
    if (name == "Triangle") verts = ClipAlgorithms::getSHTestTriangle();
    else if (name == "Star") verts = ClipAlgorithms::getSHTestStar();
    else if (name == "LShape") verts = ClipAlgorithms::getSHTestLShape();
    else if (name == "NepalFlag" || name == "Nepal") {
        verts = ClipAlgorithms::getSHTestNepalFlag();
        // Set clipping window with left border at x = 0 so the two triangles become disjoint
        m_clipRect = QRectF(0, -16, 16, 32);
        emit clipWindowChanged(m_clipRect.left(), m_clipRect.top(), m_clipRect.right(), m_clipRect.bottom());
    } else verts = ClipAlgorithms::getSHTestTriangle();
    
    QVector<QPoint> intVerts;
    for (const auto &v : verts) intVerts.append(QPoint(qRound(v.x()), qRound(v.y())));
    addPolygon(intVerts, m_boundaryColor);
    runSutherlandHodgman();
}

void Canvas::setSplitDisjointSH(bool split) {
    m_splitDisjointSH = split;
    update();
}

// ----------------------------------------------------------------------------
// Rendering
// ----------------------------------------------------------------------------
void Canvas::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Deep dark background
    painter.fillRect(rect(), QColor(13, 17, 23));

    if (m_showGrid && m_gridSize >= 4) drawGrid(painter);
    if (m_showAxes) drawAxes(painter);

    // 1. Draw Ghost (Before Transform)
    if (m_showGhost) drawGhostShapes(painter);

    // 2. Draw Pixels & Shapes
    drawPixels(painter);
    drawAllShapes(painter);

    // 3. Draw Arbitrary Line & Pivot
    if (m_showReflectLine) drawReflectLine(painter);
    if (m_showRotationPivot) drawRotationPivot(painter);

    // 4. Draw Seed Marker
    if (m_hasSeedPoint) {
        QPoint sp = logicalToScreen(m_seedPoint);
        painter.setPen(QPen(QColor(255, 221, 89), 2));
        painter.drawLine(sp.x() - 8, sp.y(), sp.x() + 8, sp.y());
        painter.drawLine(sp.x(), sp.y() - 8, sp.x(), sp.y() + 8);
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(sp, 6, 6);
    }

    if (m_showClipWindow) drawClipWindow(painter);
    if (m_showClipWindow && m_showOutcodeLabels) drawOutcodeLabels(painter);
    if (m_showClipResult) {
        if (!m_csResults.isEmpty() || !m_lbResults.isEmpty()) drawClippedLines(painter);
        if (!m_shResult.clippedVertices.isEmpty()) drawClippedPolygon(painter);
        if (!m_waResult.disjointPolygons.isEmpty()) drawWAClippedPolygon(painter);
    }
    if (!m_lines.isEmpty() || m_isPlacingLineEnd) drawLineShapes(painter);

    if (m_showHover && m_isHovering) drawHoverHighlight(painter);
}

void Canvas::drawGrid(QPainter &painter) {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);
    int w = width(), h = height();
    int cx = w / 2 + m_panOffset.x();
    int cy = h / 2 + m_panOffset.y();

    painter.setPen(QPen(QColor(33, 38, 45), 1, Qt::DotLine));
    for (int x = cx; x <= w; x += m_gridSize) painter.drawLine(x, 0, x, h);
    for (int x = cx; x >= 0; x -= m_gridSize) painter.drawLine(x, 0, x, h);
    for (int y = cy; y <= h; y += m_gridSize) painter.drawLine(0, y, w, y);
    for (int y = cy; y >= 0; y -= m_gridSize) painter.drawLine(0, y, w, y);
    painter.restore();
}

void Canvas::drawAxes(QPainter &painter) {
    painter.save();
    int w = width(), h = height();
    int cx = w / 2 + m_panOffset.x();
    int cy = h / 2 + m_panOffset.y();

    painter.fillRect(0, cy - m_gridSize, w, m_gridSize, QColor(22, 27, 34, 120));
    painter.fillRect(cx, 0, m_gridSize, h, QColor(22, 27, 34, 120));
    painter.fillRect(cx, cy - m_gridSize, m_gridSize, m_gridSize, QColor(30, 40, 60, 180));

    painter.setPen(QPen(QColor(48, 54, 61), 1));
    painter.drawLine(0, cy, w, cy);
    painter.drawLine(cx, 0, cx, h);

    if (m_gridSize >= 12) {
        painter.setPen(QColor(139, 148, 158));
        QFont f = painter.font();
        f.setPointSize(8);
        painter.setFont(f);
        painter.drawText(cx + 4, cy - 4, "(0,0)");
        painter.drawText(w - 24, cy - 4, "+X");
        painter.drawText(cx + 4, 14, "+Y");
    }
    painter.restore();
}

void Canvas::drawPixels(QPainter &painter) {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);

    // Fill Pixels
    for (auto it = m_fillPixels.constBegin(); it != m_fillPixels.constEnd(); ++it) {
        QRect cell = logicalToCellRect(it.key());
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) continue;
        painter.fillRect(cell, it.value());
    }

    // Boundary Pixels
    for (auto it = m_boundaryPixels.constBegin(); it != m_boundaryPixels.constEnd(); ++it) {
        QRect cell = logicalToCellRect(it.key());
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) continue;
        painter.fillRect(cell, it.value());
    }
    painter.restore();
}

void Canvas::drawGhostShapes(QPainter &painter) {
    if (m_ghostPolygons.isEmpty() && m_ghostCircles.isEmpty()) return;

    painter.save();
    painter.setPen(QPen(QColor(139, 148, 158, 90), 1, Qt::DashDotLine));

    for (const auto &poly : m_ghostPolygons) {
        int n = poly.vertices.size();
        for (int i = 0; i < n; ++i) {
            QPoint p1 = logicalToScreen(poly.vertices[i]);
            QPoint p2 = logicalToScreen(poly.vertices[(i + 1) % n]);
            painter.drawLine(p1, p2);
        }
    }

    for (const auto &c : m_ghostCircles) {
        QPoint centerScr = logicalToScreen(c.center);
        int scrRad = c.radius * m_gridSize;
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(centerScr, scrRad, scrRad);
    }
    painter.restore();
}

void Canvas::drawAllShapes(QPainter &painter) {
    painter.save();

    // 1. Draw Polygons
    for (const auto &poly : m_polygons) {
        if (m_showClipResult && !m_shResult.clippedVertices.isEmpty()) {
            continue; // drawClippedPolygon renders the dotted original and solid clipped polygon
        }
        bool isSelected = (poly.id == m_selectedShapeId);
        int n = poly.vertices.size();
        if (n < 2) continue;

        QColor edgeColor = isSelected ? QColor(227, 179, 65) : QColor(255, 255, 255, 160);
        painter.setPen(QPen(edgeColor, isSelected ? 2 : 1, Qt::DashLine));

        for (int i = 0; i < n; ++i) {
            QPoint p1 = logicalToScreen(poly.vertices[i]);
            QPoint p2 = logicalToScreen(poly.vertices[(i + 1) % n]);
            painter.drawLine(p1, p2);
        }

        // Vertex handles
        for (int i = 0; i < n; ++i) {
            QPoint p = logicalToScreen(poly.vertices[i]);
            painter.setPen(QPen(isSelected ? QColor(255, 221, 89) : QColor(255, 255, 255), 2));
            painter.setBrush(isSelected ? QColor(227, 179, 65) : QColor(255, 71, 87));
            painter.drawEllipse(p, isSelected ? 6 : 4, isSelected ? 6 : 4);

            if (m_showVertexLabels) {
                painter.setPen(isSelected ? QColor(255, 221, 89) : QColor(240, 246, 252));
                QFont font = painter.font();
                font.setPointSize(8);
                font.setBold(isSelected);
                painter.setFont(font);
                painter.drawText(p.x() + 6, p.y() - 4, QString("P%1.V%2(%3,%4)")
                                     .arg(poly.id).arg(i).arg(poly.vertices[i].x()).arg(poly.vertices[i].y()));
            }
        }
    }

    // 2. Draw Circles
    for (const auto &c : m_circles) {
        bool isSelected = (c.id == m_selectedShapeId);
        QPoint centerScr = logicalToScreen(c.center);
        int scrRad = c.radius * m_gridSize;

        QColor edgeColor = isSelected ? QColor(227, 179, 65) : QColor(88, 166, 255, 180);
        painter.setPen(QPen(edgeColor, isSelected ? 2 : 1, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(centerScr, scrRad, scrRad);

        // Center dot
        painter.setPen(QPen(QColor(255, 255, 255), 2));
        painter.setBrush(isSelected ? QColor(227, 179, 65) : QColor(0, 230, 255));
        painter.drawEllipse(centerScr, 4, 4);

        if (m_showVertexLabels) {
            painter.setPen(isSelected ? QColor(255, 221, 89) : QColor(88, 166, 255));
            QFont font = painter.font();
            font.setPointSize(8);
            font.setBold(isSelected);
            painter.setFont(font);
            painter.drawText(centerScr.x() + 6, centerScr.y() - 4, QString("C%1(%2,%3) r=%4")
                                 .arg(c.id).arg(c.center.x()).arg(c.center.y()).arg(c.radius));
        }
    }

    // 3. Draw In-progress Polygon
    if (!m_currentPolygonVertices.isEmpty()) {
        int n = m_currentPolygonVertices.size();
        painter.setPen(QPen(QColor(0, 230, 255, 180), 1, Qt::DashLine));
        for (int i = 0; i < n - 1; ++i) {
            QPoint p1 = logicalToScreen(m_currentPolygonVertices[i]);
            QPoint p2 = logicalToScreen(m_currentPolygonVertices[i + 1]);
            painter.drawLine(p1, p2);
        }
        for (int i = 0; i < n; ++i) {
            QPoint p = logicalToScreen(m_currentPolygonVertices[i]);
            painter.setPen(QPen(QColor(255, 255, 255), 2));
            painter.setBrush(QColor(255, 221, 89));
            painter.drawEllipse(p, 5, 5);
        }
    }

    // 4. Draw In-progress Circle
    if (m_isPlacingCircleRadius && m_isHovering) {
        QPoint centerScr = logicalToScreen(m_circleCenterPoint);
        int dx = m_hoverLogical.x() - m_circleCenterPoint.x();
        int dy = m_hoverLogical.y() - m_circleCenterPoint.y();
        int curRad = std::max(1, static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy))));
        int scrRad = curRad * m_gridSize;

        painter.setPen(QPen(QColor(255, 159, 67, 180), 1, Qt::DashLine));
        painter.setBrush(Qt::NoBrush);
        painter.drawEllipse(centerScr, scrRad, scrRad);
    }

    painter.restore();
}

void Canvas::drawRotationPivot(QPainter &painter) {
    painter.save();
    QPoint pScr = logicalToScreen(m_rotationPivot);

    // Cyan glowing crosshair with double ring
    painter.setPen(QPen(QColor(0, 230, 255), 2));
    painter.drawLine(pScr.x() - 12, pScr.y(), pScr.x() + 12, pScr.y());
    painter.drawLine(pScr.x(), pScr.y() - 12, pScr.x(), pScr.y() + 12);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(pScr, 8, 8);
    painter.drawEllipse(pScr, 3, 3);

    painter.setPen(QColor(0, 230, 255));
    QFont f = painter.font();
    f.setPointSize(8);
    f.setBold(true);
    painter.setFont(f);
    painter.drawText(pScr.x() + 12, pScr.y() - 6, QString("Pivot(%1,%2)").arg(m_rotationPivot.x()).arg(m_rotationPivot.y()));

    painter.restore();
}

void Canvas::drawReflectLine(QPainter &painter) {
    painter.save();
    int w = width();
    int minLogX = -w / (2 * m_gridSize) - 5;
    int maxLogX = w / (2 * m_gridSize) + 5;

    double y1 = m_reflectLineSlope * minLogX + m_reflectLineIntercept;
    double y2 = m_reflectLineSlope * maxLogX + m_reflectLineIntercept;

    QPoint p1Scr = logicalToScreen(QPoint(minLogX, static_cast<int>(std::round(y1))));
    QPoint p2Scr = logicalToScreen(QPoint(maxLogX, static_cast<int>(std::round(y2))));

    // Vibrant green dashed reflection mirror line
    painter.setPen(QPen(QColor(46, 213, 115, 220), 2, Qt::DashLine));
    painter.drawLine(p1Scr, p2Scr);

    // Label badge near line
    QPoint midScr = logicalToScreen(QPoint(0, static_cast<int>(std::round(m_reflectLineIntercept))));
    painter.setPen(QColor(46, 213, 115));
    QFont f = painter.font();
    f.setPointSize(8);
    f.setBold(true);
    painter.setFont(f);
    painter.drawText(midScr.x() + 10, midScr.y() - 8,
                     QString("Reflect Line: y = %1x + %2")
                         .arg(m_reflectLineSlope, 0, 'f', 2)
                         .arg(m_reflectLineIntercept, 0, 'f', 2));

    painter.restore();
}

void Canvas::drawHoverHighlight(QPainter &painter) {
    painter.save();
    QRect hoverCell = logicalToCellRect(m_hoverLogical);
    painter.setPen(QPen(QColor(255, 255, 255, 140), 1));
    painter.setBrush(QColor(255, 255, 255, 25));
    painter.drawRect(hoverCell.adjusted(0, 0, -1, -1));
    painter.restore();
}

void Canvas::drawClipWindow(QPainter &painter) {
    painter.save();
    QRect scrRect = getClipWindowScreenRect();
    int left = scrRect.left();
    int right = scrRect.right();
    int top = scrRect.top();
    int bottom = scrRect.bottom();

    // 1. Dark overlay OUTSIDE the selected clipping region
    painter.fillRect(0, 0, width(), top, QColor(0, 0, 0, 150)); // Top
    painter.fillRect(0, bottom, width(), height() - bottom, QColor(0, 0, 0, 150)); // Bottom
    painter.fillRect(0, top, left, bottom - top, QColor(0, 0, 0, 150)); // Left
    painter.fillRect(right, top, width() - right, bottom - top, QColor(0, 0, 0, 150)); // Right

    // 2. FILL THE INTERNAL SELECTED REGION with a luminous tint
    painter.fillRect(scrRect, QColor(0, 210, 255, 30));

    // Fill each discrete raster grid cell inside the internal selected region with a subtle border
    if (m_gridSize >= 6) {
        int xmin = qRound(m_clipRect.left());
        int xmax = qRound(m_clipRect.right());
        int ymin = qRound(m_clipRect.top());
        int ymax = qRound(m_clipRect.bottom());
        painter.setPen(QPen(QColor(0, 230, 255, 35), 1, Qt::DotLine));
        for (int x = xmin; x <= xmax; ++x) {
            for (int y = ymin; y <= ymax; ++y) {
                QRect c = logicalToCellRect(QPoint(x, y));
                painter.drawRect(c.adjusted(0, 0, -1, -1));
            }
        }
    }

    // 3. Bright cyan dashed border
    painter.setPen(QPen(QColor(0, 230, 255), 2, Qt::DashLine));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(scrRect);

    // 4. 8 handles (corner and edge midpoints) with white outline
    painter.setBrush(QColor(0, 230, 255));
    painter.setPen(QPen(QColor(255, 255, 255), 1));
    auto drawHandle = [&](int x, int y) { painter.drawRect(x - 4, y - 4, 8, 8); };
    int midX = left + (right - left) / 2;
    int midY = top + (bottom - top) / 2;
    drawHandle(left, top); drawHandle(midX, top); drawHandle(right, top);
    drawHandle(left, midY); drawHandle(right, midY);
    drawHandle(left, bottom); drawHandle(midX, bottom); drawHandle(right, bottom);

    // Coordinate badges
    painter.setPen(QColor(0, 230, 255));
    QFont font = painter.font();
    font.setPointSize(8);
    font.setBold(true);
    painter.setFont(font);
    painter.drawText(left + 6, bottom - 6, QString("Min(%1, %2)").arg(qRound(m_clipRect.left())).arg(qRound(m_clipRect.top())));
    painter.drawText(right - 75, top + 14, QString("Max(%1, %2)").arg(qRound(m_clipRect.right())).arg(qRound(m_clipRect.bottom())));

    painter.restore();
}

void Canvas::drawOutcodeLabels(QPainter &painter) {
    painter.save();
    int l = std::min(logicalToScreenF(QPointF(m_clipRect.left(), m_clipRect.top())).x(), logicalToScreenF(QPointF(m_clipRect.right(), m_clipRect.bottom())).x());
    int r = std::max(logicalToScreenF(QPointF(m_clipRect.left(), m_clipRect.top())).x(), logicalToScreenF(QPointF(m_clipRect.right(), m_clipRect.bottom())).x());
    int b = std::max(logicalToScreenF(QPointF(m_clipRect.left(), m_clipRect.top())).y(), logicalToScreenF(QPointF(m_clipRect.right(), m_clipRect.bottom())).y()); // screen bottom (high y)
    int t = std::min(logicalToScreenF(QPointF(m_clipRect.left(), m_clipRect.top())).y(), logicalToScreenF(QPointF(m_clipRect.right(), m_clipRect.bottom())).y()); // screen top (low y)

    painter.setPen(QColor(255, 255, 255, 150));
    painter.drawText(l - 40, t - 10, "1001");
    painter.drawText(l + (r - l)/2 - 15, t - 10, "1000");
    painter.drawText(r + 10, t - 10, "1010");
    
    painter.drawText(l - 40, t + (b - t)/2, "0001");
    painter.drawText(l + (r - l)/2 - 15, t + (b - t)/2, "0000");
    painter.drawText(r + 10, t + (b - t)/2, "0010");
    
    painter.drawText(l - 40, b + 20, "0101");
    painter.drawText(l + (r - l)/2 - 15, b + 20, "0100");
    painter.drawText(r + 10, b + 20, "0110");

    painter.restore();
}

void Canvas::drawClippedLines(QPainter &painter) {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);

    auto drawPixelCell = [&](const QPoint &logicalPt, const QColor &color, bool border = true) {
        QRect cell = logicalToCellRect(logicalPt);
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) return;
        painter.fillRect(cell, color);
        if (border && m_gridSize >= 6) {
            painter.setPen(color.darker(140));
            painter.drawRect(cell.adjusted(0, 0, -1, -1));
        }
    };

    auto drawDottedPixelCell = [&](const QPoint &logicalPt, const QColor &color) {
        QRect cell = logicalToCellRect(logicalPt);
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) return;
        painter.fillRect(cell, color);
        if (m_gridSize >= 6) {
            painter.setPen(QColor(255, 255, 255, 180));
            painter.drawRect(cell.adjusted(0, 0, -1, -1));
        }
    };

auto drawResultBlock = [&](const auto &res) {
        QPoint p1(qRound(res.originalP1.x()), qRound(res.originalP1.y()));
        QPoint p2(qRound(res.originalP2.x()), qRound(res.originalP2.y()));

        if (res.accepted) {
            QPoint cp1(qRound(res.clippedP1.x()), qRound(res.clippedP1.y()));
            QPoint cp2(qRound(res.clippedP2.x()), qRound(res.clippedP2.y()));

            auto acceptedPts = CGAlgorithms::bresenhamLine(cp1.x(), cp1.y(), cp2.x(), cp2.y());
            for (const auto &pt : acceptedPts) {
                drawPixelCell(pt, QColor(46, 213, 115));
            }

            if (p1 != cp1) {
                auto clippedPts1 = CGAlgorithms::bresenhamLine(p1.x(), p1.y(), cp1.x(), cp1.y());
                for (int i = 0; i < clippedPts1.size(); ++i) {
                    if (i % 2 == 0) drawDottedPixelCell(clippedPts1[i], QColor(255, 71, 87));
                }
            }

            if (cp2 != p2) {
                auto clippedPts2 = CGAlgorithms::bresenhamLine(cp2.x(), cp2.y(), p2.x(), p2.y());
                for (int i = 0; i < clippedPts2.size(); ++i) {
                    if (i % 2 == 0) drawDottedPixelCell(clippedPts2[i], QColor(255, 71, 87));
                }
            }

            drawPixelCell(cp1, QColor(255, 221, 89));
            drawPixelCell(cp2, QColor(255, 221, 89));
            drawPixelCell(p1, QColor(255, 71, 87));
            drawPixelCell(p2, QColor(255, 71, 87));
        } else {
            auto allPts = CGAlgorithms::bresenhamLine(p1.x(), p1.y(), p2.x(), p2.y());
            for (int i = 0; i < allPts.size(); ++i) {
                if (i % 2 == 0) drawDottedPixelCell(allPts[i], QColor(255, 71, 87));
            }
            drawPixelCell(p1, QColor(255, 71, 87));
            drawPixelCell(p2, QColor(255, 71, 87));
        }
    };

    if (!m_csResults.isEmpty()) {
        for (const auto &res : m_csResults) drawResultBlock(res);
    } else if (!m_lbResults.isEmpty()) {
        for (const auto &res : m_lbResults) drawResultBlock(res);
    }

    painter.restore();
}

void Canvas::drawClippedPolygon(QPainter &painter) {
    if (m_shResult.stages.isEmpty()) return;
    int idx = qBound(0, m_shStageIndex, m_shResult.stages.size() - 1);
    const auto &stage = m_shResult.stages[idx];

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);

    auto drawPixelCell = [&](const QPoint &logicalPt, const QColor &color, bool border = true) {
        QRect cell = logicalToCellRect(logicalPt);
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) return;
        painter.fillRect(cell, color);
        if (border && m_gridSize >= 6) {
            painter.setPen(color.darker(140));
            painter.drawRect(cell.adjusted(0, 0, -1, -1));
        }
    };

    auto drawDottedPixelCell = [&](const QPoint &logicalPt, const QColor &color) {
        QRect cell = logicalToCellRect(logicalPt);
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) return;
        painter.fillRect(cell, color);
        if (m_gridSize >= 6) {
            painter.setPen(QColor(255, 255, 255, 180));
            painter.drawRect(cell.adjusted(0, 0, -1, -1));
        }
    };

    // 1. Draw original polygon boundary as DOTTED raster pixels (showing clipped-away edges)
    if (m_shResult.stages.size() > 0) {
        const auto &orig = m_shResult.stages[0].inputVertices;
        int on = orig.size();
        if (on >= 3) {
            for (int i = 0; i < on; ++i) {
                QPoint v1(qRound(orig[i].x()), qRound(orig[i].y()));
                QPoint v2(qRound(orig[(i + 1) % on].x()), qRound(orig[(i + 1) % on].y()));
                auto edgePts = CGAlgorithms::bresenhamLine(v1.x(), v1.y(), v2.x(), v2.y());
                for (int j = 0; j < edgePts.size(); ++j) {
                    if (j % 2 == 0) {
                        drawDottedPixelCell(edgePts[j], QColor(255, 107, 129));
                    }
                }
                drawPixelCell(v1, QColor(255, 107, 129));
            }
        }
    }

    // 2. Draw clipped polygon(s) for current stage
    QVector<QVector<QPointF>> polysToDraw;
    if (m_splitDisjointSH && !stage.outputPolygons.isEmpty()) {
        polysToDraw = stage.outputPolygons;
    } else if (!stage.outputVertices.isEmpty()) {
        polysToDraw.append(stage.outputVertices);
    }

    for (const auto &polyVerts : polysToDraw) {
        QVector<QPoint> clippedIntVerts;
        for (const auto &v : polyVerts) {
            clippedIntVerts.append(QPoint(qRound(v.x()), qRound(v.y())));
        }

        if (clippedIntVerts.size() >= 3) {
            // Raster scanline fill for interior pixels
            auto fillRes = CGAlgorithms::scanlineFill(clippedIntVerts);
            for (const auto &pt : fillRes.filledPixels) {
                drawPixelCell(pt, QColor(46, 213, 115, 120), false);
            }

            // Fill internal clipped polygon area
            QPolygon poly;
            for (const auto &v : polyVerts) {
                poly << logicalToScreenF(v);
            }
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(46, 213, 115, 75));
            painter.drawPolygon(poly);

            // Draw boundary as solid raster pixels
            int cn = clippedIntVerts.size();
            for (int i = 0; i < cn; ++i) {
                QPoint c1 = clippedIntVerts[i];
                QPoint c2 = clippedIntVerts[(i + 1) % cn];
                auto edgePts = CGAlgorithms::bresenhamLine(c1.x(), c1.y(), c2.x(), c2.y());
                for (const auto &pt : edgePts) {
                    drawPixelCell(pt, QColor(46, 213, 115), true);
                }
                drawPixelCell(c1, QColor(255, 255, 255), true);
            }
        }
    }
    painter.restore();
}

void Canvas::drawWAClippedPolygon(QPainter &painter) {
    if (m_waResult.disjointPolygons.isEmpty()) return;

    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);

    auto drawPixelCell = [&](const QPoint &logicalPt, const QColor &color, bool border = true) {
        QRect cell = logicalToCellRect(logicalPt);
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) return;
        painter.fillRect(cell, color);
        if (border && m_gridSize >= 6) {
            painter.setPen(color.darker(140));
            painter.drawRect(cell.adjusted(0, 0, -1, -1));
        }
    };

    auto drawDottedPixelCell = [&](const QPoint &logicalPt, const QColor &color) {
        QRect cell = logicalToCellRect(logicalPt);
        if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) return;
        painter.fillRect(cell, color);
        if (m_gridSize >= 6) {
            painter.setPen(QColor(255, 255, 255, 180));
            painter.drawRect(cell.adjusted(0, 0, -1, -1));
        }
    };

    // Draw original polygon boundary as DOTTED
    const auto &orig = m_waResult.originalVertices;
    int on = orig.size();
    if (on >= 3) {
        for (int i = 0; i < on; ++i) {
            QPoint v1(qRound(orig[i].x()), qRound(orig[i].y()));
            QPoint v2(qRound(orig[(i + 1) % on].x()), qRound(orig[(i + 1) % on].y()));
            auto edgePts = CGAlgorithms::bresenhamLine(v1.x(), v1.y(), v2.x(), v2.y());
            for (int j = 0; j < edgePts.size(); ++j) {
                if (j % 2 == 0) drawDottedPixelCell(edgePts[j], QColor(255, 107, 129));
            }
            drawPixelCell(v1, QColor(255, 107, 129));
        }
    }

    // Draw clipped disjoint polygons
    for (const auto &polyVerts : m_waResult.disjointPolygons) {
        QVector<QPoint> clippedIntVerts;
        for (const auto &v : polyVerts) clippedIntVerts.append(QPoint(qRound(v.x()), qRound(v.y())));

        if (clippedIntVerts.size() >= 3) {
            auto fillRes = CGAlgorithms::scanlineFill(clippedIntVerts);
            for (const auto &pt : fillRes.filledPixels) drawPixelCell(pt, QColor(46, 213, 115, 120), false);

            QPolygon poly;
            for (const auto &v : polyVerts) poly << logicalToScreenF(v);
            painter.setPen(Qt::NoPen);
            painter.setBrush(QColor(46, 213, 115, 75));
            painter.drawPolygon(poly);

            int n = clippedIntVerts.size();
            for (int i = 0; i < n; ++i) {
                auto edgePts = CGAlgorithms::bresenhamLine(clippedIntVerts[i].x(), clippedIntVerts[i].y(),
                                                           clippedIntVerts[(i + 1) % n].x(), clippedIntVerts[(i + 1) % n].y());
                for (const auto &pt : edgePts) drawPixelCell(pt, QColor(46, 213, 115));
                drawPixelCell(clippedIntVerts[i], QColor(255, 221, 89));
            }
        }
    }

    painter.restore();
}


void Canvas::drawLineShapes(QPainter &painter) {
    painter.save();
    painter.setRenderHint(QPainter::Antialiasing, false);

    // When showing clipping results, drawClippedLines handles drawing the lines as raster pixels
    if (!m_showClipResult || m_csResults.isEmpty()) {
        for (const auto &l : m_lines) {
            QPoint p1(qRound(l.p1.x()), qRound(l.p1.y()));
            QPoint p2(qRound(l.p2.x()), qRound(l.p2.y()));
            auto pts = CGAlgorithms::bresenhamLine(p1.x(), p1.y(), p2.x(), p2.y());
            for (const auto &pt : pts) {
                QRect cell = logicalToCellRect(pt);
                if (cell.right() < 0 || cell.left() > width() || cell.bottom() < 0 || cell.top() > height()) continue;
                painter.fillRect(cell, l.color);
                if (m_gridSize >= 6) {
                    painter.setPen(l.color.darker(140));
                    painter.drawRect(cell.adjusted(0, 0, -1, -1));
                }
            }
            // Endpoints
            QRect cell1 = logicalToCellRect(p1);
            QRect cell2 = logicalToCellRect(p2);
            painter.fillRect(cell1, l.color.lighter(130));
            painter.fillRect(cell2, l.color.lighter(130));
        }
    }

    if (m_isPlacingLineEnd && m_isHovering) {
        QPoint p1(qRound(m_lineStartPoint.x()), qRound(m_lineStartPoint.y()));
        QPoint p2 = m_hoverLogical;
        auto pts = CGAlgorithms::bresenhamLine(p1.x(), p1.y(), p2.x(), p2.y());
        for (int i = 0; i < pts.size(); ++i) {
            if (i % 2 == 0) {
                QRect cell = logicalToCellRect(pts[i]);
                painter.fillRect(cell, QColor(88, 166, 255, 180));
            }
        }
    }
    painter.restore();
}
