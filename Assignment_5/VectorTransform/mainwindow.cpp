#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_canvas = ui->canvasWidget;

    setupDarkTheme();

    // Configure Table View for History
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableHistory->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    // ------------------------------------------------------------------------
    // Signal / Slot Connections
    // ------------------------------------------------------------------------

    // Quick Command Input
    connect(ui->btnRunQuickCommand, &QPushButton::clicked, this, &MainWindow::onRunQuickCommand);
    connect(ui->txtQuickCommand, &QLineEdit::returnPressed, this, &MainWindow::onRunQuickCommand);

    // 1. Selection & Scope
    connect(ui->comboSelectShape, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSelectShapeComboChanged);
    connect(ui->btnDeselectAll, &QPushButton::clicked, this, &MainWindow::onDeselectAllClicked);
    connect(m_canvas, &Canvas::shapeSelectionChanged, this, &MainWindow::handleShapeSelectionChanged);
    connect(m_canvas, &Canvas::shapesChanged, this, &MainWindow::handleShapesChanged);

    // 2. Translation
    connect(ui->btnApplyTranslate, &QPushButton::clicked, this, &MainWindow::onApplyTranslationClicked);
    connect(ui->btnNudgeUp, &QPushButton::clicked, this, &MainWindow::onNudgeUp);
    connect(ui->btnNudgeDown, &QPushButton::clicked, this, &MainWindow::onNudgeDown);
    connect(ui->btnNudgeLeft, &QPushButton::clicked, this, &MainWindow::onNudgeLeft);
    connect(ui->btnNudgeRight, &QPushButton::clicked, this, &MainWindow::onNudgeRight);
    connect(ui->btnNudgeReset, &QPushButton::clicked, this, &MainWindow::onNudgeReset);

    // 3. Rotation
    connect(ui->btnApplyRotate, &QPushButton::clicked, this, &MainWindow::onApplyRotationClicked);
    connect(ui->btnPickPivot, &QPushButton::clicked, this, &MainWindow::onPickPivotClicked);
    connect(ui->btnQuickRot45, &QPushButton::clicked, this, &MainWindow::onQuickRot45);
    connect(ui->btnQuickRotNeg45, &QPushButton::clicked, this, &MainWindow::onQuickRotNeg45);
    connect(ui->btnQuickRot90, &QPushButton::clicked, this, &MainWindow::onQuickRot90);
    connect(ui->btnQuickRot180, &QPushButton::clicked, this, &MainWindow::onQuickRot180);

    // 4. Scaling
    connect(ui->btnApplyScale, &QPushButton::clicked, this, &MainWindow::onApplyScalingClicked);
    connect(ui->chkUniformScale, &QCheckBox::toggled, this, &MainWindow::onUniformScaleToggled);
    connect(ui->spinScaleX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onScaleXChanged);
    connect(ui->btnQuickScale2, &QPushButton::clicked, this, &MainWindow::onQuickScale2);
    connect(ui->btnQuickScaleHalf, &QPushButton::clicked, this, &MainWindow::onQuickScaleHalf);
    connect(ui->btnQuickScale15, &QPushButton::clicked, this, &MainWindow::onQuickScale15);
    connect(ui->btnQuickScaleFlip, &QPushButton::clicked, this, &MainWindow::onQuickScaleFlip);

    // 5. Shearing
    connect(ui->btnApplyShear, &QPushButton::clicked, this, &MainWindow::onApplyShearClicked);

    // 6. Reflections
    connect(ui->btnReflectX, &QPushButton::clicked, this, &MainWindow::onReflectXClicked);
    connect(ui->btnReflectY, &QPushButton::clicked, this, &MainWindow::onReflectYClicked);
    connect(ui->btnReflectOrigin, &QPushButton::clicked, this, &MainWindow::onReflectOriginClicked);
    connect(ui->btnReflectLineYeqX, &QPushButton::clicked, this, &MainWindow::onReflectLineYeqXClicked);
    connect(ui->btnReflectLineYeqNegX, &QPushButton::clicked, this, &MainWindow::onReflectLineYeqNegXClicked);
    connect(ui->btnPickReflectLine, &QPushButton::clicked, this, &MainWindow::onPickReflectLineClicked);
    connect(ui->btnApplyReflectArbitrary, &QPushButton::clicked, this, &MainWindow::onApplyReflectArbitraryClicked);

    // 7. Custom 3x3 Matrix Input
    connect(ui->btnApplyCustomMatrix, &QPushButton::clicked, this, &MainWindow::onApplyCustomMatrixClicked);

    // 8. Matrix & History Actions
    connect(ui->btnUndoTransform, &QPushButton::clicked, this, &MainWindow::onUndoTransformClicked);
    connect(ui->btnResetTransform, &QPushButton::clicked, this, &MainWindow::onResetTransformClicked);
    connect(m_canvas, &Canvas::transformationApplied, this, &MainWindow::handleTransformationApplied);
    connect(m_canvas, &Canvas::transformationsReset, this, &MainWindow::handleTransformationsReset);

    // 9. Fills
    connect(ui->comboFillAlgorithm, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFillAlgorithmChanged);
    connect(ui->btnExecuteFill, &QPushButton::clicked, this, &MainWindow::onExecuteFillClicked);
    connect(ui->btnClearFill, &QPushButton::clicked, this, &MainWindow::onClearFillClicked);

    // 10. Shape Tools & Presets
    connect(ui->btnDrawPolygon, &QPushButton::clicked, this, &MainWindow::onDrawPolygonClicked);
    connect(ui->btnDrawCircle, &QPushButton::clicked, this, &MainWindow::onDrawCircleClicked);
    connect(ui->btnClosePolygon, &QPushButton::clicked, this, &MainWindow::onClosePolygonClicked);
    connect(ui->btnUndoShape, &QPushButton::clicked, this, &MainWindow::onUndoShapeClicked);

    connect(ui->btnPresetTriangle, &QPushButton::clicked, this, &MainWindow::onPresetTriangleClicked);
    connect(ui->btnPresetConvex, &QPushButton::clicked, this, &MainWindow::onPresetConvexClicked);
    connect(ui->btnPresetHouse, &QPushButton::clicked, this, &MainWindow::onPresetHouseClicked);
    connect(ui->btnPresetStar, &QPushButton::clicked, this, &MainWindow::onPresetStarClicked);
    connect(ui->btnPresetConcave, &QPushButton::clicked, this, &MainWindow::onPresetConcaveClicked);
    connect(ui->btnPresetNested, &QPushButton::clicked, this, &MainWindow::onPresetNestedClicked);
    connect(ui->btnPresetMultiScene, &QPushButton::clicked, this, &MainWindow::onPresetMultiSceneClicked);

    // 11. Display & Overlays
    connect(ui->spinGridSize, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onGridSizeChanged);
    connect(ui->chkShowGhost, &QCheckBox::toggled, this, &MainWindow::onToggleShowGhost);
    connect(ui->chkShowPivot, &QCheckBox::toggled, this, &MainWindow::onToggleShowPivot);
    connect(ui->chkShowReflectLine, &QCheckBox::toggled, this, &MainWindow::onToggleShowReflectLine);
    connect(ui->chkShowLabels, &QCheckBox::toggled, this, &MainWindow::onToggleShowLabels);
    connect(ui->chkShowGrid, &QCheckBox::toggled, this, &MainWindow::onToggleShowGrid);
    connect(ui->chkShowAxes, &QCheckBox::toggled, this, &MainWindow::onToggleShowAxes);
    connect(ui->btnClearAll, &QPushButton::clicked, this, &MainWindow::onClearAllClicked);

    // Canvas signals
    connect(m_canvas, &Canvas::mouseMoved, this, &MainWindow::handleMouseMoved);
    connect(m_canvas, &Canvas::pivotChanged, this, &MainWindow::handlePivotChanged);
    connect(m_canvas, &Canvas::reflectLineChanged, this, &MainWindow::handleReflectLineChanged);
    connect(m_canvas, &Canvas::fillFinished, this, &MainWindow::handleFillFinished);
    connect(m_canvas, &Canvas::statusMessage, this, &MainWindow::handleStatusMessage);

    // Initialize all spinbox limits, steps, and default values
    setupSpinBoxes();

    // Setup enter-key hooks on all spinbox text fields
    setupReturnKeyHooks();

    // Initialize Default State
    onPresetConvexClicked();
    updateMatrixInspector();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupSpinBoxes() {
    // Translation
    ui->spinTx->setRange(-1000, 1000);
    ui->spinTx->setSingleStep(5);
    ui->spinTx->setValue(5);

    ui->spinTy->setRange(-1000, 1000);
    ui->spinTy->setSingleStep(5);
    ui->spinTy->setValue(5);

    // Rotation
    ui->spinRotAngle->setDecimals(2);
    ui->spinRotAngle->setRange(-3600.0, 3600.0);
    ui->spinRotAngle->setSingleStep(1.0);
    ui->spinRotAngle->setValue(45.0);

    ui->spinRotPx->setRange(-1000, 1000);
    ui->spinRotPx->setValue(0);
    ui->spinRotPy->setRange(-1000, 1000);
    ui->spinRotPy->setValue(0);

    // Scaling
    ui->spinScaleX->setDecimals(3);
    ui->spinScaleX->setRange(-100.0, 100.0);
    ui->spinScaleX->setSingleStep(0.1);
    ui->spinScaleX->setValue(1.5);

    ui->spinScaleY->setDecimals(3);
    ui->spinScaleY->setRange(-100.0, 100.0);
    ui->spinScaleY->setSingleStep(0.1);
    ui->spinScaleY->setValue(1.5);

    ui->spinScalePx->setRange(-1000, 1000);
    ui->spinScalePx->setValue(0);
    ui->spinScalePy->setRange(-1000, 1000);
    ui->spinScalePy->setValue(0);

    // Shear
    ui->spinShearX->setDecimals(3);
    ui->spinShearX->setRange(-100.0, 100.0);
    ui->spinShearX->setSingleStep(0.1);
    ui->spinShearX->setValue(0.5);

    ui->spinShearY->setDecimals(3);
    ui->spinShearY->setRange(-100.0, 100.0);
    ui->spinShearY->setSingleStep(0.1);
    ui->spinShearY->setValue(0.0);

    ui->spinShearYRef->setRange(-1000, 1000);
    ui->spinShearYRef->setValue(0);
    ui->spinShearXRef->setRange(-1000, 1000);
    ui->spinShearXRef->setValue(0);

    // Reflection Arbitrary
    ui->spinReflectSlope->setDecimals(3);
    ui->spinReflectSlope->setRange(-1000.0, 1000.0);
    ui->spinReflectSlope->setSingleStep(0.5);
    ui->spinReflectSlope->setValue(1.0);

    ui->spinReflectIntercept->setDecimals(2);
    ui->spinReflectIntercept->setRange(-1000.0, 1000.0);
    ui->spinReflectIntercept->setSingleStep(1.0);
    ui->spinReflectIntercept->setValue(0.0);

    // Custom Matrix
    auto initMatSpin = [](QDoubleSpinBox *sb, double defVal) {
        sb->setDecimals(3);
        sb->setRange(-1000.0, 1000.0);
        sb->setSingleStep(0.1);
        sb->setValue(defVal);
    };
    initMatSpin(ui->spinM00, 1.0);
    initMatSpin(ui->spinM01, 0.0);
    initMatSpin(ui->spinM02, 0.0);
    initMatSpin(ui->spinM10, 0.0);
    initMatSpin(ui->spinM11, 1.0);
    initMatSpin(ui->spinM12, 0.0);
    initMatSpin(ui->spinM20, 0.0);
    initMatSpin(ui->spinM21, 0.0);
    initMatSpin(ui->spinM22, 1.0);
}

