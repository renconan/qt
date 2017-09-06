/****************************************************************************
** Meta object code from reading C++ file 'settingmanage.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "SetManage/settingmanage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingmanage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SettingManage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,
      40,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      52,   14,   14,   14, 0x09,
      67,   14,   14,   14, 0x09,
      91,   85,   14,   14, 0x09,
     116,   14,   14,   14, 0x09,
     134,   14,   14,   14, 0x08,
     158,   14,   14,   14, 0x08,
     184,   14,   14,   14, 0x08,
     210,   14,   14,   14, 0x08,
     247,  236,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SettingManage[] = {
    "SettingManage\0\0SIG_returnsetlink(void*)\0"
    "SIG_Close()\0saveclosewin()\0cancellclosewin()\0"
    "value\0dissettingvalue(QString)\0"
    "clearkeypadflag()\0on_pushButton_clicked()\0"
    "on_pushButton_2_clicked()\0"
    "on_pushButton_3_clicked()\0"
    "on_pushButton_4_clicked()\0row,column\0"
    "on_tableWidget_cellDoubleClicked(int,int)\0"
};

void SettingManage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        SettingManage *_t = static_cast<SettingManage *>(_o);
        switch (_id) {
        case 0: _t->SIG_returnsetlink((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 1: _t->SIG_Close(); break;
        case 2: _t->saveclosewin(); break;
        case 3: _t->cancellclosewin(); break;
        case 4: _t->dissettingvalue((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->clearkeypadflag(); break;
        case 6: _t->on_pushButton_clicked(); break;
        case 7: _t->on_pushButton_2_clicked(); break;
        case 8: _t->on_pushButton_3_clicked(); break;
        case 9: _t->on_pushButton_4_clicked(); break;
        case 10: _t->on_tableWidget_cellDoubleClicked((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData SettingManage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject SettingManage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SettingManage,
      qt_meta_data_SettingManage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SettingManage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SettingManage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SettingManage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SettingManage))
        return static_cast<void*>(const_cast< SettingManage*>(this));
    return QWidget::qt_metacast(_clname);
}

int SettingManage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void SettingManage::SIG_returnsetlink(void * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SettingManage::SIG_Close()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
