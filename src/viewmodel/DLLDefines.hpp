#ifndef GUARD_DLLDefines_h
#define GUARD_DLLDefines_h

#ifdef _WIN32
    #if defined(viewmodel_EXPORTS)
        #define VIEWMODEL_EXPORT __declspec(dllexport)
    #else
        #define VIEWMODEL_EXPORT __declspec(dllimport)
    #endif
#else
    #define VIEWMODEL_EXPORT
#endif

#endif