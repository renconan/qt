/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created: Wed Jan 18 14:31:47 2017
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../widget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'widget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Widget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      21,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,    8,    7,    7, 0x08,
      53,   51,    7,    7, 0x08,
      76,    7,    7,    7, 0x08,
     102,    7,    7,    7, 0x08,
     126,    7,    7,    7, 0x08,
     152,    7,    7,    7, 0x08,
     178,    7,    7,    7, 0x08,
     204,    7,    7,    7, 0x08,
     230,    7,    7,    7, 0x08,
     256,    7,    7,    7, 0x08,
     283,    7,    7,    7, 0x08,
     313,  308,    7,    7, 0x08,
     331,    8,    7,    7, 0x08,
     358,    7,    7,    7, 0x08,
     384,    7,    7,    7, 0x08,
     410,    7,    7,    7, 0x08,
     442,  437,    7,    7, 0x08,
     494,  437,    7,    7, 0x08,
     548,  544,    7,    7, 0x08,
     599,    7,    7,    7, 0x08,
     612,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget[] = {
    "Widget\0\0index\0on_comboBox_currentIndexChanged(int)\0"
    ",\0addtestfunc(void*,int)\0"
    "on_pushButton_3_clicked()\0"
    "on_pushButton_clicked()\0"
    "on_pushButton_6_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_5_clicked()\0"
    "on_pushButton_4_clicked()\0"
    "on_pushButton_7_clicked()\0"
    "on_pushButton_10_clicked()\0"
    "updatesettinglink(void*)\0type\0"
    "closefuncwin(int)\0on_comboBox_activated(int)\0"
    "on_pushButton_8_clicked()\0"
    "on_pushButton_9_clicked()\0"
    "on_pushButton_11_clicked()\0item\0"
    "on_listWidget_2_itemDoubleClicked(QListWidgetItem*)\0"
    "on_listWidget_itemDoubleClicked(QListWidgetItem*)\0"
    "pos\0on_listWidget_2_customContextMenuRequested(QPoint)\0"
    "UpSeedSlot()\0DownSeedsSlot()\0"
};

const QMetaObject Widget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Widget,
      qt_meta_data_Widget, 0 }
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
        switch (_id) {
        case 0: on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: addtestfunc((*reinterpret_cast< void*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: on_pushButton_3_clicked(); break;
        case 3: on_pushButton_clicked(); break;
        case 4: on_pushButton_6_clicked(); break;
        case 5: on_pushButton_2_clicked(); break;
        case 6: on_pushButton_5_clicked(); break;
        case 7: on_pushButton_4_clicked(); break;
        case 8: on_pushButton_7_clicked(); break;
        case 9: on_pushButton_10_clicked(); break;
        case 10: updatesettinglink((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 11: closefuncwin((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: on_comboBox_activated((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 13: on_pushButton_8_clicked(); break;
        case 14: on_pushButton_9_clicked(); break;
        case 15: on_pushButton_11_clicked(); break;
        case 16: on_listWidget_2_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 17: on_listWidget_itemDoubleClicked((*reinterpret_cast< QListWidgetItem*(*)>(_a[1]))); break;
        case 18: on_listWidget_2_customContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 19: UpSeedSlot(); break;
        case 20: DownSeedsSlot(); break;
        default: ;
        }
        _id -= 21;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
