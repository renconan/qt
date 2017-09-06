/****************************************************************************
** Meta object code from reading C++ file 'currentset.h'
**
** Created: Fri Jan 6 09:45:59 2017
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Currentset/currentset.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'currentset.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Currentset[] = {

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
      23,   12,   11,   11, 0x05,
      56,   51,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      71,   11,   11,   11, 0x08,
      95,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Currentset[] = {
    "Currentset\0\0value,type\0"
    "sendsettingvalue(void*,int)\0type\0"
    "sendclose(int)\0on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
};

const QMetaObject Currentset::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Currentset,
      qt_meta_data_Currentset, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Currentset::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Currentset::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Currentset::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Currentset))
        return static_cast<void*>(const_cast< Currentset*>(this));
    return QDialog::qt_metacast(_clname);
}

int Currentset::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void Currentset::sendsettingvalue(void * _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Currentset::sendclose(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
