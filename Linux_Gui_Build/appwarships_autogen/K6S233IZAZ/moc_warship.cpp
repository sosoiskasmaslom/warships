/****************************************************************************
** Meta object code from reading C++ file 'warship.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../headers/warship.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'warship.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
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
struct qt_meta_tag_ZN7WarshipE_t {};
} // unnamed namespace

template <> constexpr inline auto Warship::qt_create_metaobjectdata<qt_meta_tag_ZN7WarshipE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "Warship",
        "closeGame",
        "",
        "cellColor",
        "Point",
        "an_cellColor",
        "onCellClicked",
        "p"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'closeGame'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'cellColor'
        QtMocHelpers::SignalData<void(Point, unsigned )>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 2 }, { QMetaType::UInt, 2 },
        }}),
        // Signal 'an_cellColor'
        QtMocHelpers::SignalData<void(Point, bool)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 2 }, { QMetaType::Bool, 2 },
        }}),
        // Slot 'onCellClicked'
        QtMocHelpers::SlotData<void(const Point &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 4, 7 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<Warship, qt_meta_tag_ZN7WarshipE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject Warship::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7WarshipE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7WarshipE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN7WarshipE_t>.metaTypes,
    nullptr
} };

void Warship::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<Warship *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->closeGame(); break;
        case 1: _t->cellColor((*reinterpret_cast<std::add_pointer_t<Point>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<uint>>(_a[2]))); break;
        case 2: _t->an_cellColor((*reinterpret_cast<std::add_pointer_t<Point>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2]))); break;
        case 3: _t->onCellClicked((*reinterpret_cast<std::add_pointer_t<Point>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Point >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Point >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< Point >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (Warship::*)()>(_a, &Warship::closeGame, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (Warship::*)(Point , unsigned  )>(_a, &Warship::cellColor, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (Warship::*)(Point , bool )>(_a, &Warship::an_cellColor, 2))
            return;
    }
}

const QMetaObject *Warship::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Warship::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN7WarshipE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Warship::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Warship::closeGame()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void Warship::cellColor(Point _t1, unsigned  _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void Warship::an_cellColor(Point _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}
QT_WARNING_POP
