include( ../q3d.pri )

QT       += widgets xml

TARGET = plugim
TEMPLATE = lib

DEFINES += PLUGIM_LIBRARY

LIBS += -lmodel -lplugins -lcube

SOURCES += \
    plugim_plugin.cpp \
    plugim_action.cpp \
    plugim_process.cpp \
    plugim_dialog.cpp \
    widget_creator.cpp

HEADERS += \
        plugim_global.h \ 
    plugim_plugin.h \
    plugim_action.h \
    plugim_process.h \
    plugim_dialog.h \
    widget_creator.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    plugim_icons.qrc

DISTFILES += \
    plugim_plugin.json
