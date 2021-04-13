#pragma once

#ifdef MS_PLATFORM_WINDOWS
#ifdef MS_BUILD_DLL
#define MS_API _declspec(dllexport)
#else
#define MS_API _declspec(dllimport)
#endif // MS_BUILD_DLL
#else
#error Only Support Windows!
#endif // !MS_PLATFORM_WINDOWS

#define BIT(x) (1<<x)

#ifdef MS_ENABLE_ASSERTS
#define MS_ASSERT(x, ...) { if(!(x)) { MS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#define MS_CORE_ASSERT(x, ...) { if(!(x)) { MS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#define MS_ASSERT(x, ...)
#define MS_CORE_ASSERT(x, ...)
#endif