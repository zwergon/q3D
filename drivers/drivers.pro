include( ../q3d.pri )

TEMPLATE = lib

DEFINES += DRIVERS_LIBRARY

QT += opengl


win32 {
    LIBS += -lopengl32
}

DESTDIR = $${Q3D_DIR}/libs

LIBS += -L$${Q3D_DIR}/libs -lmodel -lmesh -lfilters -lgui -lplugins

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
    drivers_renderer_attribute_dlg.h

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
    drivers_renderer_attribute_dlg.cpp

FORMS += \
    mesh/mesh_renderer_attribute_dlg.ui \
    renderer_attribute_dlg.ui

DISTFILES += \
    driversplugin.json
