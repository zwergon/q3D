
! include( ../q3d.pri ) {
    error( "Couldn't find the q3d.pri file!" )
}

TEMPLATE = lib

DEFINES += GUI_LIBRARY

QT += opengl

win32 {
    LIBS += -lopengl32
}

DESTDIR= $${Q3D_DIR}/libs

LIBS += -L$${Q3D_DIR}/libs -lmodel -lplugins

# Input
FORMS += main_window.ui \
    level_dlg.ui \
    colormap_dlg.ui

HEADERS +=\
           gl_area.h \
           main_window.h \
    view_control.h \
    level_dlg.h \
    color_ramp.h \
    colormap_dlg.h \
    color_button.h \
    renderer_menu.h \
    abstract_tool.h \
    gui_global.h \
    glu.h \
    camera_tool.h

SOURCES += \
           gl_area.cpp \
           main_window.cpp \
    view_control.cpp \
    level_dlg.cpp \
    color_ramp.cpp \
    colormap_dlg.cpp \
    color_button.cpp \
    renderer_menu.cpp \
    abstract_tool.cpp \
    glu.cpp \
    camera_tool.cpp
