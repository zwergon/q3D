#ifndef PLUGIM_GLOBAL_H
#define PLUGIM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(PLUGIM_LIBRARY)
#  define PLUGIMSHARED_EXPORT Q_DECL_EXPORT
#else
#  define PLUGIMSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // PLUGIM_GLOBAL_H
