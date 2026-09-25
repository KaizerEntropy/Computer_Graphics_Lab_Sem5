#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QVector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void Mouse_Pressed();
    void showMousePosition(QPoint &pos);
    void on_clear_clicked();
    void on_draw_dda_clicked();
    void on_draw_bresenham_clicked();

private:
    Ui::MainWindow *ui;

    // Marks a single pixel on the canvas (used for the two clicked endpoints).
    void addPoint(int x, int y, int c = 1);

    // Pixel-generation algorithms: each returns the sequence of raster
    // pixels that approximate the straight line from p1 to p2. Both are
    // written generally (using signed step directions sx/sy and the
    // dx/dy magnitude comparison) so they work correctly for a line
    // endpoint lying in ANY of the 4 quadrants relative to the start
    // point, including horizontal, vertical and 45-degree lines.
    QVector<QPoint> computeDDA(const QPoint &p1, const QPoint &p2) const;
    QVector<QPoint> computeBresenham(const QPoint &p1, const QPoint &p2) const;

    // Efficiently paints a whole batch of pixels in a single
    // QImage/QPixmap round-trip (much faster than calling addPoint()
    // once per pixel, and keeps rendering cost out of the timed region).
    void renderPoints(const QVector<QPoint> &points, int c);

    QPoint lastPoint1;
    QPoint lastPoint2;
    int sc_x, sc_y;
    int org_x, org_y;
};

#endif // MAINWINDOW_H
