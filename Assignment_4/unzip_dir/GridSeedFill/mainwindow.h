#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QColor>
#include <QHash>
#include <QTimer>
#include <QString>
#include <array>
#include <vector>

namespace Ui {
class MainWindow;
}

// This Qt build doesn't provide a built-in qHash(QPoint), so QHash<QPoint,...>
// / QSet<QPoint> need one supplied before they're first used.
inline uint qHash(const QPoint &p, uint seed = 0)
{
    quint64 packed = (static_cast<quint64>(static_cast<quint32>(p.x())) << 32) |
                      static_cast<quint32>(p.y());
    return qHash(packed, seed);
}

// Grid canvas + shape drawing + seed-fill algorithms.
//
// Workflow:
//   Lines:
//     1. Click a first grid cell, click a second grid cell, then press
//        "Draw Line" - a Bresenham line is drawn between them in the
//        boundary colour. Repeat (2 clicks + Draw Line) for each edge until
//        the shape is closed - e.g. 4 lines for a quadrilateral.
//   Circles / Ellipses:
//     1. Click a grid cell to pick a centre.
//     2. Set radius (circle) or rx/ry (ellipse).
//     3. Press either the Polar or the Bresenham (Midpoint) button - the
//        outline is stamped into the same cellColors map, in the boundary
//        colour, exactly like a line - so it's fillable the same way.
//   Fill (either kind of shape):
//     1. Pick a fill colour (e.g. light blue) and an algorithm.
//     2. Tick "Fill mode" and click any cell inside the closed shape - that
//        becomes the seed and the chosen algorithm fills the region.
//
// The shape is stored as a sparse map of *cartesian* grid-cell coordinates
// to colour (cellColors): changing the grid-size spinner rescales and
// redraws the exact same shape instead of losing it.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showMousePosition(QPoint &pos);
    void Mouse_Pressed();

    void on_draw_line_clicked();
    void on_clear_clicked();
    void on_pushButton_2_clicked();
    void on_spinBox_valueChanged(int arg1);

    void on_boundaryColorButton_clicked();
    void on_fillColorButton_clicked();

    void on_drawCirclePolar_clicked();
    void on_drawCircleBresenham_clicked();
    void on_drawEllipsePolar_clicked();
    void on_drawEllipseBresenham_clicked();

    void animateStep();

private:
    Ui::MainWindow *ui;

    int sc_x, sc_y;       // last raw mouse-move screen position
    int org_x, org_y;     // screen position of the most recent click

    QPoint lastPoint1;    // screen coords of the point picked before the most recent click
    QPoint lastPoint2;    // screen coords of the most recently picked point (also used as centre)

    QHash<QPoint, QColor> cellColors;   // cartesian grid coord -> colour (missing = black/empty)

    QColor boundaryColor = Qt::white;
    QColor fillColor     = QColor(173, 216, 230);   // light blue

    void redrawCanvas();
    static int screenToCartesian(int screenCoord, int originCoord, int gridSize);

    std::vector<QPoint> bresenhamLine(int x0, int y0, int x1, int y1) const;

    // Cartesian coords of the last clicked point, used as centre for circle/ellipse.
    bool getPickedCenter(int &cx, int &cy) const;

    // --- Circle (ported from Assignment 2) ---
    std::array<QPoint, 8> eightWaySymmetry(int x, int y) const;
    std::vector<QPoint> computePolarCircle(int radius, qint64 &elapsedNanos) const;
    std::vector<QPoint> computeMidpointCircle(int radius, qint64 &elapsedNanos) const;

    // --- Ellipse (ported from Assignment 3) ---
    std::array<QPoint, 4> fourWaySymmetry(int x, int y) const;
    std::vector<QPoint> computePolarEllipse(int rx, int ry, qint64 &elapsedNanos) const;
    std::vector<QPoint> computeMidpointEllipse(int rx, int ry, qint64 &elapsedNanos) const;

    // Stamps an outline (relative to centre cx,cy) into cellColors in the
    // boundary colour, reports timing, and redraws.
    void stampOutline(const std::vector<QPoint> &points, int cx, int cy,
                       const QString &shapeName, const QString &algoName,
                       qint64 elapsedNanos);

    void runFill(QPoint seedCartesian);

    // Both mutate cellColors directly, return the fill order (for animation),
    // and report timing via elapsedNanos. Search is bounded to the region
    // currently visible on the canvas so it can't run away indefinitely.
    std::vector<QPoint> boundaryFill(int x, int y, const QColor &fillC, const QColor &boundaryC,
                                      bool eightConnected,
                                      int minX, int maxX, int minY, int maxY,
                                      qint64 &elapsedNanos);

    std::vector<QPoint> floodFill(int x, int y, const QColor &fillC,
                                   bool eightConnected,
                                   int minX, int maxX, int minY, int maxY,
                                   qint64 &elapsedNanos);

    // Scanline fill: no 4/8-connectivity concept here (that's specific to
    // seed-fill neighbour exploration) - instead, for every row it finds the
    // boundary-colour crossings left-to-right and fills between pairs of
    // crossings (even-odd rule), same as the classic polygon scanline
    // algorithm applied to the rasterised edges already on the grid.
    std::vector<QPoint> scanlineFill(const QColor &fillC, const QColor &boundaryC,
                                      int minX, int maxX, int minY, int maxY,
                                      qint64 &elapsedNanos);

    // --- animation playback state ---
    QTimer *animTimer;
    std::vector<QPoint> animOrder;
    int animIndex = 0;
};

#endif // MAINWINDOW_H
