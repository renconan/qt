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
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      10,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,    9,    8,    8, 0x05,
      32,    9,    8,    8, 0x05,
      55,   50,    8,    8, 0x05,
      72,    8,    8,    8, 0x05,
      98,    8,    8,    8, 0x05,
     126,    8,    8,    8, 0x05,
     155,  151,    8,    8, 0x05,
     181,    8,    8,    8, 0x05,
     208,    8,    8,    8, 0x05,
     224,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
     240,    8,    8,    8, 0x09,
     257,    8,    8,    8, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_thread1[] = {
    "thread1\0\0pos\0SIG_updatepos(int)\0"
    "SIG_enterpos(int)\0flag\0SIG_showtip(int)\0"
    "SIG_showfirstfuncofsect()\0"
    "SIG_showcurrenttestpos(int)\0"
    "SIG_cleartestitemcolor()\0str\0"
    "SIG_SendProcessStr(char*)\0"
    "SIG_AuthorizedFileExpire()\0SIG_SendClose()\0"
    "SIG_TestState()\0getprocessstr2()\0"
    "getprocessstr()\0"
};

void thread1::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        thread1 *_t = static_cast<thread1 *>(_o);
        switch (_id) {
        case 0: _t->SIG_updatepos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->SIG_enterpos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->SIG_showtip((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->SIG_showfirstfuncofsect(); break;
        case 4: _t->SIG_showcurrenttestpos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->SIG_cleartestitemcolor(); break;
        case 6: _t->SIG_SendProcessStr((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 7: _t->SIG_AuthorizedFileExpire(); break;
        case 8: _t->SIG_SendClose(); break;
        case 9: _t->SIG_TestState(); break;
        case 10: _t->getprocessstr2(); break;
        case 11: _t->getprocessstr(); break;
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
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void thread1::SIG_updatepos(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void thread1::SIG_enterpos(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void thread1::SIG_showtip(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void thread1::SIG_showfirstfuncofsect()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void thread1::SIG_showcurrenttestpos(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void thread1::SIG_cleartestitemcolor()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void thread1::SIG_SendProcessStr(char * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void thread1::SIG_AuthorizedFileExpire()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void thread1::SIG_SendClose()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}

// SIGNAL 9
void thread1::SIG_TestState()
{
    QMetaObject::activate(this, &staticMetaObject, 9, 0);
}
QT_END_MOC_NAMESPACE
