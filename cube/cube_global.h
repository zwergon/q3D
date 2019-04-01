#ifndef CUBE_GLOBAL_H
#define CUBE_GLOBAL_H

#ifdef _WIN32 || _WIN64
    #define Q_DECL_EXPORT __declspec(dllexport)
    #define Q_DECL_IMPORT __declspec(dllimport)
#else
    #define Q_DECL_EXPORT
    #define Q_DECL_IMPORT
#endif

#if defined(CUBE_LIBRARY)
#  define CUBESHARED_EXPORT Q_DECL_EXPORT
#else
#  define CUBESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CUBE_GLOBAL_H
