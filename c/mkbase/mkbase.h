
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
static const mk_lreal mk_i64Limit=9223372036854775807i64;
static const mk_ulreal mk_ui64Limit=18446744073709551615ui64;
static const unsigned int mk_uiLimit=4294967295;
#else
#if defined (__WATCOMC__)
static const mk_lreal mk_i64Limit=9223372036854775807LL;
static const mk_ulreal mk_ui64Limit=18446744073709551615ULL;
static const unsigned int mk_uiLimit=4294967295U;
#else
#if defined (__MACH__)
static const mk_lreal mk_i64Limit=9223372036854775807LL;
static const mk_ulreal mk_ui64Limit=18446744073709551615ULL;
static const unsigned int mk_uiLimit=4294967295U;
#else
static const mk_lreal mk_i64Limit=9223372036854775807LL;
static const mk_ulreal mk_ui64Limit=18446744073709551615ULL;
static const unsigned int mk_uiLimit=4294967295U;
#endif
#endif
#endif
static const int mk_iLimit=2147483647;
static const unsigned short mk_usLimit=65535;
static const short mk_sLimit=32767;
static const double mk_dLimit=1.7976931348623157e308;
static const double mk_dErr=0.000000000000001;
static const double mk_euler=2.718281828459045235;
static const double mk_pi=3.1415926535897932385;
static const double mk_rad=0.0174532925199432957694;
static const double mk_log210=0.3010299956639812;
static const double mk_loge10=0.4342944819032518;
static const double mk_log10e=2.3025850929940457;
static const unsigned short mk_dMag=308;
static const unsigned short mk_i64Mag=18;
static const unsigned short mk_ui64Mag=19;
static const unsigned short mk_dPrec=15;
static const unsigned short mk_ddPrec=3;
static const unsigned short mk_separatorGroupLen=3;
static const unsigned short mk_maxSeparatorLen=6;
static const unsigned short mk_maxArrSz=20;
static const unsigned short mk_maxintbase=36;
static const unsigned short mk_maxNumstrLen=316;
static const unsigned int mk_maxStrLen=1073741824;
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

oswinexp struct mk_vertex {
  double xyzw[4];
};

oswinexp struct mk_vertices {
  struct mk_vertex *vertexL;
  int cnt,maxcnt;
};

/* in number , return -1,1 */
inline int oswinexp mk_dsgn(double d) {
  return (((((const unsigned char *)&d)[7])>>7)>0 ? -1 : 1);
} // little endian 0 -> 7

/* in number , return -1,1 */
inline double oswinexp mk_dsign (double d) {
  return (double)mk_dsgn(d);
}

/* in number , return 0 or -1,1 */
inline int oswinexp mk_isInf(double d) {
#if defined (_MSC_VER)
  return (_fpclass(d)==_FPCLASS_NINF ? -1 : (_fpclass(d)==_FPCLASS_PINF ? 1 : 0));
#else
  return (finite(d) ? 0 : mk_dsgn(d));
#endif
}

/* in number , return 0 or -1,1 */
inline int oswinexp mk_isNan(double d) {
#if defined (_MSC_VER)
  return (_isnan(d) ? mk_dsgn(d) : 0);
#else
  return (isnan(d) ? mk_dsgn(d) : 0);
#endif
}

/* in number , return 0 or -1,1 */
inline int oswinexp mk_isFinite(double d) {
#if defined (_MSC_VER)
  return (_finite(d) ? mk_dsgn(d) : 0);
#else
  return (finite(d) ? mk_dsgn(d) : 0);
#endif
}

