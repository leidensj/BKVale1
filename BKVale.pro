#-------------------------------------------------
#
# Project created by QtCreator 2016-10-19T22:15:42
#
#-------------------------------------------------

QT       += core gui serialport sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BKVale
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tinyexpr.c \
    settingsdlg.cpp \
    printutils.cpp \
    databaseutils.cpp \
    notewidget.cpp \
    note.cpp \
    settings.cpp \
    notedatabasewidget.cpp \
    reminderwidget.cpp \
    productwidget.cpp \
    consumptionwidget.cpp

HEADERS  += mainwindow.h \
    tinyexpr.h \
    settingsdlg.h \
    printutils.h \
    databaseutils.h \
    notewidget.h \
    note.h \
    settings.h \
    notedatabasewidget.h \
    reminderwidget.h \
    reminder.h \
    productwidget.h \
    product.h \
    consumptionwidget.h

FORMS    += mainwindow.ui \
    settingsdlg.ui \
    notewidget.ui \
    notedatabasewidget.ui \
    reminderwidget.ui \
    productwidget.ui \
    consumptionwidget.ui

RESOURCES += \
    icons.qrc

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
