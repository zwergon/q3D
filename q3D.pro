
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
    itk

CONFIG+=ordered

SUBDIRS+=main
          
       
