#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "canvas.h"
#include "matrix3x3.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

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

    // Canvas signals
    void handleMouseMoved(int logX, int logY, int scrX, int scrY);
    void handlePivotChanged(QPoint pivot);
    void handleReflectLineChanged(double slope, double intercept);
    void handleFillFinished(const QString &algName, qint64 elapsedNanos, int pointCount);
    void handleStatusMessage(const QString &msg);

private:
    void setupDarkTheme();
    void setupSpinBoxes();
    void updateMatrixInspector();
    void updateShapeDropdown();
    void setupReturnKeyHooks();

    Ui::MainWindow *ui;
    Canvas *m_canvas;
};

#endif // MAINWINDOW_H
