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

// Records one ellipse that's already been drawn, so it can be replayed
// (redrawn) whenever the grid size changes or the canvas is refreshed.
struct DrawnEllipse {
    int centerX, centerY;   // cartesian coords of the centre
    int rx, ry;             // semi-major and semi-minor axes
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

    // One slot per ellipse-drawing algorithm
    void on_draw_polar_clicked();
    void on_draw_midpoint_ellipse_clicked();
    void on_draw_cartesian_clicked();

    // Fires on every animation tick to reveal one quadrant's 4 points
    void animateStep();

private:
    Ui::MainWindow *ui;
    int sc_x, sc_y;
    int org_x, org_y;
    QPoint lastPoint1;
    QPoint lastPoint2;   // last clicked point on the canvas -> used as the ellipse's centre

    // --- animation state ---
    QTimer *animTimer;
    std::vector<std::array<QPoint, 4>> animQuadrants;
    int animIndex;
    QColor animColor;

    // --- persisted drawing history, so ellipses survive a grid-size change ---
    std::vector<DrawnEllipse> drawnEllipses;
    void redrawCanvas();   // blanks the canvas, draws the grid, then replays drawnEllipses

    void addPoint(int x, int y, int c);

    // Mirrors one computed point (x,y) into the full 4-point symmetric set
    std::array<QPoint, 4> fourWaySymmetry(int x, int y) const;

    // Each of these returns points (already expanded to 4-way
    // symmetry), relative to the origin (0,0), plus how long the *math* took to run.
    std::vector<std::array<QPoint, 4>> computePolarEllipse(int rx, int ry, qint64 &elapsedNanos) const;
    std::vector<std::array<QPoint, 4>> computeMidpointEllipse(int rx, int ry, qint64 &elapsedNanos) const;
    std::vector<std::array<QPoint, 4>> computeCartesianEllipse(int rx, int ry, qint64 &elapsedNanos) const;

    // Shared plumbing: shifts points to the chosen centre, logs/shows the timing,
    // then either draws everything at once or hands off to the animation timer.
    // rx and ry are passed through purely so it can be recorded into drawnEllipses.
    void runEllipseAlgorithm(const std::vector<std::array<QPoint, 4>> &quadrants,
                             qint64 elapsedNanos,
                             const QColor &color,
                             const QString &algoName,
                             int rx, int ry);
};

#endif // MAINWINDOW_H