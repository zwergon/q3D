include( ../q3d.pri )

QT       += widgets xml

TARGET = plugim
TEMPLATE = lib

DEFINES += PLUGIM_LIBRARY

LIBS += -lmodel -lplugins -lcube -ldrivers -lgui

SOURCES += \
    plugim_plugin.cpp \
    plugim_action.cpp \
    plugim_process.cpp

HEADERS += \
        plugim_global.h \ 
    plugim_plugin.h \
    plugim_action.h \
    plugim_process.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

RESOURCES += \
    plugim_icons.qrc

DISTFILES += \
    plugim_plugin.json
