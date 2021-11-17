
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

/* cc:number as char , dd:number as double */ 
union mk_ncd {
  unsigned char cc[8];
  double dd;
};

extern union mk_ncd mk_defdnan; 
extern union mk_ncd mk_defdsnan;
extern union mk_ncd mk_defdinf;
extern union mk_ncd mk_defdsinf;

#define mk_dnan mk_defdnan.dd
#define mk_dsnan mk_defdsnan.dd
#define mk_dinf mk_defdinf.dd
#define mk_dsinf mk_defdsinf.dd

#if defined (_MSC_VER)
#define mk_i64limit 9223372036854775807i64
#define mk_ui64limit 18446744073709551615ui64
#define mk_uilimit 4294967295
#else
#if defined (__WATCOMC__)
#define mk_i64limit 9223372036854775807LL
#define mk_ui64limit 18446744073709551615ULL
#define mk_uilimit 4294967295U
#else
#if defined (__MACH__)
#define mk_i64limit 9223372036854775807LL
#define mk_ui64limit 18446744073709551615ULL
#define mk_uilimit 4294967295U
#else
#define mk_i64limit 9223372036854775807LL
#define mk_ui64limit 18446744073709551615ULL
#define mk_uilimit 4294967295U
#endif
#endif
#endif
#define mk_ilimit 2147483647
#define mk_uslimit 65535
#define mk_slimit 32767
#define mk_dlimit 1.7976931348623157e308
#define mk_derr 0.000000000000001
#define mk_euler 2.718281828459045235
#define mk_pi 3.1415926535897932385
#define mk_rad 0.0174532925199432957694
#define mk_log210 0.3010299956639812
#define mk_loge10 0.4342944819032518
#define mk_log10e 2.3025850929940457
#define mk_dmag 308
#define mk_i64mag 18
#define mk_ui64mag 19
#define mk_dprec 15

/* 1<<0 ... 1<<63
  1                      ,2                      ,4                      ,8                      ,
  16                     ,32                     ,64                     ,128                    ,
  256                    ,512                    ,1024                   ,2048                   ,
  4096                   ,8192                   ,16384                  ,32768                  ,
  65536                  ,131072                 ,262144                 ,524288                 ,
  1048576                ,2097152                ,4194304                ,8388608                ,
  16777216               ,33554432               ,67108864               ,134217728              ,
  268435456              ,536870912              ,1073741824             ,2147483648U            ,
  4294967296ULL          ,8589934592ULL          ,17179869184ULL         ,34359738368ULL         ,
  68719476736ULL         ,137438953472ULL        ,274877906944ULL        ,549755813888ULL        ,
  1099511627776ULL       ,2199023255552ULL       ,4398046511104ULL       ,8796093022208ULL       ,
  17592186044416ULL      ,35184372088832ULL      ,70368744177664ULL      ,140737488355328ULL     ,
  281474976710656ULL     ,562949953421312ULL     ,1125899906842624ULL    ,2251799813685248ULL    ,
  4503599627370496ULL    ,9007199254740992ULL    ,18014398509481984ULL   ,36028797018963968ULL   ,
  72057594037927936ULL   ,144115188075855872ULL  ,288230376151711744ULL  ,576460752303423488ULL  ,
  1152921504606846976ULL ,2305843009213693952ULL ,4611686018427387904ULL ,9223372036854775808ULL
*/

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

/* in number-1 , in number-2 , return -1|0|1 */
xtern int oswinexp mk_cmpdouble(const void *,const void *);

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
