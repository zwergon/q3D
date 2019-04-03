
include( ../q3d.pri )

QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

LIBS += -lcube

TEMPLATE = app

SOURCES += \ 
    tst_geometry.cpp \
    tst_wave.cpp \
    main.cpp

HEADERS += \
    tst_geometry.h \
    tst_wave.h
