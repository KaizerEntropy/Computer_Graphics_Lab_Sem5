#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QPointF>
#include "transform_matrix.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    void clearCanvas();
    void applyTransformation(const TransformMatrix &matrix);
    
    // Setters for showing specific geometric elements for specific transformations
    void setShowArbitraryLine(bool show, double m = 1.0, double c = 0.0);
    void setShowArbitraryPoint(bool show, double px = 0.0, double py = 0.0);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QVector<QPointF> polygon;
    QVector<QPointF> transformedPolygon;
    
    int gridSize;
    QPointF mousePos;

    bool m_showArbLine;
    double m_arbLineM, m_arbLineC;

    bool m_showArbPoint;
    double m_arbPointX, m_arbPointY;

    // Convert Logical Coordinates to Screen Coordinates
    QPointF toScreen(const QPointF &logical) const;
    // Convert Screen Coordinates to Logical Coordinates
    QPointF toLogical(const QPointF &screen) const;
};

#endif // CANVAS_H
