
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
    cube

CONFIG+=ordered

SUBDIRS+=main
          
       
