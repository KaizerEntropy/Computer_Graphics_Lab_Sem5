#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFormLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QRegularExpression>
#include <QButtonGroup>
#include <QApplication>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Assignment 6 - Clipping (Line & Polygon)");
    resize(1440, 900);
    setMinimumSize(800, 500);

    setupDarkTheme();
    buildUI();
    setupConnections();
    setupSpinBoxes();
    setupReturnKeyHooks();

    // Default: load convex polygon preset
    onPresetConvexClicked();
    updateMatrixInspector();
}

MainWindow::~MainWindow() {
}

// ============================================================================
// Dark Theme
// ============================================================================
void MainWindow::setupDarkTheme() {
    QString style = R"(
        QMainWindow, QWidget {
            background-color: #f0f2f5;
            color: #2c3e50;
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            font-size: 14px;
        }
        QGroupBox {
            font-weight: bold;
            border: 2px solid #bdc3c7;
            border-radius: 8px;
            margin-top: 12px;
            padding-top: 18px;
            background-color: #ffffff;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 15px;
            padding: 0 5px;
            color: #2980b9;
        }
        QPushButton {
            background-color: #3498db;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 8px 16px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #2980b9;
        }
        QPushButton:pressed {
            background-color: #21618c;
        }
        QTextBrowser {
            background-color: #ecf0f1;
            border: 1px solid #bdc3c7;
            border-radius: 4px;
            color: #2c3e50;
        }
        QSpinBox, QComboBox {
            border: 1px solid #bdc3c7;
            border-radius: 3px;
            padding: 4px;
            background-color: #ffffff;
        }
    )";
    qApp->setStyleSheet(style);
}
void MainWindow::buildUI() {
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    
    // Canvas on the left
    m_canvas = new Canvas(this);
    m_canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(m_canvas, 3);
    
    // Controls on the right
    QScrollArea *controlScroll = new QScrollArea();
    controlScroll->setWidgetResizable(true);
    controlScroll->setMinimumWidth(400);
    controlScroll->setMaximumWidth(450);
    
    QWidget *controlWidget = new QWidget();
    QVBoxLayout *controlLay = new QVBoxLayout(controlWidget);
    controlLay->setSpacing(15);
    
    // --- Window Coordinates Group ---
    QGroupBox *grpWindow = new QGroupBox("1. Clip Window Settings");
    QGridLayout *winLay = new QGridLayout(grpWindow);
    
    winLay->addWidget(new QLabel("X Min:"), 0, 0);
    spinClipXmin = new QSpinBox(); spinClipXmin->setRange(-500, 500);
    winLay->addWidget(spinClipXmin, 0, 1);
    
    winLay->addWidget(new QLabel("Y Min:"), 0, 2);
    spinClipYmin = new QSpinBox(); spinClipYmin->setRange(-500, 500);
    winLay->addWidget(spinClipYmin, 0, 3);
    
    winLay->addWidget(new QLabel("X Max:"), 1, 0);
    spinClipXmax = new QSpinBox(); spinClipXmax->setRange(-500, 500);
    winLay->addWidget(spinClipXmax, 1, 1);
    
    winLay->addWidget(new QLabel("Y Max:"), 1, 2);
    spinClipYmax = new QSpinBox(); spinClipYmax->setRange(-500, 500);
    winLay->addWidget(spinClipYmax, 1, 3);
    
    btnDragClipRegion = new QPushButton("Interact / Resize Region on Canvas");
    winLay->addWidget(btnDragClipRegion, 2, 0, 1, 4);
    controlLay->addWidget(grpWindow);
    
    // --- Line Clipping Group ---
    QGroupBox *grpLineClip = new QGroupBox("2. Line Clipping Algorithms");
    QVBoxLayout *lineLay = new QVBoxLayout(grpLineClip);
    
    QHBoxLayout *lineBtns = new QHBoxLayout();
    btnRunCS = new QPushButton("Cohen-Sutherland");
    btnRunLB = new QPushButton("Liang-Barsky");
    lineBtns->addWidget(btnRunCS);
    lineBtns->addWidget(btnRunLB);
    lineLay->addLayout(lineBtns);
    
    btnLoadCSTest = new QPushButton("Load Complex Line Test Set");
    lineLay->addWidget(btnLoadCSTest);
    
    txtCSInspector = new QTextBrowser();
    txtCSInspector->setMinimumHeight(150);
    lineLay->addWidget(txtCSInspector);
    controlLay->addWidget(grpLineClip);
    
    // --- Polygon Clipping Group ---
    QGroupBox *grpPolyClip = new QGroupBox("3. Polygon Clipping Algorithms");
    QVBoxLayout *polyLay = new QVBoxLayout(grpPolyClip);
    
    QHBoxLayout *polyBtns = new QHBoxLayout();
    btnRunSH = new QPushButton("Sutherland-Hodgman");
    btnRunWA = new QPushButton("Weiler-Atherton");
    polyBtns->addWidget(btnRunSH);
    polyBtns->addWidget(btnRunWA);
    polyLay->addLayout(polyBtns);
    
    QGroupBox *grpTestPolys = new QGroupBox("Load Test Polygons");
    QHBoxLayout *testPolyLay = new QHBoxLayout(grpTestPolys);
    btnLoadSHTriangle = new QPushButton("Triangle");
    btnLoadSHStar = new QPushButton("Star");
    btnLoadSHLShape = new QPushButton("L-Shape");
    btnLoadSHNepal = new QPushButton("Concave");
    testPolyLay->addWidget(btnLoadSHTriangle);
    testPolyLay->addWidget(btnLoadSHStar);
    testPolyLay->addWidget(btnLoadSHLShape);
    testPolyLay->addWidget(btnLoadSHNepal);
    polyLay->addWidget(grpTestPolys);
    
    txtSHInspector = new QTextBrowser();
    txtSHInspector->setMinimumHeight(150);
    polyLay->addWidget(txtSHInspector);
    controlLay->addWidget(grpPolyClip);
    
    // Checkboxes / Options
    QGroupBox *grpOptions = new QGroupBox("4. View Options");
    QVBoxLayout *optLay = new QVBoxLayout(grpOptions);
    chkShowClipWindow = new QCheckBox("Show Clipping Rectangle");
    chkShowOutcodeLabels = new QCheckBox("Show Line Outcodes");
    chkShowClipResult = new QCheckBox("Render Clipped Output");
    chkSplitDisjointSH = new QCheckBox("Separate Disjoint Polygons (SH Extension)");
    
    optLay->addWidget(chkShowClipWindow);
    optLay->addWidget(chkShowOutcodeLabels);
    optLay->addWidget(chkShowClipResult);
    optLay->addWidget(chkSplitDisjointSH);
    controlLay->addWidget(grpOptions);
    btnClearAll = new QPushButton("Clear All");
    btnClearAll->setStyleSheet("background-color: #e74c3c; color: white; padding: 10px; font-weight: bold; border-radius: 4px;");
    controlLay->addWidget(btnClearAll);
    
    controlLay->addStretch();
    controlWidget->setLayout(controlLay);
    controlScroll->setWidget(controlWidget);
    
    mainLayout->addWidget(controlScroll, 1);
    
    // We instantiate variables that the rest of the code expects, even if hidden, to avoid crashes.
    // The previous implementation bound a lot of unused UI elements.
    // For safety, we will just create them invisibly.
    txtQuickCommand = new QLineEdit(); btnRunQuickCommand = new QPushButton();
    tabTransforms = new QTabWidget(); m_mainTabs = new QTabWidget(); m_sidebarScroll = new QScrollArea(); m_splitter = new QSplitter();
    comboSelectShape = new QComboBox(); btnDeselectAll = new QPushButton(); lblScopeInfo = new QLabel();
    spinTx = new QSpinBox(); spinTy = new QSpinBox(); btnApplyTranslate = new QPushButton();
    btnNudgeUp = new QPushButton(); btnNudgeDown = new QPushButton(); btnNudgeLeft = new QPushButton(); btnNudgeRight = new QPushButton(); btnNudgeReset = new QPushButton();
    spinRotAngle = new QDoubleSpinBox(); spinRotPx = new QSpinBox(); spinRotPy = new QSpinBox();
    radioRotOrigin = new QRadioButton(); radioRotCentroid = new QRadioButton(); radioRotCustom = new QRadioButton();
    btnApplyRotate = new QPushButton(); btnPickPivot = new QPushButton();
    btnQuickRot45 = new QPushButton(); btnQuickRotNeg45 = new QPushButton(); btnQuickRot90 = new QPushButton(); btnQuickRot180 = new QPushButton();
    spinScaleX = new QDoubleSpinBox(); spinScaleY = new QDoubleSpinBox(); spinScalePx = new QSpinBox(); spinScalePy = new QSpinBox();
    radioScaleOrigin = new QRadioButton(); radioScaleCentroid = new QRadioButton(); radioScaleCustom = new QRadioButton();
    chkUniformScale = new QCheckBox(); btnApplyScale = new QPushButton();
    btnQuickScale2 = new QPushButton(); btnQuickScaleHalf = new QPushButton(); btnQuickScale15 = new QPushButton(); btnQuickScaleFlip = new QPushButton();
    spinShearX = new QDoubleSpinBox(); spinShearY = new QDoubleSpinBox(); spinShearYRef = new QSpinBox(); spinShearXRef = new QSpinBox(); btnApplyShear = new QPushButton();
    spinReflectSlope = new QDoubleSpinBox(); spinReflectIntercept = new QDoubleSpinBox();
    btnReflectX = new QPushButton(); btnReflectY = new QPushButton(); btnReflectOrigin = new QPushButton(); btnReflectLineYeqX = new QPushButton(); btnReflectLineYeqNegX = new QPushButton();
    btnPickReflectLine = new QPushButton(); btnApplyReflectArbitrary = new QPushButton();
    spinM00 = new QDoubleSpinBox(); spinM01 = new QDoubleSpinBox(); spinM02 = new QDoubleSpinBox(); spinM10 = new QDoubleSpinBox(); spinM11 = new QDoubleSpinBox(); spinM12 = new QDoubleSpinBox(); spinM20 = new QDoubleSpinBox(); spinM21 = new QDoubleSpinBox(); spinM22 = new QDoubleSpinBox(); btnApplyCustomMatrix = new QPushButton();
    txtMatrixDisplay = new QTextBrowser(); lblMatrixDet = new QLabel(); lblMatrixFormula = new QLabel(); tableHistory = new QTableWidget(); btnUndoTransform = new QPushButton(); btnResetTransform = new QPushButton();
    comboFillAlgorithm = new QComboBox(); btnExecuteFill = new QPushButton(); btnClearFill = new QPushButton();
    btnDrawPolygon = new QPushButton(); btnDrawCircle = new QPushButton(); btnDrawLine = new QPushButton(); btnClosePolygon = new QPushButton(); btnUndoShape = new QPushButton();
    btnPresetTriangle = new QPushButton(); btnPresetConvex = new QPushButton(); btnPresetHouse = new QPushButton(); btnPresetStar = new QPushButton(); btnPresetConcave = new QPushButton(); btnPresetNested = new QPushButton(); btnPresetMultiScene = new QPushButton();
    spinGridSize = new QSpinBox(); chkShowGhost = new QCheckBox(); chkShowPivot = new QCheckBox(); chkShowReflectLine = new QCheckBox(); chkShowLabels = new QCheckBox(); chkShowGrid = new QCheckBox(); chkShowAxes = new QCheckBox();
    btnClipCentered = new QPushButton(); btnClipWide = new QPushButton(); btnClipTall = new QPushButton(); btnClipQuadrant = new QPushButton(); btnClipSmall = new QPushButton();
    sliderSHStage = new QSlider(); lblSHStage = new QLabel();
    btnResetView = new QPushButton(); btnFitToView = new QPushButton();
}
QWidget* MainWindow::buildClippingPanel() {
    QWidget *page = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(6);
    lay->setContentsMargins(6, 6, 6, 6);

    // --- Clip Region ---
    QGroupBox *grpRegion = new QGroupBox("📐 Clipping Window (Adaptive Region)");
    QGridLayout *regionLay = new QGridLayout(grpRegion);
    regionLay->setSpacing(4);

    regionLay->addWidget(new QLabel("X_min:"), 0, 0);
    spinClipXmin = new QSpinBox(); regionLay->addWidget(spinClipXmin, 0, 1);
    regionLay->addWidget(new QLabel("Y_min:"), 0, 2);
    spinClipYmin = new QSpinBox(); regionLay->addWidget(spinClipYmin, 0, 3);
    regionLay->addWidget(new QLabel("X_max:"), 1, 0);
    spinClipXmax = new QSpinBox(); regionLay->addWidget(spinClipXmax, 1, 1);
    regionLay->addWidget(new QLabel("Y_max:"), 1, 2);
    spinClipYmax = new QSpinBox(); regionLay->addWidget(spinClipYmax, 1, 3);

    QHBoxLayout *presetRow = new QHBoxLayout();
    btnClipCentered = new QPushButton("Centered"); presetRow->addWidget(btnClipCentered);
    btnClipWide = new QPushButton("Wide"); presetRow->addWidget(btnClipWide);
    btnClipTall = new QPushButton("Tall"); presetRow->addWidget(btnClipTall);
    btnClipQuadrant = new QPushButton("Quadrant"); presetRow->addWidget(btnClipQuadrant);
    btnClipSmall = new QPushButton("Small"); presetRow->addWidget(btnClipSmall);
    regionLay->addLayout(presetRow, 2, 0, 1, 4);

    btnDragClipRegion = new QPushButton("🖱️ Drag / Resize Clip Region on Canvas");
    regionLay->addWidget(btnDragClipRegion, 3, 0, 1, 4);

    lay->addWidget(grpRegion);

    // --- Cohen-Sutherland ---
    QGroupBox *grpCS = new QGroupBox("📏 Cohen-Sutherland Line Clipping");
    QVBoxLayout *csLay = new QVBoxLayout(grpCS);

    QHBoxLayout *csActions = new QHBoxLayout();
    btnRunCS = new QPushButton("▶ CS Clip"); csActions->addWidget(btnRunCS);
    btnRunLB = new QPushButton("▶ LB Clip"); csActions->addWidget(btnRunLB);
    btnLoadCSTest = new QPushButton("📝 Load Test Lines"); csActions->addWidget(btnLoadCSTest);
    csLay->addLayout(csActions);

    txtCSInspector = new QTextBrowser();
    txtCSInspector->setMaximumHeight(180);
    txtCSInspector->setPlaceholderText("Cohen-Sutherland outcode trace will appear here...");
    csLay->addWidget(txtCSInspector);
    lay->addWidget(grpCS);

    // --- Sutherland-Hodgman ---
    QGroupBox *grpSH = new QGroupBox("🔺 Sutherland-Hodgman Polygon Clipping");
    QVBoxLayout *shLay = new QVBoxLayout(grpSH);

    QHBoxLayout *shActions = new QHBoxLayout();
    btnRunSH = new QPushButton("▶ SH Clip"); shActions->addWidget(btnRunSH);
    btnRunWA = new QPushButton("▶ WA Clip"); shActions->addWidget(btnRunWA);
    btnLoadSHTriangle = new QPushButton("△"); shActions->addWidget(btnLoadSHTriangle);
    btnLoadSHStar = new QPushButton("★"); shActions->addWidget(btnLoadSHStar);
    btnLoadSHLShape = new QPushButton("⌐"); shActions->addWidget(btnLoadSHLShape);
    btnLoadSHNepal = new QPushButton("🇳🇵 Nepal");
    btnLoadSHNepal->setToolTip("Load Nepal Flag (Concave - Demonstrates Disjoint Separation)");
    shActions->addWidget(btnLoadSHNepal);
    shLay->addLayout(shActions);

    chkSplitDisjointSH = new QCheckBox("Separate Disjoint Polygons (Nepal Flag fix)");
    chkSplitDisjointSH->setChecked(m_canvas->splitDisjointSH());
    chkSplitDisjointSH->setToolTip("Decompose multi-component clipped outputs into separate distinct polygons instead of a single bridged loop");
    shLay->addWidget(chkSplitDisjointSH);

    QHBoxLayout *stageRow = new QHBoxLayout();
    stageRow->addWidget(new QLabel("Stage:"));
    sliderSHStage = new QSlider(Qt::Horizontal);
    sliderSHStage->setRange(0, 4);
    sliderSHStage->setValue(4);
    sliderSHStage->setTickPosition(QSlider::TicksBelow);
    sliderSHStage->setTickInterval(1);
    stageRow->addWidget(sliderSHStage);
    lblSHStage = new QLabel("Stage 4 (Final)");
    stageRow->addWidget(lblSHStage);
    shLay->addLayout(stageRow);

    txtSHInspector = new QTextBrowser();
    txtSHInspector->setMaximumHeight(180);
    txtSHInspector->setPlaceholderText("Sutherland-Hodgman stage trace will appear here...");
    shLay->addWidget(txtSHInspector);
    lay->addWidget(grpSH);

    // --- Display Options for Clipping ---
    QGroupBox *grpClipDisplay = new QGroupBox("👁️ Clip Display");
    QVBoxLayout *cdLay = new QVBoxLayout(grpClipDisplay);
    chkShowClipWindow = new QCheckBox("Show Clip Window"); chkShowClipWindow->setChecked(true);
    chkShowOutcodeLabels = new QCheckBox("Show Outcode Labels (9 Regions)"); chkShowOutcodeLabels->setChecked(true);
    chkShowClipResult = new QCheckBox("Show Clipped Result"); chkShowClipResult->setChecked(true);
    cdLay->addWidget(chkShowClipWindow);
    cdLay->addWidget(chkShowOutcodeLabels);
    cdLay->addWidget(chkShowClipResult);

    QHBoxLayout *viewRow = new QHBoxLayout();
    btnResetView = new QPushButton("🏠 Reset View"); viewRow->addWidget(btnResetView);
    btnFitToView = new QPushButton("🔍 Fit to View"); viewRow->addWidget(btnFitToView);
    cdLay->addLayout(viewRow);
    lay->addWidget(grpClipDisplay);

    lay->addStretch(1);
    return page;
}

