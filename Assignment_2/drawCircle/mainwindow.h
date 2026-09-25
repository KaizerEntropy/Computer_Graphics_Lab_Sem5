#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QColor>
#include <QString>
#include <QTimer>
#include <array>
#include <vector>

namespace Ui {
class MainWindow;
}

// Records one circle that's already been drawn, so it can be replayed
// (redrawn) whenever the grid size changes or the canvas is refreshed.
struct DrawnCircle {
    int centerX, centerY;   // cartesian coords of the centre
    int radius;
    QColor color;
    QString algoName;       // which algorithm computed it (so redraw uses the same one)
};

class MainWindow : public QMainWindow
{
    QObject *parent = nullptr;
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showMousePosition(QPoint &pos);
    void Mouse_Pressed();
    void on_clear_clicked();
    void on_pushButton_2_clicked();
    void on_spinBox_valueChanged(int arg1);

    // One slot per circle-drawing algorithm
    void on_draw_polar_clicked();
    void on_draw_bresenham_circle_clicked();
    void on_draw_cartesian_clicked();

    // Fires on every animation tick to reveal one octant's 8 points
    void animateStep();

private:
    Ui::MainWindow *ui;
    int sc_x, sc_y;
    int org_x, org_y;
    QPoint lastPoint1;
    QPoint lastPoint2;   // last clicked point on the canvas -> used as the circle's centre

    // --- animation state ---
    QTimer *animTimer;
    std::vector<std::array<QPoint, 8>> animOctets;
    int animIndex;
    QColor animColor;

    // --- persisted drawing history, so circles survive a grid-size change ---
    std::vector<DrawnCircle> drawnCircles;
    void redrawCanvas();   // blanks the canvas, draws the grid, then replays drawnCircles

    void addPoint(int x, int y, int c);

    // Mirrors one computed octant point (x,y) into the full 8-point symmetric set
    std::array<QPoint, 8> eightWaySymmetry(int x, int y) const;

    // Each of these returns one octant's worth of points (already expanded to 8-way
    // symmetry), relative to the origin (0,0), plus how long the *math* took to run.
    std::vector<std::array<QPoint, 8>> computePolarCircle(int radius, qint64 &elapsedNanos) const;
    std::vector<std::array<QPoint, 8>> computeMidpointCircle(int radius, qint64 &elapsedNanos) const;
    std::vector<std::array<QPoint, 8>> computeCartesianCircle(int radius, qint64 &elapsedNanos) const;

    // Shared plumbing: shifts points to the chosen centre, logs/shows the timing,
    // then either draws everything at once or hands off to the animation timer.
    // radius is passed through purely so it can be recorded into drawnCircles.
    void runCircleAlgorithm(const std::vector<std::array<QPoint, 8>> &octets,
                            qint64 elapsedNanos,
                            const QColor &color,
                            const QString &algoName,
                            int radius);
};

#endif // MAINWINDOW_H