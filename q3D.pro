
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
    itk \
    geoanalog

CONFIG+=ordered

SUBDIRS+=main
          
       
