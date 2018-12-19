include( ../q3d.pri )

TEMPLATE = lib

CONFIG += shared 

QT -= gui
QT += opengl

win32 {
    LIBS += -lopengl32
}

DEFINES += MODEL_LIBRARY

# Input
HEADERS += \
    factory.hpp \
    model_scope.h \
    model.h \
    model_driver.h \
    model_driver_mgr.h \
    model_open_info.h \
    renderer.h \
    gl_data.h \
    colormap.h \
    renderer_attribute.h \
    renderer_attribute_dialog.h \
    matrix.h \
    point3d.h \
    model_mgr.h

SOURCES += \
    model.cpp \
    model_driver.cpp \
    model_driver_mgr.cpp \
    renderer.cpp \
    gl_data.cpp \
    colormap.cpp \
    renderer_attribute.cpp \
    renderer_attribute_dialog.cpp \
    model_mgr.cpp

