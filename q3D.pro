
TEMPLATE =subdirs

SUBDIRS = \
# q3D
    model \
    plugins\
    gui \
# drivers
    mesh \
    filters \
    drivers \
    cube \
    plugim \
# unit tests
    tests

CONFIG+=ordered

SUBDIRS+=main
          
       
