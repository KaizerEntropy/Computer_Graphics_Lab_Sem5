#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QPoint>
#include <QVector>
#include <QColor>
#include <QHash>
#include <QString>
#include <chrono>

namespace CGAlgorithms {

// Custom hash function for QPoint
inline size_t qHash(const QPoint &key, size_t seed = 0) {
    return ::qHash(static_cast<quint64>(static_cast<quint32>(key.x())) << 32 | static_cast<quint32>(key.y()), seed);
}

// Edge structure for Scanline Edge Table (ET) & Active Edge Table (AET)
struct ScanEdge {
    int edgeId = 0;          // Edge index
    int v1Index = 0;         // Starting vertex index
    int v2Index = 0;         // Ending vertex index
    int yMax = 0;            // Upper y limit of edge
    double xCurrent = 0.0;   // Current x intersection with current scanline
    double invSlope = 0.0;   // 1/m = (x2 - x1) / (y2 - y1)

    bool operator<(const ScanEdge &other) const {
        if (xCurrent != other.xCurrent)
            return xCurrent < other.xCurrent;
        return invSlope < other.invSlope;
    }
};

struct FillStep {
    QVector<QPoint> newPixels;
    QString description;
    int currentFrontierSize = 0;
    int scanlineY = 0;
    QVector<ScanEdge> activeEdges;
    QString aetDetails;
};

struct FillResult {
    QString algorithmName;
    QVector<QPoint> filledPixels;
    QVector<FillStep> steps;
    qint64 elapsedNanos = 0;
    int pointCount = 0;
    int maxMemoryUnits = 0;
    QString timeComplexity;
    QString spaceComplexity;
    QString complexityExplanation;
};

// Line and Circle drawing for boundary edges
QVector<QPoint> bresenhamLine(int x1, int y1, int x2, int y2);
QVector<QPoint> bresenhamCircle(int xc, int yc, int r);

// Scanline Polygon Fill: Row-by-row filling using Edge Table & Active Edge Table
FillResult scanlineFill(const QVector<QPoint> &vertices);

// Boundary Fill (8-Connected - BFS & DFS)
FillResult boundaryFill8_BFS(const QHash<QPoint, QColor> &grid, const QPoint &seed,
                             const QColor &boundaryColor, const QColor &fillColor,
                             int minX = -80, int maxX = 80, int minY = -80, int maxY = 80);

FillResult boundaryFill8_DFS(const QHash<QPoint, QColor> &grid, const QPoint &seed,
                             const QColor &boundaryColor, const QColor &fillColor,
                             int minX = -80, int maxX = 80, int minY = -80, int maxY = 80);

// Predefined Geometric Shapes for Vector Transformations testing
QVector<QPoint> getTrianglePreset();
QVector<QPoint> getConvexPolygonPreset();
QVector<QPoint> getConcaveArrowPreset();
QVector<QPoint> getStarPolygonPreset();
QVector<QPoint> getHousePreset();
QVector<QPoint> getHexagonPreset();
QVector<QPoint> getNestedOuterPreset();
QVector<QPoint> getNestedInnerPreset();

void getMultiShapeScenePreset(QVector<QPoint> &poly1, QVector<QPoint> &poly2, QPoint &circleCenter, int &circleRadius);

} // namespace CGAlgorithms

#endif // ALGORITHMS_H
