/****************************************************************************
** Meta object code from reading C++ file 'anytest.h'
**
** Created: Fri Jan 6 09:46:05 2017
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Anytest/anytest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'anytest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Anytest[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      20,    9,    8,    8, 0x05,
      53,   48,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      68,    8,    8,    8, 0x08,
      92,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Anytest[] = {
    "Anytest\0\0value,type\0sendsettingvalue(void*,int)\0"
    "type\0sendclose(int)\0on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
};

const QMetaObject Anytest::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Anytest,
      qt_meta_data_Anytest, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Anytest::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Anytest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Anytest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Anytest))
        return static_cast<void*>(const_cast< Anytest*>(this));
    return QWidget::qt_metacast(_clname);
}

int Anytest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendsettingvalue((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: sendclose((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: on_pushButton_clicked(); break;
        case 3: on_pushButton_2_clicked(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void Anytest::sendsettingvalue(void * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Anytest::sendclose(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE