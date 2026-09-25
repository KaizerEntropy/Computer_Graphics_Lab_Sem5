#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QHash>
#include <QVector>
#include <QPoint>
#include <QColor>
#include "matrix3x3.h"
#include "algorithms.h"

class Canvas : public QWidget
{
    Q_OBJECT

public:
    enum FillMode {
        ModeSelectShape = 0,
        ModeScanlineFill = 1,
        ModeBoundaryFill8 = 2,
        ModeDrawPolygon = 3,
        ModeDrawCircle = 4,
        ModeSetPivot = 5,
        ModeSetReflectLine = 6
    };

    struct PolygonShape {
        int id = 0;
        QVector<QPoint> vertices;
        QColor boundaryColor = QColor(255, 255, 255);
        bool isClosed = true;
    };

    struct CircleShape {
        int id = 0;
        QPoint center = QPoint(0, 0);
        int radius = 0;
        QColor boundaryColor = QColor(255, 255, 255);
    };

    struct ShapeFillRecord {
        QString algorithmName;
        QVector<QPoint> points;
        QColor color;
        qint64 elapsedNanos;
        QPoint seed;
    };

    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas() override;

    // Tool Mode & Settings
    void setFillMode(FillMode mode);
    FillMode fillMode() const { return m_fillMode; }

    void setBoundaryColor(const QColor &color);
    QColor boundaryColor() const { return m_boundaryColor; }

    void setFillColor(const QColor &color);
    QColor fillColor() const { return m_fillColor; }

    void setGridSize(int size);
    int gridSize() const { return m_gridSize; }

    void setShowGrid(bool show);
    void setShowAxes(bool show);
    void setShowHover(bool show);
    void setShowVertexLabels(bool show);
    void setShowGhost(bool show);
    void setShowRotationPivot(bool show);
    void setShowReflectLine(bool show);
    void setShowScanlineBeam(bool show);

    // Selection & Scope Management
    void selectShape(int id);
    void deselectAll(); // Reverts to Entire Space Mode (id = -1)
    int selectedShapeId() const { return m_selectedShapeId; }
    bool isEntireSpaceSelected() const { return m_selectedShapeId == -1; }
    QString getSelectedShapeName() const;
    QPoint getSelectedShapeCentroid() const;

    // Transformation Execution via Homogeneous Matrices
    void applyTransformation(const Matrix3x3 &mat, const QString &name, const QString &formula = "", const QString &details = "");
    void resetTransformations();
    void undoLastTransformation();
    const Matrix3x3& compositeMatrix() const { return m_compositeMatrix; }
    const QVector<Matrix3x3::TransformStep>& transformHistory() const { return m_transformHistory; }

    // Arbitrary Point & Line Parameters
    void setRotationPivot(const QPoint &pt);
    QPoint rotationPivot() const { return m_rotationPivot; }

    void setReflectLine(double slope, double intercept);
    double reflectLineSlope() const { return m_reflectLineSlope; }
    double reflectLineIntercept() const { return m_reflectLineIntercept; }

    // Multi-Shape Management
    void addPolygon(const QVector<QPoint> &vertices, const QColor &color = QColor());
    void addCircle(const QPoint &center, int radius, const QColor &color = QColor());
    void addPolygonVertex(const QPoint &pt);
    void closePolygon();
    void clearCurrentDrawing();
    void clearAllShapes();
    void undoLastShape();

    // Shape Presets
    void setPolygonPreset(const QVector<QPoint> &vertices);
    void setNestedPolygonPreset(const QVector<QPoint> &outerVerts, const QVector<QPoint> &innerVerts);
    void setMultiShapeScenePreset();

    // Accessors
    const QVector<PolygonShape>& polygons() const { return m_polygons; }
    const QVector<CircleShape>& circles() const { return m_circles; }
    const QVector<QPoint>& currentPolygonVertices() const { return m_currentPolygonVertices; }
    QVector<QPoint> activeOrLastPolygonVertices() const;

