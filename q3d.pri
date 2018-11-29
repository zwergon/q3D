

CONFIG += warn_on windows

Q3D_DIR=$$dirname(_PRO_FILE_)/..

INCLUDEPATH += . .. $${Q3D_DIR}/..

UI_DIR=$${Q3D_DIR}/ui

DEPENDPATH += $${Q3D_DIR}/libs

QMAKE_CXXFLAGS += -Wno-enum-compare -Wno-unused-local-typedefs





