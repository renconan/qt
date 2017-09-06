/****************************************************************************
** Meta object code from reading C++ file 'settingmanage.h'
**
** Created: Wed Dec 14 10:19:30 2016
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Settingmanage/settingmanage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'settingmanage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Settingmanage[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      38,   14,   14,   14, 0x08,
      65,   62,   14,   14, 0x08,
     104,   14,   14,   14, 0x08,
     144,  130,   14,   14, 0x08,
     176,   14,   14,   14, 0x08,
     189,   14,   14,   14, 0x08,
     215,   14,   14,   14, 0x08,
     241,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Settingmanage[] = {
    "Settingmanage\0\0savesettingnode(void*)\0"
    "on_pushButton_clicked()\0,,\0"
    "updatesetting(QString,QString,QString)\0"
    "on_pushButton_2_clicked()\0shuzu,str,len\0"
    "copytoshuzu(char[],QString,int)\0"
    "updateshow()\0on_pushButton_3_clicked()\0"
    "on_pushButton_4_clicked()\0"
    "on_pushButton_5_clicked()\0"
};

const QMetaObject Settingmanage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Settingmanage,
      qt_meta_data_Settingmanage, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Settingmanage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Settingmanage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Settingmanage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Settingmanage))
        return static_cast<void*>(const_cast< Settingmanage*>(this));
    return QWidget::qt_metacast(_clname);
}

int Settingmanage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: savesettingnode((*reinterpret_cast< void*(*)>(_a[1]))); break;
        case 1: on_pushButton_clicked(); break;
        case 2: updatesetting((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: on_pushButton_2_clicked(); break;
        case 4: copytoshuzu((*reinterpret_cast< char(*)[]>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 5: updateshow(); break;
        case 6: on_pushButton_3_clicked(); break;
        case 7: on_pushButton_4_clicked(); break;
        case 8: on_pushButton_5_clicked(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void Settingmanage::savesettingnode(void * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
