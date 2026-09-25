#include "matrix3x3.h"
#include <iomanip>
#include <sstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Matrix3x3::Matrix3x3() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

Matrix3x3::Matrix3x3(double m00, double m01, double m02,
                     double m10, double m11, double m12,
                     double m20, double m21, double m22) {
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
}

Matrix3x3 Matrix3x3::identity() {
    return Matrix3x3();
}

Matrix3x3 Matrix3x3::translation(double tx, double ty) {
    return Matrix3x3(1.0, 0.0, tx,
                     0.0, 1.0, ty,
                     0.0, 0.0, 1.0);
}

Matrix3x3 Matrix3x3::rotation(double angleDegrees) {
    double rad = angleDegrees * M_PI / 180.0;
    double c = std::cos(rad);
    double s = std::sin(rad);

    // Clean up small floating precision inaccuracies for 90, 180, 270 degrees
    if (std::abs(c) < 1e-12) c = 0.0;
    if (std::abs(s) < 1e-12) s = 0.0;

    return Matrix3x3(c,   -s,  0.0,
                     s,    c,  0.0,
                     0.0, 0.0, 1.0);
}

Matrix3x3 Matrix3x3::rotationAboutPoint(double angleDegrees, double px, double py) {
    // Composite: T(px, py) * R(theta) * T(-px, -py)
    Matrix3x3 t1 = translation(px, py);
    Matrix3x3 r  = rotation(angleDegrees);
    Matrix3x3 t2 = translation(-px, -py);
    return t1 * r * t2;
}

Matrix3x3 Matrix3x3::scaling(double sx, double sy) {
    return Matrix3x3(sx,  0.0, 0.0,
                     0.0, sy,  0.0,
                     0.0, 0.0, 1.0);
}

Matrix3x3 Matrix3x3::scalingAboutPoint(double sx, double sy, double px, double py) {
    // Composite: T(px, py) * S(sx, sy) * T(-px, -py)
    Matrix3x3 t1 = translation(px, py);
    Matrix3x3 s  = scaling(sx, sy);
    Matrix3x3 t2 = translation(-px, -py);
    return t1 * s * t2;
}

Matrix3x3 Matrix3x3::shearX(double shx, double yref) {
    if (std::abs(yref) < 1e-9) {
        return Matrix3x3(1.0, shx, 0.0,
                         0.0, 1.0, 0.0,
                         0.0, 0.0, 1.0);
    }
    // T(0, yref) * Shx * T(0, -yref)
    Matrix3x3 t1 = translation(0.0, yref);
    Matrix3x3 sh = Matrix3x3(1.0, shx, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
    Matrix3x3 t2 = translation(0.0, -yref);
    return t1 * sh * t2;
}

Matrix3x3 Matrix3x3::shearY(double shy, double xref) {
    if (std::abs(xref) < 1e-9) {
        return Matrix3x3(1.0, 0.0, 0.0,
                         shy, 1.0, 0.0,
                         0.0, 0.0, 1.0);
    }
    // T(xref, 0) * Shy * T(-xref, 0)
    Matrix3x3 t1 = translation(xref, 0.0);
    Matrix3x3 sh = Matrix3x3(1.0, 0.0, 0.0, shy, 1.0, 0.0, 0.0, 0.0, 1.0);
    Matrix3x3 t2 = translation(-xref, 0.0);
    return t1 * sh * t2;
}

Matrix3x3 Matrix3x3::shear(double shx, double shy) {
    return Matrix3x3(1.0, shx, 0.0,
                     shy, 1.0, 0.0,
                     0.0, 0.0, 1.0);
}

Matrix3x3 Matrix3x3::reflectionX() {
    return Matrix3x3(1.0,  0.0, 0.0,
                     0.0, -1.0, 0.0,
                     0.0,  0.0, 1.0);
}

Matrix3x3 Matrix3x3::reflectionY() {
    return Matrix3x3(-1.0, 0.0, 0.0,
                      0.0, 1.0, 0.0,
                      0.0, 0.0, 1.0);
}

Matrix3x3 Matrix3x3::reflectionOrigin() {
    return Matrix3x3(-1.0,  0.0, 0.0,
                      0.0, -1.0, 0.0,
                      0.0,  0.0, 1.0);
}

Matrix3x3 Matrix3x3::reflectionLineYeqX() {
    return Matrix3x3(0.0, 1.0, 0.0,
                     1.0, 0.0, 0.0,
                     0.0, 0.0, 1.0);
}

Matrix3x3 Matrix3x3::reflectionLineYeqNegX() {
    return Matrix3x3( 0.0, -1.0, 0.0,
                     -1.0,  0.0, 0.0,
                      0.0,  0.0, 1.0);
}

Matrix3x3 Matrix3x3::reflectionArbitraryLine(double mSlope, double cIntercept) {
    // Calculate angle of line: theta = atan(m)
    double thetaRad = std::atan(mSlope);
    double thetaDeg = thetaRad * 180.0 / M_PI;

    // Composite: T(0, c) * R(theta) * RefX * R(-theta) * T(0, -c)
    Matrix3x3 t1 = translation(0.0, cIntercept);
    Matrix3x3 r1 = rotation(thetaDeg);
    Matrix3x3 rx = reflectionX();
    Matrix3x3 r2 = rotation(-thetaDeg);
    Matrix3x3 t2 = translation(0.0, -cIntercept);

    return t1 * r1 * rx * r2 * t2;
}

Matrix3x3 Matrix3x3::reflectionTwoPoints(const QPointF &p1, const QPointF &p2) {
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double thetaRad = std::atan2(dy, dx);
    double thetaDeg = thetaRad * 180.0 / M_PI;

    Matrix3x3 t1 = translation(p1.x(), p1.y());
    Matrix3x3 r1 = rotation(thetaDeg);
    Matrix3x3 rx = reflectionX();
    Matrix3x3 r2 = rotation(-thetaDeg);
    Matrix3x3 t2 = translation(-p1.x(), -p1.y());

    return t1 * r1 * rx * r2 * t2;
}

Matrix3x3 Matrix3x3::operator*(const Matrix3x3 &other) const {
    Matrix3x3 res;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double sum = 0.0;
            for (int k = 0; k < 3; ++k) {
                sum += m[i][k] * other.m[k][j];
            }
            if (std::abs(sum) < 1e-12) sum = 0.0;
            res.m[i][j] = sum;
        }
    }
    return res;
}

