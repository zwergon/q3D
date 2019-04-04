
CONFIG += warn_on windows

Q3D_DIR=$$dirname(_PRO_FILE_)/..

CONFIG += mongo
MONGO_DIR=D:/lecomtje/Utils/mongo-c/x64

INCLUDEPATH += $${Q3D_DIR} $${Q3D_DIR}/..

DESTDIR=$${Q3D_DIR}/lib
LIBS += -L$${DESTDIR}


#QMAKE_CXXFLAGS += -Wno-enum-compare -Wno-unused-local-typedefs -Wno-sign-conversion





