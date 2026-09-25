#include "algorithms.h"
#include <algorithm>
#include <stack>
#include <queue>
#include <cmath>
#include <map>

namespace CGAlgorithms {

// ----------------------------------------------------------------------------
// Bresenham Line Drawing (Watertight 4-Connected for Polygon Boundaries)
// ----------------------------------------------------------------------------
QVector<QPoint> bresenhamLine(int x1, int y1, int x2, int y2) {
    QVector<QPoint> points;
    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    int x = x1;
    int y = y1;

    while (true) {
        points.append(QPoint(x, y));
        if (x == x2 && y == y2) break;
        int e2 = 2 * err;
        int xOld = x;
        int yOld = y;
        if (e2 > -dy) {
            err -= dy;
            x += sx;
        }
        if (e2 < dx) {
            err += dx;
            y += sy;
        }
        if (x != xOld && y != yOld) {
            points.append(QPoint(xOld, y));
            points.append(QPoint(x, yOld));
        }
    }
    return points;
}

// ----------------------------------------------------------------------------
// Bresenham / Midpoint Circle Drawing (Watertight 8-Way Symmetry)
// ----------------------------------------------------------------------------
QVector<QPoint> bresenhamCircle(int xc, int yc, int r) {
    QVector<QPoint> points;
    if (r <= 0) {
        points.append(QPoint(xc, yc));
        return points;
    }

    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    auto add8Way = [&](int cx, int cy, int px, int py) {
        points.append(QPoint(cx + px, cy + py));
        points.append(QPoint(cx - px, cy + py));
        points.append(QPoint(cx + px, cy - py));
        points.append(QPoint(cx - px, cy - py));
        points.append(QPoint(cx + py, cy + px));
        points.append(QPoint(cx - py, cy + px));
        points.append(QPoint(cx + py, cy - px));
        points.append(QPoint(cx - py, cy - px));
    };

    add8Way(xc, yc, x, y);

    while (y >= x) {
        int xPrev = x;
        int yPrev = y;
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        add8Way(xc, yc, x, y);

        if (y != yPrev) {
            add8Way(xc, yc, xPrev, y);
            add8Way(xc, yc, x, yPrev);
        }
    }
    return points;
}

// ----------------------------------------------------------------------------
// Boundary Fill (8-Connected - BFS Queue)
// ----------------------------------------------------------------------------
FillResult boundaryFill8_BFS(const QHash<QPoint, QColor> &grid, const QPoint &seed,
                             const QColor &boundaryColor, const QColor &fillColor,
                             int minX, int maxX, int minY, int maxY)
{
    FillResult result;
    result.algorithmName = "Boundary Fill (8-Connected BFS)";

    auto startTime = std::chrono::high_resolution_clock::now();

    if (seed.x() < minX || seed.x() > maxX || seed.y() < minY || seed.y() > maxY) {
        return result;
    }

    QColor seedCol = grid.value(seed, QColor(0, 0, 0, 0));
    if (seedCol == boundaryColor || seedCol == fillColor) {
        return result;
    }

    QHash<QPoint, QColor> localGrid = grid;
    std::queue<QPoint> q;
    q.push(seed);
    localGrid[seed] = fillColor;

    size_t maxQueue = 1;
    const int dx[8] = {1, -1, 0, 0, 1, -1, 1, -1};
    const int dy[8] = {0, 0, 1, -1, 1, 1, -1, -1};

    int batchCounter = 0;
    FillStep currentStep;
    currentStep.description = QString("BFS 8-Way Seed initialized at (%1, %2)").arg(seed.x()).arg(seed.y());

    while (!q.empty()) {
        maxQueue = std::max(maxQueue, q.size());
        QPoint p = q.front();
        q.pop();

        result.filledPixels.append(p);
        currentStep.newPixels.append(p);
        batchCounter++;

        if (batchCounter >= 8) {
            currentStep.currentFrontierSize = static_cast<int>(q.size());
            result.steps.append(currentStep);
            currentStep = FillStep();
            currentStep.description = QString("BFS Expanding wavefront (Queue size: %1)").arg(q.size());
            batchCounter = 0;
        }

        for (int i = 0; i < 8; ++i) {
            int nx = p.x() + dx[i];
            int ny = p.y() + dy[i];
            if (nx >= minX && nx <= maxX && ny >= minY && ny <= maxY) {
                QPoint neighbor(nx, ny);
                QColor col = localGrid.value(neighbor, QColor(0, 0, 0, 0));
                if (col != boundaryColor && col != fillColor) {
                    localGrid[neighbor] = fillColor;
                    q.push(neighbor);
                }
            }
        }
    }

    if (!currentStep.newPixels.isEmpty()) {
        currentStep.currentFrontierSize = 0;
        result.steps.append(currentStep);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.elapsedNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
    result.pointCount = result.filledPixels.size();
    result.maxMemoryUnits = static_cast<int>(maxQueue);
    result.timeComplexity = "O(Area)";
    result.spaceComplexity = "O(Perimeter) ~ O(sqrt(N))";
    result.complexityExplanation = "8-Connected BFS wavefront expansion with bounded queue depth.";
    return result;
}

// ----------------------------------------------------------------------------
// Boundary Fill (8-Connected - DFS Stack)
// ----------------------------------------------------------------------------
FillResult boundaryFill8_DFS(const QHash<QPoint, QColor> &grid, const QPoint &seed,
                             const QColor &boundaryColor, const QColor &fillColor,
                             int minX, int maxX, int minY, int maxY)
{
    FillResult result;
    result.algorithmName = "Boundary Fill (8-Connected DFS)";

    auto startTime = std::chrono::high_resolution_clock::now();

    if (seed.x() < minX || seed.x() > maxX || seed.y() < minY || seed.y() > maxY) {
        return result;
    }

    QColor seedCol = grid.value(seed, QColor(0, 0, 0, 0));
    if (seedCol == boundaryColor || seedCol == fillColor) {
        return result;
    }

    QHash<QPoint, QColor> localGrid = grid;
    std::stack<QPoint> s;
    s.push(seed);
    localGrid[seed] = fillColor;

    size_t maxStack = 1;
    const int dx[8] = {1, -1, 0, 0, 1, -1, 1, -1};
    const int dy[8] = {0, 0, 1, -1, 1, 1, -1, -1};

    int batchCounter = 0;
    FillStep currentStep;
    currentStep.description = QString("DFS 8-Way Seed initialized at (%1, %2)").arg(seed.x()).arg(seed.y());

    while (!s.empty()) {
        maxStack = std::max(maxStack, s.size());
        QPoint p = s.top();
        s.pop();

        result.filledPixels.append(p);
        currentStep.newPixels.append(p);
        batchCounter++;

        if (batchCounter >= 8) {
            currentStep.currentFrontierSize = static_cast<int>(s.size());
            result.steps.append(currentStep);
            currentStep = FillStep();
            currentStep.description = QString("DFS Branch exploration (Stack depth: %1)").arg(s.size());
            batchCounter = 0;
        }

        for (int i = 0; i < 8; ++i) {
            int nx = p.x() + dx[i];
            int ny = p.y() + dy[i];
            if (nx >= minX && nx <= maxX && ny >= minY && ny <= maxY) {
                QPoint neighbor(nx, ny);
                QColor col = localGrid.value(neighbor, QColor(0, 0, 0, 0));
                if (col != boundaryColor && col != fillColor) {
                    localGrid[neighbor] = fillColor;
                    s.push(neighbor);
                }
            }
        }
    }

    if (!currentStep.newPixels.isEmpty()) {
        currentStep.currentFrontierSize = 0;
        result.steps.append(currentStep);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.elapsedNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
    result.pointCount = result.filledPixels.size();
    result.maxMemoryUnits = static_cast<int>(maxStack);
    result.timeComplexity = "O(Area)";
    result.spaceComplexity = "O(Area) ~ O(N)";
    result.complexityExplanation = "8-Connected DFS deep exploration using recursion stack.";
    return result;
}

// ----------------------------------------------------------------------------
// Scanline Polygon Fill (Edge Table & Active Edge Table)
// ----------------------------------------------------------------------------
FillResult scanlineFill(const QVector<QPoint> &vertices) {
    FillResult result;
    result.algorithmName = "Scanline Polygon Fill (AET)";

    int n = vertices.size();
    if (n < 3) return result;

    auto startTime = std::chrono::high_resolution_clock::now();

    int yMinGlobal = vertices[0].y();
    int yMaxGlobal = vertices[0].y();
    for (int i = 1; i < n; ++i) {
        yMinGlobal = std::min(yMinGlobal, vertices[i].y());
        yMaxGlobal = std::max(yMaxGlobal, vertices[i].y());
    }

    std::map<int, QVector<ScanEdge>> edgeTable;

    for (int i = 0; i < n; ++i) {
        QPoint p1 = vertices[i];
        QPoint p2 = vertices[(i + 1) % n];

        if (p1.y() == p2.y()) continue; // Ignore horizontal edges

        ScanEdge edge;
        edge.edgeId = i;
        edge.v1Index = i;
        edge.v2Index = (i + 1) % n;

        int yMinEdge, yMaxEdge;
        double xAtYMin;

        if (p1.y() < p2.y()) {
            yMinEdge = p1.y();
            yMaxEdge = p2.y();
            xAtYMin = p1.x();
        } else {
            yMinEdge = p2.y();
            yMaxEdge = p1.y();
            xAtYMin = p2.x();
        }

        edge.yMax = yMaxEdge;
        edge.xCurrent = xAtYMin;
        edge.invSlope = static_cast<double>(p2.x() - p1.x()) / static_cast<double>(p2.y() - p1.y());

        edgeTable[yMinEdge].append(edge);
    }

    QVector<ScanEdge> activeEdgeTable;
    size_t maxAET = 0;

    for (int y = yMinGlobal; y <= yMaxGlobal; ++y) {
        // 1. Add edges from Edge Table starting at y
        if (edgeTable.find(y) != edgeTable.end()) {
            for (const auto &edge : edgeTable[y]) {
                activeEdgeTable.append(edge);
            }
        }

        // 2. Remove edges where y >= yMax
        for (int i = activeEdgeTable.size() - 1; i >= 0; --i) {
            if (y >= activeEdgeTable[i].yMax) {
                activeEdgeTable.removeAt(i);
            }
        }

        if (activeEdgeTable.isEmpty()) continue;

        // 3. Sort AET by xCurrent
        std::sort(activeEdgeTable.begin(), activeEdgeTable.end());
        maxAET = std::max(maxAET, static_cast<size_t>(activeEdgeTable.size()));

        FillStep step;
        step.scanlineY = y;
        step.activeEdges = activeEdgeTable;

        QString aetStr = QString("Scanline y=%1 | AET Edges (%2):\n").arg(y).arg(activeEdgeTable.size());
        for (int i = 0; i < activeEdgeTable.size(); ++i) {
            const auto &ae = activeEdgeTable[i];
            aetStr += QString("  • Edge E%1 [V%2→V%3]: x=%4, 1/m=%5, yMax=%6\n")
                          .arg(ae.edgeId).arg(ae.v1Index).arg(ae.v2Index)
                          .arg(ae.xCurrent, 0, 'f', 2).arg(ae.invSlope, 0, 'f', 3).arg(ae.yMax);
        }
        step.aetDetails = aetStr;

        // 4. Fill spans between pairs of edges
        for (int i = 0; i + 1 < activeEdgeTable.size(); i += 2) {
            int xStart = static_cast<int>(std::round(activeEdgeTable[i].xCurrent));
            int xEnd = static_cast<int>(std::round(activeEdgeTable[i + 1].xCurrent));

            for (int x = xStart; x <= xEnd; ++x) {
                QPoint pt(x, y);
                result.filledPixels.append(pt);
                step.newPixels.append(pt);
            }
        }

        step.description = QString("Scanline y=%1: filled %2 pixels across %3 AET edge pairs")
                               .arg(y).arg(step.newPixels.size()).arg(activeEdgeTable.size() / 2);
        step.currentFrontierSize = activeEdgeTable.size();
        result.steps.append(step);

        // 5. Update xCurrent for next scanline
        for (auto &edge : activeEdgeTable) {
            edge.xCurrent += edge.invSlope;
        }
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    result.elapsedNanos = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
    result.pointCount = result.filledPixels.size();
    result.maxMemoryUnits = static_cast<int>(maxAET);
    result.timeComplexity = "O(Height · log(ActiveEdges) + N)";
    result.spaceComplexity = "O(ActiveEdges) (AET size)";
    result.complexityExplanation = "Horizontal row span filling with Active Edge Table.";
    return result;
}

// ----------------------------------------------------------------------------
// Shape Presets
// ----------------------------------------------------------------------------
QVector<QPoint> getTrianglePreset() {
    return {
        QPoint(0, 15),
        QPoint(15, -12),
        QPoint(-15, -12)
    };
}

QVector<QPoint> getConvexPolygonPreset() {
    return {
        QPoint(0, 16),
        QPoint(16, 4),
        QPoint(10, -16),
        QPoint(-10, -16),
        QPoint(-16, 4)
    };
}

QVector<QPoint> getConcaveArrowPreset() {
    return {
        QPoint(0, 18),
        QPoint(14, 0),
        QPoint(5, 0),
        QPoint(5, -18),
        QPoint(-5, -18),
        QPoint(-5, 0),
        QPoint(-14, 0)
    };
}

QVector<QPoint> getStarPolygonPreset() {
    return {
        QPoint(0, 18),
        QPoint(5, 6),
        QPoint(18, 5),
        QPoint(8, -3),
        QPoint(11, -16),
        QPoint(0, -9),
        QPoint(-11, -16),
        QPoint(-8, -3),
        QPoint(-18, 5),
        QPoint(-5, 6)
    };
}

QVector<QPoint> getHousePreset() {
    return {
        QPoint(0, 16),      // Roof peak
        QPoint(14, 4),      // Right eaves
        QPoint(14, -14),    // Bottom-right
        QPoint(-14, -14),   // Bottom-left
        QPoint(-14, 4)      // Left eaves
    };
}

QVector<QPoint> getHexagonPreset() {
    return {
        QPoint(-6, 12),
        QPoint(6, 12),
        QPoint(14, 0),
        QPoint(6, -12),
        QPoint(-6, -12),
        QPoint(-14, 0)
    };
}

QVector<QPoint> getNestedOuterPreset() {
    return {
        QPoint(-18, -18),
        QPoint(18, -18),
        QPoint(18, 18),
        QPoint(-18, 18)
    };
}

QVector<QPoint> getNestedInnerPreset() {
    return {
        QPoint(-8, -8),
        QPoint(8, -8),
        QPoint(8, 8),
        QPoint(-8, 8)
    };
}

void getMultiShapeScenePreset(QVector<QPoint> &poly1, QVector<QPoint> &poly2, QPoint &circleCenter, int &circleRadius) {
    poly1 = {
        QPoint(-20, 15),
        QPoint(-8, -5),
        QPoint(-30, -5)
    };
    poly2 = {
        QPoint(6, -15),
        QPoint(24, -15),
        QPoint(24, 2),
        QPoint(6, 2)
    };
    circleCenter = QPoint(15, 16);
    circleRadius = 8;
}

} // namespace CGAlgorithms
