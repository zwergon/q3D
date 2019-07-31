include( ../q3d.pri )

QT       += opengl

TEMPLATE = lib

DEFINES += GEOANALOG_LIBRARY

LIBS += -lmodel -lplugins -lgui -lmesh -lcube -lfilters -ldrivers

SOURCES += \
        geoanalog_plugin.cpp \
    geoanalog_action.cpp \
    geoanalog_driver.cpp \
    geoanalog_exam.cpp \
    geoanalog_model.cpp

HEADERS += \
        geoanalog_plugin.h \
        geoanalog_global.h \ 
    geoanalog_action.h \
    geoanalog_driver.h \
    geoanalog_exam.h \
    geoanalog_model.h

CONFIG(mongo){
    DEFINES += WITH_MONGO

    INCLUDEPATH += \
        $${MONGO_DIR}/include/libbson-1.0 \
        $${MONGO_DIR}/include/libmongoc-1.0

    DEPENDPATH += \
        $${MONGO_DIR}/include/libbson-1.0 \
        $${MONGO_DIR}/include/libmongoc-1.0

    unix:!macx|win32 {
        LIBS += -L$${MONGO_DIR}/lib/ -lbson-1.0 -lmongoc-1.0
    }

SOURCES += \
    geoanalog_mongo_dlg.cpp

HEADERS += \
    geoanalog_mongo_dlg.h

FORMS += \
    geoanalog_mongo_dlg.ui
}

RESOURCES += \
    geanalog_icons.qrc

DISTFILES += \
    geoanalog_plugin.json
