
#ifndef _mkbase_h_
#define _mkbase_h_

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <mkbase/defs.h>
#include <mkbase/exportdefs.h>

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

#if defined (_MSC_VER)
static const mk_lreal mk_i64limit=9223372036854775807i64;
static const mk_ulreal mk_ui64limit=18446744073709551615ui64;
static const unsigned int mk_uilimit=4294967295;
#else
#if defined (__WATCOMC__)
static const mk_lreal mk_i64limit=9223372036854775807LL;
static const mk_ulreal mk_ui64limit=18446744073709551615ULL;
static const unsigned int mk_uilimit=4294967295U;
#else
#if defined (__MACH__)
static const mk_lreal mk_i64limit=9223372036854775807LL;
static const mk_ulreal mk_ui64limit=18446744073709551615ULL;
static const unsigned int mk_uilimit=4294967295U;
#else
static const mk_lreal mk_i64limit=9223372036854775807LL;
static const mk_ulreal mk_ui64limit=18446744073709551615ULL;
static const unsigned int mk_uilimit=4294967295U;
#endif
#endif
#endif
static const int mk_ilimit=2147483647;
static const unsigned short mk_uslimit=65535;
static const short mk_slimit=32767;
static const double mk_dlimit=1.7976931348623157e308;
static const double mk_derr=0.000000000000001;
static const double mk_euler=2.718281828459045235;
static const double mk_pi=3.1415926535897932385;
static const double mk_rad=0.0174532925199432957694;
static const double mk_log210=0.3010299956639812;
static const double mk_loge10=0.4342944819032518;
static const double mk_log10e=2.3025850929940457;
static const unsigned short mk_dmag=308;
static const unsigned short mk_i64mag=18;
static const unsigned short mk_ui64mag=19;
static const unsigned short mk_dprec=15;
static const unsigned short mk_ddprec=3;
static const unsigned short mk_separatorgrouplen=3;
static const unsigned short mk_maxseparatorlen=6;
static const unsigned short mk_maxarrsz=20;
static const unsigned short mk_maxintbase=36;
static const unsigned short mk_maxnumstrlen=316;
static const unsigned int mk_maxstrlen=1073741824;
static const unsigned char mk_basechar[]={
  '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F','G','H','I','J','K','L','M',
  'N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0'};
static const unsigned char mk_asciispace=32;
static const unsigned char mk_asciihash=35;
static const unsigned char mk_asciiplus=43;
static const unsigned char mk_asciiminus=45;
static const unsigned char mk_asciidec=46;
static const unsigned char mk_asciizero=48;
static const unsigned char mk_asciione=49;
static const unsigned char mk_asciifive=53;
static const unsigned char mk_asciinine=57;
static const unsigned char mk_asciiE=69;
static const unsigned char mk_asciiF=70;
static const unsigned char mk_asciiI=73;
static const unsigned char mk_asciiX=88;
static const unsigned char mk_asciib=98;
static const unsigned char mk_asciie=101;
static const unsigned char mk_asciif=102;
static const unsigned char mk_asciii=105;
static const unsigned char mk_asciix=120;
static const unsigned char mk_asciidel=127;

/* get sign , in number , return -1,1 */
inline int oswinexp mk_dsgn(double d) {
  return (((((const unsigned char *)&d)[7])>>7)>0 ? -1 : 1);
} // little endian 0 -> 7

/* get sign , in number , return -1,1 */
inline double oswinexp mk_dsign(double d) {
  return (double)mk_dsgn(d);
}

/* in number , return 0 or -1,1 */
inline int oswinexp mk_isinf(double d) {
#if defined (_MSC_VER)
  return (_fpclass(d)==_FPCLASS_NINF ? -1 : (_fpclass(d)==_FPCLASS_PINF ? 1 : 0));
#else
  return (finite(d) ? 0 : mk_dsgn(d));
#endif
}

/* in number , return 0 or -1,1 */
inline int oswinexp mk_isnan(double d) {
#if defined (_MSC_VER)
  return (_isnan(d) ? mk_dsgn(d) : 0);
#else
  return (isnan(d) ? mk_dsgn(d) : 0);
#endif
}

/* in number , return 0 or -1,1 */
inline int oswinexp mk_isfinite(double d) {
#if defined (_MSC_VER)
  return (_finite(d) ? mk_dsgn(d) : 0);
#else
  return (finite(d) ? mk_dsgn(d) : 0);
#endif
}

/* is or not a regular number , in number , return 0 or -1,1 */
inline int oswinexp mk_isbusted(double d) {
#if defined (_MSC_VER)
  return (!_isnan(d) && _finite(d) ? 0 : mk_dsgn(d));
#else
  return (!isnan(d) && finite(d) ? 0 : mk_dsgn(d));
#endif
}

