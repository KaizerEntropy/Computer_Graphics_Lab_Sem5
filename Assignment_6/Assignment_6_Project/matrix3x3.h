#ifndef MATRIX3X3_H
#define MATRIX3X3_H

#include <QPoint>
#include <QPointF>
#include <QVector>
#include <QString>
#include <cmath>

class Matrix3x3
{
public:
    double m[3][3];

    // Constructors
    Matrix3x3();
    Matrix3x3(double m00, double m01, double m02,
              double m10, double m11, double m12,
              double m20, double m21, double m22);

    // Static Factory Methods for Fundamental 2D Transformations
    static Matrix3x3 identity();
    static Matrix3x3 translation(double tx, double ty);
    static Matrix3x3 rotation(double angleDegrees);
    static Matrix3x3 rotationAboutPoint(double angleDegrees, double px, double py);
    static Matrix3x3 scaling(double sx, double sy);
    static Matrix3x3 scalingAboutPoint(double sx, double sy, double px, double py);
    static Matrix3x3 shearX(double shx, double yref = 0.0);
    static Matrix3x3 shearY(double shy, double xref = 0.0);
    static Matrix3x3 shear(double shx, double shy);

    // Reflection Matrices
    static Matrix3x3 reflectionX();               // Reflection wrt X-axis (y=0)
    static Matrix3x3 reflectionY();               // Reflection wrt Y-axis (x=0)
    static Matrix3x3 reflectionOrigin();          // Reflection wrt Origin (0,0)
    static Matrix3x3 reflectionLineYeqX();        // Reflection wrt Line y = x
    static Matrix3x3 reflectionLineYeqNegX();     // Reflection wrt Line y = -x
    static Matrix3x3 reflectionArbitraryLine(double mSlope, double cIntercept);
    static Matrix3x3 reflectionTwoPoints(const QPointF &p1, const QPointF &p2);

    // Matrix Arithmetic & Multiplications
    Matrix3x3 operator*(const Matrix3x3 &other) const;
    Matrix3x3& operator*=(const Matrix3x3 &other);

    // Vector Point Transformations
    QPoint transformPoint(const QPoint &pt) const;
    QPointF transformPointF(const QPointF &pt) const;
    QVector<QPoint> transformPoints(const QVector<QPoint> &pts) const;

    // Mathematical Properties
    double determinant() const;
    Matrix3x3 inverse() const;
    bool isIdentity(double eps = 1e-6) const;

    // Formatting & Educational Inspection
    QString toString() const;
    QString toFormattedString(int precision = 3) const;
    QString toHtmlTable() const;

    struct TransformStep;
};

struct Matrix3x3::TransformStep {
    QString name;
    Matrix3x3 matrix;
    QString formula;
    QString details;
};

#endif // MATRIX3X3_H
