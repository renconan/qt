/****************************************************************************
** Meta object code from reading C++ file 'helpdialog.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Help/helpdialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'helpdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Helpdialog[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
      30,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Helpdialog[] = {
    "Helpdialog\0\0HelpWindowClose()\0"
    "on_pushButton_clicked()\0"
};

void Helpdialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        Helpdialog *_t = static_cast<Helpdialog *>(_o);
        switch (_id) {
        case 0: _t->HelpWindowClose(); break;
        case 1: _t->on_pushButton_clicked(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData Helpdialog::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject Helpdialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_Helpdialog,
      qt_meta_data_Helpdialog, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Helpdialog::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Helpdialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Helpdialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Helpdialog))
        return static_cast<void*>(const_cast< Helpdialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int Helpdialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Helpdialog::HelpWindowClose()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}
QT_END_MOC_NAMESPACE
