
CONFIG += warn_on windows

Q3D_DIR=$$dirname(_PRO_FILE_)/..

# Mongo defines
CONFIG += mongo
MONGO_DIR=D:/lecomtje/Utils/mongo-c/x64

#Itk defines
CONFIG += itk
ITK_DIR=D:/lecomtje/Utils/Itk

INCLUDEPATH += $${Q3D_DIR} $${Q3D_DIR}/..

DESTDIR=$${Q3D_DIR}/lib
LIBS += -L$${DESTDIR}


#QMAKE_CXXFLAGS += -Wno-enum-compare -Wno-unused-local-typedefs -Wno-sign-conversion





