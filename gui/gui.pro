
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
    colormap_dlg.ui \
    renderer_attribute_dlg.ui

HEADERS +=\
           gl_area.h \
           main_window.h \
    view_control.h \
    level_dlg.h \
    color_ramp.h \
    colormap_dlg.h \
    color_button.h \
    gui_scope.h \
    renderer_attribute_dlg.h \
    renderer_menu.h

SOURCES += \
           gl_area.cpp \
           main_window.cpp \
    view_control.cpp \
    level_dlg.cpp \
    color_ramp.cpp \
    colormap_dlg.cpp \
    color_button.cpp \
    renderer_attribute_dlg.cpp \
    renderer_menu.cpp
