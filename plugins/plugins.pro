include( ../q3d.pri )


TEMPLATE = lib

CONFIG += shared

QT += widgets

DEFINES += PLUGINS_LIBRARY

DESTDIR= $${Q3D_DIR}/libs


SOURCES += plugins.cpp \
    plugin_dialog.cpp

HEADERS += plugins.h\
        plugins_global.h \
    interfaces.h \
    plugin_dialog.h

FORMS += \
    plugin_dialog.ui

