#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QMouseEvent>
#include <QPainter>
#include <QImage>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QColor>
#include <cmath>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    void setGridSize(int size);
    void setShowGrid(bool show);
    void setShowAxes(bool show);
    
    void setContinuousC1(bool c1);
    
    void addControlPoint(QPoint cartesianPoint);
    void removeLastControlPoint();
    void clearCanvas();
    void computeBezierCurve();
    
    const QVector<QPoint>& getControlPoints() const { return controlPoints; }

signals:
    void pointAdded(QPoint cartesianPoint);
    void mouseMoved(QPoint cartesianPoint);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    int gridSize;
    bool showGrid;
    bool showAxes;
    bool continuousC1;
    bool curveCalculated;
    
    QVector<QPoint> controlPoints;
    QVector<QPointF> curvePoints;
    
    QImage canvasImage;

    // Helper functions
    void drawGridAndAxes(QImage &img);
    void redraw();
    
    QPoint screenToCartesian(const QPoint &screenPos) const;
    QPoint cartesianToScreen(const QPointF &cartesianPos) const;
    
    QPointF evalCubicBezier(const QPointF &P0, const QPointF &P1,
                            const QPointF &P2, const QPointF &P3,
                            double t);
                            
    void drawBresenhamLine(QImage &img, int x0, int y0, int x1, int y1, const QColor &color);
    void fillRectPixels(QImage &img, int x, int y, int w, int h, const QColor &color);
};

#endif // CANVAS_H
