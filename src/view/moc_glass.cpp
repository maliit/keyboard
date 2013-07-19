/****************************************************************************
** Meta object code from reading C++ file 'glass.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.0.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "glass.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'glass.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.0.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MaliitKeyboard__Glass_t {
    QByteArrayData data[23];
    char stringdata[309];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MaliitKeyboard__Glass_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MaliitKeyboard__Glass_t qt_meta_stringdata_MaliitKeyboard__Glass = {
    {
QT_MOC_LITERAL(0, 0, 21),
QT_MOC_LITERAL(1, 22, 10),
QT_MOC_LITERAL(2, 33, 0),
QT_MOC_LITERAL(3, 34, 3),
QT_MOC_LITERAL(4, 38, 3),
QT_MOC_LITERAL(5, 42, 20),
QT_MOC_LITERAL(6, 63, 6),
QT_MOC_LITERAL(7, 70, 14),
QT_MOC_LITERAL(8, 85, 11),
QT_MOC_LITERAL(9, 97, 10),
QT_MOC_LITERAL(10, 108, 9),
QT_MOC_LITERAL(11, 118, 20),
QT_MOC_LITERAL(12, 139, 13),
QT_MOC_LITERAL(13, 153, 9),
QT_MOC_LITERAL(14, 163, 21),
QT_MOC_LITERAL(15, 185, 14),
QT_MOC_LITERAL(16, 200, 26),
QT_MOC_LITERAL(17, 227, 5),
QT_MOC_LITERAL(18, 233, 15),
QT_MOC_LITERAL(19, 249, 10),
QT_MOC_LITERAL(20, 260, 11),
QT_MOC_LITERAL(21, 272, 14),
QT_MOC_LITERAL(22, 287, 20)
    },
    "MaliitKeyboard::Glass\0keyPressed\0\0Key\0"
    "key\0Logic::LayoutHelper*\0layout\0"
    "keyLongPressed\0keyReleased\0keyEntered\0"
    "keyExited\0wordCandidatePressed\0"
    "WordCandidate\0candidate\0wordCandidateReleased\0"
    "keyAreaPressed\0Logic::LayoutHelper::Panel\0"
    "panel\0keyAreaReleased\0switchLeft\0"
    "switchRight\0keyboardClosed\0"
    "onLongPressTriggered\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MaliitKeyboard__Glass[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   79,    2, 0x06,
       7,    2,   84,    2, 0x06,
       8,    2,   89,    2, 0x06,
       9,    2,   94,    2, 0x06,
      10,    2,   99,    2, 0x06,
      11,    2,  104,    2, 0x06,
      14,    2,  109,    2, 0x06,
      15,    2,  114,    2, 0x06,
      18,    2,  119,    2, 0x06,
      19,    1,  124,    2, 0x06,
      20,    1,  127,    2, 0x06,
      21,    0,  130,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
      22,    0,  131,    2, 0x08,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 5,    4,    6,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 5,   13,    6,
    QMetaType::Void, 0x80000000 | 12, 0x80000000 | 5,   13,    6,
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 5,   17,    6,
    QMetaType::Void, 0x80000000 | 16, 0x80000000 | 5,   17,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void MaliitKeyboard::Glass::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Glass *_t = static_cast<Glass *>(_o);
        switch (_id) {
        case 0: _t->keyPressed((*reinterpret_cast< const Key(*)>(_a[1])),(*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[2]))); break;
        case 1: _t->keyLongPressed((*reinterpret_cast< const Key(*)>(_a[1])),(*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[2]))); break;
        case 2: _t->keyReleased((*reinterpret_cast< const Key(*)>(_a[1])),(*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[2]))); break;
        case 3: _t->keyEntered((*reinterpret_cast< const Key(*)>(_a[1])),(*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[2]))); break;
        case 4: _t->keyExited((*reinterpret_cast< const Key(*)>(_a[1])),(*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[2]))); break;
        case 5: _t->wordCandidatePressed((*reinterpret_cast< const WordCandidate(*)>(_a[1])),(*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[2]))); break;
        case 6: _t->wordCandidateReleased((*reinterpret_cast< const WordCandidate(*)>(_a[1])),(*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[2]))); break;
        case 7: _t->keyAreaPressed((*reinterpret_cast< Logic::LayoutHelper::Panel(*)>(_a[1])),(*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[2]))); break;
        case 8: _t->keyAreaReleased((*reinterpret_cast< Logic::LayoutHelper::Panel(*)>(_a[1])),(*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[2]))); break;
        case 9: _t->switchLeft((*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[1]))); break;
        case 10: _t->switchRight((*reinterpret_cast< Logic::LayoutHelper*(*)>(_a[1]))); break;
        case 11: _t->keyboardClosed(); break;
        case 12: _t->onLongPressTriggered(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Glass::*_t)(const Key & , Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::keyPressed)) {
                *result = 0;
            }
        }
        {
            typedef void (Glass::*_t)(const Key & , Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::keyLongPressed)) {
                *result = 1;
            }
        }
        {
            typedef void (Glass::*_t)(const Key & , Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::keyReleased)) {
                *result = 2;
            }
        }
        {
            typedef void (Glass::*_t)(const Key & , Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::keyEntered)) {
                *result = 3;
            }
        }
        {
            typedef void (Glass::*_t)(const Key & , Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::keyExited)) {
                *result = 4;
            }
        }
        {
            typedef void (Glass::*_t)(const WordCandidate & , Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::wordCandidatePressed)) {
                *result = 5;
            }
        }
        {
            typedef void (Glass::*_t)(const WordCandidate & , Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::wordCandidateReleased)) {
                *result = 6;
            }
        }
        {
            typedef void (Glass::*_t)(Logic::LayoutHelper::Panel , Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::keyAreaPressed)) {
                *result = 7;
            }
        }
        {
            typedef void (Glass::*_t)(Logic::LayoutHelper::Panel , Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::keyAreaReleased)) {
                *result = 8;
            }
        }
        {
            typedef void (Glass::*_t)(Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::switchLeft)) {
                *result = 9;
            }
        }
        {
            typedef void (Glass::*_t)(Logic::LayoutHelper * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::switchRight)) {
                *result = 10;
            }
        }
        {
            typedef void (Glass::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Glass::keyboardClosed)) {
                *result = 11;
            }
        }
    }
}

const QMetaObject MaliitKeyboard::Glass::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_MaliitKeyboard__Glass.data,
      qt_meta_data_MaliitKeyboard__Glass,  qt_static_metacall, 0, 0}
};


const QMetaObject *MaliitKeyboard::Glass::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MaliitKeyboard::Glass::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MaliitKeyboard__Glass.stringdata))
        return static_cast<void*>(const_cast< Glass*>(this));
    return QObject::qt_metacast(_clname);
}

int MaliitKeyboard::Glass::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void MaliitKeyboard::Glass::keyPressed(const Key & _t1, Logic::LayoutHelper * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MaliitKeyboard::Glass::keyLongPressed(const Key & _t1, Logic::LayoutHelper * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MaliitKeyboard::Glass::keyReleased(const Key & _t1, Logic::LayoutHelper * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MaliitKeyboard::Glass::keyEntered(const Key & _t1, Logic::LayoutHelper * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MaliitKeyboard::Glass::keyExited(const Key & _t1, Logic::LayoutHelper * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MaliitKeyboard::Glass::wordCandidatePressed(const WordCandidate & _t1, Logic::LayoutHelper * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MaliitKeyboard::Glass::wordCandidateReleased(const WordCandidate & _t1, Logic::LayoutHelper * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void MaliitKeyboard::Glass::keyAreaPressed(Logic::LayoutHelper::Panel _t1, Logic::LayoutHelper * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void MaliitKeyboard::Glass::keyAreaReleased(Logic::LayoutHelper::Panel _t1, Logic::LayoutHelper * _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void MaliitKeyboard::Glass::switchLeft(Logic::LayoutHelper * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void MaliitKeyboard::Glass::switchRight(Logic::LayoutHelper * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void MaliitKeyboard::Glass::keyboardClosed()
{
    QMetaObject::activate(this, &staticMetaObject, 11, 0);
}
QT_END_MOC_NAMESPACE
