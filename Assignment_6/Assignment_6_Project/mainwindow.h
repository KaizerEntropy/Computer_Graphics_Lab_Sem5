#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QScrollArea>
#include <QTabWidget>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QSlider>
#include <QLabel>
#include <QTextBrowser>
#include <QTableWidget>
#include <QLineEdit>
#include <QStatusBar>

#include "canvas.h"
#include "matrix3x3.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    // Quick Command Bar
    void onRunQuickCommand();

    // 1. Selection & Scope
    void onSelectShapeComboChanged(int index);
    void onDeselectAllClicked();
    void handleShapeSelectionChanged(int shapeId, const QString &description);
    void handleShapesChanged(int totalPolygons, int totalCircles);

    // 2. Transformations
    void onApplyTranslationClicked();
    void onNudgeUp();
    void onNudgeDown();
    void onNudgeLeft();
    void onNudgeRight();
    void onNudgeReset();

    void onApplyRotationClicked();
    void onPickPivotClicked();
    void onQuickRot45();
    void onQuickRotNeg45();
    void onQuickRot90();
    void onQuickRot180();

    void onApplyScalingClicked();
    void onUniformScaleToggled(bool checked);
    void onScaleXChanged(double val);
    void onQuickScale2();
    void onQuickScaleHalf();
    void onQuickScale15();
    void onQuickScaleFlip();

    void onApplyShearClicked();

    void onReflectXClicked();
    void onReflectYClicked();
    void onReflectOriginClicked();
    void onReflectLineYeqXClicked();
    void onReflectLineYeqNegXClicked();
    void onPickReflectLineClicked();
    void onApplyReflectArbitraryClicked();

    void onApplyCustomMatrixClicked();

    void onUndoTransformClicked();
    void onResetTransformClicked();
    void handleTransformationApplied(const Matrix3x3::TransformStep &step, const Matrix3x3 &composite);
    void handleTransformationsReset();

    // 3. Integrated Fills
    void onFillAlgorithmChanged(int index);
    void onExecuteFillClicked();
    void onClearFillClicked();

    // 4. Shape Tools & Presets
    void onDrawPolygonClicked();
    void onDrawCircleClicked();
    void onDrawLineClicked();
    void onClosePolygonClicked();
    void onUndoShapeClicked();

    void onPresetTriangleClicked();
    void onPresetConvexClicked();
    void onPresetHouseClicked();
    void onPresetStarClicked();
    void onPresetConcaveClicked();
    void onPresetNestedClicked();
    void onPresetMultiSceneClicked();

    // 5. Display & Canvas Overlays
    void onGridSizeChanged(int size);
    void onToggleShowGhost(bool checked);
    void onToggleShowPivot(bool checked);
    void onToggleShowReflectLine(bool checked);
    void onToggleShowLabels(bool checked);
    void onToggleShowGrid(bool checked);
    void onToggleShowAxes(bool checked);
    void onClearAllClicked();

    // 6. Clipping Studio
    void onClipXminChanged(int val);
    void onClipYminChanged(int val);
    void onClipXmaxChanged(int val);
    void onClipYmaxChanged(int val);
    void onClipPresetCentered();
    void onClipPresetWide();
    void onClipPresetTall();
    void onClipPresetQuadrant();
    void onClipPresetSmall();
    void onRunCohenSutherland();
    void onRunLiangBarsky();
    void onRunSutherlandHodgman();
    void onRunWeilerAtherton();
    void onLoadCSTestLines();
    void onLoadSHTriangle();
    void onLoadSHStar();
    void onLoadSHLShape();
    void onLoadSHNepal();
    void onToggleSplitDisjointSH(bool checked);
    void onSHStageChanged(int val);
    void onToggleClipWindow(bool checked);
    void onToggleOutcodeLabels(bool checked);
    void onToggleClipResult(bool checked);
    void onDragClipRegion();
    void handleClipWindowChanged(double xmin, double ymin, double xmax, double ymax);
    void handleClippingFinished(const QString &algorithm, int count);

    // 7. View Navigation
    void onResetView();
    void onFitToView();
    void handleViewChanged(int gridSize);

    // Canvas signals
    void handleMouseMoved(int logX, int logY, int scrX, int scrY);
    void handlePivotChanged(QPoint pivot);
    void handleReflectLineChanged(double slope, double intercept);
    void handleFillFinished(const QString &algName, qint64 elapsedNanos, int pointCount);
    void handleStatusMessage(const QString &msg);

