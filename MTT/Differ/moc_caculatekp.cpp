/****************************************************************************
** Meta object code from reading C++ file 'caculatekp.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "CaculateKp/caculatekp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'caculatekp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CaculateKp[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      42,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      58,   11,   11,   11, 0x08,
      82,   11,   11,   11, 0x08,
     108,   11,   11,   11, 0x08,
     140,  134,   11,   11, 0x08,
     169,  165,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_CaculateKp[] = {
    "CaculateKp\0\0SIG_SendVolchangerinfo(void*)\0"
    "SIG_SendClose()\0on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_3_clicked()\0event\0"
    "closeEvent(QCloseEvent*)\0str\0"
    "setlineedittext(QString)\0"
};

void CaculateKp::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        CaculateKp *_t = static_cast<CaculateKp *>(_o);
        switch (_id) {
        case 0: _t->SIG_SendVolchangerinfo((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 1: _t->SIG_SendClose(); break;
        case 2: _t->on_pushButton_clicked(); break;
        case 3: _t->on_pushButton_2_clicked(); break;
        case 4: _t->on_pushButton_3_clicked(); break;
        case 5: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 6: _t->setlineedittext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData CaculateKp::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject CaculateKp::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_CaculateKp,
      qt_meta_data_CaculateKp, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CaculateKp::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CaculateKp::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CaculateKp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CaculateKp))
        return static_cast<void*>(const_cast< CaculateKp*>(this));
    return QDialog::qt_metacast(_clname);
}

int CaculateKp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void CaculateKp::SIG_SendVolchangerinfo(void * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CaculateKp::SIG_SendClose()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
