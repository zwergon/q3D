#ifndef ITK_GLOBAL_H
#define ITK_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ITK_LIBRARY)
#  define ITKSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ITKSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ITK_GLOBAL_H
