/****************************************************************************
** Meta object code from reading C++ file 'calculatorwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../calculatorwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calculatorwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CalculatorPushButton_t {
    QByteArrayData data[6];
    char stringdata0[112];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CalculatorPushButton_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CalculatorPushButton_t qt_meta_stringdata_CalculatorPushButton = {
    {
QT_MOC_LITERAL(0, 0, 20), // "CalculatorPushButton"
QT_MOC_LITERAL(1, 21, 29), // "calculatorButtonClickedSignal"
QT_MOC_LITERAL(2, 51, 0), // ""
QT_MOC_LITERAL(3, 52, 18), // "Calculator::Button"
QT_MOC_LITERAL(4, 71, 6), // "button"
QT_MOC_LITERAL(5, 78, 33) // "emitCalculatorButtonClickedSi..."

    },
    "CalculatorPushButton\0calculatorButtonClickedSignal\0"
    "\0Calculator::Button\0button\0"
    "emitCalculatorButtonClickedSignal"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalculatorPushButton[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   27,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Int, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CalculatorPushButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CalculatorPushButton *_t = static_cast<CalculatorPushButton *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->calculatorButtonClickedSignal((*reinterpret_cast< Calculator::Button(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->emitCalculatorButtonClickedSignal(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef int (CalculatorPushButton::*_t)(Calculator::Button );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CalculatorPushButton::calculatorButtonClickedSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CalculatorPushButton::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_CalculatorPushButton.data,
      qt_meta_data_CalculatorPushButton,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CalculatorPushButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalculatorPushButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalculatorPushButton.stringdata0))
        return static_cast<void*>(const_cast< CalculatorPushButton*>(this));
    return QPushButton::qt_metacast(_clname);
}

int CalculatorPushButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
int CalculatorPushButton::calculatorButtonClickedSignal(Calculator::Button _t1)
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}
struct qt_meta_stringdata_CalculatorWidget_t {
    QByteArrayData data[11];
    char stringdata0[119];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CalculatorWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CalculatorWidget_t qt_meta_stringdata_CalculatorWidget = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CalculatorWidget"
QT_MOC_LITERAL(1, 17, 11), // "printSignal"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 4), // "text"
QT_MOC_LITERAL(4, 35, 23), // "calculatorButtonClicked"
QT_MOC_LITERAL(5, 59, 18), // "Calculator::Button"
QT_MOC_LITERAL(6, 78, 6), // "button"
QT_MOC_LITERAL(7, 85, 15), // "emitPrintSignal"
QT_MOC_LITERAL(8, 101, 5), // "value"
QT_MOC_LITERAL(9, 107, 5), // "clear"
QT_MOC_LITERAL(10, 113, 5) // "reset"

    },
    "CalculatorWidget\0printSignal\0\0text\0"
    "calculatorButtonClicked\0Calculator::Button\0"
    "button\0emitPrintSignal\0value\0clear\0"
    "reset"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CalculatorWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   42,    2, 0x08 /* Private */,
       7,    2,   45,    2, 0x08 /* Private */,
       9,    0,   50,    2, 0x08 /* Private */,
      10,    0,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Int, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::Double, 0x80000000 | 5,    8,    6,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CalculatorWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CalculatorWidget *_t = static_cast<CalculatorWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { int _r = _t->printSignal((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 1: _t->calculatorButtonClicked((*reinterpret_cast< Calculator::Button(*)>(_a[1]))); break;
        case 2: _t->emitPrintSignal((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< Calculator::Button(*)>(_a[2]))); break;
        case 3: _t->clear(); break;
        case 4: _t->reset(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef int (CalculatorWidget::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CalculatorWidget::printSignal)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject CalculatorWidget::staticMetaObject = {
    { &QFrame::staticMetaObject, qt_meta_stringdata_CalculatorWidget.data,
      qt_meta_data_CalculatorWidget,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CalculatorWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CalculatorWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CalculatorWidget.stringdata0))
        return static_cast<void*>(const_cast< CalculatorWidget*>(this));
    return QFrame::qt_metacast(_clname);
}

int CalculatorWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QFrame::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
int CalculatorWidget::printSignal(const QString & _t1)
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(&_t0)), const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
    return _t0;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