/* in number , return 0 or -1,1 */
inline int oswinexp mk_isBusted(double d) {
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
xtern mk_ulreal oswinexp mk_nextCnt();

/* return time-cnt */
xtern mk_ulreal oswinexp mk_nextT();

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

/* inout str , return 0,1 */
xtern int oswinexp mk_exp2simple (char numstr[mk_klen]);

/* in str , in str dec=. , in str group=0 , return 0,1 */
xtern int oswinexp mk_removeSeparators(char *,const char * const,const char * const);

/* in str , in maxlen , in str dec=0 , in str group=0 , return 0,1 */
xtern int oswinexp mk_insertSeparators(char *,int,const char * const,const char * const);

/* in str , inout *base=0 , out *sgn=0 , in str group=0 , return number or 0 */
xtern mk_ulreal oswinexp mk_parseInt(const char *,int *,int *,const char *);

/* in str , out *type=0 , out *prec=0 , in str dec=. , in str group=0 , return number or nan */
xtern double oswinexp mk_parseFloat(const char *,int *,int *,const char *,const char *);

/* in str , inout *base=0 , out *sign=0 , in str group=0 , return number */
xtern mk_ulreal oswinexp mk_a2ui_(int,...);

/* in str , inout *base=0 , in str group=0 , return number */
xtern mk_lreal oswinexp mk_a2i(const char *,int *,const char *);

/* in number , out str , in base=10 , in width=0 , in padzero=1 , in str group=0 , return base */
xtern int oswinexp mk_ui2a(mk_ulreal,char [mk_klen],int,int,int,const char *);

/* in number , out str , in base=10 , in width=0 , in padzero=1 , in str group=0 , return base */
xtern int oswinexp mk_i2a(mk_lreal,char [mk_klen],int,int,int,const char *);

/* in number , in number , out *overflow=0 , return result */
xtern mk_ulreal oswinexp mk_binAdd(mk_ulreal,mk_ulreal,int *);

/* in number , in number , out *overflow=0 , return result */
xtern mk_ulreal oswinexp mk_binMult(mk_ulreal,mk_ulreal,int *);

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

/* in number , in number , out *cmp=0 , return 0,1 */
xtern int oswinexp mk_dbusted(double,double,int *);

/* in number , in number , return 0,1 */
xtern int oswinexp mk_deq(double,double);

/* in number , in number , return 0,1 */
xtern int oswinexp mk_dlt(double,double);

/* in number , in number , return 0,1 */
xtern int oswinexp mk_dgt(double,double);

/* in number , in number , in epsilon=e-15 , in toleranceafterepsilondiff=e-6 , return result */
xtern double oswinexp mk_diff_(int,...);

/* in xlow , in xhigh , in ylow , in yhigh , in xarg , return result */
xtern double oswinexp mk_linEq(double,double,double,double,double);

/* in xlow , in xhigh , in ylow , in yhigh , in xarg , return result */
xtern double oswinexp mk_lgEq(double,double,double,double,double);

/* in number , in pos=0 , in toleranceafterposition=e-4 , return rounded-number */
xtern double oswinexp mk_round2_(int,...);

/* in number , in pos , in toleranceafter=3 , return rounded-number */
xtern double oswinexp mk_roundUp_(int,...);

/* in number , in pos , in toleranceafter=3 , return rounded-number */
xtern double oswinexp mk_roundDown_(int,...);

/* in number , in base , return result */
xtern double oswinexp mk_logm(double,double);

/* in number , in exponent , return result */
xtern double oswinexp mk_powrat(double,float);

/* in number , in array , out indexarray */
xtern void oswinexp mk_indextab1(int,const double[],int []);

/* in number , in array , out indexarray */
xtern void oswinexp mk_iindextab1(int,const int [],int []);

/* in number , in array , out indexarray */
xtern void oswinexp mk_indextab2(int,const double [],int []);

/* in number , in array , out indexarray */
xtern void oswinexp mk_iindextab2(int,const int [],int []);

/* in number , return 0,1 */
xtern int oswinexp mk_isPrim(int);

/* in number , return 0,1 */
xtern int oswinexp mk_isPrim3(int);

/* in number , return result */
xtern int oswinexp mk_nextLargerPrim(int);

/* in number , in number , return result */
xtern int oswinexp /*lowest common multiplier*/ mk_lcm(int,int);

/* in number , in number , return result */
xtern int oswinexp /*greatest common divisor*/ mk_gcd(int,int);

/* in number , in number , return result */
xtern double oswinexp mk_doubleMod(double,double);

/* in number , return result */
xtern double oswinexp mk_factorial(int);

/* in number set , in number subset , return result */
xtern double oswinexp mk_binomialCoeff(int,int);

/*
  in rect.left , in rect.right , in rect.bottom , in rect.top ,
  in line.p1.x , in line.p1.y ,  in line.p2.x , in line.p2.y ,
  out *p.bottom.x , out *p.bottom.y , out *p.left.x , out *p.left.y ,
  out *p.top.x , out *p.top.y , out *p.right.x , out *p.right.y ,
  out *cutbottom 0,1 , out *cutleft 0,1 , out *cuttop 0,1 , out *cutright 0,1 ,
  in cliptorect 0,1
*/
xtern void oswinexp mk_intersectionpointsLineRect(
  double,double,double,double,double,double,double,double,double *,double *,double *,double *,
  double *,double *,double *,double *,int *,int *,int *,int *,int);

/*
  in polygon1.numbervertices , in *polygon1.px , in *polygon1.py ,
  in polygon2.numbervertices , in *polygon2.px , in *polygon2.py ,
  out *intersection.px , out *intersection.py , return numberintersectionpoints
*/
xtern int oswinexp mk_polygonintersection(
  int,double *,double *,int,double *,double *,double *,double *);

/*
  in line1.x1 , in line1.y1 , in line1.x2 , in line1.y2 ,
  in line2.x1 , in line2.y1 , in line2.x2 , in line2.y2 ,
  out *intersection.px , out *intersection.py ,
  in prec=3 , in xbox=0 0,1 , in ybox=0 0,1 , return 0,1
*/
xtern int oswinexp mk_linesintersection(
  double,double,double,double,double,double,double,double,double *,double *,int,int,int);

/* in numberelements , inout **array , in cmp-function */
xtern void oswinexp mk_heapsort(int,void **,int (*)(const void *,const void *));

/*
  in *look-for , in numberelements , in **array , in cmp-function ,
  in guess-likely-index=-1 , return index-in-array
*/
xtern int oswinexp mk_binsearch(const void *,int,const void **,int (*)(const void *,const void *),int);

/*
  in *vertex-copy-from , out *vertex-copy-to , return 0,1
*/
xtern int oswinexp mk_vertexcopy(struct mk_vertex *,struct mk_vertex *);

/*
  in *vertex , return length
*/
xtern double oswinexp mk_vertexlen(struct mk_vertex *);

/*
  in *vertex-cmp1 , in *vertex-cmp2 , return -1,0,1
*/
xtern int oswinexp mk_vertexcmp(struct mk_vertex *,struct mk_vertex *);

/*
  inout *vertex-mod , in *vertex-addend  , return 0,1
*/
xtern int oswinexp mk_vertexadd(struct mk_vertex *,struct mk_vertex *);

/*
  inout *vertex-mod , in *vertexaddend  , return 0,1
*/
xtern int oswinexp mk_vertexsubs(struct mk_vertex *,struct mk_vertex *);

/*
  inout *vertex-mod , in multiplier  , return 0,1
*/
xtern int oswinexp mk_vertexmult(struct mk_vertex *,double);

/*
  inout *vertex-mod , in divisor  , return 0,1
*/
xtern int oswinexp mk_vertexdiv(struct mk_vertex *,double);

/*
  in *vertex1 , in *vertex2  , return dotproduct
*/
xtern double oswinexp mk_vertexdot(struct mk_vertex *,struct mk_vertex *);

/*
  inout *vertex-mod , return 0,1
*/
xtern int oswinexp mk_vertexnorm(struct mk_vertex *);

/*
  inout *mod-vertex , in *multiplier  , return 0,1
*/
xtern int oswinexp mk_vertexcross(struct mk_vertex *,struct mk_vertex *);

/*
  in *vertex1 , int *vertex2 , return angle(rad)-vertex1-vertex2
*/
xtern double oswinexp mk_vertexangrad(struct mk_vertex *,struct mk_vertex *);

/*
  in *vertex1 , in *vertex2 , return angle(deg)-vertex1-vertex2
*/
xtern double oswinexp mk_vertexangdeg(struct mk_vertex *,struct mk_vertex *);

/*
  inout list-of-vertices* , return 0,1
*/
xtern int oswinexp mk_verticesalloc(struct mk_vertices *);

/*
  in list-of-vertices* , int arrindex , return item-at-index*
*/
xtern struct mk_vertex *oswinexp mk_verticesget(struct mk_vertices *,int);

/*
  inout list-of-*vertices , in *vertex , in maxcntvertices , return list-index
*/
xtern int oswinexp mk_verticesappend(struct mk_vertices *,struct mk_vertex *);

#ifdef __cplusplus
}
#endif

#endif
