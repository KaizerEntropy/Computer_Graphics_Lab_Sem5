#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Polygon Transformation");
    resize(1000, 700);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Left side: Canvas
    canvas = new Canvas(this);
    mainLayout->addWidget(canvas, 1);

    // Right side: Controls
    QVBoxLayout *controlsLayout = new QVBoxLayout();
    controlsLayout->setAlignment(Qt::AlignTop);
    mainLayout->addLayout(controlsLayout);

    // 1. Translation
    QGroupBox *translationGroup = new QGroupBox("Translation");
    QFormLayout *tLayout = new QFormLayout();
    txSpin = new QDoubleSpinBox(); txSpin->setRange(-100, 100); txSpin->setValue(2);
    tySpin = new QDoubleSpinBox(); tySpin->setRange(-100, 100); tySpin->setValue(2);
    tLayout->addRow("tx:", txSpin);
    tLayout->addRow("ty:", tySpin);
    QPushButton *btnTranslate = new QPushButton("Translate");
    connect(btnTranslate, &QPushButton::clicked, this, &MainWindow::onTranslate);
    tLayout->addRow(btnTranslate);
    translationGroup->setLayout(tLayout);
    controlsLayout->addWidget(translationGroup);

    // 2. Rotation (Origin)
    QGroupBox *rotGroup = new QGroupBox("Rotation (Origin)");
    QFormLayout *rLayout = new QFormLayout();
    angleSpin = new QDoubleSpinBox(); angleSpin->setRange(-360, 360); angleSpin->setValue(45);
    rLayout->addRow("Angle:", angleSpin);
    QPushButton *btnRotate = new QPushButton("Rotate");
    connect(btnRotate, &QPushButton::clicked, this, &MainWindow::onRotateOrigin);
    rLayout->addRow(btnRotate);
    rotGroup->setLayout(rLayout);
    controlsLayout->addWidget(rotGroup);

    // 3. Scaling
    QGroupBox *scaleGroup = new QGroupBox("Scaling");
    QFormLayout *sLayout = new QFormLayout();
    sxSpin = new QDoubleSpinBox(); sxSpin->setRange(-10, 10); sxSpin->setValue(1.5); sxSpin->setSingleStep(0.1);
    sySpin = new QDoubleSpinBox(); sySpin->setRange(-10, 10); sySpin->setValue(1.5); sySpin->setSingleStep(0.1);
    sLayout->addRow("sx:", sxSpin);
    sLayout->addRow("sy:", sySpin);
    QPushButton *btnScale = new QPushButton("Scale");
    connect(btnScale, &QPushButton::clicked, this, &MainWindow::onScale);
    sLayout->addRow(btnScale);
    scaleGroup->setLayout(sLayout);
    controlsLayout->addWidget(scaleGroup);

    // 4. Shear
    QGroupBox *shearGroup = new QGroupBox("Shear");
    QFormLayout *shLayout = new QFormLayout();
    shxSpin = new QDoubleSpinBox(); shxSpin->setRange(-10, 10); shxSpin->setValue(0.5); shxSpin->setSingleStep(0.1);
    shySpin = new QDoubleSpinBox(); shySpin->setRange(-10, 10); shySpin->setValue(0.5); shySpin->setSingleStep(0.1);
    shLayout->addRow("shx:", shxSpin);
    shLayout->addRow("shy:", shySpin);
    QPushButton *btnShear = new QPushButton("Shear");
    connect(btnShear, &QPushButton::clicked, this, &MainWindow::onShear);
    shLayout->addRow(btnShear);
    shearGroup->setLayout(shLayout);
    controlsLayout->addWidget(shearGroup);

    // 5. Reflection (Axes)
    QGroupBox *refGroup = new QGroupBox("Reflection (Axes)");
    QVBoxLayout *refLayout = new QVBoxLayout();
    QPushButton *btnRefX = new QPushButton("Reflect X-Axis");
    QPushButton *btnRefY = new QPushButton("Reflect Y-Axis");
    connect(btnRefX, &QPushButton::clicked, this, &MainWindow::onReflectX);
    connect(btnRefY, &QPushButton::clicked, this, &MainWindow::onReflectY);
    refLayout->addWidget(btnRefX);
    refLayout->addWidget(btnRefY);
    refGroup->setLayout(refLayout);
    controlsLayout->addWidget(refGroup);

    // 6. Reflection (Arbitrary Line y = mx + c)
    QGroupBox *refArbGroup = new QGroupBox("Reflection (y = mx + c)");
    QFormLayout *raLayout = new QFormLayout();
    mSpin = new QDoubleSpinBox(); mSpin->setRange(-100, 100); mSpin->setValue(1); mSpin->setSingleStep(0.1);
    cSpin = new QDoubleSpinBox(); cSpin->setRange(-100, 100); cSpin->setValue(0);
    raLayout->addRow("m:", mSpin);
    raLayout->addRow("c:", cSpin);
    QPushButton *btnRefArb = new QPushButton("Reflect Line");
    connect(btnRefArb, &QPushButton::clicked, this, &MainWindow::onReflectArbitraryLine);
    raLayout->addRow(btnRefArb);
    refArbGroup->setLayout(raLayout);
    controlsLayout->addWidget(refArbGroup);

    // 7. Rotation (Arbitrary Point)
    QGroupBox *rotArbGroup = new QGroupBox("Rotation (Point px, py)");
    QFormLayout *rpLayout = new QFormLayout();
    arbPxSpin = new QDoubleSpinBox(); arbPxSpin->setRange(-100, 100); arbPxSpin->setValue(5);
    arbPySpin = new QDoubleSpinBox(); arbPySpin->setRange(-100, 100); arbPySpin->setValue(5);
    arbAngleSpin = new QDoubleSpinBox(); arbAngleSpin->setRange(-360, 360); arbAngleSpin->setValue(45);
    rpLayout->addRow("px:", arbPxSpin);
    rpLayout->addRow("py:", arbPySpin);
    rpLayout->addRow("Angle:", arbAngleSpin);
    QPushButton *btnRotArb = new QPushButton("Rotate Point");
    connect(btnRotArb, &QPushButton::clicked, this, &MainWindow::onRotateArbitraryPoint);
    rpLayout->addRow(btnRotArb);
    rotArbGroup->setLayout(rpLayout);
    controlsLayout->addWidget(rotArbGroup);

    // Utilities
    QPushButton *btnClear = new QPushButton("Clear Canvas");
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::onClearCanvas);
    controlsLayout->addWidget(btnClear);
}