private:
    void setupDarkTheme();
    void buildUI();
    QWidget* buildClippingPanel();
    QWidget* buildTransformPanel();
    QWidget* buildShapePanel();
    QWidget* buildDisplayPanel();
    void setupConnections();
    void setupSpinBoxes();
    void updateMatrixInspector();
    void updateShapeDropdown();
    void updateCSInspector();
    void updateSHInspector();
    void setupReturnKeyHooks();

    Canvas *m_canvas;

    // Main Layout
    QSplitter *m_splitter;
    QScrollArea *m_sidebarScroll;
    QTabWidget *m_mainTabs;

    // Quick Command
    QLineEdit *txtQuickCommand;
    QPushButton *btnRunQuickCommand;

    // Selection
    QComboBox *comboSelectShape;
    QPushButton *btnDeselectAll;
    QLabel *lblScopeInfo;

    // Transform tabs
    QTabWidget *tabTransforms;

    // Translation
    QSpinBox *spinTx, *spinTy;
    QPushButton *btnApplyTranslate;
    QPushButton *btnNudgeUp, *btnNudgeDown, *btnNudgeLeft, *btnNudgeRight, *btnNudgeReset;

    // Rotation
    QDoubleSpinBox *spinRotAngle;
    QSpinBox *spinRotPx, *spinRotPy;
    QRadioButton *radioRotOrigin, *radioRotCentroid, *radioRotCustom;
    QPushButton *btnApplyRotate, *btnPickPivot;
    QPushButton *btnQuickRot45, *btnQuickRotNeg45, *btnQuickRot90, *btnQuickRot180;

    // Scaling
    QDoubleSpinBox *spinScaleX, *spinScaleY;
    QSpinBox *spinScalePx, *spinScalePy;
    QRadioButton *radioScaleOrigin, *radioScaleCentroid, *radioScaleCustom;
    QCheckBox *chkUniformScale;
    QPushButton *btnApplyScale;
    QPushButton *btnQuickScale2, *btnQuickScaleHalf, *btnQuickScale15, *btnQuickScaleFlip;

    // Shear
    QDoubleSpinBox *spinShearX, *spinShearY;
    QSpinBox *spinShearYRef, *spinShearXRef;
    QPushButton *btnApplyShear;

    // Reflection
    QDoubleSpinBox *spinReflectSlope, *spinReflectIntercept;
    QPushButton *btnReflectX, *btnReflectY, *btnReflectOrigin;
    QPushButton *btnReflectLineYeqX, *btnReflectLineYeqNegX;
    QPushButton *btnPickReflectLine, *btnApplyReflectArbitrary;

    // Custom Matrix
    QDoubleSpinBox *spinM00, *spinM01, *spinM02;
    QDoubleSpinBox *spinM10, *spinM11, *spinM12;
    QDoubleSpinBox *spinM20, *spinM21, *spinM22;
    QPushButton *btnApplyCustomMatrix;

    // Matrix Inspector
    QTextBrowser *txtMatrixDisplay;
    QLabel *lblMatrixDet, *lblMatrixFormula;
    QTableWidget *tableHistory;
    QPushButton *btnUndoTransform, *btnResetTransform;

    // Fills
    QComboBox *comboFillAlgorithm;
    QPushButton *btnExecuteFill, *btnClearFill;

    // Shape Tools
    QPushButton *btnDrawPolygon, *btnDrawCircle, *btnDrawLine, *btnClosePolygon, *btnUndoShape;
    QPushButton *btnPresetTriangle, *btnPresetConvex, *btnPresetHouse;
    QPushButton *btnPresetStar, *btnPresetConcave, *btnPresetNested, *btnPresetMultiScene;

    // Display
    QSpinBox *spinGridSize;
    QCheckBox *chkShowGhost, *chkShowPivot, *chkShowReflectLine;
    QCheckBox *chkShowLabels, *chkShowGrid, *chkShowAxes;
    QPushButton *btnClearAll;

    // Clipping Studio
    QSpinBox *spinClipXmin, *spinClipYmin, *spinClipXmax, *spinClipYmax;
    QPushButton *btnClipCentered, *btnClipWide, *btnClipTall, *btnClipQuadrant, *btnClipSmall;
    QPushButton *btnRunCS, *btnRunLB, *btnRunSH, *btnRunWA;
    QPushButton *btnLoadCSTest, *btnLoadSHTriangle, *btnLoadSHStar, *btnLoadSHLShape, *btnLoadSHNepal;
    QPushButton *btnDragClipRegion;
    QSlider *sliderSHStage;
    QLabel *lblSHStage;
    QCheckBox *chkShowClipWindow, *chkShowOutcodeLabels, *chkShowClipResult, *chkSplitDisjointSH;
    QTextBrowser *txtCSInspector, *txtSHInspector;

    // View
    QPushButton *btnResetView, *btnFitToView;
};

#endif // MAINWINDOW_H