// ============================================================================
// Transform Panel (inherits from old project)
// ============================================================================
QWidget* MainWindow::buildTransformPanel() {
    QWidget *page = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(6);
    lay->setContentsMargins(6, 6, 6, 6);

    // --- Selection ---
    QGroupBox *grpSel = new QGroupBox("🎯 Scope & Selection");
    QVBoxLayout *selLay = new QVBoxLayout(grpSel);
    comboSelectShape = new QComboBox();
    btnDeselectAll = new QPushButton("Deselect All");
    lblScopeInfo = new QLabel("🌐 Entire Space");
    selLay->addWidget(comboSelectShape);
    selLay->addWidget(lblScopeInfo);
    selLay->addWidget(btnDeselectAll);
    lay->addWidget(grpSel);

    // --- Transform Tabs ---
    tabTransforms = new QTabWidget();

    // Tab 0: Translation
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tl = new QVBoxLayout(tab);
        QFormLayout *f = new QFormLayout();
        spinTx = new QSpinBox(); spinTy = new QSpinBox();
        f->addRow("Tx:", spinTx);
        f->addRow("Ty:", spinTy);
        tl->addLayout(f);
        btnApplyTranslate = new QPushButton("▶ Apply Translation");
        tl->addWidget(btnApplyTranslate);

        QHBoxLayout *nudge = new QHBoxLayout();
        btnNudgeUp = new QPushButton("↑"); btnNudgeDown = new QPushButton("↓");
        btnNudgeLeft = new QPushButton("←"); btnNudgeRight = new QPushButton("→");
        btnNudgeReset = new QPushButton("⊙");
        nudge->addWidget(btnNudgeLeft); nudge->addWidget(btnNudgeUp);
        nudge->addWidget(btnNudgeDown); nudge->addWidget(btnNudgeRight);
        nudge->addWidget(btnNudgeReset);
        tl->addLayout(nudge);
        tabTransforms->addTab(tab, "T");
    }

    // Tab 1: Rotation
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tl = new QVBoxLayout(tab);
        QFormLayout *f = new QFormLayout();
        spinRotAngle = new QDoubleSpinBox();
        spinRotPx = new QSpinBox(); spinRotPy = new QSpinBox();
        f->addRow("θ (degrees):", spinRotAngle);
        tl->addLayout(f);

        radioRotOrigin = new QRadioButton("About Origin (0,0)"); radioRotOrigin->setChecked(true);
        radioRotCentroid = new QRadioButton("About Centroid");
        radioRotCustom = new QRadioButton("About Pivot:");
        tl->addWidget(radioRotOrigin);
        tl->addWidget(radioRotCentroid);
        QHBoxLayout *pivRow = new QHBoxLayout();
        pivRow->addWidget(radioRotCustom);
        pivRow->addWidget(new QLabel("Px:")); pivRow->addWidget(spinRotPx);
        pivRow->addWidget(new QLabel("Py:")); pivRow->addWidget(spinRotPy);
        tl->addLayout(pivRow);

        QButtonGroup *rotGrp = new QButtonGroup(tab);
        rotGrp->addButton(radioRotOrigin); rotGrp->addButton(radioRotCentroid); rotGrp->addButton(radioRotCustom);

        btnApplyRotate = new QPushButton("▶ Apply Rotation");
        btnPickPivot = new QPushButton("🎯 Pick Pivot");
        tl->addWidget(btnApplyRotate);
        tl->addWidget(btnPickPivot);

        QHBoxLayout *qr = new QHBoxLayout();
        btnQuickRot45 = new QPushButton("+45°"); btnQuickRotNeg45 = new QPushButton("-45°");
        btnQuickRot90 = new QPushButton("+90°"); btnQuickRot180 = new QPushButton("180°");
        qr->addWidget(btnQuickRot45); qr->addWidget(btnQuickRotNeg45);
        qr->addWidget(btnQuickRot90); qr->addWidget(btnQuickRot180);
        tl->addLayout(qr);
        tabTransforms->addTab(tab, "R");
    }

    // Tab 2: Scaling
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tl = new QVBoxLayout(tab);
        QFormLayout *f = new QFormLayout();
        spinScaleX = new QDoubleSpinBox(); spinScaleY = new QDoubleSpinBox();
        f->addRow("Sx:", spinScaleX);
        f->addRow("Sy:", spinScaleY);
        tl->addLayout(f);
        chkUniformScale = new QCheckBox("Uniform Scale (Sx = Sy)"); chkUniformScale->setChecked(true);
        tl->addWidget(chkUniformScale);

        radioScaleOrigin = new QRadioButton("About Origin"); radioScaleOrigin->setChecked(true);
        radioScaleCentroid = new QRadioButton("About Centroid");
        radioScaleCustom = new QRadioButton("About Fixed Point:");
        spinScalePx = new QSpinBox(); spinScalePy = new QSpinBox();
        tl->addWidget(radioScaleOrigin);
        tl->addWidget(radioScaleCentroid);
        QHBoxLayout *fpRow = new QHBoxLayout();
        fpRow->addWidget(radioScaleCustom);
        fpRow->addWidget(new QLabel("Fx:")); fpRow->addWidget(spinScalePx);
        fpRow->addWidget(new QLabel("Fy:")); fpRow->addWidget(spinScalePy);
        tl->addLayout(fpRow);

        QButtonGroup *scGrp = new QButtonGroup(tab);
        scGrp->addButton(radioScaleOrigin); scGrp->addButton(radioScaleCentroid); scGrp->addButton(radioScaleCustom);

        btnApplyScale = new QPushButton("▶ Apply Scaling");
        tl->addWidget(btnApplyScale);

        QHBoxLayout *qs = new QHBoxLayout();
        btnQuickScale2 = new QPushButton("×2"); btnQuickScaleHalf = new QPushButton("×0.5");
        btnQuickScale15 = new QPushButton("×1.5"); btnQuickScaleFlip = new QPushButton("Flip X");
        qs->addWidget(btnQuickScale2); qs->addWidget(btnQuickScaleHalf);
        qs->addWidget(btnQuickScale15); qs->addWidget(btnQuickScaleFlip);
        tl->addLayout(qs);
        tabTransforms->addTab(tab, "S");
    }

    // Tab 3: Shear
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tl = new QVBoxLayout(tab);
        QFormLayout *f = new QFormLayout();
        spinShearX = new QDoubleSpinBox(); spinShearY = new QDoubleSpinBox();
        spinShearYRef = new QSpinBox(); spinShearXRef = new QSpinBox();
        f->addRow("Shx:", spinShearX);
        f->addRow("Shy:", spinShearY);
        f->addRow("Y ref:", spinShearYRef);
        f->addRow("X ref:", spinShearXRef);
        tl->addLayout(f);
        btnApplyShear = new QPushButton("▶ Apply Shear");
        tl->addWidget(btnApplyShear);
        tabTransforms->addTab(tab, "Sh");
    }

    // Tab 4: Reflection
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tl = new QVBoxLayout(tab);
        QHBoxLayout *r1 = new QHBoxLayout();
        btnReflectX = new QPushButton("X-axis"); btnReflectY = new QPushButton("Y-axis");
        btnReflectOrigin = new QPushButton("Origin");
        r1->addWidget(btnReflectX); r1->addWidget(btnReflectY); r1->addWidget(btnReflectOrigin);
        tl->addLayout(r1);

        QHBoxLayout *r2 = new QHBoxLayout();
        btnReflectLineYeqX = new QPushButton("y = x"); btnReflectLineYeqNegX = new QPushButton("y = -x");
        r2->addWidget(btnReflectLineYeqX); r2->addWidget(btnReflectLineYeqNegX);
        tl->addLayout(r2);

        QFormLayout *f = new QFormLayout();
        spinReflectSlope = new QDoubleSpinBox(); spinReflectIntercept = new QDoubleSpinBox();
        f->addRow("Slope m:", spinReflectSlope);
        f->addRow("Intercept c:", spinReflectIntercept);
        tl->addLayout(f);

        btnPickReflectLine = new QPushButton("🎯 Pick Line on Canvas");
        btnApplyReflectArbitrary = new QPushButton("▶ Reflect About y = mx + c");
        tl->addWidget(btnPickReflectLine);
        tl->addWidget(btnApplyReflectArbitrary);
        tabTransforms->addTab(tab, "Ref");
    }

    // Tab 5: Custom Matrix
    {
        QWidget *tab = new QWidget();
        QVBoxLayout *tl = new QVBoxLayout(tab);
        QGridLayout *mg = new QGridLayout();
        spinM00 = new QDoubleSpinBox(); spinM01 = new QDoubleSpinBox(); spinM02 = new QDoubleSpinBox();
        spinM10 = new QDoubleSpinBox(); spinM11 = new QDoubleSpinBox(); spinM12 = new QDoubleSpinBox();
        spinM20 = new QDoubleSpinBox(); spinM21 = new QDoubleSpinBox(); spinM22 = new QDoubleSpinBox();
        mg->addWidget(spinM00, 0, 0); mg->addWidget(spinM01, 0, 1); mg->addWidget(spinM02, 0, 2);
        mg->addWidget(spinM10, 1, 0); mg->addWidget(spinM11, 1, 1); mg->addWidget(spinM12, 1, 2);
        mg->addWidget(spinM20, 2, 0); mg->addWidget(spinM21, 2, 1); mg->addWidget(spinM22, 2, 2);
        tl->addLayout(mg);
        btnApplyCustomMatrix = new QPushButton("▶ Apply Custom 3×3 Matrix");
        tl->addWidget(btnApplyCustomMatrix);
        tabTransforms->addTab(tab, "M");
    }

    lay->addWidget(tabTransforms);

    // Matrix Inspector
    QGroupBox *grpInsp = new QGroupBox("🧮 Composite Matrix Inspector");
    QVBoxLayout *iLay = new QVBoxLayout(grpInsp);
    txtMatrixDisplay = new QTextBrowser();
    txtMatrixDisplay->setMaximumHeight(100);
    lblMatrixDet = new QLabel("Det(M): 1.000");
    lblMatrixFormula = new QLabel("M = I (Identity)");
    iLay->addWidget(txtMatrixDisplay);
    iLay->addWidget(lblMatrixFormula);
    iLay->addWidget(lblMatrixDet);

    QHBoxLayout *hiActions = new QHBoxLayout();
    btnUndoTransform = new QPushButton("↩ Undo");
    btnResetTransform = new QPushButton("🔄 Reset");
    hiActions->addWidget(btnUndoTransform);
    hiActions->addWidget(btnResetTransform);
    iLay->addLayout(hiActions);

    tableHistory = new QTableWidget(0, 3);
    tableHistory->setHorizontalHeaderLabels({"#", "Transform", "Details"});
    tableHistory->setMaximumHeight(150);
    tableHistory->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableHistory->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableHistory->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    iLay->addWidget(tableHistory);
    lay->addWidget(grpInsp);

    lay->addStretch(1);
    return page;
}

