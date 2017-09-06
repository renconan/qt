/****************************************************************************
** Meta object code from reading C++ file 'showtestitempara.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ShowTestItemPara/showtestitempara.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'showtestitempara.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ShowTestitemPara[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      18,   17,   17,   17, 0x05,

 // slots: signature, parameters, type, tag, flags
      29,   17,   17,   17, 0x08,
      53,   17,   17,   17, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_ShowTestitemPara[] = {
    "ShowTestitemPara\0\0sendflag()\0"
    "on_pushButton_clicked()\0"
    "closeEvent(QCloseEvent*)\0"
};

void ShowTestitemPara::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ShowTestitemPara *_t = static_cast<ShowTestitemPara *>(_o);
        switch (_id) {
        case 0: _t->sendflag(); break;
        case 1: _t->on_pushButton_clicked(); break;
        case 2: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ShowTestitemPara::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ShowTestitemPara::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ShowTestitemPara,
      qt_meta_data_ShowTestitemPara, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ShowTestitemPara::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ShowTestitemPara::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ShowTestitemPara::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ShowTestitemPara))
        return static_cast<void*>(const_cast< ShowTestitemPara*>(this));
    return QDialog::qt_metacast(_clname);
}

int ShowTestitemPara::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void ShowTestitemPara::sendflag()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
