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
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      19,    8,    7,    7, 0x0a,
      54,   50,    7,    7, 0x0a,
      80,   74,    7,    7, 0x0a,
      98,    7,    7,    7, 0x0a,
     122,  116,    7,    7, 0x0a,
     145,    7,    7,    7, 0x0a,
     159,    7,    7,    7, 0x0a,
     174,    7,    7,    7, 0x0a,
     192,    7,    7,    7, 0x08,
     218,    7,    7,    7, 0x08,
     248,  244,    7,    7, 0x08,
     273,    7,    7,    7, 0x08,
     297,  291,    7,    7, 0x08,
     334,    7,    7,    7, 0x08,
     358,  291,    7,    7, 0x08,
     391,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget[] = {
    "Widget\0\0flag,value\0DisplayChangeValue(bool,float)\0"
    "num\0showtestresult(int)\0state\0"
    "showdspstate(int)\0HelpDialogClose()\0"
    "event\0moveEvent(QMoveEvent*)\0CloseWidget()\0"
    "GetTestState()\0GetTeststate(int)\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_3_clicked()\0str\0"
    "setlineedittext(QString)\0clearkeypadflag()\0"
    "index\0on_comboBox_currentIndexChanged(int)\0"
    "on_pushButton_clicked()\0"
    "on_tabWidget_currentChanged(int)\0"
    "on_pushButton_4_clicked()\0"
};

void Widget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Widget *_t = static_cast<Widget *>(_o);
        switch (_id) {
        case 0: _t->DisplayChangeValue((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 1: _t->showtestresult((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->showdspstate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->HelpDialogClose(); break;
        case 4: _t->moveEvent((*reinterpret_cast< QMoveEvent*(*)>(_a[1]))); break;
        case 5: _t->CloseWidget(); break;
        case 6: _t->GetTestState(); break;
        case 7: _t->GetTeststate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->on_pushButton_2_clicked(); break;
        case 9: _t->on_pushButton_3_clicked(); break;
        case 10: _t->setlineedittext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->clearkeypadflag(); break;
        case 12: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: _t->on_pushButton_clicked(); break;
        case 14: _t->on_tabWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_pushButton_4_clicked(); break;
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
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
