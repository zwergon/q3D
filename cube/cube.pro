include( ../q3d.pri )

QT       += opengl

QT       -= gui

TARGET = cube
TEMPLATE = lib

DEFINES += CUBE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = $${Q3D_DIR}/libs

LIBS += -L$${Q3D_DIR}/libs -lmodel -lplugins
win32 {
    LIBS += -lopengl32 -lgdi32
}

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cube.cpp \
    cube_model.cpp \
    fda_cube_driver.cpp \
    cube_renderer.cpp \
    cube_plugin.cpp \
    slicer.cpp

HEADERS += \
        cube.h \
        cube_global.h \ 
        cube_model.h \
    fda_cube_driver.h \
    cube_renderer.h \
    cube_plugin.h \
    slicer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    cubeplugin.json