void MainWindow::onTranslate() {
    canvas->setShowArbitraryLine(false);
    canvas->setShowArbitraryPoint(false);
    canvas->applyTransformation(TransformMatrix::translation(txSpin->value(), tySpin->value()));
}

void MainWindow::onRotateOrigin() {
    canvas->setShowArbitraryLine(false);
    canvas->setShowArbitraryPoint(false);
    canvas->applyTransformation(TransformMatrix::rotation(angleSpin->value()));
}

void MainWindow::onScale() {
    canvas->setShowArbitraryLine(false);
    canvas->setShowArbitraryPoint(false);
    canvas->applyTransformation(TransformMatrix::scaling(sxSpin->value(), sySpin->value()));
}

void MainWindow::onShear() {
    canvas->setShowArbitraryLine(false);
    canvas->setShowArbitraryPoint(false);
    TransformMatrix sx = TransformMatrix::shearX(shxSpin->value());
    TransformMatrix sy = TransformMatrix::shearY(shySpin->value());
    // Applying successively
    canvas->applyTransformation(sy * sx);
}

void MainWindow::onReflectX() {
    canvas->setShowArbitraryLine(false);
    canvas->setShowArbitraryPoint(false);
    canvas->applyTransformation(TransformMatrix::reflectionX());
}

void MainWindow::onReflectY() {
    canvas->setShowArbitraryLine(false);
    canvas->setShowArbitraryPoint(false);
    canvas->applyTransformation(TransformMatrix::reflectionY());
}

void MainWindow::onReflectArbitraryLine() {
    canvas->setShowArbitraryPoint(false);
    double m = mSpin->value();
    double c = cSpin->value();
    canvas->setShowArbitraryLine(true, m, c);
    canvas->applyTransformation(TransformMatrix::reflectionLine(m, c));
}

void MainWindow::onRotateArbitraryPoint() {
    canvas->setShowArbitraryLine(false);
    double px = arbPxSpin->value();
    double py = arbPySpin->value();
    canvas->setShowArbitraryPoint(true, px, py);
    canvas->applyTransformation(TransformMatrix::rotationAboutPoint(arbAngleSpin->value(), px, py));
}

void MainWindow::onClearCanvas() {
    canvas->clearCanvas();
}
