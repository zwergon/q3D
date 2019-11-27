
TEMPLATE =subdirs

SUBDIRS = \
# q3D
    model \
    plugins\
    gui \
# drivers
    mesh \
    cube \
    filters \
    drivers \
    plugim \
# unit tests
    tests \
    geoanalog

CONFIG(debug, debug|release){
    SUBDIRS += itk
}

CONFIG+=ordered

SUBDIRS+=main
          
       
