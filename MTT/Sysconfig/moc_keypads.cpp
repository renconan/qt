/****************************************************************************
** Meta object code from reading C++ file 'keypads.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "keypads.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'keypads.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_keypads[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      28,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,    8,    8,    8, 0x08,
      69,    8,    8,    8, 0x08,
      99,    8,    8,    8, 0x08,
     128,    8,    8,    8, 0x08,
     158,    8,    8,    8, 0x08,
     187,    8,    8,    8, 0x08,
     213,    8,    8,    8, 0x08,
     239,    8,    8,    8, 0x08,
     265,    8,    8,    8, 0x08,
     291,    8,    8,    8, 0x08,
     317,    8,    8,    8, 0x08,
     343,    8,    8,    8, 0x08,
     369,    8,    8,    8, 0x08,
     395,    8,    8,    8, 0x08,
     421,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_keypads[] = {
    "keypads\0\0sendvalue(QString)\0sendflag()\0"
    "on_toolButton_enter_clicked()\0"
    "on_toolButton_clear_clicked()\0"
    "on_toolButton_back_clicked()\0"
    "on_toolButton_point_clicked()\0"
    "on_toolButton_sign_clicked()\0"
    "on_toolButton_0_clicked()\0"
    "on_toolButton_1_clicked()\0"
    "on_toolButton_2_clicked()\0"
    "on_toolButton_3_clicked()\0"
    "on_toolButton_4_clicked()\0"
    "on_toolButton_5_clicked()\0"
    "on_toolButton_6_clicked()\0"
    "on_toolButton_7_clicked()\0"
    "on_toolButton_8_clicked()\0"
    "on_toolButton_9_clicked()\0"
};

void keypads::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        keypads *_t = static_cast<keypads *>(_o);
        switch (_id) {
        case 0: _t->sendvalue((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->sendflag(); break;
        case 2: _t->on_toolButton_enter_clicked(); break;
        case 3: _t->on_toolButton_clear_clicked(); break;
        case 4: _t->on_toolButton_back_clicked(); break;
        case 5: _t->on_toolButton_point_clicked(); break;
        case 6: _t->on_toolButton_sign_clicked(); break;
        case 7: _t->on_toolButton_0_clicked(); break;
        case 8: _t->on_toolButton_1_clicked(); break;
        case 9: _t->on_toolButton_2_clicked(); break;
        case 10: _t->on_toolButton_3_clicked(); break;
        case 11: _t->on_toolButton_4_clicked(); break;
        case 12: _t->on_toolButton_5_clicked(); break;
        case 13: _t->on_toolButton_6_clicked(); break;
        case 14: _t->on_toolButton_7_clicked(); break;
        case 15: _t->on_toolButton_8_clicked(); break;
        case 16: _t->on_toolButton_9_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData keypads::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject keypads::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_keypads,
      qt_meta_data_keypads, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &keypads::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *keypads::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *keypads::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_keypads))
        return static_cast<void*>(const_cast< keypads*>(this));
    return QDialog::qt_metacast(_clname);
}

int keypads::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void keypads::sendvalue(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void keypads::sendflag()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
