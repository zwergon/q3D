include( ../q3d.pri )

TEMPLATE = lib

CONFIG += shared 

QT -= gui
QT += opengl xml

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
    colormap.h \
    renderer_attribute.h \
    renderer_attribute_dialog.h \
    matrix.h \
    point3d.h \
    model_mgr.h \
    geometry.h \
    picking.h \
    process_info.h \
    process_mgr.h \
    process_.h \
    process_log.h \
    renderer_area.h \
    uv_quad.h \
    params_info.h

SOURCES += \
    model.cpp \
    model_driver.cpp \
    model_driver_mgr.cpp \
    renderer.cpp \
    colormap.cpp \
    renderer_attribute.cpp \
    renderer_attribute_dialog.cpp \
    model_mgr.cpp \
    geometry.cpp \
    picking.cpp \
    process_info.cpp \
    process_mgr.cpp \
    process_.cpp \
    process_log.cpp \
    uv_quad.cpp \
    renderer_area.cpp \
    params_info.cpp

