#ifndef TRANSFORM_MATRIX_H
#define TRANSFORM_MATRIX_H

#include <QPointF>
#include <QVector>
#include <cmath>

class TransformMatrix {
public:
    double m[3][3];

    TransformMatrix();
    TransformMatrix(double m00, double m01, double m02,
                    double m10, double m11, double m12,
                    double m20, double m21, double m22);

    static TransformMatrix identity();
    static TransformMatrix translation(double tx, double ty);
    static TransformMatrix rotation(double angleDegrees);
    static TransformMatrix scaling(double sx, double sy);
    static TransformMatrix shearX(double shx);
    static TransformMatrix shearY(double shy);

    static TransformMatrix reflectionX();
    static TransformMatrix reflectionY();
    static TransformMatrix reflectionLine(double slope, double intercept);
    static TransformMatrix rotationAboutPoint(double angleDegrees, double px, double py);

    TransformMatrix operator*(const TransformMatrix &other) const;
    TransformMatrix& operator*=(const TransformMatrix &other);

    QPointF transform(const QPointF &pt) const;
    QVector<QPointF> transform(const QVector<QPointF> &pts) const;
};

#endif // TRANSFORM_MATRIX_H
