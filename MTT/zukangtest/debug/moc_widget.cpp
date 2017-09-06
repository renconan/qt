/****************************************************************************
** Meta object code from reading C++ file 'widget.h'
**
** Created: Thu Jun 16 16:13:11 2016
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
      27,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,    8,    7,    7, 0x0a,
      38,   32,    7,    7, 0x0a,
      56,    8,    7,    7, 0x0a,
      73,    7,    7,    7, 0x0a,
      88,    7,    7,    7, 0x0a,
     106,    7,    7,    7, 0x0a,
     120,    7,    7,    7, 0x0a,
     135,    7,    7,    7, 0x08,
     161,    7,    7,    7, 0x08,
     187,    7,    7,    7, 0x08,
     212,    7,    7,    7, 0x08,
     239,    7,    7,    7, 0x08,
     266,    7,    7,    7, 0x08,
     293,    7,    7,    7, 0x08,
     320,    7,    7,    7, 0x08,
     347,    7,    7,    7, 0x08,
     374,    7,    7,    7, 0x08,
     401,    7,    7,    7, 0x08,
     432,  428,    7,    7, 0x08,
     457,    7,    7,    7, 0x08,
     475,    7,    7,    7, 0x08,
     499,    7,    7,    7, 0x08,
     521,    7,    7,    7, 0x08,
     547,    7,    7,    7, 0x08,
     582,  576,    7,    7, 0x08,
     615,    7,    7,    7, 0x08,
     646,  641,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Widget[] = {
    "Widget\0\0num\0showtestresult(int)\0state\0"
    "showdspstate(int)\0showwarnint(int)\0"
    "changetab(int)\0HelpDialogClose()\0"
    "CloseWidget()\0GetTestState()\0"
    "on_pushButton_3_clicked()\0"
    "on_pushButton_4_clicked()\0"
    "on_radioButton_clicked()\0"
    "on_radioButton_2_clicked()\0"
    "on_radioButton_3_clicked()\0"
    "on_radioButton_4_clicked()\0"
    "on_radioButton_5_clicked()\0"
    "on_radioButton_6_clicked()\0"
    "on_radioButton_7_clicked()\0"
    "on_radioButton_8_clicked()\0"
    "on_radioButton_9_clicked()\0str\0"
    "setlineedittext(QString)\0clearkeypadflag()\0"
    "on_pushButton_clicked()\0on_checkBox_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_textBrowser_textChanged()\0index\0"
    "on_tabWidget_currentChanged(int)\0"
    "on_pushButton_5_clicked()\0arg1\0"
    "on_lineEdit_6_textChanged(QString)\0"
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
        case 0: showtestresult((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: showdspstate((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: showwarnint((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: changetab((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: HelpDialogClose(); break;
        case 5: CloseWidget(); break;
        case 6: GetTestState(); break;
        case 7: on_pushButton_3_clicked(); break;
        case 9: on_radioButton_clicked(); break;
        case 10: on_radioButton_2_clicked(); break;
        case 11: on_radioButton_3_clicked(); break;
        case 12: on_radioButton_4_clicked(); break;
        case 13: on_radioButton_5_clicked(); break;
        case 14: on_radioButton_6_clicked(); break;
        case 15: on_radioButton_7_clicked(); break;
        case 16: on_radioButton_8_clicked(); break;
        case 17: on_radioButton_9_clicked(); break;
        case 18: setlineedittext((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: clearkeypadflag(); break;
        case 20: on_pushButton_clicked(); break;
        case 21: on_checkBox_clicked(); break;
        case 22: on_pushButton_2_clicked(); break;
        case 23: on_textBrowser_textChanged(); break;
        case 24: on_tabWidget_currentChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: on_pushButton_5_clicked(); break;

        default: ;
        }
        _id -= 27;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
