/****************************************************************************
** Meta object code from reading C++ file 'Keypadthread1.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Keypad/Keypadthread1.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Keypadthread1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Wholepadthread1[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      38,   36,   16,   16, 0x05,
      64,   16,   16,   16, 0x05,
      84,   16,   16,   16, 0x05,
     105,  101,   16,   16, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_Wholepadthread1[] = {
    "Wholepadthread1\0\0SIG_updatepos(int)\0"
    ",\0SIG_updateshow(bool,bool)\0"
    "SIG_keyboardclose()\0SIG_dealdelete()\0"
    "key\0SIG_getkey(char)\0"
};

void Wholepadthread1::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Wholepadthread1 *_t = static_cast<Wholepadthread1 *>(_o);
        switch (_id) {
        case 0: _t->SIG_updatepos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->SIG_updateshow((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->SIG_keyboardclose(); break;
        case 3: _t->SIG_dealdelete(); break;
        case 4: _t->SIG_getkey((*reinterpret_cast< char(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Wholepadthread1::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Wholepadthread1::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_Wholepadthread1,
      qt_meta_data_Wholepadthread1, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Wholepadthread1::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Wholepadthread1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Wholepadthread1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Wholepadthread1))
        return static_cast<void*>(const_cast< Wholepadthread1*>(this));
    return QThread::qt_metacast(_clname);
}

int Wholepadthread1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Wholepadthread1::SIG_updatepos(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Wholepadthread1::SIG_updateshow(bool _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Wholepadthread1::SIG_keyboardclose()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void Wholepadthread1::SIG_dealdelete()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void Wholepadthread1::SIG_getkey(char _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_END_MOC_NAMESPACE
