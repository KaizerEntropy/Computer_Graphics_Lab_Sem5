#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLCDNumber>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QElapsedTimer>
#include <QVector>
#include <QPoint>
#include <QColor>
#include <QMap>
#include <QString>
#include <QApplication>

// ---------------------------------------------------------------------------
// GridWidget: the raster canvas. Every grid cell (X,Y) is treated as exactly
// one "pixel" of the raster grid, regardless of how many screen pixels it
// occupies (gridOffset). (0,0) is at the centre of the widget so that lines
// can be drawn into all four quadrants (+X/+Y, -X/+Y, -X/-Y, +X/-Y).
// ---------------------------------------------------------------------------
class GridWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GridWidget(QWidget *parent = nullptr);

    int gridOffset = 20;     // screen pixels per raster "pixel" (cell size)
    QPixmap canvas;          // persistent drawing surface

    void resetCanvas();
    // draws the faint reference grid lines, then colours every single whole
    // raster pixel along X=0 and Y=0 red -- the axis is "pixelated" just
    // like every other line drawn on the canvas, not a thin painter line.
    void drawGridAndAxes();
    // grid (raster) coordinates -> top-left screen coordinates of that cell
    QPoint gridToScreen(const QPoint &gridPt) const;
    // screen coordinates -> grid (raster) coordinates
    QPoint screenToGrid(const QPoint &screenPt) const;
    // fill exactly one raster "pixel" (grid cell) with a colour
    void colorPoint(const QPoint &gridPt, const QColor &color);

signals:
    void gridPointClicked(QPoint gridPt);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
};

// ---------------------------------------------------------------------------
// MainWindow
// ---------------------------------------------------------------------------
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Algorithms: both operate purely in raster/grid space (integer points).
    QVector<QPoint> generate_dda_linePts(QPoint startPt, QPoint endPt);
    QVector<QPoint> generate_bresenham_linePts(QPoint startPt, QPoint endPt);

private slots:
    void on_DDADraw_clicked();
    void on_BresenhamLineDraw_clicked();
    void on_BothDraw_clicked();
    void on_ClearGrid_clicked();
    void on_gridPointClicked(QPoint gridPt);
    void on_SetPoints_clicked();

private:
    // ---- nested timer helper, mirrors the original myTimer class ----
    class myTimer {
        QString algoName;
        QElapsedTimer timer;
        MainWindow *mainWindow;
    public:
        myTimer(QString algoName, MainWindow *mainWindow);
        void measureTime();   // reads elapsed time, writes it into the UI + mainWindow->lastElapsedMs
    };

    // ---- UI (built programmatically, no .ui file needed) ----
    GridWidget   *workArea;
    QLabel       *timeMessage;
    QLCDNumber   *timeTaken;
    QLabel       *pointsLabel;
    QLabel       *comparisonLabel;   // shows "DDA: x ms | Bresenham: y ms" for the Both mode
    QPushButton  *ddaButton;
    QPushButton  *bresenhamButton;
    QPushButton  *bothButton;
    QPushButton  *clearButton;

    // ---- manual coordinate entry (X1,Y1) -> (X2,Y2) ----
    QSpinBox     *x1Spin;
    QSpinBox     *y1Spin;
    QSpinBox     *x2Spin;
    QSpinBox     *y2Spin;
    QPushButton  *setPointsButton;

    void setupUi();
    void setupColorPalette();

    // ---- state ----
    QVector<QPoint> points;   // at most the 2 points of the *current* pair being selected
    QPoint start, end;        // the locked-in pair, once 2 points have been clicked
    bool pairReady = false;   // true once start/end are valid and drawable
    QVector<QPoint> line;     // last-generated line's raster points
    double lastElapsedMs = 0.0;
    QMap<QString, QColor> colorPalette;

    void color_pts(const QVector<QPoint> &linePts, const QColor &color);
    void delay(int ms);       // small animation delay that still pumps the event loop
    void updatePointsLabel();
};
#endif // MAINWINDOW_H
