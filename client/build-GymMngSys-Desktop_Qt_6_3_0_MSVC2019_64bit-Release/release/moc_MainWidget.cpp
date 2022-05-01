/****************************************************************************
** Meta object code from reading C++ file 'MainWidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../GymMngSys/MainWidget.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWidget_t {
    const uint offsetsAndSize[32];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_MainWidget_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_MainWidget_t qt_meta_stringdata_MainWidget = {
    {
QT_MOC_LITERAL(0, 10), // "MainWidget"
QT_MOC_LITERAL(11, 16), // "flush_ListWidget"
QT_MOC_LITERAL(28, 0), // ""
QT_MOC_LITERAL(29, 12), // "change_Table"
QT_MOC_LITERAL(42, 10), // "add_Msgbox"
QT_MOC_LITERAL(53, 10), // "add_Rprbox"
QT_MOC_LITERAL(64, 9), // "add_RCbox"
QT_MOC_LITERAL(74, 8), // "del_func"
QT_MOC_LITERAL(83, 7), // "del_rpr"
QT_MOC_LITERAL(91, 7), // "in_func"
QT_MOC_LITERAL(99, 8), // "out_func"
QT_MOC_LITERAL(108, 11), // "flush_Table"
QT_MOC_LITERAL(120, 8), // "find_Msg"
QT_MOC_LITERAL(129, 10), // "flush_Info"
QT_MOC_LITERAL(140, 8), // "recv_Msg"
QT_MOC_LITERAL(149, 8) // "add_Ubox"

    },
    "MainWidget\0flush_ListWidget\0\0change_Table\0"
    "add_Msgbox\0add_Rprbox\0add_RCbox\0"
    "del_func\0del_rpr\0in_func\0out_func\0"
    "flush_Table\0find_Msg\0flush_Info\0"
    "recv_Msg\0add_Ubox"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWidget[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   98,    2, 0x0a,    1 /* Public */,
       3,    2,  101,    2, 0x0a,    3 /* Public */,
       4,    0,  106,    2, 0x0a,    6 /* Public */,
       5,    0,  107,    2, 0x0a,    7 /* Public */,
       6,    0,  108,    2, 0x0a,    8 /* Public */,
       7,    0,  109,    2, 0x0a,    9 /* Public */,
       8,    0,  110,    2, 0x0a,   10 /* Public */,
       9,    0,  111,    2, 0x0a,   11 /* Public */,
      10,    0,  112,    2, 0x0a,   12 /* Public */,
      11,    0,  113,    2, 0x0a,   13 /* Public */,
      12,    0,  114,    2, 0x0a,   14 /* Public */,
      13,    0,  115,    2, 0x0a,   15 /* Public */,
      14,    0,  116,    2, 0x0a,   16 /* Public */,
      15,    0,  117,    2, 0x0a,   17 /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    2,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->flush_ListWidget((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 1: _t->change_Table((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->add_Msgbox(); break;
        case 3: _t->add_Rprbox(); break;
        case 4: _t->add_RCbox(); break;
        case 5: _t->del_func(); break;
        case 6: _t->del_rpr(); break;
        case 7: _t->in_func(); break;
        case 8: _t->out_func(); break;
        case 9: _t->flush_Table(); break;
        case 10: _t->find_Msg(); break;
        case 11: _t->flush_Info(); break;
        case 12: _t->recv_Msg(); break;
        case 13: _t->add_Ubox(); break;
        default: ;
        }
    }
}

const QMetaObject MainWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_MainWidget.offsetsAndSize,
    qt_meta_data_MainWidget,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_MainWidget_t
, QtPrivate::TypeAndForceComplete<MainWidget, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<int, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *MainWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int MainWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
