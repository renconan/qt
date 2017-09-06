/****************************************************************************
** Meta object code from reading C++ file 'manualtest.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "manualtest.h"
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
      36,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      37,   31,   11,   11, 0x0a,
      57,   55,   11,   11, 0x0a,
      90,   84,   78,   11, 0x0a,
     107,   11,   11,   11, 0x0a,
     122,   11,   11,   11, 0x0a,
     140,   11,   11,   11, 0x0a,
     163,   11,   11,   11, 0x0a,
     179,  177,   11,   11, 0x0a,
     200,   11,   11,   11, 0x0a,
     218,   11,   11,   11, 0x0a,
     238,  233,   11,   11, 0x0a,
     266,   11,   11,   11, 0x08,
     292,   11,   11,   11, 0x08,
     318,   11,   11,   11, 0x08,
     340,   11,   11,   11, 0x08,
     364,   11,   11,   11, 0x08,
     388,   11,   11,   11, 0x08,
     412,   11,   11,   11, 0x08,
     436,   11,   11,   11, 0x08,
     460,   11,   11,   11, 0x08,
     485,   11,   11,   11, 0x08,
     512,   11,   11,   11, 0x08,
     539,   11,   11,   11, 0x08,
     566,   11,   11,   11, 0x08,
     593,   11,   11,   11, 0x08,
     624,  620,   11,   11, 0x08,
     649,   11,   11,   11, 0x08,
     667,   11,   11,   11, 0x08,
     685,   11,   11,   11, 0x08,
     711,   11,   11,   11, 0x08,
     737,   11,   11,   11, 0x08,
     761,   11,   11,   11, 0x08,
     785,   11,   11,   11, 0x08,
     809,   11,   11,   11, 0x08,
     833,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Manualtest[] = {
    "Manualtest\0\0SIG_Manualoutput()\0state\0"
    "showdspstate(int)\0k\0showtestresult(bool)\0"
    "float\0value\0showPhase(float)\0"
    "setVanocheck()\0HelpDialogClose()\0"
    "moveEvent(QMoveEvent*)\0CloseWidget()\0"
    ",\0ShowWarning(int,int)\0WarningWinclose()\0"
    "GetTestState()\0flag\0DisplayAutoChangeValue(int)\0"
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
};

void Manualtest::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Manualtest *_t = static_cast<Manualtest *>(_o);
        switch (_id) {
        case 0: _t->SIG_Manualoutput(); break;
        case 1: _t->showdspstate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->showtestresult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: { float _r = _t->showPhase((*reinterpret_cast< float(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< float*>(_a[0]) = _r; }  break;
        case 4: _t->setVanocheck(); break;
        case 5: _t->HelpDialogClose(); break;
        case 6: _t->moveEvent((*reinterpret_cast< QMoveEvent*(*)>(_a[1]))); break;
        case 7: _t->CloseWidget(); break;
        case 8: _t->ShowWarning((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 9: _t->WarningWinclose(); break;
        case 10: _t->GetTestState(); break;
        case 11: _t->DisplayAutoChangeValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: _t->on_pushButton_2_clicked(); break;
        case 13: _t->on_pushButton_3_clicked(); break;
        case 14: _t->on_checkBox_clicked(); break;
        case 15: _t->on_checkBox_2_clicked(); break;
        case 16: _t->on_checkBox_3_clicked(); break;
        case 17: _t->on_checkBox_4_clicked(); break;
        case 18: _t->on_checkBox_5_clicked(); break;
        case 19: _t->on_checkBox_6_clicked(); break;
        case 20: _t->on_radioButton_clicked(); break;
        case 21: _t->on_radioButton_2_clicked(); break;
        case 22: _t->on_radioButton_3_clicked(); break;
        case 23: _t->on_radioButton_4_clicked(); break;
        case 24: _t->on_radioButton_5_clicked(); break;
        case 25: _t->on_radioButton_6_clicked(); break;
        case 26: _t->setlineedittext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 27: _t->clearkeypadflag(); break;
        case 28: _t->GetTeststate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->on_pushButton_4_clicked(); break;
        case 30: _t->on_pushButton_5_clicked(); break;
        case 31: _t->on_pushButton_clicked(); break;
        case 32: _t->on_checkBox_7_clicked(); break;
        case 33: _t->on_checkBox_8_clicked(); break;
        case 34: _t->on_checkBox_9_clicked(); break;
        case 35: _t->on_pushButton_6_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Manualtest::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Manualtest::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Manualtest,
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
    return QWidget::qt_metacast(_clname);
}

int Manualtest::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    }
    return _id;
}

// SIGNAL 0
void Manualtest::SIG_Manualoutput()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
