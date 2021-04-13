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