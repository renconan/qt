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
      26,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x0a,
      38,   32,    7,    7, 0x0a,
      62,   56,    7,    7, 0x0a,
      89,   85,    7,    7, 0x0a,
     114,    7,    7,    7, 0x0a,
     132,    7,    7,    7, 0x0a,
     152,    7,    7,    7, 0x0a,
     168,    7,    7,    7, 0x0a,
     182,    7,    7,    7, 0x0a,
     205,    7,    7,    7, 0x0a,
     223,    7,    7,    7, 0x0a,
     241,    7,    7,    7, 0x0a,
     256,    7,    7,    7, 0x08,
     282,    7,    7,    7, 0x08,
     308,    7,    7,    7, 0x08,
     340,  334,    7,    7, 0x08,
     377,  334,    7,    7, 0x08,
     416,  334,    7,    7, 0x08,
     455,    7,    7,    7, 0x08,
     479,    7,    7,    7, 0x08,
     504,    7,    7,    7, 0x08,
     531,    7,    7,    7, 0x08,
     558,    7,    7,    7, 0x08,
     585,    7,    7,    7, 0x08,
     612,    7,    7,    7, 0x08,
     639,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget[] = {
    "Widget\0\0num\0showtestresult(int)\0state\0"
    "showdspstate(int)\0event\0moveEvent(QMoveEvent*)\0"
    "str\0setlineedittext(QString)\0"
    "clearkeypadflag()\0UpdateCtinfo(void*)\0"
    "SetAuxcalflag()\0CloseWidget()\0"
    "ShowChangeValue(float)\0HelpDialogClose()\0"
    "GetTeststate(int)\0GetTestState()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_3_clicked()\0"
    "on_pushButton_4_clicked()\0index\0"
    "on_comboBox_currentIndexChanged(int)\0"
    "on_comboBox_2_currentIndexChanged(int)\0"
    "on_comboBox_3_currentIndexChanged(int)\0"
    "on_pushButton_clicked()\0"
    "on_radioButton_clicked()\0"
    "on_radioButton_2_clicked()\0"
    "on_radioButton_3_clicked()\0"
    "on_radioButton_4_clicked()\0"
    "on_radioButton_5_clicked()\0"
    "on_radioButton_6_clicked()\0"
    "on_pushButton_5_clicked()\0"
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Widget *_t = static_cast<Widget *>(_o);
        switch (_id) {
        case 0: _t->showtestresult((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->showdspstate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->moveEvent((*reinterpret_cast< QMoveEvent*(*)>(_a[1]))); break;
        case 3: _t->setlineedittext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->clearkeypadflag(); break;
        case 5: _t->UpdateCtinfo((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 6: _t->SetAuxcalflag(); break;
        case 7: _t->CloseWidget(); break;
        case 8: _t->ShowChangeValue((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->HelpDialogClose(); break;
        case 10: _t->GetTeststate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->GetTestState(); break;
        case 12: _t->on_pushButton_2_clicked(); break;
        case 13: _t->on_pushButton_3_clicked(); break;
        case 14: _t->on_pushButton_4_clicked(); break;
        case 15: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->on_comboBox_2_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->on_comboBox_3_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->on_pushButton_clicked(); break;
        case 19: _t->on_radioButton_clicked(); break;
        case 20: _t->on_radioButton_2_clicked(); break;
        case 21: _t->on_radioButton_3_clicked(); break;
        case 22: _t->on_radioButton_4_clicked(); break;
        case 23: _t->on_radioButton_5_clicked(); break;
        case 24: _t->on_radioButton_6_clicked(); break;
        case 25: _t->on_pushButton_5_clicked(); break;
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
        if (_id < 26)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 26;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
