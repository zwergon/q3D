include( ../q3d.pri )


TEMPLATE = lib

CONFIG += shared 

QT += opengl

win32 {
    LIBS += -lopengl32
}

DEFINES += MODEL_LIBRARY

DESTDIR= $${Q3D_DIR}/libs

# Input
HEADERS += factory.hpp \
           model_scope.h \
           model.h \
           model_driver.h \
           model_driver_mgr.h \
           model_open_info.h \
    renderer.h \
    gl_data.h \
    colormap.h \
    types.h \
    renderer_attribute.h \
    models_data.h \
    renderer_attribute_dialog.h

SOURCES += model.cpp \
           model_driver.cpp \
           model_driver_mgr.cpp \
    renderer.cpp \
    gl_data.cpp \
    colormap.cpp \
    renderer_attribute.cpp \
    models_data.cpp \
    renderer_attribute_dialog.cpp

