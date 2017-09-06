/****************************************************************************
** Meta object code from reading C++ file 'threadkeyboard.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SetManage/Keyborad/threadkeyboard.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'threadkeyboard.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_threadkeyboard[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      25,   16,   15,   15, 0x05,
      51,   47,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_threadkeyboard[] = {
    "threadkeyboard\0\0position\0SIG_sendfocuspos(int)\0"
    "pos\0SIG_sendenterpos(int)\0"
};

void threadkeyboard::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        threadkeyboard *_t = static_cast<threadkeyboard *>(_o);
        switch (_id) {
        case 0: _t->SIG_sendfocuspos((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->SIG_sendenterpos((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData threadkeyboard::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject threadkeyboard::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_threadkeyboard,
      qt_meta_data_threadkeyboard, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &threadkeyboard::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *threadkeyboard::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *threadkeyboard::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_threadkeyboard))
        return static_cast<void*>(const_cast< threadkeyboard*>(this));
    return QThread::qt_metacast(_clname);
}

int threadkeyboard::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void threadkeyboard::SIG_sendfocuspos(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void threadkeyboard::SIG_sendenterpos(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
