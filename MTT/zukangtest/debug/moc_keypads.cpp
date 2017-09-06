/****************************************************************************
** Meta object code from reading C++ file 'keypads.h'
**
** Created: Thu Jun 16 15:38:26 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../keypads.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'keypads.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_keypads[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x05,
      28,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      39,    8,    8,    8, 0x08,
      69,    8,    8,    8, 0x08,
      99,    8,    8,    8, 0x08,
     128,    8,    8,    8, 0x08,
     158,    8,    8,    8, 0x08,
     187,    8,    8,    8, 0x08,
     213,    8,    8,    8, 0x08,
     239,    8,    8,    8, 0x08,
     265,    8,    8,    8, 0x08,
     291,    8,    8,    8, 0x08,
     317,    8,    8,    8, 0x08,
     343,    8,    8,    8, 0x08,
     369,    8,    8,    8, 0x08,
     395,    8,    8,    8, 0x08,
     421,    8,    8,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_keypads[] = {
    "keypads\0\0sendvalue(QString)\0sendflag()\0"
    "on_toolButton_enter_clicked()\0"
    "on_toolButton_clear_clicked()\0"
    "on_toolButton_back_clicked()\0"
    "on_toolButton_point_clicked()\0"
    "on_toolButton_sign_clicked()\0"
    "on_toolButton_0_clicked()\0"
    "on_toolButton_1_clicked()\0"
    "on_toolButton_2_clicked()\0"
    "on_toolButton_3_clicked()\0"
    "on_toolButton_4_clicked()\0"
    "on_toolButton_5_clicked()\0"
    "on_toolButton_6_clicked()\0"
    "on_toolButton_7_clicked()\0"
    "on_toolButton_8_clicked()\0"
    "on_toolButton_9_clicked()\0"
};

const QMetaObject keypads::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_keypads,
      qt_meta_data_keypads, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &keypads::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *keypads::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *keypads::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_keypads))
        return static_cast<void*>(const_cast< keypads*>(this));
    return QDialog::qt_metacast(_clname);
}

int keypads::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendvalue((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: sendflag(); break;
        case 2: on_toolButton_enter_clicked(); break;
        case 3: on_toolButton_clear_clicked(); break;
        case 4: on_toolButton_back_clicked(); break;
        case 5: on_toolButton_point_clicked(); break;
        case 6: on_toolButton_sign_clicked(); break;
        case 7: on_toolButton_0_clicked(); break;
        case 8: on_toolButton_1_clicked(); break;
        case 9: on_toolButton_2_clicked(); break;
        case 10: on_toolButton_3_clicked(); break;
        case 11: on_toolButton_4_clicked(); break;
        case 12: on_toolButton_5_clicked(); break;
        case 13: on_toolButton_6_clicked(); break;
        case 14: on_toolButton_7_clicked(); break;
        case 15: on_toolButton_8_clicked(); break;
        case 16: on_toolButton_9_clicked(); break;
        default: ;
        }
        _id -= 17;
    }
    return _id;
}

// SIGNAL 0
void keypads::sendvalue(const QString & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void keypads::sendflag()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
