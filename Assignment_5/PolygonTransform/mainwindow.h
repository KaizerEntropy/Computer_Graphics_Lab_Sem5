#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>
#include "canvas.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void onTranslate();
    void onRotateOrigin();
    void onScale();
    void onShear();
    void onReflectX();
    void onReflectY();
    void onReflectArbitraryLine();
    void onRotateArbitraryPoint();
    void onClearCanvas();

private:
    Canvas *canvas;

    QDoubleSpinBox *txSpin;
    QDoubleSpinBox *tySpin;
    
    QDoubleSpinBox *angleSpin;
    
    QDoubleSpinBox *sxSpin;
    QDoubleSpinBox *sySpin;
    
    QDoubleSpinBox *shxSpin;
    QDoubleSpinBox *shySpin;

    QDoubleSpinBox *mSpin;
    QDoubleSpinBox *cSpin;

    QDoubleSpinBox *arbPxSpin;
    QDoubleSpinBox *arbPySpin;
    QDoubleSpinBox *arbAngleSpin;
};

#endif // MAINWINDOW_H
