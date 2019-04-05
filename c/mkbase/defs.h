
#ifndef _def_h_
#define _def_h_

#include <stdarg.h>

#ifdef __cplusplus
#define xtern extern "C"
#else
#define xtern extern
#endif

/* int types */
#if defined (_MSC_VER)
#include <basetsd.h>
#if defined (_WIN64)
typedef int64_t mk_lreal;
typedef uint64_t mk_ulreal;
#else
typedef __int64__ mk_lreal;
typedef __uint64__ mk_ulreal;
#endif
#else
#if defined (__WATCOMC__)
typedef int64_t mk_lreal;
typedef uint64_t mk_ulreal;
#define finite(x) isfinite(x)
#else
#if defined (__MACH__)
#include <inttypes.h>
typedef long long int mk_lreal;
typedef unsigned long long int  mk_ulreal;
#else
#include <inttypes.h>
typedef long long int mk_lreal;
typedef unsigned long long int mk_ulreal;
#endif
#endif
#endif

#if !defined MIN
#define MIN(x,y) ((x)<(y) ? (x) : (y))
#endif
#if !defined MAX
#define MAX(x,y) ((x)<(y) ? (y) : (x))
#endif

#define _ARG2(_0,_1,_2,...) _2
#define NARG2(...) _ARG2(__VA_ARGS__,2,1,0)
#define _ARG3(_0,_1,_2,_3,...) _3
#define NARG3(...) _ARG3(__VA_ARGS__,3,2,1,0)
#define _ARG4(_0,_1,_2,_3,_4,...) _4
#define NARG4(...) _ARG4(__VA_ARGS__,4,3,2,1,0)
#define _ARG5(_0,_1,_2,_3,_4,_5,...) _5
#define NARG5(...) _ARG5(__VA_ARGS__,5,4,3,2,1,0)
#define _ARG6(_0,_1,_2,_3,_4,_5,_6,...) _6
#define NARG6(...) _ARG6(__VA_ARGS__,6,5,4,3,2,1,0)
#define _ARG7(_0,_1,_2,_3,_4,_5,_6,_7,...) _7
#define NARG7(...) _ARG7(__VA_ARGS__,7,6,5,4,3,2,1,0)

#define mk_diff(...) mk_diff_(NARG4(__VA_ARGS__),__VA_ARGS__)
#define mk_round2(...) mk_round2_(NARG3(__VA_ARGS__),__VA_ARGS__)
#define mk_roundup(...) mk_roundup_(NARG3(__VA_ARGS__),__VA_ARGS__)
#define mk_rounddown(...) mk_rounddown_(NARG3(__VA_ARGS__),__VA_ARGS__)
#define mk_txtchknum(...) mk_txtchknum_(NARG3(__VA_ARGS__),__VA_ARGS__)
#define mk_mag(...) mk_mag_(NARG2(__VA_ARGS__),__VA_ARGS__)
#define mk_a2ui(...) mk_a2ui_(NARG4(__VA_ARGS__),__VA_ARGS__)
#define mk_a2i(...) mk_a2i_(NARG3(__VA_ARGS__),__VA_ARGS__)
#define mk_a2d(...) mk_a2d_(NARG4(__VA_ARGS__),__VA_ARGS__)
#define mk_ui2a(...) mk_ui2a_(NARG6(__VA_ARGS__),__VA_ARGS__)
#define mk_i2a(...) mk_i2a_(NARG6(__VA_ARGS__),__VA_ARGS__)
#define mk_d2a(...) mk_d2a_(NARG7(__VA_ARGS__),__VA_ARGS__)

union tp_ucpd {
  unsigned char ucd[8];
  double d;
};
/* const union tp_ucpd defdnan={{127,248,0,0,0,0,0,0}};  big endian */
static const union tp_ucpd mk_defdnan={{0,0,0,0,0,0,248,127}};  /* little endian */
/* const union tp_ucpd defdsnan={{255,248,0,0,0,0,0,0}};  big endian */
static const union tp_ucpd mk_defdsnan={{0,0,0,0,0,0,248,255}}; /* little endian */
/* const union tp_ucpd defdinf={{127,240,0,0,0,0,0,0}};  big endian */
static const union tp_ucpd mk_defdinf={{0,0,0,0,0,0,240,127}}; /* little endian */
/* const union tp_ucpd defdsinf={{255,240,0,0,0,0,0,0}};  big endian */
static const union tp_ucpd mk_defdsinf={{0,0,0,0,0,0,240,255}}; /* little endian */

#define mk_dnan *((const double *)&mk_defdnan.d)
#define mk_dsnan *((const double *)&mk_defdsnan.d)
#define mk_dinf *((const double *)&mk_defdinf.d)
#define mk_dsinf *((const double *)&mk_defdsinf.d)

union tp_ucpf {
  unsigned char ucf[4];
  float f;
};
/* const union tp_ucpf deffnan={{127,192,0,0}};  big endian */
static const union tp_ucpf mk_deffnan={{0,0,192,127}};  /* little endian */
/* const union tp_ucpf deffsnan={{255,192,0,0}};  big endian */
static const union tp_ucpf mk_deffsnan={{0,0,192,255}};  /* little endian */
/* const union tp_ucpf deffinf={{127,128,0,0}};  big endian */
static const union tp_ucpf mk_deffinf={{0,0,128,127}};   /* little endian */
/* const union tp_ucpf deffsinf={{255,128,0,0}};  big endian */
static const union tp_ucpf mk_deffsinf={{0,0,128,255}};  /* little endian */

#define mk_fnan *((const double *)&mk_deffnan.d)
#define mk_fsnan *((const double *)&mk_deffsnan.d)
#define mk_finf *((const double *)&mk_deffinf.d)
#define mk_fsinf *((const double *)&mk_deffsinf.d)

#define mk_sz 1024
#define mk_maxarrsz 20
#define mk_maxintbase 36
#define mk_maxmem 2147483647

#endif