#ifndef CUBE_GLOBAL_H
#define CUBE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CUBE_LIBRARY)
#  define CUBESHARED_EXPORT Q_DECL_EXPORT
#else
#  define CUBESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CUBE_GLOBAL_H
