/****************************************************************************
** Meta object code from reading C++ file 'thread1.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "thread1.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'thread1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_thread1[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,    9,    8,    8, 0x05,
      33,    9,    8,    8, 0x05,
      56,   54,    8,    8, 0x05,
      77,    8,    8,    8, 0x05,
      96,    8,    8,    8, 0x05,
     112,    8,    8,    8, 0x05,
     133,  131,    8,    8, 0x05,
     158,    8,    8,    8, 0x05,
     179,  174,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
     211,    8,    8,    8, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_thread1[] = {
    "thread1\0\0state\0SIG_dspstate(int)\0"
    "SIG_binarystate(int)\0k\0SIG_testresult(bool)\0"
    "SIG_setVanocheck()\0SIG_SendClose()\0"
    "SIG_Teststate(int)\0,\0SIG_SendWarning(int,int)\0"
    "SIG_TestState()\0flag\0"
    "SIG_DisplayAutoChangeValue(int)\0"
    "Manualoutput()\0"
};

void thread1::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        thread1 *_t = static_cast<thread1 *>(_o);
        switch (_id) {
        case 0: _t->SIG_dspstate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->SIG_binarystate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->SIG_testresult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->SIG_setVanocheck(); break;
        case 4: _t->SIG_SendClose(); break;
        case 5: _t->SIG_Teststate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->SIG_SendWarning((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->SIG_TestState(); break;
        case 8: _t->SIG_DisplayAutoChangeValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 9: _t->Manualoutput(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData thread1::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject thread1::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_thread1,
      qt_meta_data_thread1, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &thread1::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *thread1::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *thread1::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_thread1))
        return static_cast<void*>(const_cast< thread1*>(this));
    return QThread::qt_metacast(_clname);
}

int thread1::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void thread1::SIG_dspstate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void thread1::SIG_binarystate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void thread1::SIG_testresult(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void thread1::SIG_setVanocheck()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void thread1::SIG_SendClose()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}

// SIGNAL 5
void thread1::SIG_Teststate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void thread1::SIG_SendWarning(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void thread1::SIG_TestState()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void thread1::SIG_DisplayAutoChangeValue(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_END_MOC_NAMESPACE
