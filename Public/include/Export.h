#pragma once
#ifndef _LIB
#pragma once
#ifdef ONVIFLIB_EXPORTS
#define ONVIFLIB_API __declspec(dllexport)
#else
#define ONVIFLIB_API __declspec(dllimport)
#endif
#else
#define ONVIFLIB_API
#endif