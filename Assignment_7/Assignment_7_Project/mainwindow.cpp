#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Bezier Curve Drawing");
    resize(900, 600);
    
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    
    // Left Sidebar Layout
    QVBoxLayout *sidebarLayout = new QVBoxLayout();
    sidebarLayout->setContentsMargins(10, 10, 10, 10);
    sidebarLayout->setSpacing(15);
    
    // --- Grid Settings Group ---
    QGroupBox *gridGroup = new QGroupBox("Grid Settings");
    QFormLayout *gridLayout = new QFormLayout(gridGroup);
    
    gridSizeSpinBox = new QSpinBox();
    gridSizeSpinBox->setRange(1, 100);
    gridSizeSpinBox->setValue(10);
    connect(gridSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onGridSizeChanged);
    
    showGridCheckBox = new QCheckBox("Show Grid");
    showGridCheckBox->setChecked(true);
    connect(showGridCheckBox, &QCheckBox::toggled, this, &MainWindow::onToggleGrid);
    
    showAxesCheckBox = new QCheckBox("Show Axes");
    showAxesCheckBox->setChecked(true);
    connect(showAxesCheckBox, &QCheckBox::toggled, this, &MainWindow::onToggleAxes);
    
    gridLayout->addRow("Grid Size:", gridSizeSpinBox);
    gridLayout->addRow(showGridCheckBox);
    gridLayout->addRow(showAxesCheckBox);
    
    // --- Bezier Controls Group ---
    QGroupBox *bezierGroup = new QGroupBox("Bezier Controls");
    QVBoxLayout *bezierLayout = new QVBoxLayout(bezierGroup);
    
    c1RadioBtn = new QRadioButton("Continuous (C1)");
    c1RadioBtn->setChecked(true);
    c0RadioBtn = new QRadioButton("Independent (C0)");
    
    bezierLayout->addWidget(c1RadioBtn);
    bezierLayout->addWidget(c0RadioBtn);
    
    connect(c1RadioBtn, &QRadioButton::toggled, [=](bool checked){
        if (canvas) {
            canvas->setContinuousC1(checked);
            updatePointList();
        }
    });
    
    drawBtn = new QPushButton("Draw Bezier Curve");
    connect(drawBtn, &QPushButton::clicked, this, &MainWindow::onDrawBezier);
    
    undoBtn = new QPushButton("Undo Last Point");
    connect(undoBtn, &QPushButton::clicked, this, &MainWindow::onUndoPoint);
    
    clearBtn = new QPushButton("Clear Canvas");
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearCanvas);
    
    bezierLayout->addWidget(drawBtn);
    bezierLayout->addWidget(undoBtn);
    bezierLayout->addWidget(clearBtn);
    
    // --- Points List ---
    QGroupBox *pointsGroup = new QGroupBox("Control Points");
    QVBoxLayout *pointsLayout = new QVBoxLayout(pointsGroup);
    pointListWidget = new QListWidget();
    pointsLayout->addWidget(pointListWidget);
    
    // Status and Mouse
    statusLabel = new QLabel("Click 4 control points on the canvas.\nThen press \"Draw Bezier Curve\" or it will auto-draw.");
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet("color: gray;");
    
    mousePositionLabel = new QLabel("X: 0, Y: 0");
    mousePositionLabel->setAlignment(Qt::AlignCenter);
    mousePositionLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    
    // Assemble sidebar
    sidebarLayout->addWidget(gridGroup);
    sidebarLayout->addWidget(bezierGroup);
    sidebarLayout->addWidget(pointsGroup);
    sidebarLayout->addWidget(statusLabel);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(mousePositionLabel);
    
    // Canvas
    canvas = new Canvas();
    canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    connect(canvas, &Canvas::pointAdded, this, &MainWindow::onCanvasPointAdded);
    connect(canvas, &Canvas::mouseMoved, this, &MainWindow::onCanvasMouseMoved);
    
    // Assemble Main Layout
    mainLayout->addLayout(sidebarLayout, 1);
    mainLayout->addWidget(canvas, 4);
    
    // Sync initial states
    canvas->setGridSize(gridSizeSpinBox->value());
    canvas->setShowGrid(showGridCheckBox->isChecked());
    canvas->setShowAxes(showAxesCheckBox->isChecked());
    canvas->setContinuousC1(c1RadioBtn->isChecked());
}

void MainWindow::onGridSizeChanged(int value)
{
    canvas->setGridSize(value);
}

void MainWindow::onToggleGrid(bool checked)
{
    canvas->setShowGrid(checked);
}

void MainWindow::onToggleAxes(bool checked)
{
    canvas->setShowAxes(checked);
}

void MainWindow::onClearCanvas()
{
    canvas->clearCanvas();
    updatePointList();
    statusLabel->setText("Canvas cleared.\nClick 4 control points to start.");
}

void MainWindow::onUndoPoint()
{
    canvas->removeLastControlPoint();
    updatePointList();
}

void MainWindow::onDrawBezier()
{
    canvas->computeBezierCurve();
    canvas->update();
}

void MainWindow::onCanvasPointAdded(QPoint point)
{
    updatePointList();
}

void MainWindow::updatePointList()
{
    pointListWidget->clear();
    const QVector<QPoint> &pts = canvas->getControlPoints();
    
    bool c1 = c1RadioBtn->isChecked();
    
    for (int i = 0; i < pts.size(); ++i) {
        QString label = QString("P%1: (%2, %3)").arg(i).arg(pts[i].x()).arg(pts[i].y());
        pointListWidget->addItem(label);
    }
    
    int n = pts.size();
    if (n < 4) {
        statusLabel->setText(QString("Added P%1.\nNeed %2 more point(s) for the first segment.")
                             .arg(n-1).arg(4 - n));
    } else {
        if (c1) {
            if ((n - 4) % 2 == 0) {
                statusLabel->setText(QString("Segment complete.\nTotal points: %1.\nClick 2 more for next segment.").arg(n));
            } else {
                statusLabel->setText(QString("Added P%1.\nNeed 1 more point to complete next segment.").arg(n-1));
            }
        } else {
            if (n % 4 == 0) {
                statusLabel->setText(QString("Segment complete.\nTotal points: %1.\nClick 4 more for next independent segment.").arg(n));
            } else {
                statusLabel->setText(QString("Added P%1.\nNeed %2 more point(s) to complete next segment.")
                                     .arg(n-1).arg(4 - (n % 4)));
            }
        }
    }
}

void MainWindow::onCanvasMouseMoved(QPoint point)
{
    mousePositionLabel->setText(QString("X: %1, Y: %2").arg(point.x()).arg(point.y()));
}