    // Fill Triggers
    void triggerFill();
    void triggerFillAt(const QPoint &seed);
    void setSeedPoint(const QPoint &pt);
    QPoint seedPoint() const { return m_seedPoint; }
    bool hasSeedPoint() const { return m_hasSeedPoint; }

    void undoLastFill();
    void clearFillOnly();
    void clearAll();

    // Coordinates conversion
    QPoint screenToLogical(const QPoint &screenPos) const;
    QPoint logicalToScreen(const QPoint &logicalPos) const;
    QRect logicalToCellRect(const QPoint &logicalPos) const;

signals:
    void mouseMoved(int logX, int logY, int scrX, int scrY);
    void pointClicked(QPoint logicalPt);
    void shapeSelectionChanged(int shapeId, const QString &description);
    void transformationApplied(const Matrix3x3::TransformStep &step, const Matrix3x3 &composite);
    void transformationsReset();
    void shapesChanged(int totalPolygons, int totalCircles);
    void seedPointSet(QPoint seed);
    void pivotChanged(QPoint pivot);
    void reflectLineChanged(double slope, double intercept);
    void fillFinished(const QString &algorithmName, qint64 elapsedNanos, int pointCount);
    void statusMessage(const QString &msg);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;

private:
    void rebuildAllBoundaryPixels();
    int findShapeAt(const QPoint &pt) const;

    void drawGrid(QPainter &painter);
    void drawAxes(QPainter &painter);
    void drawPixels(QPainter &painter);
    void drawGhostShapes(QPainter &painter);
    void drawAllShapes(QPainter &painter);
    void drawRotationPivot(QPainter &painter);
    void drawReflectLine(QPainter &painter);
    void drawActiveScanline(QPainter &painter);
    void drawHoverHighlight(QPainter &painter);

    // State
    FillMode m_fillMode = ModeSelectShape;
    int m_gridSize = 15;
    bool m_showGrid = true;
    bool m_showAxes = true;
    bool m_showHover = true;
    bool m_showVertexLabels = true;
    bool m_showGhost = true;
    bool m_showRotationPivot = true;
    bool m_showReflectLine = true;
    bool m_showScanlineBeam = true;

    QColor m_boundaryColor = QColor(255, 255, 255);
    QColor m_fillColor = QColor(0, 230, 255);

    // Selection
    int m_selectedShapeId = -1; // -1 = Entire Space / All Shapes

    // Transformations
    Matrix3x3 m_compositeMatrix;
    QVector<Matrix3x3::TransformStep> m_transformHistory;

    // Ghost copy for before/after comparison
    QVector<PolygonShape> m_ghostPolygons;
    QVector<CircleShape> m_ghostCircles;

    // Arbitrary Point & Line Parameters
    QPoint m_rotationPivot = QPoint(0, 0);
    double m_reflectLineSlope = 1.0;
    double m_reflectLineIntercept = 0.0;
    QPoint m_reflectLinePt1 = QPoint(-20, -20);
    QPoint m_reflectLinePt2 = QPoint(20, 20);

    // Persistent Geometry Collections
    QVector<PolygonShape> m_polygons;
    QVector<CircleShape> m_circles;
    QVector<QPoint> m_currentPolygonVertices;
    int m_nextShapeId = 1;

    // Circle interactive drawing state
    bool m_isPlacingCircleRadius = false;
    QPoint m_circleCenterPoint = QPoint(0, 0);

    // Seed Point
    QPoint m_seedPoint = QPoint(0, 0);
    bool m_hasSeedPoint = false;

    // Raster Grid Layers
    QHash<QPoint, QColor> m_boundaryPixels;
    QHash<QPoint, QColor> m_fillPixels;
    QVector<ShapeFillRecord> m_fillHistory;

    // Fill Result
    CGAlgorithms::FillResult m_fillResult;

    // Hover
    bool m_isHovering = false;
    QPoint m_hoverLogical = QPoint(0, 0);
};

#endif // CANVAS_H
