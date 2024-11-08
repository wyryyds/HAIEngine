#pragma once

#ifdef HE_PLATFORM_WINDOWS
#if HE_DYNAMIC_LINK
    #ifdef HE_BUILD_DLL
       #define HE_API __declspec(dllexport)
    #else
       #define HE_API __declspec(dllimport)
    #endif // HE_BUILD_DLL
#else 
    #define HE_API
#endif
#else 
    #error HAIEngine only support Windows!
#endif // HE_PLATFORM_WINDOWS

#ifdef HE_ENABLE_ASSERTS
    #define HE_ASSERT(x,...) { if(!(x)) {HE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define HE_CORE_ASSERT(x,...){ if(!(x)) { HE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define HE_ASSERT(x,...)
    #define HE_CORE_ASSERT(x,...)
#endif // HE_ENABLE_ASSERTS

#define BIT(x) (1 << (x))

#define HE_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define ROOTPATH "../../../../"
#define SHADERPATH "../../../../Assets/Shaders/"
#define ASSETSPATH "../../../../Assets/"
