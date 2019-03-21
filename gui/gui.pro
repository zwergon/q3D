
include( ../q3d.pri )

TEMPLATE = lib

DEFINES += GUI_LIBRARY

QT += opengl

LIBS += -lmodel -lplugins

win32 {
    LIBS += -lopengl32
}

# Input
FORMS += main_window.ui \
    level_dlg.ui \
    colormap_dlg.ui

HEADERS +=\
    gl_area.h \
    main_window.h \
    level_dlg.h \
    color_ramp.h \
    colormap_dlg.h \
    color_button.h \
    renderer_menu.h \
    abstract_tool.h \
    gui_global.h \
    glu.h \
    camera_tool.h \
    tool_manager.h \
    picking_tool.h \
    terminal_log.h \
    arc_ball.h \
    gl_data.h \
    view_control.h
SOURCES += \
    gl_area.cpp \
    main_window.cpp \
    level_dlg.cpp \
    color_ramp.cpp \
    colormap_dlg.cpp \
    color_button.cpp \
    renderer_menu.cpp \
    abstract_tool.cpp \
    glu.cpp \
    camera_tool.cpp \
    tool_manager.cpp \
    picking_tool.cpp \
    terminal_log.cpp \
    arc_ball.cpp \
    gl_data.cpp \
    view_control.cpp