// ============================================================================
// Shape Panel
// ============================================================================
QWidget* MainWindow::buildShapePanel() {
    QWidget *page = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(6);
    lay->setContentsMargins(6, 6, 6, 6);

    // Shape Tools
    QGroupBox *grpTools = new QGroupBox("🛠 Drawing Tools");
    QVBoxLayout *tLay = new QVBoxLayout(grpTools);
    QHBoxLayout *dr = new QHBoxLayout();
    btnDrawPolygon = new QPushButton("📐 Polygon"); dr->addWidget(btnDrawPolygon);
    btnDrawCircle = new QPushButton("⭕ Circle"); dr->addWidget(btnDrawCircle);
    btnDrawLine = new QPushButton("📏 Line"); dr->addWidget(btnDrawLine);
    tLay->addLayout(dr);

    QHBoxLayout *dr2 = new QHBoxLayout();
    btnClosePolygon = new QPushButton("Close Polygon"); dr2->addWidget(btnClosePolygon);
    btnUndoShape = new QPushButton("Undo Shape"); dr2->addWidget(btnUndoShape);
    tLay->addLayout(dr2);
    lay->addWidget(grpTools);

    // Presets
    QGroupBox *grpPreset = new QGroupBox("📦 Shape Presets");
    QVBoxLayout *pLay = new QVBoxLayout(grpPreset);
    QHBoxLayout *p1 = new QHBoxLayout();
    btnPresetTriangle = new QPushButton("△ Triangle"); p1->addWidget(btnPresetTriangle);
    btnPresetConvex = new QPushButton("⬠ Convex"); p1->addWidget(btnPresetConvex);
    btnPresetHouse = new QPushButton("🏠 House"); p1->addWidget(btnPresetHouse);
    pLay->addLayout(p1);

    QHBoxLayout *p2 = new QHBoxLayout();
    btnPresetStar = new QPushButton("★ Star"); p2->addWidget(btnPresetStar);
    btnPresetConcave = new QPushButton("➤ Arrow"); p2->addWidget(btnPresetConcave);
    btnPresetNested = new QPushButton("◎ Nested"); p2->addWidget(btnPresetNested);
    pLay->addLayout(p2);

    btnPresetMultiScene = new QPushButton("🎬 Multi-Shape Scene");
    pLay->addWidget(btnPresetMultiScene);
    lay->addWidget(grpPreset);

    // Fill
    QGroupBox *grpFill = new QGroupBox("🎨 Polygon Fill");
    QVBoxLayout *fLay = new QVBoxLayout(grpFill);
    comboFillAlgorithm = new QComboBox();
    comboFillAlgorithm->addItem("Scanline Fill (AET)");
    comboFillAlgorithm->addItem("Boundary Fill (8-Connected BFS)");
    fLay->addWidget(comboFillAlgorithm);
    QHBoxLayout *fr = new QHBoxLayout();
    btnExecuteFill = new QPushButton("▶ Fill"); fr->addWidget(btnExecuteFill);
    btnClearFill = new QPushButton("Clear Fill"); fr->addWidget(btnClearFill);
    fLay->addLayout(fr);
    lay->addWidget(grpFill);

    lay->addStretch(1);
    return page;
}

