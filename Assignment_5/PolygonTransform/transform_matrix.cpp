#include "transform_matrix.h"

TransformMatrix::TransformMatrix() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            m[i][j] = (i == j) ? 1.0 : 0.0;
        }
    }
}

TransformMatrix::TransformMatrix(double m00, double m01, double m02,
                                 double m10, double m11, double m12,
                                 double m20, double m21, double m22) {
    m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
    m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
    m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
}

TransformMatrix TransformMatrix::identity() {
    return TransformMatrix();
}

TransformMatrix TransformMatrix::translation(double tx, double ty) {
    return TransformMatrix(1, 0, tx,
                           0, 1, ty,
                           0, 0, 1);
}

TransformMatrix TransformMatrix::rotation(double angleDegrees) {
    double rad = angleDegrees * M_PI / 180.0;
    double c = std::cos(rad);
    double s = std::sin(rad);
    return TransformMatrix(c, -s, 0,
                           s,  c, 0,
                           0,  0, 1);
}

TransformMatrix TransformMatrix::scaling(double sx, double sy) {
    return TransformMatrix(sx, 0, 0,
                           0, sy, 0,
                           0, 0, 1);
}

TransformMatrix TransformMatrix::shearX(double shx) {
    return TransformMatrix(1, shx, 0,
                           0,   1, 0,
                           0,   0, 1);
}

TransformMatrix TransformMatrix::shearY(double shy) {
    return TransformMatrix(1,   0, 0,
                           shy, 1, 0,
                           0,   0, 1);
}

TransformMatrix TransformMatrix::reflectionX() {
    return TransformMatrix(1,  0, 0,
                           0, -1, 0,
                           0,  0, 1);
}

TransformMatrix TransformMatrix::reflectionY() {
    return TransformMatrix(-1, 0, 0,
                            0, 1, 0,
                            0, 0, 1);
}

TransformMatrix TransformMatrix::reflectionLine(double slope, double intercept) {
    // Translate intercept to origin
    TransformMatrix t1 = translation(0, -intercept);
    // Rotate to x-axis
    double angle = std::atan(slope) * 180.0 / M_PI;
    TransformMatrix r1 = rotation(-angle);
    // Reflect across x-axis
    TransformMatrix ref = reflectionX();
    // Rotate back
    TransformMatrix r2 = rotation(angle);
    // Translate back
    TransformMatrix t2 = translation(0, intercept);

    return t2 * r2 * ref * r1 * t1;
}

TransformMatrix TransformMatrix::rotationAboutPoint(double angleDegrees, double px, double py) {
    TransformMatrix t1 = translation(-px, -py);
    TransformMatrix r = rotation(angleDegrees);
    TransformMatrix t2 = translation(px, py);

    return t2 * r * t1;
}

TransformMatrix TransformMatrix::operator*(const TransformMatrix &other) const {
    TransformMatrix result(0,0,0,0,0,0,0,0,0);
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            for (int k = 0; k < 3; ++k) {
                result.m[i][j] += m[i][k] * other.m[k][j];
            }
        }
    }
    return result;
}

TransformMatrix& TransformMatrix::operator*=(const TransformMatrix &other) {
    *this = *this * other;
    return *this;
}

QPointF TransformMatrix::transform(const QPointF &pt) const {
    double x = pt.x();
    double y = pt.y();
    double nx = m[0][0] * x + m[0][1] * y + m[0][2] * 1.0;
    double ny = m[1][0] * x + m[1][1] * y + m[1][2] * 1.0;
    double w  = m[2][0] * x + m[2][1] * y + m[2][2] * 1.0;
    
    if (std::abs(w) > 1e-6) {
        return QPointF(nx / w, ny / w);
    }
    return QPointF(nx, ny);
}

QVector<QPointF> TransformMatrix::transform(const QVector<QPointF> &pts) const {
    QVector<QPointF> res;
    res.reserve(pts.size());
    for (const auto &p : pts) {
        res.append(transform(p));
    }
    return res;
}
