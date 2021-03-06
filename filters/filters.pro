! include( ../q3d.pri ) {
    error( "Couldn't find the q3d.pri file!" )
}

QT       -= gui

TEMPLATE = lib

DEFINES += FILTERS_LIBRARY

LIBS += -lmesh

SOURCES += \
    gmsh_mesh_importer.cpp \
    mesh_importer.cpp \
    gmsh_mesh_exporter.cpp

HEADERS +=\
        filters_global.h \
    gmsh_mesh_importer.h \
    mesh_importer.h \
    gmsh_mesh_exporter.h


