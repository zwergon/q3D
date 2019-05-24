include(../q3d.pri)

TARGET = itk
TEMPLATE = lib

QT += opengl xml

DEFINES += ITK_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


CONFIG(itk){

ITK_LIBS= \
    -lITKCommon-4.13 \
    -lITKIOImageBase-4.13 \
    -lITKIOJPEG-4.13 \
    -lITKIOMesh-4.13 \
    -lITKIOVTK-4.13 \
    -lITKVNLInstantiation-4.13 \
    -litkvnl-4.13 \
    -litkvnl_algo-4.13 \
    -litkvcl-4.13 \
    -litksys-4.13 \
    -litkv3p_netlib-4.13 \
    -litkgiftiio-4.13 \
    -lITKniftiio-4.13 \
    -lITKEXPAT-4.13 \
    -litkzlib-4.13 \
    -litkznz-4.13 \
    -litkdouble-conversion-4.13


INCLUDEPATH += $${ITK_DIR}/include/ITK-4.13
DEPENDPATH +=  $${ITK_DIR}/include/ITK-4.13

SOURCES += \
        itk_plugin.cpp

HEADERS += \
        itk_plugin.h \
        itk_global.h 

LIBS += \
    -L$${ITK_DIR}/lib $${ITK_LIBS} \
    -lcube -lmodel -lplugins -ldrivers -lgui

win32 {
    LIBS += -lopengl32 -ladvapi32 -lgdi32 -lshell32 -luser32
}

HEADERS += \
    itk_mesh_driver.h \
    itk_mesh_model.h \
    itk_mesh_renderer.h \
    itk_mesh_renderer_attribute.h \
    itk_mesh_renderer_attribute_dlg.h

SOURCES += \
    itk_mesh_driver.cpp \
    itk_mesh_model.cpp \
    itk_mesh_renderer.cpp \
    itk_mesh_renderer_attribute.cpp \
    itk_mesh_renderer_attribute_dlg.cpp

DISTFILES += \
    itkplugin.json

FORMS += \
    itk_mesh_renderer_attribute_dlg.ui
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS += \
    itk_cube_actions.h \
    itk_cube_2_image.h

SOURCES += \
    itk_cube_actions.cpp

RESOURCES += \
    itk_icons.qrc

DISTFILES += \
    median_filter.xml

