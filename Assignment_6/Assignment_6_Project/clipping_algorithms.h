#ifndef CLIPPING_ALGORITHMS_H
#define CLIPPING_ALGORITHMS_H

#include <QPoint>
#include <QPointF>
#include <QVector>
#include <QRect>
#include <QString>
#include <QColor>

namespace ClipAlgorithms {

// ---- Cohen-Sutherland Line Clipping ----

// 4-bit outcode constants
enum Outcode : int {
    INSIDE = 0,  // 0000
    LEFT   = 1,  // 0001
    RIGHT  = 2,  // 0010
    BOTTOM = 4,  // 0100
    TOP    = 8   // 1000
};

// A step in Cohen-Sutherland tracing
struct CSStep {
    int stepNumber = 0;
    int code1 = 0;
    int code2 = 0;
    QPointF p1, p2;           // Current endpoints
    QString description;       // e.g. "Clipping against LEFT boundary"
    QString outcodeStr1;       // e.g. "1001" 
    QString outcodeStr2;
    bool isAccepted = false;
    bool isRejected = false;
};

struct CSResult {
    QPointF originalP1, originalP2;  // Original unclipped line
    QPointF clippedP1, clippedP2;    // Clipped result (if accepted)
    bool accepted = false;
    QVector<CSStep> steps;           // Step-by-step trace
};

// Compute 4-bit outcode for a point relative to clip window
int computeOutcode(double x, double y, double xmin, double ymin, double xmax, double ymax);

// Convert outcode to 4-bit string like "1001"
QString outcodeToString(int code);

// Full Cohen-Sutherland line clipping with step trace
CSResult cohenSutherlandClip(QPointF p1, QPointF p2, double xmin, double ymin, double xmax, double ymax);


// ---- Liang-Barsky Line Clipping ----
struct LBResult {
    QPointF originalP1, originalP2;
    QPointF clippedP1, clippedP2;
    bool accepted = false;
    double t0 = 0.0;
    double t1 = 1.0;
    QString trace;
};

LBResult liangBarskyClip(QPointF p1, QPointF p2, double xmin, double ymin, double xmax, double ymax);


// ---- Sutherland-Hodgman Polygon Clipping ----

// Represents one stage of the pipeline
struct SHStage {
    int stageNumber = 0;       // 0=Original, 1=Left, 2=Right, 3=Bottom, 4=Top
    QString edgeName;          // "Left", "Right", "Bottom", "Top"
    QVector<QPointF> inputVertices;
    QVector<QPointF> outputVertices;          // Combined classic single polygon loop
    QVector<QVector<QPointF>> outputPolygons; // Disjoint separated polygons
    QVector<QString> decisions;  // Per-edge decisions like "In→Out: output I"
};

struct SHResult {
    QVector<QPointF> originalVertices;
    QVector<QPointF> clippedVertices;           // Final combined classic single polygon
    QVector<QVector<QPointF>> disjointPolygons;  // Final disjoint separated polygons
    QVector<SHStage> stages;                    // 4 stages trace
    bool wasSplit = false;                      // True if polygon was divided into disjoint components
};

// Clip polygon vertices against one edge (classic single loop)
QVector<QPointF> clipAgainstEdge(const QVector<QPointF> &vertices, 
                                  const QPointF &edgeStart, const QPointF &edgeEnd,
                                  QVector<QString> *decisions = nullptr);

// Clip polygon vertices against one edge, segmenting into disjoint polygons
QVector<QVector<QPointF>> clipPolygonAgainstEdge(const QVector<QPointF> &vertices,
                                                 const QPointF &edgeStart, const QPointF &edgeEnd,
                                                 QVector<QString> *decisions = nullptr);

// Full Sutherland-Hodgman polygon clipping with stage trace
SHResult sutherlandHodgmanClip(const QVector<QPointF> &vertices, 
                                double xmin, double ymin, double xmax, double ymax);

// ---- Line Segment Data for Scene ----
struct LineSegment {
    QPointF p1, p2;
    QColor color;
    int id = 0;
};

// ---- Test Presets ----
// Returns a set of test line segments that span all 9 Cohen-Sutherland regions
QVector<LineSegment> getCSTestLines();

// Returns various test polygons for Sutherland-Hodgman
QVector<QPointF> getSHTestTriangle();
QVector<QPointF> getSHTestStar();
QVector<QPointF> getSHTestLShape();
QVector<QPointF> getSHTestNepalFlag();


// ---- Weiler-Atherton Polygon Clipping ----
struct WAResult {
    QVector<QPointF> originalVertices;
    QVector<QVector<QPointF>> disjointPolygons;
    QString trace;
};

WAResult weilerAthertonClip(const QVector<QPointF> &vertices, double xmin, double ymin, double xmax, double ymax);


} // namespace ClipAlgorithms

#endif // CLIPPING_ALGORITHMS_H
