/****************************************************************************
** Meta object code from reading C++ file 'harmonic.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "harmonic.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'harmonic.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Harmonic[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   10,    9,    9, 0x0a,
      42,   36,    9,    9, 0x0a,
      62,   60,    9,    9, 0x0a,
      87,    9,    9,    9, 0x0a,
     111,  105,    9,    9, 0x0a,
     134,    9,    9,    9, 0x0a,
     148,    9,    9,    9, 0x0a,
     163,    9,    9,    9, 0x0a,
     181,    9,    9,    9, 0x08,
     205,    9,    9,    9, 0x08,
     231,    9,    9,    9, 0x08,
     261,  257,    9,    9, 0x08,
     286,    9,    9,    9, 0x08,
     304,    9,    9,    9, 0x08,
     329,    9,    9,    9, 0x08,
     356,    9,    9,    9, 0x08,
     383,    9,    9,    9, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Harmonic[] = {
    "Harmonic\0\0num\0showtestresult(float)\0"
    "state\0showdspstate(int)\0,\0"
    "displaychange(int,float)\0HelpDialogClose()\0"
    "event\0moveEvent(QMoveEvent*)\0CloseWidget()\0"
    "GetTestState()\0GetTeststate(int)\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_3_clicked()\0str\0"
    "setlineedittext(QString)\0clearkeypadflag()\0"
    "on_radioButton_clicked()\0"
    "on_radioButton_2_clicked()\0"
    "on_radioButton_3_clicked()\0"
    "on_pushButton_4_clicked()\0"
};

void Harmonic::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Harmonic *_t = static_cast<Harmonic *>(_o);
        switch (_id) {
        case 0: _t->showtestresult((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: _t->showdspstate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->displaychange((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 3: _t->HelpDialogClose(); break;
        case 4: _t->moveEvent((*reinterpret_cast< QMoveEvent*(*)>(_a[1]))); break;
        case 5: _t->CloseWidget(); break;
        case 6: _t->GetTestState(); break;
        case 7: _t->GetTeststate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_pushButton_clicked(); break;
        case 9: _t->on_pushButton_2_clicked(); break;
        case 10: _t->on_pushButton_3_clicked(); break;
        case 11: _t->setlineedittext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->clearkeypadflag(); break;
        case 13: _t->on_radioButton_clicked(); break;
        case 14: _t->on_radioButton_2_clicked(); break;
        case 15: _t->on_radioButton_3_clicked(); break;
        case 16: _t->on_pushButton_4_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Harmonic::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Harmonic::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Harmonic,
      qt_meta_data_Harmonic, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Harmonic::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Harmonic::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Harmonic::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Harmonic))
        return static_cast<void*>(const_cast< Harmonic*>(this));
    return QWidget::qt_metacast(_clname);
}

int Harmonic::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
