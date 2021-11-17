
#ifndef _exportdef_h_
#define _exportdef_h_

#if defined (_MSC_VER)
#if defined (OSWINDLL)
#define oswinexp __declspec(dllexport)
#else
#define oswinexp __declspec(dllimport)
#endif
#pragma warning(disable: 4061)
#pragma warning(disable: 4127)
#pragma warning(disable: 4251)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4628)
#pragma warning(disable: 4820)
#pragma warning(disable: 4986)
#elif defined (__WATCOMC__)
#if defined (OSWINDLL)
#define oswinexp __export
#else
#define oswinexp
#endif
#else
#define oswinexp
#endif

#endif
