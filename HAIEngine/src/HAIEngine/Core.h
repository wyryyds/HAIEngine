#pragma once

#ifdef HE_PLATFORM_WINDOWS

    #ifdef HE_BUILD_DLL
       #define HE_API __declspec(dllexport)
    #else
       #define HE_API __declspec(dllimport)
    #endif // HE_BUILD_DLL
#else
    #error HAIEngine only support Windows!
#endif // HE_PLATFORM_WINDOWS
