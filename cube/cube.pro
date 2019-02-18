include( ../q3d.pri )

QT       += opengl

TARGET = cube
CONFIG += shared

TEMPLATE = lib

DEFINES += CUBE_LIBRARY

LIBS += -lmodel -lplugins

win32 {
    LIBS += -lopengl32 -lgdi32
}

SOURCES += \
    cube.cpp \
    cube_model.cpp \
    fda_cube_driver.cpp \
    cube_renderer.cpp \
    cube_plugin.cpp \
    slicer.cpp \
    cube_renderer_attribute.cpp \
    cube_renderer_attribute_dlg.cpp \
    cube_api.cpp


HEADERS += \
    cube.h \
    cube_global.h \
    cube_model.h \
    fda_cube_driver.h \
    cube_renderer.h \
    cube_plugin.h \
    slicer.h \
    cube_renderer_attribute.h \
    cube_renderer_attribute_dlg.h \
    cube_api.h

FORMS += \
    cube_renderer_attribute_dlg.ui

CONFIG(mongo){
    DEFINES += WITH_MONGO

    INCLUDEPATH += \
        $${MONGO_DIR}/include/libbson-1.0 \
        $${MONGO_DIR}/include/libmongoc-1.0

    DEPENDPATH += \
        $${MONGO_DIR}/include/libbson-1.0 \
        $${MONGO_DIR}/include/libmongoc-1.0

    unix:!macx|win32 {
        LIBS += -L$${MONGO_DIR}/lib/ -lbson-1.0 -lmongoc-1.0
    }

    HEADERS += \
        cube_load_mongo_dlg.h \
        mongo_cube_driver.h\
        mongo_load_action.h

    SOURCES += mongo_cube_driver.cpp \
            cube_load_mongo_dlg.cpp \
            mongo_load_action.cpp

    FORMS += cube_load_mongo_dlg.ui
}


unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    cubeplugin.json \
    mongodb.png

RESOURCES += \
    cube_icons.qrc