// ============================================================================
// Display Panel
// ============================================================================
QWidget* MainWindow::buildDisplayPanel() {
    QWidget *page = new QWidget();
    QVBoxLayout *lay = new QVBoxLayout(page);
    lay->setSpacing(6);
    lay->setContentsMargins(6, 6, 6, 6);

    QGroupBox *grpOverlay = new QGroupBox("👁️ Canvas Overlays");
    QVBoxLayout *oLay = new QVBoxLayout(grpOverlay);
    chkShowGrid = new QCheckBox("Show Grid"); chkShowGrid->setChecked(true);
    chkShowAxes = new QCheckBox("Show Axes"); chkShowAxes->setChecked(true);
    chkShowLabels = new QCheckBox("Show Vertex Labels"); chkShowLabels->setChecked(true);
    chkShowGhost = new QCheckBox("Show Ghost (Before Transform)"); chkShowGhost->setChecked(true);
    chkShowPivot = new QCheckBox("Show Rotation Pivot"); chkShowPivot->setChecked(true);
    chkShowReflectLine = new QCheckBox("Show Reflection Line"); chkShowReflectLine->setChecked(true);
    oLay->addWidget(chkShowGrid);
    oLay->addWidget(chkShowAxes);
    oLay->addWidget(chkShowLabels);
    oLay->addWidget(chkShowGhost);
    oLay->addWidget(chkShowPivot);
    oLay->addWidget(chkShowReflectLine);
    lay->addWidget(grpOverlay);

    QGroupBox *grpGrid = new QGroupBox("📏 Grid Size");
    QHBoxLayout *gLay = new QHBoxLayout(grpGrid);
    spinGridSize = new QSpinBox();
    spinGridSize->setRange(4, 60);
    spinGridSize->setValue(15);
    gLay->addWidget(new QLabel("Cell Size:"));
    gLay->addWidget(spinGridSize);
    lay->addWidget(grpGrid);

    btnClearAll = new QPushButton("🗑️ Clear All");
    lay->addWidget(btnClearAll);

    lay->addStretch(1);
    return page;
}

// ============================================================================
// Setup Connections
// ============================================================================
void MainWindow::setupConnections() {
    // Quick Command
    connect(btnRunQuickCommand, &QPushButton::clicked, this, &MainWindow::onRunQuickCommand);
    connect(txtQuickCommand, &QLineEdit::returnPressed, this, &MainWindow::onRunQuickCommand);

    // Selection
    connect(comboSelectShape, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSelectShapeComboChanged);
    connect(btnDeselectAll, &QPushButton::clicked, this, &MainWindow::onDeselectAllClicked);
    connect(m_canvas, &Canvas::shapeSelectionChanged, this, &MainWindow::handleShapeSelectionChanged);
    connect(m_canvas, &Canvas::shapesChanged, this, &MainWindow::handleShapesChanged);

    // Translation
    connect(btnApplyTranslate, &QPushButton::clicked, this, &MainWindow::onApplyTranslationClicked);
    connect(btnNudgeUp, &QPushButton::clicked, this, &MainWindow::onNudgeUp);
    connect(btnNudgeDown, &QPushButton::clicked, this, &MainWindow::onNudgeDown);
    connect(btnNudgeLeft, &QPushButton::clicked, this, &MainWindow::onNudgeLeft);
    connect(btnNudgeRight, &QPushButton::clicked, this, &MainWindow::onNudgeRight);
    connect(btnNudgeReset, &QPushButton::clicked, this, &MainWindow::onNudgeReset);

    // Rotation
    connect(btnApplyRotate, &QPushButton::clicked, this, &MainWindow::onApplyRotationClicked);
    connect(btnPickPivot, &QPushButton::clicked, this, &MainWindow::onPickPivotClicked);
    connect(btnQuickRot45, &QPushButton::clicked, this, &MainWindow::onQuickRot45);
    connect(btnQuickRotNeg45, &QPushButton::clicked, this, &MainWindow::onQuickRotNeg45);
    connect(btnQuickRot90, &QPushButton::clicked, this, &MainWindow::onQuickRot90);
    connect(btnQuickRot180, &QPushButton::clicked, this, &MainWindow::onQuickRot180);

    // Scaling
    connect(btnApplyScale, &QPushButton::clicked, this, &MainWindow::onApplyScalingClicked);
    connect(chkUniformScale, &QCheckBox::toggled, this, &MainWindow::onUniformScaleToggled);
    connect(spinScaleX, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::onScaleXChanged);
    connect(btnQuickScale2, &QPushButton::clicked, this, &MainWindow::onQuickScale2);
    connect(btnQuickScaleHalf, &QPushButton::clicked, this, &MainWindow::onQuickScaleHalf);
    connect(btnQuickScale15, &QPushButton::clicked, this, &MainWindow::onQuickScale15);
    connect(btnQuickScaleFlip, &QPushButton::clicked, this, &MainWindow::onQuickScaleFlip);

    // Shear
    connect(btnApplyShear, &QPushButton::clicked, this, &MainWindow::onApplyShearClicked);

    // Reflection
    connect(btnReflectX, &QPushButton::clicked, this, &MainWindow::onReflectXClicked);
    connect(btnReflectY, &QPushButton::clicked, this, &MainWindow::onReflectYClicked);
    connect(btnReflectOrigin, &QPushButton::clicked, this, &MainWindow::onReflectOriginClicked);
    connect(btnReflectLineYeqX, &QPushButton::clicked, this, &MainWindow::onReflectLineYeqXClicked);
    connect(btnReflectLineYeqNegX, &QPushButton::clicked, this, &MainWindow::onReflectLineYeqNegXClicked);
    connect(btnPickReflectLine, &QPushButton::clicked, this, &MainWindow::onPickReflectLineClicked);
    connect(btnApplyReflectArbitrary, &QPushButton::clicked, this, &MainWindow::onApplyReflectArbitraryClicked);

    // Custom Matrix
    connect(btnApplyCustomMatrix, &QPushButton::clicked, this, &MainWindow::onApplyCustomMatrixClicked);

    // History
    connect(btnUndoTransform, &QPushButton::clicked, this, &MainWindow::onUndoTransformClicked);
    connect(btnResetTransform, &QPushButton::clicked, this, &MainWindow::onResetTransformClicked);
    connect(m_canvas, &Canvas::transformationApplied, this, &MainWindow::handleTransformationApplied);
    connect(m_canvas, &Canvas::transformationsReset, this, &MainWindow::handleTransformationsReset);

    // Fills
    connect(comboFillAlgorithm, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onFillAlgorithmChanged);
    connect(btnExecuteFill, &QPushButton::clicked, this, &MainWindow::onExecuteFillClicked);
    connect(btnClearFill, &QPushButton::clicked, this, &MainWindow::onClearFillClicked);

    // Shape Tools
    connect(btnDrawPolygon, &QPushButton::clicked, this, &MainWindow::onDrawPolygonClicked);
    connect(btnDrawCircle, &QPushButton::clicked, this, &MainWindow::onDrawCircleClicked);
    connect(btnDrawLine, &QPushButton::clicked, this, &MainWindow::onDrawLineClicked);
    connect(btnClosePolygon, &QPushButton::clicked, this, &MainWindow::onClosePolygonClicked);
    connect(btnUndoShape, &QPushButton::clicked, this, &MainWindow::onUndoShapeClicked);

    connect(btnPresetTriangle, &QPushButton::clicked, this, &MainWindow::onPresetTriangleClicked);
    connect(btnPresetConvex, &QPushButton::clicked, this, &MainWindow::onPresetConvexClicked);
    connect(btnPresetHouse, &QPushButton::clicked, this, &MainWindow::onPresetHouseClicked);
    connect(btnPresetStar, &QPushButton::clicked, this, &MainWindow::onPresetStarClicked);
    connect(btnPresetConcave, &QPushButton::clicked, this, &MainWindow::onPresetConcaveClicked);
    connect(btnPresetNested, &QPushButton::clicked, this, &MainWindow::onPresetNestedClicked);
    connect(btnPresetMultiScene, &QPushButton::clicked, this, &MainWindow::onPresetMultiSceneClicked);

    // Display
    connect(spinGridSize, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onGridSizeChanged);
    connect(chkShowGhost, &QCheckBox::toggled, this, &MainWindow::onToggleShowGhost);
    connect(chkShowPivot, &QCheckBox::toggled, this, &MainWindow::onToggleShowPivot);
    connect(chkShowReflectLine, &QCheckBox::toggled, this, &MainWindow::onToggleShowReflectLine);
    connect(chkShowLabels, &QCheckBox::toggled, this, &MainWindow::onToggleShowLabels);
    connect(chkShowGrid, &QCheckBox::toggled, this, &MainWindow::onToggleShowGrid);
    connect(chkShowAxes, &QCheckBox::toggled, this, &MainWindow::onToggleShowAxes);
    connect(btnClearAll, &QPushButton::clicked, this, &MainWindow::onClearAllClicked);

    // Clipping
    connect(spinClipXmin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onClipXminChanged);
    connect(spinClipYmin, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onClipYminChanged);
    connect(spinClipXmax, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onClipXmaxChanged);
    connect(spinClipYmax, QOverload<int>::of(&QSpinBox::valueChanged), this, &MainWindow::onClipYmaxChanged);
    connect(btnClipCentered, &QPushButton::clicked, this, &MainWindow::onClipPresetCentered);
    connect(btnClipWide, &QPushButton::clicked, this, &MainWindow::onClipPresetWide);
    connect(btnClipTall, &QPushButton::clicked, this, &MainWindow::onClipPresetTall);
    connect(btnClipQuadrant, &QPushButton::clicked, this, &MainWindow::onClipPresetQuadrant);
    connect(btnClipSmall, &QPushButton::clicked, this, &MainWindow::onClipPresetSmall);
    connect(btnRunCS, &QPushButton::clicked, this, &MainWindow::onRunCohenSutherland);
    connect(btnRunLB, &QPushButton::clicked, this, &MainWindow::onRunLiangBarsky);
    connect(btnRunSH, &QPushButton::clicked, this, &MainWindow::onRunSutherlandHodgman);
    connect(btnRunWA, &QPushButton::clicked, this, &MainWindow::onRunWeilerAtherton);
    connect(btnLoadCSTest, &QPushButton::clicked, this, &MainWindow::onLoadCSTestLines);
    connect(btnLoadSHTriangle, &QPushButton::clicked, this, &MainWindow::onLoadSHTriangle);
    connect(btnLoadSHStar, &QPushButton::clicked, this, &MainWindow::onLoadSHStar);
    connect(btnLoadSHLShape, &QPushButton::clicked, this, &MainWindow::onLoadSHLShape);
    connect(btnLoadSHNepal, &QPushButton::clicked, this, &MainWindow::onLoadSHNepal);
    connect(chkSplitDisjointSH, &QCheckBox::toggled, this, &MainWindow::onToggleSplitDisjointSH);
    connect(sliderSHStage, &QSlider::valueChanged, this, &MainWindow::onSHStageChanged);
    connect(chkShowClipWindow, &QCheckBox::toggled, this, &MainWindow::onToggleClipWindow);
    connect(chkShowOutcodeLabels, &QCheckBox::toggled, this, &MainWindow::onToggleOutcodeLabels);
    connect(chkShowClipResult, &QCheckBox::toggled, this, &MainWindow::onToggleClipResult);
    connect(btnDragClipRegion, &QPushButton::clicked, this, &MainWindow::onDragClipRegion);
    connect(m_canvas, &Canvas::clipWindowChanged, this, &MainWindow::handleClipWindowChanged);
    connect(m_canvas, &Canvas::clippingFinished, this, &MainWindow::handleClippingFinished);

    // View
    connect(btnResetView, &QPushButton::clicked, this, &MainWindow::onResetView);
    connect(btnFitToView, &QPushButton::clicked, this, &MainWindow::onFitToView);
    connect(m_canvas, &Canvas::viewChanged, this, &MainWindow::handleViewChanged);

    // Canvas
    connect(m_canvas, &Canvas::mouseMoved, this, &MainWindow::handleMouseMoved);
    connect(m_canvas, &Canvas::pivotChanged, this, &MainWindow::handlePivotChanged);
    connect(m_canvas, &Canvas::reflectLineChanged, this, &MainWindow::handleReflectLineChanged);
    connect(m_canvas, &Canvas::fillFinished, this, &MainWindow::handleFillFinished);
    connect(m_canvas, &Canvas::statusMessage, this, &MainWindow::handleStatusMessage);
}

