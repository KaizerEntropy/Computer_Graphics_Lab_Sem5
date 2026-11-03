import re

with open('mainwindow.cpp', 'r') as f:
    content = f.read()

# 1. We rewrite the setupDarkTheme to a sleek modern Light/Dark distinct theme
new_theme = """
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
"""

# Replace setupDarkTheme
content = re.sub(r'void MainWindow::setupDarkTheme\(\)\s*\{.*?(?=\nvoid MainWindow::buildUI\(\))', new_theme.strip(), content, flags=re.DOTALL)

# 2. Rewrite buildUI to put the sidebar on the right and remove unnecessary tabs, using QToolBox or simple QVBoxLayout
new_buildUI = """
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
    
    controlLay->addStretch();
    controlWidget->setLayout(controlLay);
    controlScroll->setWidget(controlWidget);
    
    mainLayout->addWidget(controlScroll, 1);
    
    // We instantiate variables that the rest of the code expects, even if hidden, to avoid crashes.
    // The previous implementation bound a lot of unused UI elements.
    // For safety, we will just create them invisibly.
    txtQuickCommand = new QLineEdit(); btnRunQuickCommand = new QPushButton();
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
    spinGridSize = new QSpinBox(); chkShowGhost = new QCheckBox(); chkShowPivot = new QCheckBox(); chkShowReflectLine = new QCheckBox(); chkShowLabels = new QCheckBox(); chkShowGrid = new QCheckBox(); chkShowAxes = new QCheckBox(); btnClearAll = new QPushButton();
    btnClipCentered = new QPushButton(); btnClipWide = new QPushButton(); btnClipTall = new QPushButton(); btnClipQuadrant = new QPushButton(); btnClipSmall = new QPushButton();
    sliderSHStage = new QSlider(); lblSHStage = new QLabel();
    btnResetView = new QPushButton(); btnFitToView = new QPushButton();
}
"""

content = re.sub(r'void MainWindow::buildUI\(\)\s*\{.*?(?=\nQWidget\* MainWindow::buildClippingPanel\(\))', new_buildUI.strip(), content, flags=re.DOTALL)

with open('mainwindow.cpp', 'w') as f:
    f.write(content)
