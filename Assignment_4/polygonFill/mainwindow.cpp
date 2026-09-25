#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <QPainter>
#include <QDebug>
#include <QElapsedTimer>
#include <algorithm>
#include <cmath>
#include <stack>
#include <QCoreApplication>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Polygon Fill Algorithms");

    lastPoint2 = QPoint(-1, -1);
    selectedPoint = QPoint(-1, -1);

    on_spinBox_valueChanged(ui->spinBox->value());

    connect(ui->frame, SIGNAL(Mouse_Left()), this, SLOT(Mouse_Left()));
    connect(ui->frame, SIGNAL(Mouse_Right()), this, SLOT(Mouse_Right()));
    connect(ui->frame, SIGNAL(sendMousePosition(QPoint&)), this, SLOT(showMousePosition(QPoint&)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void drawAxesAndGrid(QPainter &painter, int width, int height, int gridSize)
{
    if (gridSize <= 0) gridSize = 1;

    int originX = (width / 2 / gridSize) * gridSize;
    int originY = (height / 2 / gridSize) * gridSize;

    painter.setPen(QPen(Qt::white, 1));
    for (int x = gridSize; x < width; x += gridSize) {
        painter.drawLine(x, 0, x, height - 1);
    }
    for (int y = gridSize; y < height; y += gridSize) {
        painter.drawLine(0, y, width - 1, y);
    }

    painter.setPen(QPen(Qt::white, 1));
    painter.setBrush(QBrush(Qt::white));
    for (int x = 0; x < width; x += gridSize) {
        painter.drawRect(x, originY, gridSize, gridSize);
    }

    for (int y = 0; y < height; y += gridSize) {
        painter.drawRect(originX, y, gridSize, gridSize);
    }

    painter.setBrush(QBrush(Qt::blue));
    painter.drawRect(originX, originY, gridSize, gridSize);
}

void MainWindow::showMousePosition(QPoint &pos)
{
    sc_x = pos.x();
    sc_y = pos.y();
    int gridSize = ui->spinBox->value();

    if (gridSize <= 0) gridSize = 1;

    int width = ui->frame->width();
    int height = ui->frame->height();

    int originX = (width / 2 / gridSize) * gridSize;
    int originY = (height / 2 / gridSize) * gridSize;

    int cartesian_x = std::floor(static_cast<double>(sc_x - originX) / gridSize);
    int cartesian_y = -std::floor(static_cast<double>(sc_y - originY) / gridSize);

    ui->mouse_movement->setText("X : " + QString::number(cartesian_x) +
                                ", Y : " + QString::number(cartesian_y));
}

void MainWindow::Mouse_Left() {
    int cx = sc_x;
    int cy = sc_y;
    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    int originX = (ui->frame->width() / 2 / gridSize) * gridSize;
    int originY = (ui->frame->height() / 2 / gridSize) * gridSize;
    
    int gridX = std::floor(static_cast<double>(cx - originX) / gridSize);
    int gridY = -std::floor(static_cast<double>(cy - originY) / gridSize);
    
    QPoint pt(gridX, gridY);
    
    if (currentMode == DrawMode::Vertex) {
        polygonVertices.push_back(pt);
        drawDDA(pt, pt, Qt::red); 
        ui->center_label->setText("Added Vertex: (" + QString::number(gridX) + ", " + QString::number(gridY) + ")");
    } else if (currentMode == DrawMode::Seed) {
        lastPoint2 = pt;
        ui->center_label->setText("Seed Set: (" + QString::number(gridX) + ", " + QString::number(gridY) + ")");
    }
}

void MainWindow::Mouse_Right() {
    // Also support right-click mapping to Mouse_Left for convenience
    Mouse_Left();
}

void MainWindow::redrawCanvas()
{
    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;

    QPixmap pm(ui->frame->width(), ui->frame->height());
    pm.fill(Qt::black);

    QPainter painter(&pm);
    drawAxesAndGrid(painter, pm.width(), pm.height(), gridSize);
    painter.end();
    ui->frame->setPixmap(pm);
}

void MainWindow::on_clear_clicked()
{
    polygonVertices.clear();
    ui->time_label->setText("Stats will appear here...");
    ui->center_label->setText("Selected: None");
    lastPoint2 = QPoint(-1, -1);
    selectedPoint = QPoint(-1, -1);
    redrawCanvas();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->spinBox->setValue(10);
    on_spinBox_valueChanged(10);
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    if (arg1 <= 0) return;
    redrawCanvas();
}

QColor MainWindow::getSelectedColor() {
    QString text = ui->color_box->currentText();
    if (text == "Yellow") return Qt::yellow;
    if (text == "Red") return Qt::red;
    if (text == "Green") return Qt::green;
    if (text == "Blue") return Qt::blue;
    if (text == "White") return Qt::white;
    return Qt::yellow;
}

void MainWindow::drawDDA(QPoint p1, QPoint p2, QColor color) {
    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    QPixmap pm = ui->frame->pixmap();
    QPainter p(&pm);
    p.setBrush(color);
    p.setPen(Qt::white);
    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;
    
    int dx = p2.x() - p1.x();
    int dy = p2.y() - p1.y();
    int steps = std::max(std::abs(dx), std::abs(dy));
    
    if (steps == 0) {
        int drawX = originX + p1.x() * gridSize;
        int drawY = originY - p1.y() * gridSize;
        p.drawRect(drawX, drawY, gridSize, gridSize);
    } else {
        float xInc = dx / (float) steps;
        float yInc = dy / (float) steps;
        float x = p1.x();
        float y = p1.y();
        int prev_qx = std::round(x);
        int prev_qy = std::round(y);
        for (int i = 0; i <= steps; i++) {
            int qx = std::round(x);
            int qy = std::round(y);
            
            // Add extra pixel for diagonal steps to make line 4-connected
            // This prevents 8-way fills from leaking through the boundary
            if (i > 0 && qx != prev_qx && qy != prev_qy) {
                int drawX = originX + prev_qx * gridSize;
                int drawY = originY - qy * gridSize;
                p.drawRect(drawX, drawY, gridSize, gridSize);
            }
            
            int drawX = originX + qx * gridSize;
            int drawY = originY - qy * gridSize;
            p.drawRect(drawX, drawY, gridSize, gridSize);
            
            prev_qx = qx;
            prev_qy = qy;
            
            x += xInc;
            y += yInc;
        }
    }
    p.end();
    ui->frame->setPixmap(pm);
}

void MainWindow::on_set_vertex_clicked() {
    currentMode = DrawMode::Vertex;
    ui->center_label->setText("Mode: Set Vertex (Click on Canvas)");
}

void MainWindow::on_close_polygon_clicked() {
    // Draws boundary between the last 2 points
    if (polygonVertices.size() >= 2) {
        drawDDA(polygonVertices[polygonVertices.size() - 2], polygonVertices.back(), Qt::red);
    }
}

void MainWindow::on_set_seed_clicked() {
    currentMode = DrawMode::Seed;
    ui->center_label->setText("Mode: Set Seed (Click on Canvas)");
}

void MainWindow::boundaryFill(int x, int y, QColor fillColor, QColor boundaryColor, bool is8Way) {
    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    QPixmap pm = ui->frame->pixmap();
    QImage img = pm.toImage();
    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;
    
    auto getCol = [&](int qx, int qy) {
        int drawX = originX + qx * gridSize;
        int drawY = originY - qy * gridSize;
        int cx = drawX + gridSize / 2;
        int cy = drawY + gridSize / 2;
        if (cx >= 0 && cx < img.width() && cy >= 0 && cy < img.height()) {
            return img.pixelColor(cx, cy);
        }
        return QColor(boundaryColor); 
    };
    
    QPainter p(&img);
    p.setBrush(fillColor);
    p.setPen(Qt::white);
    
    auto setCol = [&](int qx, int qy) {
        int drawX = originX + qx * gridSize;
        int drawY = originY - qy * gridSize;
        p.drawRect(drawX, drawY, gridSize, gridSize);
    };

    std::stack<QPoint> s;
    s.push(QPoint(x, y));
    int iterations = 0;
    while (!s.empty()) {
        QPoint pt = s.top();
        s.pop();
        int px = pt.x();
        int py = pt.y();
        
        QColor curr = getCol(px, py);
        if (curr != boundaryColor && curr != fillColor) {
            setCol(px, py);
            s.push(QPoint(px + 1, py));
            s.push(QPoint(px - 1, py));
            s.push(QPoint(px, py + 1));
            s.push(QPoint(px, py - 1));
            
            if (is8Way) {
                s.push(QPoint(px + 1, py + 1));
                s.push(QPoint(px - 1, py + 1));
                s.push(QPoint(px + 1, py - 1));
                s.push(QPoint(px - 1, py - 1));
            }
            
            iterations++;
            if (iterations % 10 == 0) { // Gradually update
                ui->frame->setPixmap(QPixmap::fromImage(img));
                QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
                QThread::msleep(5);
            }
        }
    }
    
    p.end();
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::floodFill(int x, int y, QColor fillColor, QColor oldColor, bool is8Way) {
    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    QPixmap pm = ui->frame->pixmap();
    QImage img = pm.toImage();
    int originX = (pm.width() / 2 / gridSize) * gridSize;
    int originY = (pm.height() / 2 / gridSize) * gridSize;
    
    auto getCol = [&](int qx, int qy) {
        int drawX = originX + qx * gridSize;
        int drawY = originY - qy * gridSize;
        int cx = drawX + gridSize / 2;
        int cy = drawY + gridSize / 2;
        if (cx >= 0 && cx < img.width() && cy >= 0 && cy < img.height()) {
            return img.pixelColor(cx, cy);
        }
        return QColor(Qt::transparent);
    };
    
    QPainter p(&img);
    p.setBrush(fillColor);
    p.setPen(Qt::white);
    
    auto setCol = [&](int qx, int qy) {
        int drawX = originX + qx * gridSize;
        int drawY = originY - qy * gridSize;
        p.drawRect(drawX, drawY, gridSize, gridSize);
    };

    std::stack<QPoint> s;
    s.push(QPoint(x, y));
    int iterations = 0;
    while (!s.empty()) {
        QPoint pt = s.top();
        s.pop();
        int px = pt.x();
        int py = pt.y();
        
        QColor curr = getCol(px, py);
        if (curr == oldColor && curr != fillColor) {
            setCol(px, py);
            s.push(QPoint(px + 1, py));
            s.push(QPoint(px - 1, py));
            s.push(QPoint(px, py + 1));
            s.push(QPoint(px, py - 1));
            
            if (is8Way) {
                s.push(QPoint(px + 1, py + 1));
                s.push(QPoint(px - 1, py + 1));
                s.push(QPoint(px + 1, py - 1));
                s.push(QPoint(px - 1, py - 1));
            }
            
            iterations++;
            if (iterations % 10 == 0) { // Gradually update
                ui->frame->setPixmap(QPixmap::fromImage(img));
                QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
                QThread::msleep(5);
            }
        }
    }
    
    p.end();
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_boundary_fill_clicked() {
    if (lastPoint2 == QPoint(-1, -1)) return;
    
    bool is8Way = ui->connectivity_box->currentText() == "8-Way";
    QElapsedTimer timer; timer.start();
    boundaryFill(lastPoint2.x(), lastPoint2.y(), getSelectedColor(), Qt::red, is8Way); 
    qint64 elapsed = timer.nsecsElapsed();
    ui->time_label->setText("Boundary Fill Done: " + QString::number(elapsed / 1000000) + " ms");
}

void MainWindow::on_flood_fill_clicked() {
    if (lastPoint2 == QPoint(-1, -1)) return;
    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    int originX = (ui->frame->width() / 2 / gridSize) * gridSize;
    int originY = (ui->frame->height() / 2 / gridSize) * gridSize;
    
    QPixmap pm = ui->frame->pixmap();
    int drawX = originX + lastPoint2.x() * gridSize;
    int drawY = originY - lastPoint2.y() * gridSize;
    int cx = drawX + gridSize / 2;
    int cy = drawY + gridSize / 2;
    QColor oldColor = pm.toImage().pixelColor(cx, cy);
    
    bool is8Way = ui->connectivity_box->currentText() == "8-Way";
    QElapsedTimer timer; timer.start();
    floodFill(lastPoint2.x(), lastPoint2.y(), getSelectedColor(), oldColor, is8Way);
    qint64 elapsed = timer.nsecsElapsed();
    ui->time_label->setText("Flood Fill Done: " + QString::number(elapsed / 1000000) + " ms");
}

void MainWindow::scanlineFill() {
    if (polygonVertices.size() < 3) return;
    int gridSize = ui->spinBox->value();
    if (gridSize <= 0) gridSize = 1;
    QPixmap pm = ui->frame->pixmap();
    QImage img = pm.toImage();
    QPainter p(&img);
    p.setBrush(getSelectedColor());
    p.setPen(Qt::white);
    int originX = (img.width() / 2 / gridSize) * gridSize;
    int originY = (img.height() / 2 / gridSize) * gridSize;

    int min_y = polygonVertices[0].y();
    int max_y = polygonVertices[0].y();
    for (const auto& pt : polygonVertices) {
        min_y = std::min(min_y, pt.y());
        max_y = std::max(max_y, pt.y());
    }

    struct Edge {
        float x;
        float dx; 
        int y_max;
        bool operator<(const Edge& other) const {
            return x < other.x;
        }
    };

    std::vector<std::vector<Edge>> edgeTable(max_y - min_y + 1);

    for (size_t i = 0; i < polygonVertices.size(); i++) {
        QPoint p1 = polygonVertices[i];
        QPoint p2 = polygonVertices[(i + 1) % polygonVertices.size()];

        if (p1.y() == p2.y()) continue; 

        if (p1.y() > p2.y()) std::swap(p1, p2);

        Edge e;
        e.x = p1.x();
        e.dx = (float)(p2.x() - p1.x()) / (p2.y() - p1.y());
        e.y_max = p2.y();
        
        edgeTable[p1.y() - min_y].push_back(e);
    }

    std::vector<Edge> activeEdgeTable;

    for (int y = min_y; y <= max_y; y++) {
        for (const auto& e : edgeTable[y - min_y]) {
            activeEdgeTable.push_back(e);
        }

        activeEdgeTable.erase(std::remove_if(activeEdgeTable.begin(), activeEdgeTable.end(),
            [y](const Edge& e) { return e.y_max == y; }), activeEdgeTable.end());

        std::sort(activeEdgeTable.begin(), activeEdgeTable.end());

        for (size_t i = 0; i + 1 < activeEdgeTable.size(); i += 2) {
            int x_start = std::round(activeEdgeTable[i].x);
            int x_end = std::round(activeEdgeTable[i + 1].x);
            for (int x = x_start; x <= x_end; x++) {
                int drawX = originX + x * gridSize;
                int drawY = originY - y * gridSize;
                p.drawRect(drawX, drawY, gridSize, gridSize);
            }
        }

        for (auto& e : activeEdgeTable) {
            e.x += e.dx;
        }
        
        // Gradually update row by row
        ui->frame->setPixmap(QPixmap::fromImage(img));
        QCoreApplication::processEvents(QEventLoop::AllEvents, 5);
        QThread::msleep(50);
    }
    
    p.end();
    ui->frame->setPixmap(QPixmap::fromImage(img));
}

void MainWindow::on_scanline_fill_clicked() {
    QElapsedTimer timer; timer.start();
    scanlineFill();
    qint64 elapsed = timer.nsecsElapsed();
    ui->time_label->setText("Scanline Fill Done: " + QString::number(elapsed / 1000000) + " ms");
}