// ============================================================================
// SpinBox configuration
// ============================================================================
void MainWindow::setupSpinBoxes() {
    // Translation
    spinTx->setRange(-1000, 1000); spinTx->setSingleStep(5); spinTx->setValue(5);
    spinTy->setRange(-1000, 1000); spinTy->setSingleStep(5); spinTy->setValue(5);

    // Rotation
    spinRotAngle->setDecimals(2); spinRotAngle->setRange(-3600.0, 3600.0);
    spinRotAngle->setSingleStep(1.0); spinRotAngle->setValue(45.0);
    spinRotPx->setRange(-1000, 1000); spinRotPx->setValue(0);
    spinRotPy->setRange(-1000, 1000); spinRotPy->setValue(0);

    // Scaling
    spinScaleX->setDecimals(3); spinScaleX->setRange(-100.0, 100.0);
    spinScaleX->setSingleStep(0.1); spinScaleX->setValue(1.5);
    spinScaleY->setDecimals(3); spinScaleY->setRange(-100.0, 100.0);
    spinScaleY->setSingleStep(0.1); spinScaleY->setValue(1.5);
    spinScalePx->setRange(-1000, 1000); spinScalePx->setValue(0);
    spinScalePy->setRange(-1000, 1000); spinScalePy->setValue(0);

    // Shear
    spinShearX->setDecimals(3); spinShearX->setRange(-100.0, 100.0);
    spinShearX->setSingleStep(0.1); spinShearX->setValue(0.5);
    spinShearY->setDecimals(3); spinShearY->setRange(-100.0, 100.0);
    spinShearY->setSingleStep(0.1); spinShearY->setValue(0.0);
    spinShearYRef->setRange(-1000, 1000); spinShearYRef->setValue(0);
    spinShearXRef->setRange(-1000, 1000); spinShearXRef->setValue(0);

    // Reflection
    spinReflectSlope->setDecimals(3); spinReflectSlope->setRange(-1000.0, 1000.0);
    spinReflectSlope->setSingleStep(0.5); spinReflectSlope->setValue(1.0);
    spinReflectIntercept->setDecimals(2); spinReflectIntercept->setRange(-1000.0, 1000.0);
    spinReflectIntercept->setSingleStep(1.0); spinReflectIntercept->setValue(0.0);

    // Custom Matrix
    auto initMatSpin = [](QDoubleSpinBox *sb, double defVal) {
        sb->setDecimals(3); sb->setRange(-1000.0, 1000.0);
        sb->setSingleStep(0.1); sb->setValue(defVal);
    };
    initMatSpin(spinM00, 1.0); initMatSpin(spinM01, 0.0); initMatSpin(spinM02, 0.0);
    initMatSpin(spinM10, 0.0); initMatSpin(spinM11, 1.0); initMatSpin(spinM12, 0.0);
    initMatSpin(spinM20, 0.0); initMatSpin(spinM21, 0.0); initMatSpin(spinM22, 1.0);

    // Clipping
    spinClipXmin->setRange(-100, 100); spinClipXmin->setValue(-10);
    spinClipYmin->setRange(-100, 100); spinClipYmin->setValue(-10);
    spinClipXmax->setRange(-100, 100); spinClipXmax->setValue(10);
    spinClipYmax->setRange(-100, 100); spinClipYmax->setValue(10);
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

    hookReturn(spinTx, &MainWindow::onApplyTranslationClicked, this);
    hookReturn(spinTy, &MainWindow::onApplyTranslationClicked, this);
    hookReturn(spinRotAngle, &MainWindow::onApplyRotationClicked, this);
    hookReturn(spinRotPx, &MainWindow::onApplyRotationClicked, this);
    hookReturn(spinRotPy, &MainWindow::onApplyRotationClicked, this);
    hookReturn(spinScaleX, &MainWindow::onApplyScalingClicked, this);
    hookReturn(spinScaleY, &MainWindow::onApplyScalingClicked, this);
    hookReturn(spinScalePx, &MainWindow::onApplyScalingClicked, this);
    hookReturn(spinScalePy, &MainWindow::onApplyScalingClicked, this);
    hookReturn(spinShearX, &MainWindow::onApplyShearClicked, this);
    hookReturn(spinShearY, &MainWindow::onApplyShearClicked, this);
    hookReturn(spinShearYRef, &MainWindow::onApplyShearClicked, this);
    hookReturn(spinShearXRef, &MainWindow::onApplyShearClicked, this);
    hookReturn(spinReflectSlope, &MainWindow::onApplyReflectArbitraryClicked, this);
    hookReturn(spinReflectIntercept, &MainWindow::onApplyReflectArbitraryClicked, this);
    hookReturn(spinM00, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(spinM01, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(spinM02, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(spinM10, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(spinM11, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(spinM12, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(spinM20, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(spinM21, &MainWindow::onApplyCustomMatrixClicked, this);
    hookReturn(spinM22, &MainWindow::onApplyCustomMatrixClicked, this);
}

// ============================================================================
// Quick Command Bar
// ============================================================================
void MainWindow::onRunQuickCommand() {
    QString raw = txtQuickCommand->text().trimmed();
    if (raw.isEmpty()) return;

    QStringList parts = raw.split(QRegularExpression("[\\s,]+"), Qt::SkipEmptyParts);
    if (parts.isEmpty()) return;

    QString cmd = parts[0].toLower();

    // Clipping commands
    if (cmd == "clip" && parts.size() >= 5) {
        int xmin = parts[1].toInt();
        int ymin = parts[2].toInt();
        int xmax = parts[3].toInt();
        int ymax = parts[4].toInt();
        spinClipXmin->setValue(xmin); spinClipYmin->setValue(ymin);
        spinClipXmax->setValue(xmax); spinClipYmax->setValue(ymax);
        m_canvas->setClipRect(xmin, ymin, xmax, ymax);
        txtQuickCommand->clear();
        return;
    }
    if (cmd == "cs") { onRunCohenSutherland(); txtQuickCommand->clear(); return; }
    if (cmd == "sh") { onRunSutherlandHodgman(); txtQuickCommand->clear(); return; }

    bool firstIsNumber = false;
    double numVal = parts[0].toDouble(&firstIsNumber);

    if (firstIsNumber) {
        int currentTab = tabTransforms->currentIndex();
        if (currentTab == 0) {
            int tx = static_cast<int>(numVal);
            int ty = (parts.size() >= 2) ? parts[1].toInt() : 0;
            spinTx->setValue(tx); spinTy->setValue(ty);
            onApplyTranslationClicked();
        } else if (currentTab == 1) {
            spinRotAngle->setValue(numVal);
            if (parts.size() >= 3) {
                radioRotCustom->setChecked(true);
                spinRotPx->setValue(parts[1].toInt());
                spinRotPy->setValue(parts[2].toInt());
            }
            onApplyRotationClicked();
        } else if (currentTab == 2) {
            double sx = numVal;
            double sy = (parts.size() >= 2) ? parts[1].toDouble() : sx;
            spinScaleX->setValue(sx); spinScaleY->setValue(sy);
            if (parts.size() >= 4) {
                radioScaleCustom->setChecked(true);
                spinScalePx->setValue(parts[2].toInt());
                spinScalePy->setValue(parts[3].toInt());
            }
            onApplyScalingClicked();
        } else if (currentTab == 3) {
            double shx = numVal;
            double shy = (parts.size() >= 2) ? parts[1].toDouble() : 0.0;
            spinShearX->setValue(shx); spinShearY->setValue(shy);
            onApplyShearClicked();
        } else if (currentTab == 4) {
            double m = numVal;
            double c = (parts.size() >= 2) ? parts[1].toDouble() : 0.0;
            spinReflectSlope->setValue(m); spinReflectIntercept->setValue(c);
            onApplyReflectArbitraryClicked();
        }
        txtQuickCommand->clear();
        return;
    }

    if (cmd == "r" || cmd == "rot" || cmd == "rotate") {
        if (parts.size() >= 2) {
            double angle = parts[1].toDouble();
            spinRotAngle->setValue(angle);
            if (parts.size() >= 4) {
                radioRotCustom->setChecked(true);
                spinRotPx->setValue(parts[2].toInt());
                spinRotPy->setValue(parts[3].toInt());
            }
            onApplyRotationClicked();
        }
    } else if (cmd == "t" || cmd == "trans" || cmd == "translate") {
        if (parts.size() >= 3) {
            spinTx->setValue(parts[1].toInt()); spinTy->setValue(parts[2].toInt());
            onApplyTranslationClicked();
        } else if (parts.size() == 2) {
            spinTx->setValue(parts[1].toInt()); spinTy->setValue(0);
            onApplyTranslationClicked();
        }
    } else if (cmd == "s" || cmd == "scale") {
        if (parts.size() >= 2) {
            double sx = parts[1].toDouble();
            double sy = (parts.size() >= 3) ? parts[2].toDouble() : sx;
            spinScaleX->setValue(sx); spinScaleY->setValue(sy);
            if (parts.size() >= 5) {
                radioScaleCustom->setChecked(true);
                spinScalePx->setValue(parts[3].toInt());
                spinScalePy->setValue(parts[4].toInt());
            }
            onApplyScalingClicked();
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
                spinReflectSlope->setValue(m); spinReflectIntercept->setValue(c);
                onApplyReflectArbitraryClicked();
            }
        }
    } else if (cmd == "undo") {
        onUndoTransformClicked();
    } else if (cmd == "reset" || cmd == "i") {
        onResetTransformClicked();
    } else {
        statusBar()->showMessage(QString("Unrecognized: '%1'. Try 'T 5 5', 'R 45', 'clip -10 -10 10 10', 'cs', 'sh'").arg(raw), 4000);
        return;
    }
    txtQuickCommand->clear();
}

// ============================================================================
// Selection & Scope
// ============================================================================
void MainWindow::onSelectShapeComboChanged(int index) {
    if (index == 0) m_canvas->deselectAll();
    else {
        int targetId = comboSelectShape->itemData(index).toInt();
        m_canvas->selectShape(targetId);
    }
}
void MainWindow::onDeselectAllClicked() { m_canvas->deselectAll(); comboSelectShape->setCurrentIndex(0); }
void MainWindow::handleShapeSelectionChanged(int shapeId, const QString &description) {
    lblScopeInfo->setText(description);
    for (int i = 0; i < comboSelectShape->count(); ++i) {
        if (comboSelectShape->itemData(i).toInt() == shapeId) {
            comboSelectShape->blockSignals(true);
            comboSelectShape->setCurrentIndex(i);
            comboSelectShape->blockSignals(false);
            break;
        }
    }
}
void MainWindow::handleShapesChanged(int, int) { updateShapeDropdown(); }
void MainWindow::updateShapeDropdown() {
    comboSelectShape->blockSignals(true);
    comboSelectShape->clear();
    comboSelectShape->addItem(QString::fromUtf8("🌐 Entire Space (All Shapes & Grid)"), -1);
    for (const auto &poly : m_canvas->polygons())
        comboSelectShape->addItem(QString::fromUtf8("🎯 Polygon #%1 (%2 vertices)").arg(poly.id).arg(poly.vertices.size()), poly.id);
    for (const auto &c : m_canvas->circles())
        comboSelectShape->addItem(QString::fromUtf8("🎯 Circle #%1 (Center (%2,%3), r=%4)").arg(c.id).arg(c.center.x()).arg(c.center.y()).arg(c.radius), c.id);
    int curId = m_canvas->selectedShapeId();
    for (int i = 0; i < comboSelectShape->count(); ++i) {
        if (comboSelectShape->itemData(i).toInt() == curId) { comboSelectShape->setCurrentIndex(i); break; }
    }
    comboSelectShape->blockSignals(false);
}

// ============================================================================
// Transformations
// ============================================================================
void MainWindow::onApplyTranslationClicked() {
    spinTx->interpretText(); spinTy->interpretText();
    int tx = spinTx->value(), ty = spinTy->value();
    Matrix3x3 mat = Matrix3x3::translation(tx, ty);
    m_canvas->applyTransformation(mat, QString("Translation T(%1, %2)").arg(tx).arg(ty),
                                   QString("T(%1, %2)").arg(tx).arg(ty),
                                   QString("x' = x + %1,  y' = y + %2").arg(tx).arg(ty));
}
void MainWindow::onNudgeUp() { m_canvas->applyTransformation(Matrix3x3::translation(0, 5), "Nudge +Y (+5)", "T(0, 5)", "y' = y + 5"); }
void MainWindow::onNudgeDown() { m_canvas->applyTransformation(Matrix3x3::translation(0, -5), "Nudge -Y (-5)", "T(0, -5)", "y' = y - 5"); }
void MainWindow::onNudgeLeft() { m_canvas->applyTransformation(Matrix3x3::translation(-5, 0), "Nudge -X (-5)", "T(-5, 0)", "x' = x - 5"); }
void MainWindow::onNudgeRight() { m_canvas->applyTransformation(Matrix3x3::translation(5, 0), "Nudge +X (+5)", "T(5, 0)", "x' = x + 5"); }
void MainWindow::onNudgeReset() { spinTx->setValue(0); spinTy->setValue(0); }

void MainWindow::onApplyRotationClicked() {
    spinRotAngle->interpretText(); spinRotPx->interpretText(); spinRotPy->interpretText();
    double angle = spinRotAngle->value();
    Matrix3x3 mat; QString name, formula, details;
    if (radioRotOrigin->isChecked()) {
        mat = Matrix3x3::rotation(angle);
        name = QString("Rotation R(%1°) wrt Origin (0,0)").arg(angle, 0, 'f', 1);
        formula = QString("R(%1°)").arg(angle, 0, 'f', 1);
        details = QString("x' = x·cos(%1°) - y·sin(%1°), y' = x·sin(%1°) + y·cos(%1°)").arg(angle, 0, 'f', 1);
    } else if (radioRotCentroid->isChecked()) {
        QPoint c = m_canvas->getSelectedShapeCentroid();
        mat = Matrix3x3::rotationAboutPoint(angle, c.x(), c.y());
        name = QString("Rotation R(%1°) wrt Centroid (%2,%3)").arg(angle, 0, 'f', 1).arg(c.x()).arg(c.y());
        formula = QString("T(%2,%3) · R(%1°) · T(-%2,-%3)").arg(angle, 0, 'f', 1).arg(c.x()).arg(c.y());
        details = QString("Rotate %1° around Centroid (%2,%3)").arg(angle, 0, 'f', 1).arg(c.x()).arg(c.y());
    } else {
        int px = spinRotPx->value(), py = spinRotPy->value();
        mat = Matrix3x3::rotationAboutPoint(angle, px, py);
        name = QString("Rotation R(%1°) wrt Pivot (%2,%3)").arg(angle, 0, 'f', 1).arg(px).arg(py);
        formula = QString("T(%2,%3) · R(%1°) · T(-%2,-%3)").arg(angle, 0, 'f', 1).arg(px).arg(py);
        details = QString("Rotate %1° around Arbitrary Point (%2,%3)").arg(angle, 0, 'f', 1).arg(px).arg(py);
    }
    m_canvas->applyTransformation(mat, name, formula, details);
}
void MainWindow::onPickPivotClicked() { m_canvas->setFillMode(Canvas::ModeSetPivot); statusBar()->showMessage("Click any point on the canvas to set the Rotation Pivot Point.", 5000); }
void MainWindow::onQuickRot45() { spinRotAngle->setValue(45.0); onApplyRotationClicked(); }
void MainWindow::onQuickRotNeg45() { spinRotAngle->setValue(-45.0); onApplyRotationClicked(); }
void MainWindow::onQuickRot90() { spinRotAngle->setValue(90.0); onApplyRotationClicked(); }
void MainWindow::onQuickRot180() { spinRotAngle->setValue(180.0); onApplyRotationClicked(); }

void MainWindow::onUniformScaleToggled(bool checked) { if (checked) spinScaleY->setValue(spinScaleX->value()); }
void MainWindow::onScaleXChanged(double val) { if (chkUniformScale->isChecked()) spinScaleY->setValue(val); }

void MainWindow::onApplyScalingClicked() {
    spinScaleX->interpretText(); spinScaleY->interpretText();
    spinScalePx->interpretText(); spinScalePy->interpretText();
    double sx = spinScaleX->value();
    double sy = chkUniformScale->isChecked() ? sx : spinScaleY->value();
    Matrix3x3 mat; QString name, formula, details;
    if (radioScaleOrigin->isChecked()) {
        mat = Matrix3x3::scaling(sx, sy);
        name = QString("Scaling S(%1, %2) wrt Origin").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2);
        formula = QString("S(%1, %2)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2);
        details = QString("x' = %1·x,  y' = %2·y").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2);
    } else if (radioScaleCentroid->isChecked()) {
        QPoint c = m_canvas->getSelectedShapeCentroid();
        mat = Matrix3x3::scalingAboutPoint(sx, sy, c.x(), c.y());
        name = QString("Scaling S(%1, %2) wrt Centroid (%3,%4)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2).arg(c.x()).arg(c.y());
        formula = QString("T(%3,%4) · S(%1,%2) · T(-%3,-%4)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2).arg(c.x()).arg(c.y());
        details = QString("Local scaling about Centroid (%1,%2)").arg(c.x()).arg(c.y());
    } else {
        int fx = spinScalePx->value(), fy = spinScalePy->value();
        mat = Matrix3x3::scalingAboutPoint(sx, sy, fx, fy);
        name = QString("Scaling S(%1, %2) wrt Fixed Point (%3,%4)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2).arg(fx).arg(fy);
        formula = QString("T(%3,%4) · S(%1,%2) · T(-%3,-%4)").arg(sx, 0, 'f', 2).arg(sy, 0, 'f', 2).arg(fx).arg(fy);
        details = QString("Fixed point scaling about (%1,%2)").arg(fx).arg(fy);
    }
    m_canvas->applyTransformation(mat, name, formula, details);
}
void MainWindow::onQuickScale2() { spinScaleX->setValue(2.0); spinScaleY->setValue(2.0); onApplyScalingClicked(); }
void MainWindow::onQuickScaleHalf() { spinScaleX->setValue(0.5); spinScaleY->setValue(0.5); onApplyScalingClicked(); }
void MainWindow::onQuickScale15() { spinScaleX->setValue(1.5); spinScaleY->setValue(1.5); onApplyScalingClicked(); }
void MainWindow::onQuickScaleFlip() { spinScaleX->setValue(-1.0); spinScaleY->setValue(1.0); onApplyScalingClicked(); }

void MainWindow::onApplyShearClicked() {
    spinShearX->interpretText(); spinShearY->interpretText();
    spinShearYRef->interpretText(); spinShearXRef->interpretText();
    double shx = spinShearX->value(), shy = spinShearY->value();
    int yref = spinShearYRef->value(), xref = spinShearXRef->value();
    Matrix3x3 mat = Matrix3x3::shearX(shx, yref) * Matrix3x3::shearY(shy, xref);
    m_canvas->applyTransformation(mat,
        QString("Shear Sh(x=%1, y=%2)").arg(shx, 0, 'f', 2).arg(shy, 0, 'f', 2),
        QString("Sh_x(%1) · Sh_y(%2)").arg(shx, 0, 'f', 2).arg(shy, 0, 'f', 2),
        QString("x' = x + %1(y - %2), y' = y + %3(x - %4)").arg(shx, 0, 'f', 2).arg(yref).arg(shy, 0, 'f', 2).arg(xref));
}

void MainWindow::onReflectXClicked() { m_canvas->applyTransformation(Matrix3x3::reflectionX(), "Reflection wrt X-Axis (y=0)", "Ref_x", "x' = x,  y' = -y"); }
void MainWindow::onReflectYClicked() { m_canvas->applyTransformation(Matrix3x3::reflectionY(), "Reflection wrt Y-Axis (x=0)", "Ref_y", "x' = -x,  y' = y"); }
void MainWindow::onReflectOriginClicked() { m_canvas->applyTransformation(Matrix3x3::reflectionOrigin(), "Reflection wrt Origin (0,0)", "Ref_origin", "x' = -x,  y' = -y"); }
void MainWindow::onReflectLineYeqXClicked() { m_canvas->applyTransformation(Matrix3x3::reflectionLineYeqX(), "Reflection wrt Line y = x", "Ref_y=x", "x' = y,  y' = x"); }
void MainWindow::onReflectLineYeqNegXClicked() { m_canvas->applyTransformation(Matrix3x3::reflectionLineYeqNegX(), "Reflection wrt Line y = -x", "Ref_y=-x", "x' = -y,  y' = -x"); }
void MainWindow::onPickReflectLineClicked() { m_canvas->setFillMode(Canvas::ModeSetReflectLine); statusBar()->showMessage("Click two points on canvas to define the Arbitrary Reflection Line.", 5000); }

void MainWindow::onApplyReflectArbitraryClicked() {
    spinReflectSlope->interpretText(); spinReflectIntercept->interpretText();
    double m = spinReflectSlope->value(), c = spinReflectIntercept->value();
    Matrix3x3 mat = Matrix3x3::reflectionArbitraryLine(m, c);
    m_canvas->setReflectLine(m, c);
    m_canvas->applyTransformation(mat,
        QString("Reflection Across Line y = %1x + %2").arg(m, 0, 'f', 2).arg(c, 0, 'f', 2),
        QString("T(0,%1) · R(θ) · Ref_x · R(-θ) · T(0,-%1)").arg(c, 0, 'f', 2),
        QString("θ = atan(%1) = %2°").arg(m, 0, 'f', 2).arg(std::atan(m) * 180.0 / M_PI, 0, 'f', 1));
}

void MainWindow::onApplyCustomMatrixClicked() {
    spinM00->interpretText(); spinM01->interpretText(); spinM02->interpretText();
    spinM10->interpretText(); spinM11->interpretText(); spinM12->interpretText();
    spinM20->interpretText(); spinM21->interpretText(); spinM22->interpretText();
    Matrix3x3 customMat(spinM00->value(), spinM01->value(), spinM02->value(),
                         spinM10->value(), spinM11->value(), spinM12->value(),
                         spinM20->value(), spinM21->value(), spinM22->value());
    m_canvas->applyTransformation(customMat, "Custom 3x3 Matrix Transformation", customMat.toString(),
                                   QString("Det(M) = %1").arg(customMat.determinant(), 0, 'f', 3));
}

// ============================================================================
// Matrix Inspector
// ============================================================================
void MainWindow::onUndoTransformClicked() { m_canvas->undoLastTransformation(); updateMatrixInspector(); }
void MainWindow::onResetTransformClicked() { m_canvas->resetTransformations(); updateMatrixInspector(); }
void MainWindow::handleTransformationApplied(const Matrix3x3::TransformStep &, const Matrix3x3 &) { updateMatrixInspector(); }
void MainWindow::handleTransformationsReset() { updateMatrixInspector(); }

void MainWindow::updateMatrixInspector() {
    const Matrix3x3 &comp = m_canvas->compositeMatrix();
    txtMatrixDisplay->setHtml(comp.toHtmlTable());
    lblMatrixDet->setText(QString("Det(M): %1").arg(comp.determinant(), 0, 'f', 3));
    const auto &history = m_canvas->transformHistory();
    lblMatrixFormula->setText(history.isEmpty() ? "M_composite = I (Identity)" :
                                  QString("M_composite = %1 steps chained").arg(history.size()));
    tableHistory->setRowCount(history.size());
    for (int i = 0; i < history.size(); ++i) {
        const auto &step = history[i];
        tableHistory->setItem(i, 0, new QTableWidgetItem(QString("#%1").arg(i + 1)));
        tableHistory->setItem(i, 1, new QTableWidgetItem(step.name));
        tableHistory->setItem(i, 2, new QTableWidgetItem(step.details.isEmpty() ? step.formula : step.details));
    }
    tableHistory->scrollToBottom();
    if (tabTransforms->currentIndex() != 5) {
        spinM00->setValue(comp.m[0][0]); spinM01->setValue(comp.m[0][1]); spinM02->setValue(comp.m[0][2]);
        spinM10->setValue(comp.m[1][0]); spinM11->setValue(comp.m[1][1]); spinM12->setValue(comp.m[1][2]);
        spinM20->setValue(comp.m[2][0]); spinM21->setValue(comp.m[2][1]); spinM22->setValue(comp.m[2][2]);
    }
}

// ============================================================================
// Fills
// ============================================================================
void MainWindow::onFillAlgorithmChanged(int index) {
    m_canvas->setFillMode(index == 0 ? Canvas::ModeScanlineFill : Canvas::ModeBoundaryFill8);
}
void MainWindow::onExecuteFillClicked() { m_canvas->triggerFill(); }
void MainWindow::onClearFillClicked() { m_canvas->clearFillOnly(); }

// ============================================================================
// Shape Tools
// ============================================================================
void MainWindow::onDrawPolygonClicked() { m_canvas->setFillMode(Canvas::ModeDrawPolygon); statusBar()->showMessage("Click canvas to place polygon vertices. Right-click to close.", 5000); }
void MainWindow::onDrawCircleClicked() { m_canvas->setFillMode(Canvas::ModeDrawCircle); statusBar()->showMessage("Click center, then outer point for radius.", 5000); }
void MainWindow::onDrawLineClicked() { m_canvas->setFillMode(Canvas::ModeDrawLine); statusBar()->showMessage("Click first endpoint, then second endpoint to draw a line segment.", 5000); }
void MainWindow::onClosePolygonClicked() { m_canvas->closePolygon(); }
void MainWindow::onUndoShapeClicked() { m_canvas->undoLastShape(); }

void MainWindow::onPresetTriangleClicked() { m_canvas->setPolygonPreset(CGAlgorithms::getTrianglePreset()); }
void MainWindow::onPresetConvexClicked() { m_canvas->setPolygonPreset(CGAlgorithms::getConvexPolygonPreset()); }
void MainWindow::onPresetHouseClicked() { m_canvas->setPolygonPreset(CGAlgorithms::getHousePreset()); }
void MainWindow::onPresetStarClicked() { m_canvas->setPolygonPreset(CGAlgorithms::getStarPolygonPreset()); }
void MainWindow::onPresetConcaveClicked() { m_canvas->setPolygonPreset(CGAlgorithms::getConcaveArrowPreset()); }
void MainWindow::onPresetNestedClicked() { m_canvas->setNestedPolygonPreset(CGAlgorithms::getNestedOuterPreset(), CGAlgorithms::getNestedInnerPreset()); }
void MainWindow::onPresetMultiSceneClicked() { m_canvas->setMultiShapeScenePreset(); }

// ============================================================================
// Display
// ============================================================================
void MainWindow::onGridSizeChanged(int size) { m_canvas->setGridSize(size); }
void MainWindow::onToggleShowGhost(bool checked) { m_canvas->setShowGhost(checked); }
void MainWindow::onToggleShowPivot(bool checked) { m_canvas->setShowRotationPivot(checked); }
void MainWindow::onToggleShowReflectLine(bool checked) { m_canvas->setShowReflectLine(checked); }
void MainWindow::onToggleShowLabels(bool checked) { m_canvas->setShowVertexLabels(checked); }
void MainWindow::onToggleShowGrid(bool checked) { m_canvas->setShowGrid(checked); }
void MainWindow::onToggleShowAxes(bool checked) { m_canvas->setShowAxes(checked); }
void MainWindow::onClearAllClicked() { m_canvas->clearAll(); }

// ============================================================================
// Clipping Studio
// ============================================================================
void MainWindow::onClipXminChanged(int val) { m_canvas->setClipRect(val, spinClipYmin->value(), spinClipXmax->value(), spinClipYmax->value()); }
void MainWindow::onClipYminChanged(int val) { m_canvas->setClipRect(spinClipXmin->value(), val, spinClipXmax->value(), spinClipYmax->value()); }
void MainWindow::onClipXmaxChanged(int val) { m_canvas->setClipRect(spinClipXmin->value(), spinClipYmin->value(), val, spinClipYmax->value()); }
void MainWindow::onClipYmaxChanged(int val) { m_canvas->setClipRect(spinClipXmin->value(), spinClipYmin->value(), spinClipXmax->value(), val); }

void MainWindow::onClipPresetCentered() { spinClipXmin->setValue(-10); spinClipYmin->setValue(-10); spinClipXmax->setValue(10); spinClipYmax->setValue(10); }
void MainWindow::onClipPresetWide() { spinClipXmin->setValue(-20); spinClipYmin->setValue(-5); spinClipXmax->setValue(20); spinClipYmax->setValue(5); }
void MainWindow::onClipPresetTall() { spinClipXmin->setValue(-5); spinClipYmin->setValue(-15); spinClipXmax->setValue(5); spinClipYmax->setValue(15); }
void MainWindow::onClipPresetQuadrant() { spinClipXmin->setValue(0); spinClipYmin->setValue(0); spinClipXmax->setValue(15); spinClipYmax->setValue(15); }
void MainWindow::onClipPresetSmall() { spinClipXmin->setValue(-5); spinClipYmin->setValue(-5); spinClipXmax->setValue(5); spinClipYmax->setValue(5); }


void MainWindow::onRunLiangBarsky() {
    m_canvas->runLiangBarsky();
    // Update inspector
    QString html;
    html += "<h3>Liang-Barsky Results</h3>";
    const auto &results = m_canvas->lastLBResults();
    for (int i = 0; i < results.size(); ++i) {
        const auto &res = results[i];
        html += QString("<b>Line %1:</b> %2<br>").arg(i+1).arg(res.accepted ? "<font color='green'>Accepted</font>" : "<font color='red'>Rejected</font>");
        html += QString("<pre>%1</pre><hr>").arg(res.trace);
    }
    txtCSInspector->setHtml(html);
}

void MainWindow::onRunCohenSutherland() {
    m_canvas->runCohenSutherland();
    updateCSInspector();
}


void MainWindow::onRunWeilerAtherton() {
    m_canvas->runWeilerAtherton();
    QString html;
    html += "<h3>Weiler-Atherton Results</h3>";
    const auto &res = m_canvas->lastWAResult();
    html += QString("<b>Disjoint Polygons Generated:</b> %1<br>").arg(res.disjointPolygons.size());
    html += QString("<pre>%1</pre>").arg(res.trace);
    txtSHInspector->setHtml(html);
}

void MainWindow::onRunSutherlandHodgman() {
    m_canvas->runSutherlandHodgman();
    updateSHInspector();
}

void MainWindow::onLoadCSTestLines() {
    m_canvas->loadCSTestLines();
    updateCSInspector();
}

void MainWindow::onLoadSHTriangle() { m_canvas->loadSHTestPreset("Triangle"); updateSHInspector(); }
void MainWindow::onLoadSHStar() { m_canvas->loadSHTestPreset("Star"); updateSHInspector(); }
void MainWindow::onLoadSHLShape() { m_canvas->loadSHTestPreset("LShape"); updateSHInspector(); }
void MainWindow::onLoadSHNepal() { m_canvas->loadSHTestPreset("NepalFlag"); updateSHInspector(); }
void MainWindow::onToggleSplitDisjointSH(bool checked) {
    m_canvas->setSplitDisjointSH(checked);
    m_canvas->update();
    updateSHInspector();
}

void MainWindow::onSHStageChanged(int val) {
    m_canvas->setSHStage(val);
    const char *names[] = {"Original", "Left Clip", "Right Clip", "Bottom Clip", "Top Clip (Final)"};
    lblSHStage->setText(QString("Stage %1: %2").arg(val).arg(names[val]));
    updateSHInspector();
}

void MainWindow::onToggleClipWindow(bool checked) { m_canvas->setShowClipWindow(checked); }
void MainWindow::onToggleOutcodeLabels(bool checked) { m_canvas->setShowOutcodeLabels(checked); }
void MainWindow::onToggleClipResult(bool checked) { m_canvas->setShowClipResult(checked); }
void MainWindow::onDragClipRegion() {
    m_canvas->setFillMode(Canvas::ModeClipRegionDrag);
    statusBar()->showMessage("Drag inside clip window to move, drag edges/corners to resize.", 5000);
}

void MainWindow::handleClipWindowChanged(double xmin, double ymin, double xmax, double ymax) {
    spinClipXmin->blockSignals(true); spinClipXmin->setValue(static_cast<int>(xmin)); spinClipXmin->blockSignals(false);
    spinClipYmin->blockSignals(true); spinClipYmin->setValue(static_cast<int>(ymin)); spinClipYmin->blockSignals(false);
    spinClipXmax->blockSignals(true); spinClipXmax->setValue(static_cast<int>(xmax)); spinClipXmax->blockSignals(false);
    spinClipYmax->blockSignals(true); spinClipYmax->setValue(static_cast<int>(ymax)); spinClipYmax->blockSignals(false);
}

void MainWindow::handleClippingFinished(const QString &algorithm, int count) {
    statusBar()->showMessage(QString("%1 clipping completed: %2 items processed.").arg(algorithm).arg(count), 5000);
    if (algorithm.contains("Cohen")) updateCSInspector();
    else updateSHInspector();
}

void MainWindow::updateCSInspector() {
    const auto &results = m_canvas->lastCSResults();
    if (results.isEmpty()) {
        txtCSInspector->setHtml("<i>No Cohen-Sutherland results. Load test lines and run clip.</i>");
        return;
    }
    QString html = "<style>td{padding:2px 4px;} .acc{color:#2ed573;} .rej{color:#ff4757;} .code{color:#ffa502;}</style>";
    html += QString("<b>Cohen-Sutherland Line Clipping — %1 lines</b><br>").arg(results.size());
    html += "<table border='1' cellspacing='0' style='border-color:#30363d;width:100%;'>";
    html += "<tr><th>#</th><th>Original</th><th>Code₁</th><th>Code₂</th><th>Result</th><th>Clipped</th></tr>";
    for (int i = 0; i < results.size(); ++i) {
        const auto &r = results[i];
        QString cls = r.accepted ? "acc" : "rej";
        html += QString("<tr><td>%1</td><td>(%2,%3)→(%4,%5)</td>")
                    .arg(i + 1)
                    .arg(r.originalP1.x(), 0, 'f', 0).arg(r.originalP1.y(), 0, 'f', 0)
                    .arg(r.originalP2.x(), 0, 'f', 0).arg(r.originalP2.y(), 0, 'f', 0);
        int c1 = ClipAlgorithms::computeOutcode(r.originalP1.x(), r.originalP1.y(),
                                                 m_canvas->clipRect().left(), m_canvas->clipRect().top(),
                                                 m_canvas->clipRect().right(), m_canvas->clipRect().bottom());
        int c2 = ClipAlgorithms::computeOutcode(r.originalP2.x(), r.originalP2.y(),
                                                 m_canvas->clipRect().left(), m_canvas->clipRect().top(),
                                                 m_canvas->clipRect().right(), m_canvas->clipRect().bottom());
        html += QString("<td class='code'>%1</td><td class='code'>%2</td>")
                    .arg(ClipAlgorithms::outcodeToString(c1)).arg(ClipAlgorithms::outcodeToString(c2));
        html += QString("<td class='%1'>%2</td>").arg(cls).arg(r.accepted ? "✓ Accept" : "✗ Reject");
        if (r.accepted) {
            html += QString("<td>(%1,%2)→(%3,%4)</td>")
                        .arg(r.clippedP1.x(), 0, 'f', 1).arg(r.clippedP1.y(), 0, 'f', 1)
                        .arg(r.clippedP2.x(), 0, 'f', 1).arg(r.clippedP2.y(), 0, 'f', 1);
        } else {
            html += "<td>—</td>";
        }
        html += "</tr>";
    }
    html += "</table>";

    // Step trace for the first line with steps
    for (int i = 0; i < results.size() && i < 3; ++i) {
        const auto &r = results[i];
        if (!r.steps.isEmpty()) {
            html += QString("<br><b>Trace for Line %1:</b><br>").arg(i + 1);
            for (const auto &step : r.steps) {
                html += QString("<span class='code'>Step %1:</span> Code₁=%2 Code₂=%3 | %4<br>")
                            .arg(step.stepNumber).arg(step.outcodeStr1).arg(step.outcodeStr2).arg(step.description);
            }
        }
    }
    txtCSInspector->setHtml(html);
}

void MainWindow::updateSHInspector() {
    const auto &result = m_canvas->lastSHResult();
    if (result.stages.isEmpty()) {
        txtSHInspector->setHtml("<i>No Sutherland-Hodgman results. Load a polygon and run clip.</i>");
        return;
    }
    QString html = "<style>td{padding:2px 4px;} .in{color:#2ed573;} .out{color:#ff4757;} .edge{color:#ffa502;} .warn{color:#ff9f43;}</style>";
    html += "<b>Sutherland-Hodgman Polygon Clipping (4-Stage Pipeline)</b><br>";
    html += QString("Original vertices: %1<br>")
                .arg(result.originalVertices.size());

    if (m_canvas->splitDisjointSH() && result.disjointPolygons.size() > 1) {
        html += QString("<div class='warn'><b>Disjoint Components Detected:</b> Divided into <b>%1 separate polygons</b> (no spurious bridging edges!):</div>")
                    .arg(result.disjointPolygons.size());
        for (int p = 0; p < result.disjointPolygons.size(); ++p) {
            html += QString("• <b>Polygon %1:</b> %2 vertices [").arg(p + 1).arg(result.disjointPolygons[p].size());
            for (int v = 0; v < result.disjointPolygons[p].size(); ++v) {
                const auto &pt = result.disjointPolygons[p][v];
                html += QString("(%1,%2)%3").arg(pt.x(), 0, 'f', 1).arg(pt.y(), 0, 'f', 1)
                            .arg(v + 1 < result.disjointPolygons[p].size() ? " " : "");
            }
            html += "]<br>";
        }
    } else {
        html += QString("Final vertices: %1 (Single Polygon)<br>")
                    .arg(result.clippedVertices.size());
    }
    html += "<br>";

    for (const auto &stage : result.stages) {
        html += QString("<b class='edge'>Stage %1: %2 Boundary</b><br>").arg(stage.stageNumber).arg(stage.edgeName);
        if (m_canvas->splitDisjointSH() && stage.outputPolygons.size() > 1) {
            html += QString("Input: %1 vertices → Output: <b>%2 disjoint polygons</b><br>")
                        .arg(stage.inputVertices.size()).arg(stage.outputPolygons.size());
        } else {
            html += QString("Input: %1 vertices → Output: %2 vertices<br>")
                        .arg(stage.inputVertices.size()).arg(stage.outputVertices.size());
        }

        if (!stage.decisions.isEmpty()) {
            html += "<table border='1' cellspacing='0' style='border-color:#30363d;width:100%;'>";
            html += "<tr><th>Edge</th><th>Decision</th></tr>";
            for (int j = 0; j < stage.decisions.size(); ++j) {
                html += QString("<tr><td>E%1</td><td>%2</td></tr>").arg(j).arg(stage.decisions[j]);
            }
            html += "</table>";
        }
        html += "<br>";
    }
    txtSHInspector->setHtml(html);
}

// ============================================================================
// View Navigation
// ============================================================================
void MainWindow::onResetView() { m_canvas->resetView(); }
void MainWindow::onFitToView() { m_canvas->fitToView(); }
void MainWindow::handleViewChanged(int gridSize) { spinGridSize->blockSignals(true); spinGridSize->setValue(gridSize); spinGridSize->blockSignals(false); }

// ============================================================================
// Canvas Signals
// ============================================================================
void MainWindow::handleMouseMoved(int logX, int logY, int, int) {
    statusBar()->showMessage(QString("Cursor: (%1, %2) | Scope: %3").arg(logX).arg(logY).arg(m_canvas->getSelectedShapeName()), 2000);
}
void MainWindow::handlePivotChanged(QPoint pivot) { spinRotPx->setValue(pivot.x()); spinRotPy->setValue(pivot.y()); }
void MainWindow::handleReflectLineChanged(double slope, double intercept) { spinReflectSlope->setValue(slope); spinReflectIntercept->setValue(intercept); }
void MainWindow::handleFillFinished(const QString &algName, qint64 elapsedNanos, int pointCount) {
    statusBar()->showMessage(QString("%1 completed: %2 pixels filled in %3 ns").arg(algName).arg(pointCount).arg(elapsedNanos), 5000);
}
void MainWindow::handleStatusMessage(const QString &msg) { statusBar()->showMessage(msg, 4000); }