void MainWindow::setupReturnKeyHooks() {
    auto hookReturn = [](QAbstractSpinBox *spinBox, auto slotFunc, MainWindow *mw) {
        if (QLineEdit *le = spinBox->findChild<QLineEdit*>()) {
            QObject::connect(le, &QLineEdit::returnPressed, mw, [spinBox, slotFunc, mw]() {
                spinBox->interpretText();
                (mw->*slotFunc)();
            });
        }
    };

    hookReturn(ui->spinTx, &MainWindow::onApplyTranslationClicked, this);
    hookReturn(ui->spinTy, &MainWindow::onApplyTranslationClicked, this);

    hookReturn(ui->spinRotAngle, &MainWindow::onApplyRotationClicked, this);
    hookReturn(ui->spinRotPx, &MainWindow::onApplyRotationClicked, this);
    hookReturn(ui->spinRotPy, &MainWindow::onApplyRotationClicked, this);

    hookReturn(ui->spinScaleX, &MainWindow::onApplyScalingClicked, this);
    hookReturn(ui->spinScaleY, &MainWindow::onApplyScalingClicked, this);
    hookReturn(ui->spinScalePx, &MainWindow::onApplyScalingClicked, this);
    hookReturn(ui->spinScalePy, &MainWindow::onApplyScalingClicked, this);

    hookReturn(ui->spinShearX, &MainWindow::onApplyShearClicked, this);
    hookReturn(ui->spinShearY, &MainWindow::onApplyShearClicked, this);
    hookReturn(ui->spinShearYRef, &MainWindow::onApplyShearClicked, this);
    hookReturn(ui->spinShearXRef, &MainWindow::onApplyShearClicked, this);

    hookReturn(ui->spinReflectSlope, &MainWindow::onApplyReflectArbitraryClicked, this);
    hookReturn(ui->spinReflectIntercept, &MainWindow::onApplyReflectArbitraryClicked, this);

    hookReturn(ui->spinM00, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(ui->spinM01, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(ui->spinM02, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(ui->spinM10, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(ui->spinM11, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(ui->spinM12, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(ui->spinM20, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(ui->spinM21, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(ui->spinM22, &MainWindow::onApplyCustomMatrixClicked, this);
}

void MainWindow::setupDarkTheme() {
    QString style = R"(
        QMainWindow, QWidget#centralwidget {
            background-color: #0d1117;
            color: #c9d1d9;
            font-family: 'Segoe UI', Inter, -apple-system, sans-serif;
            font-size: 13px;
        }
        QGroupBox {
            font-weight: bold;
            border: 1px solid #30363d;
            border-radius: 6px;
            margin-top: 8px;
            padding-top: 14px;
            background-color: #161b22;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 4px;
            color: #58a6ff;
        }
        QTabWidget::pane {
            border: 1px solid #30363d;
            background-color: #161b22;
            border-radius: 4px;
        }
        QTabBar::tab {
            background-color: #21262d;
            color: #8b949e;
            padding: 6px 10px;
            border: 1px solid #30363d;
            border-bottom: none;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            margin-right: 2px;
        }
        QTabBar::tab:selected {
            background-color: #161b22;
            color: #58a6ff;
            font-weight: bold;
            border-bottom: 2px solid #58a6ff;
        }
        QPushButton {
            background-color: #21262d;
            color: #c9d1d9;
            border: 1px solid #30363d;
            border-radius: 4px;
            padding: 5px 8px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #30363d;
            border-color: #8b949e;
        }
        QPushButton:pressed {
            background-color: #1f6feb;
            color: #ffffff;
        }
        QComboBox, QLineEdit {
            background-color: #0d1117;
            color: #c9d1d9;
            border: 1px solid #30363d;
            border-radius: 4px;
            padding: 4px 8px;
            font-family: monospace;
            font-size: 13px;
        }
        QSpinBox, QDoubleSpinBox {
            background-color: #0d1117;
            color: #58a6ff;
            border: 1px solid #30363d;
            border-radius: 4px;
            padding: 4px 6px;
            font-family: monospace;
            font-size: 13px;
            font-weight: bold;
            min-height: 22px;
        }
        QSpinBox:focus, QDoubleSpinBox:focus, QLineEdit:focus {
            border: 1px solid #58a6ff;
            background-color: #161b22;
        }
        QSpinBox::up-button, QDoubleSpinBox::up-button {
            subcontrol-origin: border;
            subcontrol-position: top right;
            width: 16px;
            border-left: 1px solid #30363d;
            background-color: #21262d;
        }
        QSpinBox::down-button, QDoubleSpinBox::down-button {
            subcontrol-origin: border;
            subcontrol-position: bottom right;
            width: 16px;
            border-left: 1px solid #30363d;
            background-color: #21262d;
        }
        QTableWidget {
            background-color: #0d1117;
            alternate-background-color: #161b22;
            color: #c9d1d9;
            gridline-color: #30363d;
            border: 1px solid #30363d;
            border-radius: 4px;
        }
        QHeaderView::section {
            background-color: #21262d;
            color: #58a6ff;
            font-weight: bold;
            border: 1px solid #30363d;
            padding: 3px;
        }
        QRadioButton, QCheckBox {
            color: #c9d1d9;
            spacing: 6px;
        }
        QRadioButton::indicator:checked, QCheckBox::indicator:checked {
            background-color: #1f6feb;
            border: 1px solid #58a6ff;
        }
        QStatusBar {
            background-color: #161b22;
            color: #8b949e;
            border-top: 1px solid #30363d;
        }
    )";
    setStyleSheet(style);
}

// ----------------------------------------------------------------------------
// Quick Command Input Bar
// ----------------------------------------------------------------------------
void MainWindow::onRunQuickCommand() {
    QString raw = ui->txtQuickCommand->text().trimmed();
    if (raw.isEmpty()) return;

    QStringList parts = raw.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
    if (parts.isEmpty()) return;

    QString cmd = parts[0].toLower();
    bool firstIsNumber = false;
    double numVal = parts[0].toDouble(&firstIsNumber);

    // If user typed pure numbers, apply based on current active tab!
    if (firstIsNumber) {
        int currentTab = ui->tabTransforms->currentIndex();
        if (currentTab == 0) { // Translate
            int tx = static_cast<int>(numVal);
            int ty = (parts.size() >= 2) ? parts[1].toInt() : 0;
            ui->spinTx->setValue(tx);
            ui->spinTy->setValue(ty);
            onApplyTranslationClicked();
        } else if (currentTab == 1) { // Rotate
            ui->spinRotAngle->setValue(numVal);
            if (parts.size() >= 3) {
                ui->radioRotCustom->setChecked(true);
                ui->spinRotPx->setValue(parts[1].toInt());
                ui->spinRotPy->setValue(parts[2].toInt());
            }
            onApplyRotationClicked();
        } else if (currentTab == 2) { // Scale
            double sx = numVal;
            double sy = (parts.size() >= 2) ? parts[1].toDouble() : sx;
            ui->spinScaleX->setValue(sx);
            ui->spinScaleY->setValue(sy);
            if (parts.size() >= 4) {
                ui->radioScaleCustom->setChecked(true);
                ui->spinScalePx->setValue(parts[2].toInt());
                ui->spinScalePy->setValue(parts[3].toInt());
            }
            onApplyScalingClicked();
        } else if (currentTab == 3) { // Shear
            double shx = numVal;
            double shy = (parts.size() >= 2) ? parts[1].toDouble() : 0.0;
            ui->spinShearX->setValue(shx);
            ui->spinShearY->setValue(shy);
            onApplyShearClicked();
        } else if (currentTab == 4) { // Reflect
            double m = numVal;
            double c = (parts.size() >= 2) ? parts[1].toDouble() : 0.0;
            ui->spinReflectSlope->setValue(m);
            ui->spinReflectIntercept->setValue(c);
            onApplyReflectArbitraryClicked();
        }
        ui->txtQuickCommand->clear();
        return;
    }

    // Explicit command keywords
    if (cmd == "r" || cmd == "rot" || cmd == "rotate") {
        if (parts.size() >= 2) {
            double angle = parts[1].toDouble();
            ui->spinRotAngle->setValue(angle);
            if (parts.size() >= 4) {
                ui->radioRotCustom->setChecked(true);
                ui->spinRotPx->setValue(parts[2].toInt());
                ui->spinRotPy->setValue(parts[3].toInt());
            }
            onApplyRotationClicked();
        }
    } else if (cmd == "t" || cmd == "trans" || cmd == "translate") {
        if (parts.size() >= 3) {
            ui->spinTx->setValue(parts[1].toInt());
            ui->spinTy->setValue(parts[2].toInt());
            onApplyTranslationClicked();
        } else if (parts.size() == 2) {
            ui->spinTx->setValue(parts[1].toInt());
            ui->spinTy->setValue(0);
            onApplyTranslationClicked();
        }
    } else if (cmd == "s" || cmd == "scale") {
        if (parts.size() >= 2) {
            double sx = parts[1].toDouble();
            double sy = (parts.size() >= 3) ? parts[2].toDouble() : sx;
            ui->spinScaleX->setValue(sx);
            ui->spinScaleY->setValue(sy);
            if (parts.size() >= 5) {
                ui->radioScaleCustom->setChecked(true);
                ui->spinScalePx->setValue(parts[3].toInt());
                ui->spinScalePy->setValue(parts[4].toInt());
            }
            onApplyScalingClicked();
        }
    } else if (cmd == "sh" || cmd == "shear") {
        if (parts.size() >= 2) {
            double shx = parts[1].toDouble();
            double shy = (parts.size() >= 3) ? parts[2].toDouble() : 0.0;
            ui->spinShearX->setValue(shx);
            ui->spinShearY->setValue(shy);
            onApplyShearClicked();
        }
    } else if (cmd == "ref" || cmd == "reflect") {
        if (parts.size() >= 2) {
            QString refTarget = parts[1].toLower();
            if (refTarget == "x") onReflectXClicked();
            else if (refTarget == "y") onReflectYClicked();
            else if (refTarget == "orig" || refTarget == "origin" || refTarget == "0") onReflectOriginClicked();
            else if (refTarget == "y=x" || refTarget == "x=y") onReflectLineYeqXClicked();
            else if (refTarget == "y=-x" || refTarget == "-x") onReflectLineYeqNegXClicked();
            else if (parts.size() >= 3) {
                double m = parts[1].toDouble();
                double c = parts[2].toDouble();
                ui->spinReflectSlope->setValue(m);
                ui->spinReflectIntercept->setValue(c);
                onApplyReflectArbitraryClicked();
            }
        }
    } else if (cmd == "refx") {
        onReflectXClicked();
    } else if (cmd == "refy") {
        onReflectYClicked();
    } else if (cmd == "reforig" || cmd == "reforigin") {
        onReflectOriginClicked();
    } else if (cmd == "refline") {
        if (parts.size() >= 3) {
            double m = parts[1].toDouble();
            double c = parts[2].toDouble();
            ui->spinReflectSlope->setValue(m);
            ui->spinReflectIntercept->setValue(c);
            onApplyReflectArbitraryClicked();
        }
    } else if (cmd == "undo") {
        onUndoTransformClicked();
    } else if (cmd == "reset" || cmd == "i") {
        onResetTransformClicked();
    } else {
        statusBar()->showMessage(QString("Command unrecognized: '%1'. Try 'R 45', 'T 10 5', 'S 2', 'Ref X'").arg(raw), 4000);
        return;
    }

    ui->txtQuickCommand->clear();
}

// ----------------------------------------------------------------------------
// 1. Selection & Scope
// ----------------------------------------------------------------------------
void MainWindow::onSelectShapeComboChanged(int index) {
    if (index == 0) {
        m_canvas->deselectAll();
    } else {
        int targetId = ui->comboSelectShape->itemData(index).toInt();
        m_canvas->selectShape(targetId);
    }
}

void MainWindow::onDeselectAllClicked() {
    m_canvas->deselectAll();
    ui->comboSelectShape->setCurrentIndex(0);
}

void MainWindow::handleShapeSelectionChanged(int shapeId, const QString &description) {
    ui->lblScopeInfo->setText(description);

    for (int i = 0; i < ui->comboSelectShape->count(); ++i) {
        if (ui->comboSelectShape->itemData(i).toInt() == shapeId) {
            ui->comboSelectShape->blockSignals(true);
            ui->comboSelectShape->setCurrentIndex(i);
            ui->comboSelectShape->blockSignals(false);
            break;
        }
    }
}

void MainWindow::handleShapesChanged(int totalPolygons, int totalCircles) {
    Q_UNUSED(totalPolygons);
    Q_UNUSED(totalCircles);
    updateShapeDropdown();
}

void MainWindow::updateShapeDropdown() {
    ui->comboSelectShape->blockSignals(true);
    ui->comboSelectShape->clear();

    ui->comboSelectShape->addItem("🌐 Entire Space (All Shapes & Grid)", -1);

    for (const auto &poly : m_canvas->polygons()) {
        ui->comboSelectShape->addItem(QString("🎯 Polygon #%1 (%2 vertices)").arg(poly.id).arg(poly.vertices.size()), poly.id);
    }

    for (const auto &c : m_canvas->circles()) {
        ui->comboSelectShape->addItem(QString("🎯 Circle #%1 (Center (%2,%3), r=%4)").arg(c.id).arg(c.center.x()).arg(c.center.y()).arg(c.radius), c.id);
    }

    int curId = m_canvas->selectedShapeId();
    for (int i = 0; i < ui->comboSelectShape->count(); ++i) {
        if (ui->comboSelectShape->itemData(i).toInt() == curId) {
            ui->comboSelectShape->setCurrentIndex(i);
            break;
        }
    }
    ui->comboSelectShape->blockSignals(false);
}

// ----------------------------------------------------------------------------
// 2. Transformations Execution
// ----------------------------------------------------------------------------
void MainWindow::onApplyTranslationClicked() {
    ui->spinTx->interpretText();
    ui->spinTy->interpretText();

    int tx = ui->spinTx->value();
    int ty = ui->spinTy->value();
    Matrix3x3 mat = Matrix3x3::translation(tx, ty);

    QString name = QString("Translation T(%1, %2)").arg(tx).arg(ty);
    QString formula = QString("T(%1, %2)").arg(tx).arg(ty);
    QString details = QString("x' = x + %1,  y' = y + %2").arg(tx).arg(ty);

    m_canvas->applyTransformation(mat, name, formula, details);
}

void MainWindow::onNudgeUp() {
    Matrix3x3 mat = Matrix3x3::translation(0, 5);
    m_canvas->applyTransformation(mat, "Nudge +Y (+5)", "T(0, 5)", "y' = y + 5");
}

void MainWindow::onNudgeDown() {
    Matrix3x3 mat = Matrix3x3::translation(0, -5);
    m_canvas->applyTransformation(mat, "Nudge -Y (-5)", "T(0, -5)", "y' = y - 5");
}

void MainWindow::onNudgeLeft() {
    Matrix3x3 mat = Matrix3x3::translation(-5, 0);
    m_canvas->applyTransformation(mat, "Nudge -X (-5)", "T(-5, 0)", "x' = x - 5");
}

void MainWindow::onNudgeRight() {
    Matrix3x3 mat = Matrix3x3::translation(5, 0);
    m_canvas->applyTransformation(mat, "Nudge +X (+5)", "T(5, 0)", "x' = x + 5");
}

void MainWindow::onNudgeReset() {
    ui->spinTx->setValue(0);
    ui->spinTy->setValue(0);
}

// Rotation
void MainWindow::onApplyRotationClicked() {
    ui->spinRotAngle->interpretText();
    ui->spinRotPx->interpretText();
    ui->spinRotPy->interpretText();

    double angle = ui->spinRotAngle->value();
    Matrix3x3 mat;
    QString name, formula, details;

    if (ui->radioRotOrigin->isChecked()) {
        mat = Matrix3x3::rotation(angle);
        name = QString("Rotation R(%1°) wrt Origin (0,0)").arg(angle, 0, 'f', 1);
        formula = QString("R(%1°)").arg(angle, 0, 'f', 1);
        details = QString("x' = x·cos(%1°) - y·sin(%1°), y' = x·sin(%1°) + y·cos(%1°)").arg(angle, 0, 'f', 1);
    } else if (ui->radioRotCentroid->isChecked()) {
        QPoint c = m_canvas->getSelectedShapeCentroid();
        mat = Matrix3x3::rotationAboutPoint(angle, c.x(), c.y());
        name = QString("Rotation R(%1°) wrt Centroid (%2,%3)").arg(angle, 0, 'f', 1).arg(c.x()).arg(c.y());
        formula = QString("T(%2,%3) · R(%1°) · T(-%2,-%3)").arg(angle, 0, 'f', 1).arg(c.x()).arg(c.y());
        details = QString("Rotate %1° around Centroid (%2,%3)").arg(angle, 0, 'f', 1).arg(c.x()).arg(c.y());
    } else {
        int px = ui->spinRotPx->value();
        int py = ui->spinRotPy->value();
        mat = Matrix3x3::rotationAboutPoint(angle, px, py);
        name = QString("Rotation R(%1°) wrt Pivot (%2,%3)").arg(angle, 0, 'f', 1).arg(px).arg(py);
        formula = QString("T(%2,%3) · R(%1°) · T(-%2,-%3)").arg(angle, 0, 'f', 1).arg(px).arg(py);
        details = QString("Rotate %1° around Arbitrary Point (%2,%3)").arg(angle, 0, 'f', 1).arg(px).arg(py);
    }

    m_canvas->applyTransformation(mat, name, formula, details);
}

void MainWindow::onPickPivotClicked() {
    m_canvas->setFillMode(Canvas::ModeSetPivot);
    statusBar()->showMessage("Click any point on the canvas to set the Rotation Pivot Point.", 5000);
}

void MainWindow::onQuickRot45() {
    ui->spinRotAngle->setValue(45.0);
    onApplyRotationClicked();
}

void MainWindow::onQuickRotNeg45() {
    ui->spinRotAngle->setValue(-45.0);
    onApplyRotationClicked();
}

void MainWindow::onQuickRot90() {
    ui->spinRotAngle->setValue(90.0);
    onApplyRotationClicked();
}

void MainWindow::onQuickRot180() {
    ui->spinRotAngle->setValue(180.0);
    onApplyRotationClicked();
}

// Scaling
void MainWindow::onUniformScaleToggled(bool checked) {
    if (checked) {
        ui->spinScaleY->setValue(ui->spinScaleX->value());
    }
}

void MainWindow::onScaleXChanged(double val) {
    if (ui->chkUniformScale->isChecked()) {
        ui->spinScaleY->setValue(val);
    }
}

void MainWindow::onApplyScalingClicked() {
    ui->spinScaleX->interpretText();
    ui->spinScaleY->interpretText();
    ui->spinScalePx->interpretText();
    ui->spinScalePy->interpretText();

    double sx = ui->spinScaleX->value();
    double sy = ui->chkUniformScale->isChecked() ? sx : ui->spinScaleY->value();

    Matrix3x3 mat;
    QString name, formula, details;

    if (ui->radioScaleOrigin->isChecked()) {
        mat = Matrix3x3::scaling(sx, sy);
        name = QString("Scaling S(%1, %2) wrt Origin").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2);
        formula = QString("S(%1, %2)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2);
        details = QString("x' = %1·x,  y' = %2·y").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2);
    } else if (ui->radioScaleCentroid->isChecked()) {
        QPoint c = m_canvas->getSelectedShapeCentroid();
        mat = Matrix3x3::scalingAboutPoint(sx, sy, c.x(), c.y());
        name = QString("Scaling S(%1, %2) wrt Centroid (%3,%4)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2).arg(c.x()).arg(c.y());
        formula = QString("T(%3,%4) · S(%1,%2) · T(-%3,-%4)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2).arg(c.x()).arg(c.y());
        details = QString("Local scaling about Centroid (%1,%2)").arg(c.x()).arg(c.y());
    } else {
        int fx = ui->spinScalePx->value();
        int fy = ui->spinScalePy->value();
        mat = Matrix3x3::scalingAboutPoint(sx, sy, fx, fy);
        name = QString("Scaling S(%1, %2) wrt Fixed Point (%3,%4)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2).arg(fx).arg(fy);
        formula = QString("T(%3,%4) · S(%1,%2) · T(-%3,-%4)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2).arg(fx).arg(fy);
        details = QString("Fixed point scaling about (%1,%2)").arg(fx).arg(fy);
    }

    m_canvas->applyTransformation(mat, name, formula, details);
}

void MainWindow::onQuickScale2() {
    ui->spinScaleX->setValue(2.0);
    ui->spinScaleY->setValue(2.0);
    onApplyScalingClicked();
}

void MainWindow::onQuickScaleHalf() {
    ui->spinScaleX->setValue(0.5);
    ui->spinScaleY->setValue(0.5);
    onApplyScalingClicked();
}

void MainWindow::onQuickScale15() {
    ui->spinScaleX->setValue(1.5);
    ui->spinScaleY->setValue(1.5);
    onApplyScalingClicked();
}

void MainWindow::onQuickScaleFlip() {
    ui->spinScaleX->setValue(-1.0);
    ui->spinScaleY->setValue(1.0);
    onApplyScalingClicked();
}

// Shearing
void MainWindow::onApplyShearClicked() {
    ui->spinShearX->interpretText();
    ui->spinShearY->interpretText();
    ui->spinShearYRef->interpretText();
    ui->spinShearXRef->interpretText();

    double shx = ui->spinShearX->value();
    double shy = ui->spinShearY->value();
    int yref = ui->spinShearYRef->value();
    int xref = ui->spinShearXRef->value();

    Matrix3x3 mat = Matrix3x3::shearX(shx, yref) * Matrix3x3::shearY(shy, xref);
    QString name = QString("Shear Sh(x=%1, y=%2)").arg(shx, 0, 'f', 2).arg(shy, 0, 'f', 2);
    QString formula = QString("Sh_x(%1) · Sh_y(%2)").arg(shx, 0, 'f', 2).arg(shy, 0, 'f', 2);
    QString details = QString("x' = x + %1(y - %2), y' = y + %3(x - %4)").arg(shx, 0, 'f', 2).arg(yref).arg(shy, 0, 'f', 2).arg(xref);

    m_canvas->applyTransformation(mat, name, formula, details);
}

// Reflections
void MainWindow::onReflectXClicked() {
    Matrix3x3 mat = Matrix3x3::reflectionX();
    m_canvas->applyTransformation(mat, "Reflection wrt X-Axis (y=0)", "Ref_x", "x' = x,  y' = -y");
}

void MainWindow::onReflectYClicked() {
    Matrix3x3 mat = Matrix3x3::reflectionY();
    m_canvas->applyTransformation(mat, "Reflection wrt Y-Axis (x=0)", "Ref_y", "x' = -x,  y' = y");
}

void MainWindow::onReflectOriginClicked() {
    Matrix3x3 mat = Matrix3x3::reflectionOrigin();
    m_canvas->applyTransformation(mat, "Reflection wrt Origin (0,0)", "Ref_origin", "x' = -x,  y' = -y");
}

void MainWindow::onReflectLineYeqXClicked() {
    Matrix3x3 mat = Matrix3x3::reflectionLineYeqX();
    m_canvas->applyTransformation(mat, "Reflection wrt Line y = x", "Ref_y=x", "x' = y,  y' = x");
}

void MainWindow::onReflectLineYeqNegXClicked() {
    Matrix3x3 mat = Matrix3x3::reflectionLineYeqNegX();
    m_canvas->applyTransformation(mat, "Reflection wrt Line y = -x", "Ref_y=-x", "x' = -y,  y' = -x");
}

void MainWindow::onPickReflectLineClicked() {
    m_canvas->setFillMode(Canvas::ModeSetReflectLine);
    statusBar()->showMessage("Click two points on canvas to define the Arbitrary Reflection Line.", 5000);
}

void MainWindow::onApplyReflectArbitraryClicked() {
    ui->spinReflectSlope->interpretText();
    ui->spinReflectIntercept->interpretText();

    double m = ui->spinReflectSlope->value();
    double c = ui->spinReflectIntercept->value();

    Matrix3x3 mat = Matrix3x3::reflectionArbitraryLine(m, c);
    QString name = QString("Reflection Across Line y = %1x + %2").arg(m, 0, 'f', 2).arg(c, 0, 'f', 2);
    QString formula = QString("T(0,%2) · R(θ) · Ref_x · R(-θ) · T(0,-%2)").arg(c, 0, 'f', 2);
    QString details = QString("θ = atan(%1) = %2°").arg(m, 0, 'f', 2).arg(std::atan(m) * 180.0 / M_PI, 0, 'f', 1);

    m_canvas->setReflectLine(m, c);
    m_canvas->applyTransformation(mat, name, formula, details);
}

// Custom 3x3 Matrix Input
void MainWindow::onApplyCustomMatrixClicked() {
    ui->spinM00->interpretText(); ui->spinM01->interpretText(); ui->spinM02->interpretText();
    ui->spinM10->interpretText(); ui->spinM11->interpretText(); ui->spinM12->interpretText();
    ui->spinM20->interpretText(); ui->spinM21->interpretText(); ui->spinM22->interpretText();

    double m00 = ui->spinM00->value();
    double m01 = ui->spinM01->value();
    double m02 = ui->spinM02->value();

    double m10 = ui->spinM10->value();
    double m11 = ui->spinM11->value();
    double m12 = ui->spinM12->value();

    double m20 = ui->spinM20->value();
    double m21 = ui->spinM21->value();
    double m22 = ui->spinM22->value();

    Matrix3x3 customMat(m00, m01, m02,
                        m10, m11, m12,
                        m20, m21, m22);

    QString name = "Custom 3x3 Matrix Transformation";
    QString formula = customMat.toString();
    QString details = QString("Det(M) = %1").arg(customMat.determinant(), 0, 'f', 3);

    m_canvas->applyTransformation(customMat, name, formula, details);
}

// ----------------------------------------------------------------------------
// Matrix Inspector & History
// ----------------------------------------------------------------------------
void MainWindow::onUndoTransformClicked() {
    m_canvas->undoLastTransformation();
    updateMatrixInspector();
}

void MainWindow::onResetTransformClicked() {
    m_canvas->resetTransformations();
    updateMatrixInspector();
}

void MainWindow::handleTransformationApplied(const Matrix3x3::TransformStep &step, const Matrix3x3 &composite) {
    Q_UNUSED(step);
    Q_UNUSED(composite);
    updateMatrixInspector();
}

void MainWindow::handleTransformationsReset() {
    updateMatrixInspector();
}

void MainWindow::updateMatrixInspector() {
    const Matrix3x3 &comp = m_canvas->compositeMatrix();

    ui->txtMatrixDisplay->setHtml(comp.toHtmlTable());
    ui->lblMatrixDet->setText(QString("Det(M): %1").arg(comp.determinant(), 0, 'f', 3));

    const auto &history = m_canvas->transformHistory();
    ui->lblMatrixFormula->setText(history.isEmpty() ? "M_composite = I (Identity)" :
                                      QString("M_composite = %1 steps chained").arg(history.size()));

    ui->tableHistory->setRowCount(history.size());
    for (int i = 0; i < history.size(); ++i) {
        const auto &step = history[i];
        ui->tableHistory->setItem(i, 0, new QTableWidgetItem(QString("#%1").arg(i + 1)));
        ui->tableHistory->setItem(i, 1, new QTableWidgetItem(step.name));
        ui->tableHistory->setItem(i, 2, new QTableWidgetItem(step.details.isEmpty() ? step.formula : step.details));
    }
    ui->tableHistory->scrollToBottom();

    // Only sync Custom Matrix tab spinboxes if the user is NOT currently editing that tab
    if (ui->tabTransforms->currentIndex() != 5) {
        ui->spinM00->setValue(comp.m[0][0]);
        ui->spinM01->setValue(comp.m[0][1]);
        ui->spinM02->setValue(comp.m[0][2]);
        ui->spinM10->setValue(comp.m[1][0]);
        ui->spinM11->setValue(comp.m[1][1]);
        ui->spinM12->setValue(comp.m[1][2]);
        ui->spinM20->setValue(comp.m[2][0]);
        ui->spinM21->setValue(comp.m[2][1]);
        ui->spinM22->setValue(comp.m[2][2]);
    }
}

// ----------------------------------------------------------------------------
// 3. Fills
// ----------------------------------------------------------------------------
void MainWindow::onFillAlgorithmChanged(int index) {
    if (index == 0) {
        m_canvas->setFillMode(Canvas::ModeScanlineFill);
    } else {
        m_canvas->setFillMode(Canvas::ModeBoundaryFill8);
    }
}

void MainWindow::onExecuteFillClicked() {
    m_canvas->triggerFill();
}

void MainWindow::onClearFillClicked() {
    m_canvas->clearFillOnly();
}

// ----------------------------------------------------------------------------
// 4. Shape Tools & Presets
// ----------------------------------------------------------------------------
void MainWindow::onDrawPolygonClicked() {
    m_canvas->setFillMode(Canvas::ModeDrawPolygon);
    statusBar()->showMessage("Click canvas to place polygon vertices. Right-click or 'Close Poly' when done.", 5000);
}

void MainWindow::onDrawCircleClicked() {
    m_canvas->setFillMode(Canvas::ModeDrawCircle);
    statusBar()->showMessage("Click canvas for Center, then click outer point for Radius.", 5000);
}

void MainWindow::onClosePolygonClicked() {
    m_canvas->closePolygon();
}

void MainWindow::onUndoShapeClicked() {
    m_canvas->undoLastShape();
}

void MainWindow::onPresetTriangleClicked() {
    m_canvas->setPolygonPreset(CGAlgorithms::getTrianglePreset());
}

void MainWindow::onPresetConvexClicked() {
    m_canvas->setPolygonPreset(CGAlgorithms::getConvexPolygonPreset());
}

void MainWindow::onPresetHouseClicked() {
    m_canvas->setPolygonPreset(CGAlgorithms::getHousePreset());
}

void MainWindow::onPresetStarClicked() {
    m_canvas->setPolygonPreset(CGAlgorithms::getStarPolygonPreset());
}

void MainWindow::onPresetConcaveClicked() {
    m_canvas->setPolygonPreset(CGAlgorithms::getConcaveArrowPreset());
}

void MainWindow::onPresetNestedClicked() {
    m_canvas->setNestedPolygonPreset(CGAlgorithms::getNestedOuterPreset(), CGAlgorithms::getNestedInnerPreset());
}

void MainWindow::onPresetMultiSceneClicked() {
    m_canvas->setMultiShapeScenePreset();
}

// ----------------------------------------------------------------------------
// 5. Display & Overlays
// ----------------------------------------------------------------------------
void MainWindow::onGridSizeChanged(int size) { m_canvas->setGridSize(size); }
void MainWindow::onToggleShowGhost(bool checked) { m_canvas->setShowGhost(checked); }
void MainWindow::onToggleShowPivot(bool checked) { m_canvas->setShowRotationPivot(checked); }
void MainWindow::onToggleShowReflectLine(bool checked) { m_canvas->setShowReflectLine(checked); }
void MainWindow::onToggleShowLabels(bool checked) { m_canvas->setShowVertexLabels(checked); }
void MainWindow::onToggleShowGrid(bool checked) { m_canvas->setShowGrid(checked); }
void MainWindow::onToggleShowAxes(bool checked) { m_canvas->setShowAxes(checked); }
void MainWindow::onClearAllClicked() { m_canvas->clearAll(); }

// Canvas Signals
void MainWindow::handleMouseMoved(int logX, int logY, int scrX, int scrY) {
    Q_UNUSED(scrX);
    Q_UNUSED(scrY);
    statusBar()->showMessage(QString("Cursor: (%1, %2) | Scope: %3")
                                 .arg(logX).arg(logY).arg(m_canvas->getSelectedShapeName()), 2000);
}

void MainWindow::handlePivotChanged(QPoint pivot) {
    ui->spinRotPx->setValue(pivot.x());
    ui->spinRotPy->setValue(pivot.y());
}

void MainWindow::handleReflectLineChanged(double slope, double intercept) {
    ui->spinReflectSlope->setValue(slope);
    ui->spinReflectIntercept->setValue(intercept);
}

void MainWindow::handleFillFinished(const QString &algName, qint64 elapsedNanos, int pointCount) {
    statusBar()->showMessage(QString("%1 completed: %2 pixels filled in %3 ns")
                                 .arg(algName).arg(pointCount).arg(elapsedNanos), 5000);
}

void MainWindow::handleStatusMessage(const QString &msg) {
    statusBar()->showMessage(msg, 4000);
}
