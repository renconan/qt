/****************************************************************************
** Meta object code from reading C++ file 'harmonicpara.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "HarmonicPara/harmonicpara.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'harmonicpara.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_HarmonicPara[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x05,
      25,   13,   13,   13, 0x05,

 // slots: signature, parameters, type, tag, flags
      45,   41,   13,   13, 0x09,
      68,   13,   13,   13, 0x09,
      86,   13,   13,   13, 0x08,
     110,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_HarmonicPara[] = {
    "HarmonicPara\0\0sendflag()\0Sendpara(void*)\0"
    "str\0setlineedittext(char*)\0clearkeypadflag()\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
};

void HarmonicPara::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        HarmonicPara *_t = static_cast<HarmonicPara *>(_o);
        switch (_id) {
        case 0: _t->sendflag(); break;
        case 1: _t->Sendpara((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 2: _t->setlineedittext((*reinterpret_cast< char*(*)>(_a[1]))); break;
        case 3: _t->clearkeypadflag(); break;
        case 4: _t->on_pushButton_clicked(); break;
        case 5: _t->on_pushButton_2_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData HarmonicPara::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject HarmonicPara::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_HarmonicPara,
      qt_meta_data_HarmonicPara, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &HarmonicPara::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *HarmonicPara::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *HarmonicPara::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_HarmonicPara))
        return static_cast<void*>(const_cast< HarmonicPara*>(this));
    return QDialog::qt_metacast(_clname);
}

int HarmonicPara::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void HarmonicPara::sendflag()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void HarmonicPara::Sendpara(void * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
