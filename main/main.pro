include( ../q3d.pri )

TEMPLATE = app

QT += opengl

RESOURCES += ../icons/icons.qrc

LIBS += -L$${Q3D_DIR}/libs \
 -lmodel\
 -lplugins\
 -lgui

TARGET = main
CONFIG   -= app_bundle

SOURCES += main.cpp
