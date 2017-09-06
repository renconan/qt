/****************************************************************************
** Meta object code from reading C++ file 'manualtest.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Manual/manualtest.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'manualtest.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Manualtest[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      41,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,
      39,   31,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      73,   67,   11,   11, 0x0a,
      93,   91,   11,   11, 0x0a,
     126,  120,  114,   11, 0x0a,
     143,   11,   11,   11, 0x0a,
     158,   11,   11,   11, 0x0a,
     176,   11,   11,   11, 0x0a,
     199,   11,   11,   11, 0x0a,
     215,  213,   11,   11, 0x0a,
     236,   11,   11,   11, 0x0a,
     254,   11,   11,   11, 0x0a,
     280,  275,   11,   11, 0x0a,
     308,   11,   11,   11, 0x08,
     334,   11,   11,   11, 0x08,
     360,   11,   11,   11, 0x08,
     382,   11,   11,   11, 0x08,
     406,   11,   11,   11, 0x08,
     430,   11,   11,   11, 0x08,
     454,   11,   11,   11, 0x08,
     478,   11,   11,   11, 0x08,
     502,   11,   11,   11, 0x08,
     527,   11,   11,   11, 0x08,
     554,   11,   11,   11, 0x08,
     581,   11,   11,   11, 0x08,
     608,   11,   11,   11, 0x08,
     635,   11,   11,   11, 0x08,
     666,  662,   11,   11, 0x08,
     691,   11,   11,   11, 0x08,
     709,   11,   11,   11, 0x08,
     727,   11,   11,   11, 0x08,
     753,   11,   11,   11, 0x08,
     779,   11,   11,   11, 0x08,
     803,   11,   11,   11, 0x08,
     827,   11,   11,   11, 0x08,
     851,   11,   11,   11, 0x08,
     875,   11,   11,   11, 0x08,
     901,   11,   11,   11, 0x08,
     926,   11,   11,   11, 0x08,
     951,   11,   11,   11, 0x08,
     977,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Manualtest[] = {
    "Manualtest\0\0SIG_Manualoutput()\0testStr\0"
    "SIG_SendTestResult(QString)\0state\0"
    "showdspstate(int)\0k\0showtestresult(bool)\0"
    "float\0value\0showPhase(float)\0"
    "setVanocheck()\0HelpDialogClose()\0"
    "moveEvent(QMoveEvent*)\0CloseWidget()\0"
    ",\0ShowWarning(int,int)\0WarningWinclose()\0"
    "GetTestButtenstate()\0flag\0"
    "DisplayAutoChangeValue(int)\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_3_clicked()\0"
    "on_checkBox_clicked()\0on_checkBox_2_clicked()\0"
    "on_checkBox_3_clicked()\0on_checkBox_4_clicked()\0"
    "on_checkBox_5_clicked()\0on_checkBox_6_clicked()\0"
    "on_radioButton_clicked()\0"
    "on_radioButton_2_clicked()\0"
    "on_radioButton_3_clicked()\0"
    "on_radioButton_4_clicked()\0"
    "on_radioButton_5_clicked()\0"
    "on_radioButton_6_clicked()\0str\0"
    "setlineedittext(QString)\0clearkeypadflag()\0"
    "GetTeststate(int)\0on_pushButton_4_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_clicked()\0on_checkBox_7_clicked()\0"
    "on_checkBox_8_clicked()\0on_checkBox_9_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_checkBox_10_clicked()\0"
    "on_checkBox_11_clicked()\0"
    "on_pushButton_7_clicked()\0"
    "on_pushButton_8_clicked()\0"
};

void Manualtest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Manualtest *_t = static_cast<Manualtest *>(_o);
        switch (_id) {
        case 0: _t->SIG_Manualoutput(); break;
        case 1: _t->SIG_SendTestResult((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->showdspstate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->showtestresult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: { float _r = _t->showPhase((*reinterpret_cast< float(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = _r; }  break;
        case 5: _t->setVanocheck(); break;
        case 6: _t->HelpDialogClose(); break;
        case 7: _t->moveEvent((*reinterpret_cast< QMoveEvent*(*)>(_a[1]))); break;
        case 8: _t->CloseWidget(); break;
        case 9: _t->ShowWarning((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->WarningWinclose(); break;
        case 11: _t->GetTestButtenstate(); break;
        case 12: _t->DisplayAutoChangeValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_pushButton_2_clicked(); break;
        case 14: _t->on_pushButton_3_clicked(); break;
        case 15: _t->on_checkBox_clicked(); break;
        case 16: _t->on_checkBox_2_clicked(); break;
        case 17: _t->on_checkBox_3_clicked(); break;
        case 18: _t->on_checkBox_4_clicked(); break;
        case 19: _t->on_checkBox_5_clicked(); break;
        case 20: _t->on_checkBox_6_clicked(); break;
        case 21: _t->on_radioButton_clicked(); break;
        case 22: _t->on_radioButton_2_clicked(); break;
        case 23: _t->on_radioButton_3_clicked(); break;
        case 24: _t->on_radioButton_4_clicked(); break;
        case 25: _t->on_radioButton_5_clicked(); break;
        case 26: _t->on_radioButton_6_clicked(); break;
        case 27: _t->setlineedittext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 28: _t->clearkeypadflag(); break;
        case 29: _t->GetTeststate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->on_pushButton_4_clicked(); break;
        case 31: _t->on_pushButton_5_clicked(); break;
        case 32: _t->on_pushButton_clicked(); break;
        case 33: _t->on_checkBox_7_clicked(); break;
        case 34: _t->on_checkBox_8_clicked(); break;
        case 35: _t->on_checkBox_9_clicked(); break;
        case 36: _t->on_pushButton_6_clicked(); break;
        case 37: _t->on_checkBox_10_clicked(); break;
        case 38: _t->on_checkBox_11_clicked(); break;
        case 39: _t->on_pushButton_7_clicked(); break;
        case 40: _t->on_pushButton_8_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Manualtest::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Manualtest::staticMetaObject = {
    { &CWindowsBase::staticMetaObject, qt_meta_stringdata_Manualtest,
      qt_meta_data_Manualtest, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Manualtest::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Manualtest::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Manualtest::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Manualtest))
        return static_cast<void*>(const_cast< Manualtest*>(this));
    return CWindowsBase::qt_metacast(_clname);
}

int Manualtest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CWindowsBase::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 41)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 41;
    }
    return _id;
}

// SIGNAL 0
void Manualtest::SIG_Manualoutput()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void Manualtest::SIG_SendTestResult(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
