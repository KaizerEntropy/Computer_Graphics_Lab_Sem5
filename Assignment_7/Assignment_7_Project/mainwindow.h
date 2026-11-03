#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include "canvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGridSizeChanged(int value);
    void onToggleGrid(bool checked);
    void onToggleAxes(bool checked);
    void onClearCanvas();
    void onUndoPoint();
    void onDrawBezier();
    void onCanvasPointAdded(QPoint point);
    void onCanvasMouseMoved(QPoint point);
    void updatePointList();
    
private:
    void setupUI();

    Canvas *canvas;
    
    QSpinBox *gridSizeSpinBox;
    QCheckBox *showGridCheckBox;
    QCheckBox *showAxesCheckBox;
    
    QRadioButton *c0RadioBtn;
    QRadioButton *c1RadioBtn;
    
    QPushButton *drawBtn;
    QPushButton *clearBtn;
    QPushButton *undoBtn;
    
    QListWidget *pointListWidget;
    QLabel *statusLabel;
    QLabel *mousePositionLabel;
};

#endif // MAINWINDOW_H
