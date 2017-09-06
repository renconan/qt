/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Widget[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      24,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x0a,
      38,   32,    7,    7, 0x0a,
      58,   56,    7,    7, 0x0a,
      89,    7,    7,    7, 0x0a,
     113,  107,    7,    7, 0x0a,
     136,    7,    7,    7, 0x0a,
     150,    7,    7,    7, 0x0a,
     165,    7,    7,    7, 0x08,
     191,    7,    7,    7, 0x08,
     217,    7,    7,    7, 0x08,
     241,    7,    7,    7, 0x08,
     266,    7,    7,    7, 0x08,
     297,  293,    7,    7, 0x08,
     322,    7,    7,    7, 0x08,
     340,    7,    7,    7, 0x08,
     373,  367,    7,    7, 0x08,
     406,    7,    7,    7, 0x08,
     433,    7,    7,    7, 0x08,
     460,    7,    7,    7, 0x08,
     487,    7,    7,    7, 0x08,
     514,    7,    7,    7, 0x08,
     536,    7,    7,    7, 0x08,
     562,    7,    7,    7, 0x08,
     588,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget[] = {
    "Widget\0\0num\0showtestresult(int)\0state\0"
    "showdspstate(int)\0,\0DisplayChangeValue(bool,float)\0"
    "HelpDialogClose()\0event\0moveEvent(QMoveEvent*)\0"
    "CloseWidget()\0GetTestState()\0"
    "on_pushButton_3_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_clicked()\0"
    "on_radioButton_clicked()\0"
    "on_radioButton_2_clicked()\0str\0"
    "setlineedittext(QString)\0clearkeypadflag()\0"
    "on_radioButton_3_clicked()\0index\0"
    "on_tabWidget_currentChanged(int)\0"
    "on_radioButton_4_clicked()\0"
    "on_radioButton_5_clicked()\0"
    "on_radioButton_6_clicked()\0"
    "on_radioButton_7_clicked()\0"
    "on_checkBox_clicked()\0on_pushButton_4_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_7_clicked()\0"
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Widget *_t = static_cast<Widget *>(_o);
        switch (_id) {
        case 0: _t->showtestresult((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->showdspstate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->DisplayChangeValue((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 3: _t->HelpDialogClose(); break;
        case 4: _t->moveEvent((*reinterpret_cast< QMoveEvent*(*)>(_a[1]))); break;
        case 5: _t->CloseWidget(); break;
        case 6: _t->GetTestState(); break;
        case 7: _t->on_pushButton_3_clicked(); break;
        case 8: _t->on_pushButton_2_clicked(); break;
        case 9: _t->on_pushButton_clicked(); break;
        case 10: _t->on_radioButton_clicked(); break;
        case 11: _t->on_radioButton_2_clicked(); break;
        case 12: _t->setlineedittext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 13: _t->clearkeypadflag(); break;
        case 14: _t->on_radioButton_3_clicked(); break;
        case 15: _t->on_tabWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->on_radioButton_4_clicked(); break;
        case 17: _t->on_radioButton_5_clicked(); break;
        case 18: _t->on_radioButton_6_clicked(); break;
        case 19: _t->on_radioButton_7_clicked(); break;
        case 20: _t->on_checkBox_clicked(); break;
        case 21: _t->on_pushButton_4_clicked(); break;
        case 22: _t->on_pushButton_6_clicked(); break;
        case 23: _t->on_pushButton_7_clicked(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData Widget::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Widget,
      qt_meta_data_Widget, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Widget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Widget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Widget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Widget))
        return static_cast<void*>(const_cast< Widget*>(this));
    return QWidget::qt_metacast(_clname);
}

int Widget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 24)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 24;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
