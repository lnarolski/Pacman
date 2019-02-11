/****************************************************************************
** Meta object code from reading C++ file 'gra.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../gra.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gra.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Gra_t {
    QByteArrayData data[8];
    char stringdata0[103];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Gra_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Gra_t qt_meta_stringdata_Gra = {
    {
QT_MOC_LITERAL(0, 0, 3), // "Gra"
QT_MOC_LITERAL(1, 4, 13), // "dzialanie_gry"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 9), // "aktywacja"
QT_MOC_LITERAL(4, 29, 25), // "mozliwosc_zjadania_duchow"
QT_MOC_LITERAL(5, 55, 15), // "aktywacja_duch0"
QT_MOC_LITERAL(6, 71, 15), // "aktywacja_duch1"
QT_MOC_LITERAL(7, 87, 15) // "aktywacja_duch2"

    },
    "Gra\0dzialanie_gry\0\0aktywacja\0"
    "mozliwosc_zjadania_duchow\0aktywacja_duch0\0"
    "aktywacja_duch1\0aktywacja_duch2"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Gra[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x08 /* Private */,
       3,    0,   45,    2, 0x08 /* Private */,
       4,    0,   46,    2, 0x08 /* Private */,
       5,    0,   47,    2, 0x08 /* Private */,
       6,    0,   48,    2, 0x08 /* Private */,
       7,    0,   49,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Gra::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Gra *_t = static_cast<Gra *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dzialanie_gry(); break;
        case 1: _t->aktywacja(); break;
        case 2: _t->mozliwosc_zjadania_duchow(); break;
        case 3: _t->aktywacja_duch0(); break;
        case 4: _t->aktywacja_duch1(); break;
        case 5: _t->aktywacja_duch2(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject Gra::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Gra.data,
      qt_meta_data_Gra,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Gra::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Gra::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Gra.stringdata0))
        return static_cast<void*>(const_cast< Gra*>(this));
    return QWidget::qt_metacast(_clname);
}

int Gra::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
