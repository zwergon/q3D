include( ../q3d.pri )

TEMPLATE = lib

CONFIG += shared

QT += widgets

LIBS += -lmodel -lplugins

DEFINES += PLUGINS_LIBRARY

SOURCES += \
    plugins.cpp \
    plugin_dialog.cpp \
    plugin_action.cpp \
    plugin_collection.cpp

HEADERS += \
    plugins.h\
    plugins_global.h \
    plugin_dialog.h \
    plugin_action.h \
    plugin_collection.h \
    plugin_interfaces.h

FORMS += \
    plugin_dialog.ui

