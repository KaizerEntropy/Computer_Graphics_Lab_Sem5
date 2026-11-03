/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../mainwindow.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "onRunQuickCommand",
        "",
        "onSelectShapeComboChanged",
        "index",
        "onDeselectAllClicked",
        "handleShapeSelectionChanged",
        "shapeId",
        "description",
        "handleShapesChanged",
        "totalPolygons",
        "totalCircles",
        "onApplyTranslationClicked",
        "onNudgeUp",
        "onNudgeDown",
        "onNudgeLeft",
        "onNudgeRight",
        "onNudgeReset",
        "onApplyRotationClicked",
        "onPickPivotClicked",
        "onQuickRot45",
        "onQuickRotNeg45",
        "onQuickRot90",
        "onQuickRot180",
        "onApplyScalingClicked",
        "onUniformScaleToggled",
        "checked",
        "onScaleXChanged",
        "val",
        "onQuickScale2",
        "onQuickScaleHalf",
        "onQuickScale15",
        "onQuickScaleFlip",
        "onApplyShearClicked",
        "onReflectXClicked",
        "onReflectYClicked",
        "onReflectOriginClicked",
        "onReflectLineYeqXClicked",
        "onReflectLineYeqNegXClicked",
        "onPickReflectLineClicked",
        "onApplyReflectArbitraryClicked",
        "onApplyCustomMatrixClicked",
        "onUndoTransformClicked",
        "onResetTransformClicked",
        "handleTransformationApplied",
        "Matrix3x3::TransformStep",
        "step",
        "Matrix3x3",
        "composite",
        "handleTransformationsReset",
        "onFillAlgorithmChanged",
        "onExecuteFillClicked",
        "onClearFillClicked",
        "onDrawPolygonClicked",
        "onDrawCircleClicked",
        "onDrawLineClicked",
        "onClosePolygonClicked",
        "onUndoShapeClicked",
        "onPresetTriangleClicked",
        "onPresetConvexClicked",
        "onPresetHouseClicked",
        "onPresetStarClicked",
        "onPresetConcaveClicked",
        "onPresetNestedClicked",
        "onPresetMultiSceneClicked",
        "onGridSizeChanged",
        "size",
        "onToggleShowGhost",
        "onToggleShowPivot",
        "onToggleShowReflectLine",
        "onToggleShowLabels",
        "onToggleShowGrid",
        "onToggleShowAxes",
        "onClearAllClicked",
        "onClipXminChanged",
        "onClipYminChanged",
        "onClipXmaxChanged",
        "onClipYmaxChanged",
        "onClipPresetCentered",
        "onClipPresetWide",
        "onClipPresetTall",
        "onClipPresetQuadrant",
        "onClipPresetSmall",
        "onRunCohenSutherland",
        "onRunLiangBarsky",
        "onRunSutherlandHodgman",
        "onRunWeilerAtherton",
        "onLoadCSTestLines",
        "onLoadSHTriangle",
        "onLoadSHStar",
        "onLoadSHLShape",
        "onLoadSHNepal",
        "onToggleSplitDisjointSH",
        "onSHStageChanged",
        "onToggleClipWindow",
        "onToggleOutcodeLabels",
        "onToggleClipResult",
        "onDragClipRegion",
        "handleClipWindowChanged",
        "xmin",
        "ymin",
        "xmax",
        "ymax",
        "handleClippingFinished",
        "algorithm",
        "count",
        "onResetView",
        "onFitToView",
        "handleViewChanged",
        "gridSize",
        "handleMouseMoved",
        "logX",
        "logY",
        "scrX",
        "scrY",
        "handlePivotChanged",
        "QPoint",
        "pivot",
        "handleReflectLineChanged",
        "slope",
        "intercept",
        "handleFillFinished",
        "algName",
        "elapsedNanos",
        "pointCount",
        "handleStatusMessage",
        "msg"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'onRunQuickCommand'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSelectShapeComboChanged'
        QtMocHelpers::SlotData<void(int)>(3, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Slot 'onDeselectAllClicked'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleShapeSelectionChanged'
        QtMocHelpers::SlotData<void(int, const QString &)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 7 }, { QMetaType::QString, 8 },
        }}),
        // Slot 'handleShapesChanged'
        QtMocHelpers::SlotData<void(int, int)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 10 }, { QMetaType::Int, 11 },
        }}),
        // Slot 'onApplyTranslationClicked'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNudgeUp'
        QtMocHelpers::SlotData<void()>(13, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNudgeDown'
        QtMocHelpers::SlotData<void()>(14, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNudgeLeft'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNudgeRight'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onNudgeReset'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onApplyRotationClicked'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPickPivotClicked'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuickRot45'
        QtMocHelpers::SlotData<void()>(20, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuickRotNeg45'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuickRot90'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuickRot180'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onApplyScalingClicked'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onUniformScaleToggled'
        QtMocHelpers::SlotData<void(bool)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onScaleXChanged'
        QtMocHelpers::SlotData<void(double)>(27, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 28 },
        }}),
        // Slot 'onQuickScale2'
        QtMocHelpers::SlotData<void()>(29, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuickScaleHalf'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuickScale15'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onQuickScaleFlip'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onApplyShearClicked'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReflectXClicked'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReflectYClicked'
        QtMocHelpers::SlotData<void()>(35, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReflectOriginClicked'
        QtMocHelpers::SlotData<void()>(36, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReflectLineYeqXClicked'
        QtMocHelpers::SlotData<void()>(37, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onReflectLineYeqNegXClicked'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPickReflectLineClicked'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onApplyReflectArbitraryClicked'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onApplyCustomMatrixClicked'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onUndoTransformClicked'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onResetTransformClicked'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleTransformationApplied'
        QtMocHelpers::SlotData<void(const Matrix3x3::TransformStep &, const Matrix3x3 &)>(44, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 45, 46 }, { 0x80000000 | 47, 48 },
        }}),
        // Slot 'handleTransformationsReset'
        QtMocHelpers::SlotData<void()>(49, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFillAlgorithmChanged'
        QtMocHelpers::SlotData<void(int)>(50, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 4 },
        }}),
        // Slot 'onExecuteFillClicked'
        QtMocHelpers::SlotData<void()>(51, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClearFillClicked'
        QtMocHelpers::SlotData<void()>(52, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDrawPolygonClicked'
        QtMocHelpers::SlotData<void()>(53, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDrawCircleClicked'
        QtMocHelpers::SlotData<void()>(54, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onDrawLineClicked'
        QtMocHelpers::SlotData<void()>(55, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClosePolygonClicked'
        QtMocHelpers::SlotData<void()>(56, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onUndoShapeClicked'
        QtMocHelpers::SlotData<void()>(57, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPresetTriangleClicked'
        QtMocHelpers::SlotData<void()>(58, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPresetConvexClicked'
        QtMocHelpers::SlotData<void()>(59, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPresetHouseClicked'
        QtMocHelpers::SlotData<void()>(60, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPresetStarClicked'
        QtMocHelpers::SlotData<void()>(61, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPresetConcaveClicked'
        QtMocHelpers::SlotData<void()>(62, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPresetNestedClicked'
        QtMocHelpers::SlotData<void()>(63, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onPresetMultiSceneClicked'
        QtMocHelpers::SlotData<void()>(64, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onGridSizeChanged'
        QtMocHelpers::SlotData<void(int)>(65, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 66 },
        }}),
        // Slot 'onToggleShowGhost'
        QtMocHelpers::SlotData<void(bool)>(67, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onToggleShowPivot'
        QtMocHelpers::SlotData<void(bool)>(68, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onToggleShowReflectLine'
        QtMocHelpers::SlotData<void(bool)>(69, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onToggleShowLabels'
        QtMocHelpers::SlotData<void(bool)>(70, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onToggleShowGrid'
        QtMocHelpers::SlotData<void(bool)>(71, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onToggleShowAxes'
        QtMocHelpers::SlotData<void(bool)>(72, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onClearAllClicked'
        QtMocHelpers::SlotData<void()>(73, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClipXminChanged'
        QtMocHelpers::SlotData<void(int)>(74, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 28 },
        }}),
        // Slot 'onClipYminChanged'
        QtMocHelpers::SlotData<void(int)>(75, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 28 },
        }}),
        // Slot 'onClipXmaxChanged'
        QtMocHelpers::SlotData<void(int)>(76, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 28 },
        }}),
        // Slot 'onClipYmaxChanged'
        QtMocHelpers::SlotData<void(int)>(77, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 28 },
        }}),
        // Slot 'onClipPresetCentered'
        QtMocHelpers::SlotData<void()>(78, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClipPresetWide'
        QtMocHelpers::SlotData<void()>(79, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClipPresetTall'
        QtMocHelpers::SlotData<void()>(80, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClipPresetQuadrant'
        QtMocHelpers::SlotData<void()>(81, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onClipPresetSmall'
        QtMocHelpers::SlotData<void()>(82, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRunCohenSutherland'
        QtMocHelpers::SlotData<void()>(83, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRunLiangBarsky'
        QtMocHelpers::SlotData<void()>(84, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRunSutherlandHodgman'
        QtMocHelpers::SlotData<void()>(85, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onRunWeilerAtherton'
        QtMocHelpers::SlotData<void()>(86, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLoadCSTestLines'
        QtMocHelpers::SlotData<void()>(87, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLoadSHTriangle'
        QtMocHelpers::SlotData<void()>(88, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLoadSHStar'
        QtMocHelpers::SlotData<void()>(89, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLoadSHLShape'
        QtMocHelpers::SlotData<void()>(90, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLoadSHNepal'
        QtMocHelpers::SlotData<void()>(91, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onToggleSplitDisjointSH'
        QtMocHelpers::SlotData<void(bool)>(92, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onSHStageChanged'
        QtMocHelpers::SlotData<void(int)>(93, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 28 },
        }}),
        // Slot 'onToggleClipWindow'
        QtMocHelpers::SlotData<void(bool)>(94, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onToggleOutcodeLabels'
        QtMocHelpers::SlotData<void(bool)>(95, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onToggleClipResult'
        QtMocHelpers::SlotData<void(bool)>(96, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 26 },
        }}),
        // Slot 'onDragClipRegion'
        QtMocHelpers::SlotData<void()>(97, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleClipWindowChanged'
        QtMocHelpers::SlotData<void(double, double, double, double)>(98, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 99 }, { QMetaType::Double, 100 }, { QMetaType::Double, 101 }, { QMetaType::Double, 102 },
        }}),
        // Slot 'handleClippingFinished'
        QtMocHelpers::SlotData<void(const QString &, int)>(103, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 104 }, { QMetaType::Int, 105 },
        }}),
        // Slot 'onResetView'
        QtMocHelpers::SlotData<void()>(106, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onFitToView'
        QtMocHelpers::SlotData<void()>(107, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'handleViewChanged'
        QtMocHelpers::SlotData<void(int)>(108, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 109 },
        }}),
        // Slot 'handleMouseMoved'
        QtMocHelpers::SlotData<void(int, int, int, int)>(110, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 111 }, { QMetaType::Int, 112 }, { QMetaType::Int, 113 }, { QMetaType::Int, 114 },
        }}),
        // Slot 'handlePivotChanged'
        QtMocHelpers::SlotData<void(QPoint)>(115, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 116, 117 },
        }}),
        // Slot 'handleReflectLineChanged'
        QtMocHelpers::SlotData<void(double, double)>(118, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 119 }, { QMetaType::Double, 120 },
        }}),
        // Slot 'handleFillFinished'
        QtMocHelpers::SlotData<void(const QString &, qint64, int)>(121, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 122 }, { QMetaType::LongLong, 123 }, { QMetaType::Int, 124 },
        }}),
        // Slot 'handleStatusMessage'
        QtMocHelpers::SlotData<void(const QString &)>(125, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 126 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->onRunQuickCommand(); break;
        case 1: _t->onSelectShapeComboChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 2: _t->onDeselectAllClicked(); break;
        case 3: _t->handleShapeSelectionChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->handleShapesChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->onApplyTranslationClicked(); break;
        case 6: _t->onNudgeUp(); break;
        case 7: _t->onNudgeDown(); break;
        case 8: _t->onNudgeLeft(); break;
        case 9: _t->onNudgeRight(); break;
        case 10: _t->onNudgeReset(); break;
        case 11: _t->onApplyRotationClicked(); break;
        case 12: _t->onPickPivotClicked(); break;
        case 13: _t->onQuickRot45(); break;
        case 14: _t->onQuickRotNeg45(); break;
        case 15: _t->onQuickRot90(); break;
        case 16: _t->onQuickRot180(); break;
        case 17: _t->onApplyScalingClicked(); break;
        case 18: _t->onUniformScaleToggled((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 19: _t->onScaleXChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 20: _t->onQuickScale2(); break;
        case 21: _t->onQuickScaleHalf(); break;
        case 22: _t->onQuickScale15(); break;
        case 23: _t->onQuickScaleFlip(); break;
        case 24: _t->onApplyShearClicked(); break;
        case 25: _t->onReflectXClicked(); break;
        case 26: _t->onReflectYClicked(); break;
        case 27: _t->onReflectOriginClicked(); break;
        case 28: _t->onReflectLineYeqXClicked(); break;
        case 29: _t->onReflectLineYeqNegXClicked(); break;
        case 30: _t->onPickReflectLineClicked(); break;
        case 31: _t->onApplyReflectArbitraryClicked(); break;
        case 32: _t->onApplyCustomMatrixClicked(); break;
        case 33: _t->onUndoTransformClicked(); break;
        case 34: _t->onResetTransformClicked(); break;
        case 35: _t->handleTransformationApplied((*reinterpret_cast<std::add_pointer_t<Matrix3x3::TransformStep>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<Matrix3x3>>(_a[2]))); break;
        case 36: _t->handleTransformationsReset(); break;
        case 37: _t->onFillAlgorithmChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 38: _t->onExecuteFillClicked(); break;
        case 39: _t->onClearFillClicked(); break;
        case 40: _t->onDrawPolygonClicked(); break;
        case 41: _t->onDrawCircleClicked(); break;
        case 42: _t->onDrawLineClicked(); break;
        case 43: _t->onClosePolygonClicked(); break;
        case 44: _t->onUndoShapeClicked(); break;
        case 45: _t->onPresetTriangleClicked(); break;
        case 46: _t->onPresetConvexClicked(); break;
        case 47: _t->onPresetHouseClicked(); break;
        case 48: _t->onPresetStarClicked(); break;
        case 49: _t->onPresetConcaveClicked(); break;
        case 50: _t->onPresetNestedClicked(); break;
        case 51: _t->onPresetMultiSceneClicked(); break;
        case 52: _t->onGridSizeChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 53: _t->onToggleShowGhost((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 54: _t->onToggleShowPivot((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 55: _t->onToggleShowReflectLine((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 56: _t->onToggleShowLabels((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 57: _t->onToggleShowGrid((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 58: _t->onToggleShowAxes((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 59: _t->onClearAllClicked(); break;
        case 60: _t->onClipXminChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 61: _t->onClipYminChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 62: _t->onClipXmaxChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 63: _t->onClipYmaxChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 64: _t->onClipPresetCentered(); break;
        case 65: _t->onClipPresetWide(); break;
        case 66: _t->onClipPresetTall(); break;
        case 67: _t->onClipPresetQuadrant(); break;
        case 68: _t->onClipPresetSmall(); break;
        case 69: _t->onRunCohenSutherland(); break;
        case 70: _t->onRunLiangBarsky(); break;
        case 71: _t->onRunSutherlandHodgman(); break;
        case 72: _t->onRunWeilerAtherton(); break;
        case 73: _t->onLoadCSTestLines(); break;
        case 74: _t->onLoadSHTriangle(); break;
        case 75: _t->onLoadSHStar(); break;
        case 76: _t->onLoadSHLShape(); break;
        case 77: _t->onLoadSHNepal(); break;
        case 78: _t->onToggleSplitDisjointSH((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 79: _t->onSHStageChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 80: _t->onToggleClipWindow((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 81: _t->onToggleOutcodeLabels((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 82: _t->onToggleClipResult((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1]))); break;
        case 83: _t->onDragClipRegion(); break;
        case 84: _t->handleClipWindowChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4]))); break;
        case 85: _t->handleClippingFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 86: _t->onResetView(); break;
        case 87: _t->onFitToView(); break;
        case 88: _t->handleViewChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 89: _t->handleMouseMoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 90: _t->handlePivotChanged((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 91: _t->handleReflectLineChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 92: _t->handleFillFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 93: _t->handleStatusMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 94)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 94;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 94)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 94;
    }
    return _id;
}
QT_WARNING_POP
