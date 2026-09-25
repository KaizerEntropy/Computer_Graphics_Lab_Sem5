#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QElapsedTimer>
#include <QMainWindow>
#include <QPoint>

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
    void on_draw_line_clicked();

    void on_pushButton_2_clicked();
    // void on_pushButton_3_clicked();

    void on_spinBox_valueChanged(int arg1);

    // void on_pushButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    // ... existing variables ...
    int gridSize = 10; // Default grid cell size

    void addPoint(int x, int y, int c = 1);

    QPoint lastPoint1;
    QPoint lastPoint2;
    int sc_x, sc_y;
    int org_x, org_y;
};

#endif // MAINWINDOW_H
