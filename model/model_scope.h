#ifndef MODEL_GLOBAL_H
#define MODEL_GLOBAL_H

#include <QtGlobal>

#if defined(MODEL_LIBRARY)
#  define MODELSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MODELSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MODEL_GLOBAL_H
