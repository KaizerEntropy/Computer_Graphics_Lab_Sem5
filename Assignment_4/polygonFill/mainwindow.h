#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QColor>
#include <QString>
#include <QTimer>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    QObject *parent = nullptr;
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showMousePosition(QPoint &pos);
    void Mouse_Left();
    void Mouse_Right();
    void on_clear_clicked();
    void on_pushButton_2_clicked();
    void on_spinBox_valueChanged(int arg1);

    void on_set_vertex_clicked();
    void on_close_polygon_clicked();
    void on_set_seed_clicked();
    void on_boundary_fill_clicked();
    void on_flood_fill_clicked();
    void on_scanline_fill_clicked();

private:
    Ui::MainWindow *ui;
    int sc_x, sc_y;
    int org_x, org_y;
    QPoint lastPoint2;   // seed point
    QPoint selectedPoint; // currently clicked point

    enum class DrawMode {
        None,
        Vertex,
        Seed
    };
    DrawMode currentMode = DrawMode::None;

    std::vector<QPoint> polygonVertices;

    void redrawCanvas();
    void drawDDA(QPoint p1, QPoint p2, QColor color);
    
    void boundaryFill(int x, int y, QColor fillColor, QColor boundaryColor, bool is8Way);
    void floodFill(int x, int y, QColor fillColor, QColor oldColor, bool is8Way);
    void scanlineFill();

    QColor getSelectedColor();
};

#endif // MAINWINDOW_H