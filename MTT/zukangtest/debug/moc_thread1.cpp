/****************************************************************************
** Meta object code from reading C++ file 'thread1.h'
**
** Created: Thu Jun 16 15:38:27 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../thread1.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'thread1.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_thread1[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      31,   25,    8,    8, 0x05,
      53,   49,    8,    8, 0x05,
      73,   49,    8,    8, 0x05,
      94,    8,    8,    8, 0x05,
     113,    8,    8,    8, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_thread1[] = {
    "thread1\0\0SIG_SendClose()\0state\0"
    "SIG_dspstate(int)\0num\0SIG_testresult(int)\0"
    "SIG_sendwarning(int)\0SIG_changetab(int)\0"
    "SIG_TestState()\0"
};

const QMetaObject thread1::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_thread1,
      qt_meta_data_thread1, 0 }
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
        switch (_id) {
        case 0: SIG_SendClose(); break;
        case 1: SIG_dspstate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: SIG_testresult((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: SIG_sendwarning((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: SIG_changetab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: SIG_TestState(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void thread1::SIG_SendClose()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void thread1::SIG_dspstate(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void thread1::SIG_testresult(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void thread1::SIG_sendwarning(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void thread1::SIG_changetab(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void thread1::SIG_TestState()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}
QT_END_MOC_NAMESPACE
