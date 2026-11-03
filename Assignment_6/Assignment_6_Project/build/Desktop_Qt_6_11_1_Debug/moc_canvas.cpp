/****************************************************************************
** Meta object code from reading C++ file 'canvas.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../canvas.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'canvas.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN6CanvasE_t {};
} // unnamed namespace

template <> constexpr inline auto Canvas::qt_create_metaobjectdata<qt_meta_tag_ZN6CanvasE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Canvas",
        "mouseMoved",
        "",
        "logX",
        "logY",
        "scrX",
        "scrY",
        "pointClicked",
        "QPoint",
        "logicalPt",
        "shapeSelectionChanged",
        "shapeId",
        "description",
        "transformationApplied",
        "Matrix3x3::TransformStep",
        "step",
        "Matrix3x3",
        "composite",
        "transformationsReset",
        "shapesChanged",
        "totalPolygons",
        "totalCircles",
        "seedPointSet",
        "seed",
        "pivotChanged",
        "pivot",
        "reflectLineChanged",
        "slope",
        "intercept",
        "fillFinished",
        "algorithmName",
        "elapsedNanos",
        "pointCount",
        "statusMessage",
        "msg",
        "clipWindowChanged",
        "xmin",
        "ymin",
        "xmax",
        "ymax",
        "clippingFinished",
        "algorithm",
        "linesClipped",
        "viewChanged",
        "gridSize"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'mouseMoved'
        QtMocHelpers::SignalData<void(int, int, int, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 }, { QMetaType::Int, 5 }, { QMetaType::Int, 6 },
        }}),
        // Signal 'pointClicked'
        QtMocHelpers::SignalData<void(QPoint)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'shapeSelectionChanged'
        QtMocHelpers::SignalData<void(int, const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::QString, 12 },
        }}),
        // Signal 'transformationApplied'
        QtMocHelpers::SignalData<void(const Matrix3x3::TransformStep &, const Matrix3x3 &)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 14, 15 }, { 0x80000000 | 16, 17 },
        }}),
        // Signal 'transformationsReset'
        QtMocHelpers::SignalData<void()>(18, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'shapesChanged'
        QtMocHelpers::SignalData<void(int, int)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 20 }, { QMetaType::Int, 21 },
        }}),
        // Signal 'seedPointSet'
        QtMocHelpers::SignalData<void(QPoint)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 23 },
        }}),
        // Signal 'pivotChanged'
        QtMocHelpers::SignalData<void(QPoint)>(24, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 25 },
        }}),
        // Signal 'reflectLineChanged'
        QtMocHelpers::SignalData<void(double, double)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 27 }, { QMetaType::Double, 28 },
        }}),
        // Signal 'fillFinished'
        QtMocHelpers::SignalData<void(const QString &, qint64, int)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 30 }, { QMetaType::LongLong, 31 }, { QMetaType::Int, 32 },
        }}),
        // Signal 'statusMessage'
        QtMocHelpers::SignalData<void(const QString &)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 34 },
        }}),
        // Signal 'clipWindowChanged'
        QtMocHelpers::SignalData<void(double, double, double, double)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 36 }, { QMetaType::Double, 37 }, { QMetaType::Double, 38 }, { QMetaType::Double, 39 },
        }}),
        // Signal 'clippingFinished'
        QtMocHelpers::SignalData<void(const QString &, int)>(40, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 41 }, { QMetaType::Int, 42 },
        }}),
        // Signal 'viewChanged'
        QtMocHelpers::SignalData<void(int)>(43, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 44 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Canvas, qt_meta_tag_ZN6CanvasE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Canvas::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6CanvasE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6CanvasE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN6CanvasE_t>.metaTypes,
    nullptr
} };

void Canvas::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Canvas *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->mouseMoved((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 1: _t->pointClicked((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 2: _t->shapeSelectionChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->transformationApplied((*reinterpret_cast<std::add_pointer_t<Matrix3x3::TransformStep>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<Matrix3x3>>(_a[2]))); break;
        case 4: _t->transformationsReset(); break;
        case 5: _t->shapesChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->seedPointSet((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 7: _t->pivotChanged((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 8: _t->reflectLineChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 9: _t->fillFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<qint64>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3]))); break;
        case 10: _t->statusMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->clipWindowChanged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[4]))); break;
        case 12: _t->clippingFinished((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 13: _t->viewChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(int , int , int , int )>(_a, &Canvas::mouseMoved, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(QPoint )>(_a, &Canvas::pointClicked, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(int , const QString & )>(_a, &Canvas::shapeSelectionChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(const Matrix3x3::TransformStep & , const Matrix3x3 & )>(_a, &Canvas::transformationApplied, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)()>(_a, &Canvas::transformationsReset, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(int , int )>(_a, &Canvas::shapesChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(QPoint )>(_a, &Canvas::seedPointSet, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(QPoint )>(_a, &Canvas::pivotChanged, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(double , double )>(_a, &Canvas::reflectLineChanged, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(const QString & , qint64 , int )>(_a, &Canvas::fillFinished, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(const QString & )>(_a, &Canvas::statusMessage, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(double , double , double , double )>(_a, &Canvas::clipWindowChanged, 11))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(const QString & , int )>(_a, &Canvas::clippingFinished, 12))
            return;
        if (QtMocHelpers::indexOfMethod<void (Canvas::*)(int )>(_a, &Canvas::viewChanged, 13))
            return;
    }
}

const QMetaObject *Canvas::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Canvas::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN6CanvasE_t>.strings))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int Canvas::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void Canvas::mouseMoved(int _t1, int _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 1
void Canvas::pointClicked(QPoint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void Canvas::shapeSelectionChanged(int _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void Canvas::transformationApplied(const Matrix3x3::TransformStep & _t1, const Matrix3x3 & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}

// SIGNAL 4
void Canvas::transformationsReset()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void Canvas::shapesChanged(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void Canvas::seedPointSet(QPoint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void Canvas::pivotChanged(QPoint _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void Canvas::reflectLineChanged(double _t1, double _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1, _t2);
}

// SIGNAL 9
void Canvas::fillFinished(const QString & _t1, qint64 _t2, int _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2, _t3);
}

// SIGNAL 10
void Canvas::statusMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void Canvas::clipWindowChanged(double _t1, double _t2, double _t3, double _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 12
void Canvas::clippingFinished(const QString & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 12, nullptr, _t1, _t2);
}

// SIGNAL 13
void Canvas::viewChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 13, nullptr, _t1);
}
QT_WARNING_POP
