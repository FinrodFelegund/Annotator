/****************************************************************************
** Meta object code from reading C++ file 'AnnotatorController.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../AnnotatorApplication/AnnotatorController.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AnnotatorController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_AnnotatorController_t {
    uint offsetsAndSizes[22];
    char stringdata0[20];
    char stringdata1[16];
    char stringdata2[1];
    char stringdata3[12];
    char stringdata4[9];
    char stringdata5[19];
    char stringdata6[5];
    char stringdata7[15];
    char stringdata8[8];
    char stringdata9[14];
    char stringdata10[14];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(sizeof(qt_meta_stringdata_AnnotatorController_t::offsetsAndSizes) + ofs), len 
static const qt_meta_stringdata_AnnotatorController_t qt_meta_stringdata_AnnotatorController = {
    {
        QT_MOC_LITERAL(0, 19),  // "AnnotatorController"
        QT_MOC_LITERAL(20, 15),  // "initializeImage"
        QT_MOC_LITERAL(36, 0),  // ""
        QT_MOC_LITERAL(37, 11),  // "std::string"
        QT_MOC_LITERAL(49, 8),  // "fileName"
        QT_MOC_LITERAL(58, 18),  // "fieldOfViewChanged"
        QT_MOC_LITERAL(77, 4),  // "rect"
        QT_MOC_LITERAL(82, 14),  // "clickTriggered"
        QT_MOC_LITERAL(97, 7),  // "checked"
        QT_MOC_LITERAL(105, 13),  // "drawTriggered"
        QT_MOC_LITERAL(119, 13)   // "exitTriggered"
    },
    "AnnotatorController",
    "initializeImage",
    "",
    "std::string",
    "fileName",
    "fieldOfViewChanged",
    "rect",
    "clickTriggered",
    "checked",
    "drawTriggered",
    "exitTriggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AnnotatorController[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x0a,    1 /* Public */,
       5,    1,   47,    2, 0x0a,    3 /* Public */,
       7,    1,   50,    2, 0x0a,    5 /* Public */,
       9,    1,   53,    2, 0x0a,    7 /* Public */,
      10,    1,   56,    2, 0x0a,    9 /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QRectF,    6,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,
    QMetaType::Void, QMetaType::Bool,    8,

       0        // eod
};

void AnnotatorController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<AnnotatorController *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->initializeImage((*reinterpret_cast< std::add_pointer_t<std::string>>(_a[1]))); break;
        case 1: _t->fieldOfViewChanged((*reinterpret_cast< std::add_pointer_t<QRectF>>(_a[1]))); break;
        case 2: _t->clickTriggered((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 3: _t->drawTriggered((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 4: _t->exitTriggered((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject AnnotatorController::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_AnnotatorController.offsetsAndSizes,
    qt_meta_data_AnnotatorController,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_AnnotatorController_t
, QtPrivate::TypeAndForceComplete<AnnotatorController, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<std::string, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QRectF, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>


>,
    nullptr
} };


const QMetaObject *AnnotatorController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AnnotatorController::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_AnnotatorController.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int AnnotatorController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
