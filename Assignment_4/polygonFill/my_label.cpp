#include "my_label.h"

my_label::my_label(QWidget *parent) : QLabel(parent)
{
    this->setMouseTracking(true);
}

void my_label::mouseMoveEvent(QMouseEvent *ev)
{
    QPoint pos = ev->pos();
    if (pos.x() >= 0 && pos.y() >= 0 && pos.x() < this->width() && pos.y() < this->height()) {
        emit sendMousePosition(pos);
    }
}

void my_label::mousePressEvent(QMouseEvent *ev)
{
    x = ev->x();
    y = ev->y();
    if (ev->button() == Qt::LeftButton) {
        emit Mouse_Left();
    } else if (ev->button() == Qt::RightButton) {
        emit Mouse_Right();
    }
}
