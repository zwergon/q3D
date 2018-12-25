include( ../q3d.pri )

TEMPLATE = app
CONFIG   -= app_bundle

QT += opengl

RESOURCES += ../icons/icons.qrc

LIBS +=  -lmodel -lplugins -lgui


TARGET = main

SOURCES += main.cpp

