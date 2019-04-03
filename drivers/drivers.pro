include( ../q3d.pri )

TEMPLATE = lib

DEFINES += DRIVERS_LIBRARY

QT += opengl

LIBS += -lmodel -lplugins -lgui -lmesh -lcube -lfilters

win32 {
    LIBS += -lopengl32
}

# Input
HEADERS += \
    cpgmesh/cpgmesh.h \
    cpgmesh/cpgmesh_driver.h \
    cpgmesh/cpgmesh_renderer.h \
    cpgmesh/cpgtopo.h \
    mesh/mesh_driver.h \
    mesh/mesh_model.h \
    mesh/mesh_renderer.h \
    mesh/mesh_renderer_attribute.h \
    mesh/mesh_renderer_menu.h \
    mesh/mesh_renderer_attribute_dlg.h \
    drivers_plugin.h \
    drivers_global.h \
    drivers_renderer_attribute.h \
    drivers_renderer_attribute_dlg.h \
    cube/cube_model.h \
    cube/cube_renderer.h \
    cube/cube_renderer_attribute.h \
    cube/cube_renderer_attribute_dlg.h \
    cube/fda_cube_driver.h \
    cube/cube_actions.h

SOURCES += \
    cpgmesh/cpgmesh.cpp \
    cpgmesh/cpgmesh_driver.cpp \
    cpgmesh/cpgmesh_renderer.cpp \
    cpgmesh/cpgtopo.cpp \
    mesh/mesh_driver.cpp \
    mesh/mesh_model.cpp \
    mesh/mesh_renderer.cpp \
    mesh/mesh_renderer_attribute.cpp \
    mesh/mesh_renderer_menu.cpp \
    mesh/mesh_renderer_attribute_dlg.cpp \
    drivers_plugin.cpp \
    drivers_renderer_attribute.cpp \
    drivers_renderer_attribute_dlg.cpp \
    cube/cube_model.cpp \
    cube/cube_renderer.cpp \
    cube/cube_renderer_attribute.cpp \
    cube/cube_renderer_attribute_dlg.cpp \
    cube/fda_cube_driver.cpp \
    cube/cube_actions.cpp

FORMS += \
    mesh/mesh_renderer_attribute_dlg.ui \
    renderer_attribute_dlg.ui \
    cube/cube_renderer_attribute_dlg.ui


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
        cube/cube_load_mongo_dlg.h \
        cube/mongo_cube_driver.h\
        cube/mongo_load_action.h

    SOURCES += cube/mongo_cube_driver.cpp \
            cube/cube_load_mongo_dlg.cpp \
            cube/mongo_load_action.cpp

    FORMS += cube/cube_load_mongo_dlg.ui
}


DISTFILES += \
    driversplugin.json \
    mongodb.png

RESOURCES += \
    drivers_icons.qrc
