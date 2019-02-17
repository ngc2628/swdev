
#ifndef _mkmath_h_
#define _mkmath_h_

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <mkbase/defs.h>
#include <mkbase/exportdefs.h>

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

#ifdef __cplusplus
/* #pragma message "cplusplus" */
extern "C" {
#endif

/* get sign , in number , return -1,1 */
xtern int oswinexp mk_dsgn(double);

/* get sign , in number , return -1,1 */
xtern double oswinexp mk_dsign(double);

/* in number , return 0 or -1,1 */
xtern int oswinexp mk_isinf(double);

/* in number , return 0 or -1,1 */
xtern int oswinexp mk_isnan(double);

/* in number , return 0 or -1,1 */
xtern int oswinexp mk_isfinite(double);

/* is or not a regular number , in number , return 0 or -1,1 */
xtern int oswinexp mk_isbusted(double);

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

/* in number , in number , out *overflow=0 , return result */
xtern mk_ulreal oswinexp mk_binadd(mk_ulreal,mk_ulreal,int *);

/* in number , in number , out *overflow=0 , return result */
xtern mk_ulreal oswinexp mk_binmult(mk_ulreal,mk_ulreal,int *);

/* in number , in calcedge=1 , return magnitude-of-number */
xtern int oswinexp mk_mag_(int,...);

/* [diff of] regular or no regular numbers , in number , in number , 
   out *cmp=0 [-1,0,1] , return 0,1 
*/
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

/* in number , return result */
xtern double oswinexp mk_factorial(int);

/* in number set , in number subset , return result */
xtern double oswinexp mk_binomialcoeff(int,int);

#ifdef __cplusplus
}
#endif

#endif