#ifdef __cplusplus
/* #pragma message "cplusplus" */
extern "C" {
#endif

/* return global-cnt */
xtern mk_ulreal oswinexp mk_nextcnt();

/* return time-cnt */
xtern mk_ulreal oswinexp mk_nextt();

/* inout *number1 , inout *number2 */
xtern void oswinexp mk_swapi(int *,int *);

/* inout *number1 , inout *number2 */
xtern void oswinexp mk_swapf(double *,double *);

/* inout *string1 , inout *string2 */
xtern void oswinexp mk_swapc(char **,char **);

/* in exponent , return result */
xtern double oswinexp mk_ipow10(int);

/* in exponent , return result */
xtern double oswinexp mk_ipow2(int);

/* in str , in str dec=. , in str group=0 , return 0,1 */
xtern int oswinexp mk_removeseparators(char *,const char * const,const char * const);

/* in str , in maxlen , in str dec=0 , in str group=0 , return 0,1 */
xtern int oswinexp mk_insertseparators(char *,int,const char * const,const char * const);

/* in str , inout *base=0 , out *sgn=0 , in str group=0 , return number or 0 */
xtern mk_ulreal oswinexp mk_parseint(const char *,int *,int *,const char *);

/* in str , out *type=0 , out *prec=0 , in str dec=. , in str group=0 , return number or nan */
xtern double oswinexp mk_parsefloat(const char *,int *,int *,const char *,const char *);

/* in str , inout *base=0 , out *sign=0 , in str group=0 , return number */
xtern mk_ulreal oswinexp mk_a2ui_(int,...);

/* in str , inout *base=0 , in str group=0 , return number */
xtern mk_lreal oswinexp mk_a2i(const char *,int *,const char *);

/* in number , out str , in base=10 , in width=0 , in padzero=1 , in str group=0 , return base */
xtern int oswinexp mk_ui2a(mk_ulreal,char [mk_klen],int,int,int,const char *);

/* in number , out str , in base=10 , in width=0 , in padzero=1 , in str group=0 , return base */
xtern int oswinexp mk_i2a(mk_lreal,char [mk_klen],int,int,int,const char *);

/* in number , in number , out *overflow=0 , return result */
xtern mk_ulreal oswinexp mk_binadd(mk_ulreal,mk_ulreal,int *);

/* in number , in number , out *overflow=0 , return result */
xtern mk_ulreal oswinexp mk_binmult(mk_ulreal,mk_ulreal,int *);

/* in str , out *prec=0 , in str dec=. , in str group=0 , return number or nan */
xtern double oswinexp mk_a2d_(int,...);

/* in number , out str , in prec , in fmt=-1 , in pad=-1 , return 0,1 */
xtern int oswinexp mk_dconvert (double,char [mk_klen],int,char,int);

/*
  in number , out str , in prec=15 , in fmt=-1 , in pad=-1 , in str dec=. ,
  in str group=0 , return 0,1
*/
xtern int oswinexp mk_d2a(double,char [mk_klen],int,char,int,const char *,const char *);

/* inout str , return 0,1 */
xtern int oswinexp mk_exp2simple(char [mk_klen]);

/* in number , in calcedge=1 , return magnitude-of-number */
xtern int oswinexp mk_mag_(int,...);

/* [diff of] regular or no regular numbers , in number , in number , out *cmp=0 [-1,0,1] , return 0,1 */
xtern int oswinexp mk_dbusted(double,double,int *);

/* raw double equal , in number , in number , return 0,1 */
xtern int oswinexp mk_deq(double,double);

/* raw double lower , in number , in number , return 0,1 */
xtern int oswinexp mk_dlt(double,double);

/* raw double greater , in number , in number , return 0,1 */
xtern int oswinexp mk_dgt(double,double);

/* in number , in number , in epsilon=e-15 , in toleranceafterepsilondiff=e-6 , return result */
xtern double oswinexp mk_diff_(int,...);

/* in number , in pos=0 , in toleranceafterposition=e-4 , return rounded-number */
xtern double oswinexp mk_round2_(int,...);

/* in number , in pos , in toleranceafter=3 , return rounded-number */
xtern double oswinexp mk_roundup_(int,...);

/* in number , in pos , in toleranceafter=3 , return rounded-number */
xtern double oswinexp mk_rounddown_(int,...);

/* in number , in base , return result */
xtern double oswinexp mk_logm(double,double);

/* in number , in exponent , return result */
xtern double oswinexp mk_powrat(double,float);

/* in number , in array , out indexarray */
xtern void oswinexp mk_indextab1(int,double[],int []);

/* in number , in array , out indexarray */
xtern void oswinexp mk_indextab2(int,double [],int []);

/* in number , return 0,1 */
xtern int oswinexp mk_isprim(int);

/* in number , return 0,1 */
xtern int oswinexp mk_isprim3(int);

/* in number , return result */
xtern int oswinexp mk_nextlargerprim(int);

/* lowest common multiplier, in number , in number , return result */
xtern int oswinexp mk_lcm(int,int);

/* greatest common divisor, in number , in number , return result */
xtern int oswinexp mk_gcd(int,int);

/* double%double , in number , in number , return result */
xtern double oswinexp mk_doublemod(double,double);

/* in numberelements , inout **array , in cmp-function */
xtern void oswinexp mk_heapsort(int,void **,int (*)(const void *,const void *));

/*
  in *look-for , in numberelements , in **array , in cmp-function ,
  in guess-likely-index=-1 , return index-in-array
*/
xtern int oswinexp mk_binsearch(const void *,int,const void **,int (*)(const void *,const void *),int);

#ifdef __cplusplus
}
#endif

#endif
