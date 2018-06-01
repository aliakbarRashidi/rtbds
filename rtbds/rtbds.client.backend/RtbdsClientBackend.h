#pragma once

#define RTBDSCLIENTBACKEND_VERSION ("0.1.0")

#if defined(_MSC_VER)
#if defined(RTBDSCLIENTBACKEND_EXPORTS)
#define RTBDSCLIENTBACKEND_DLL __declspec(dllexport)
#else
#define RTBDSCLIENTBACKEND_DLL __declspec(dllimport)
#endif
#else
#define RTBDSCLIENTBACKEND_DLL
#endif
