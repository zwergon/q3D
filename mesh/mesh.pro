include( ../q3d.pri )

TEMPLATE = lib

DEFINES += MESH_LIBRARY

QT       -= gui

# Input
HEADERS += \
    mesh.h \
    local_cell_topology.h \
    cells.h \
    mesh_global.h \
    types.h \
    property_cube.h \
    property_db.h \
    property.h \
    region_db.h \
    region.h
SOURCES += \
    mesh.cpp \
    local_cell_topology.cpp \
    cells.cpp \
    property_cube.cpp \
    property_db.cpp \
    property.cpp \
    region_db.cpp \
    region.cpp