Matrix3x3& Matrix3x3::operator*=(const Matrix3x3 &other) {
    *this = (*this) * other;
    return *this;
}

QPointF Matrix3x3::transformPointF(const QPointF &pt) const {
    double x = m[0][0] * pt.x() + m[0][1] * pt.y() + m[0][2];
    double y = m[1][0] * pt.x() + m[1][1] * pt.y() + m[1][2];
    double w = m[2][0] * pt.x() + m[2][1] * pt.y() + m[2][2];

    if (std::abs(w - 1.0) > 1e-9 && std::abs(w) > 1e-9) {
        x /= w;
        y /= w;
    }
    return QPointF(x, y);
}

QPoint Matrix3x3::transformPoint(const QPoint &pt) const {
    QPointF pf = transformPointF(QPointF(pt.x(), pt.y()));
    return QPoint(static_cast<int>(std::round(pf.x())),
                  static_cast<int>(std::round(pf.y())));
}

QVector<QPoint> Matrix3x3::transformPoints(const QVector<QPoint> &pts) const {
    QVector<QPoint> res;
    res.reserve(pts.size());
    for (const auto &p : pts) {
        res.append(transformPoint(p));
    }
    return res;
}

double Matrix3x3::determinant() const {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
           m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

Matrix3x3 Matrix3x3::inverse() const {
    double det = determinant();
    if (std::abs(det) < 1e-12) {
        return identity(); // Non-invertible fallback
    }

    double invDet = 1.0 / det;
    Matrix3x3 inv;

    inv.m[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    inv.m[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
    inv.m[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;

    inv.m[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
    inv.m[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    inv.m[1][2] = (m[0][2] * m[1][0] - m[0][0] * m[1][2]) * invDet;

    inv.m[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
    inv.m[2][1] = (m[0][1] * m[2][0] - m[0][0] * m[2][1]) * invDet;
    inv.m[2][2] = (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

    return inv;
}

bool Matrix3x3::isIdentity(double eps) const {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            double expected = (i == j) ? 1.0 : 0.0;
            if (std::abs(m[i][j] - expected) > eps) return false;
        }
    }
    return true;
}

QString Matrix3x3::toString() const {
    return QString("[[%1, %2, %3], [%4, %5, %6], [%7, %8, %9]]")
        .arg(m[0][0], 0, 'f', 2).arg(m[0][1], 0, 'f', 2).arg(m[0][2], 0, 'f', 2)
        .arg(m[1][0], 0, 'f', 2).arg(m[1][1], 0, 'f', 2).arg(m[1][2], 0, 'f', 2)
        .arg(m[2][0], 0, 'f', 2).arg(m[2][1], 0, 'f', 2).arg(m[2][2], 0, 'f', 2);
}

QString Matrix3x3::toFormattedString(int precision) const {
    return QString("│ %1  %2  %3 │\n│ %4  %5  %6 │\n│ %7  %8  %9 │")
        .arg(m[0][0], 7, 'f', precision).arg(m[0][1], 7, 'f', precision).arg(m[0][2], 7, 'f', precision)
        .arg(m[1][0], 7, 'f', precision).arg(m[1][1], 7, 'f', precision).arg(m[1][2], 7, 'f', precision)
        .arg(m[2][0], 7, 'f', precision).arg(m[2][1], 7, 'f', precision).arg(m[2][2], 7, 'f', precision);
}

QString Matrix3x3::toHtmlTable() const {
    QString html = "<table style='border-collapse: collapse; font-family: monospace; font-size: 13px; color: #58a6ff; text-align: right; background-color: #0d1117; padding: 6px; border-radius: 6px; border: 1px solid #30363d;'>";
    for (int i = 0; i < 3; ++i) {
        html += "<tr>";
        for (int j = 0; j < 3; ++j) {
            QString valStr = (std::abs(m[i][j]) < 1e-6) ? "0.000" : QString::number(m[i][j], 'f', 3);
            QString color = (i == 2 && j == 2) ? "#2ea043" : ((j == 2) ? "#e3b341" : "#58a6ff");
            html += QString("<td style='padding: 4px 10px; color: %1;'>%2</td>").arg(color).arg(valStr);
        }
        html += "</tr>";
    }
    html += "</table>";
    return html;
}
