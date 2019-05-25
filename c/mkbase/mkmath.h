
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

/* ucd:number as char , ff:number as float 
union tp_ucpf {
  unsigned char ucf[4];
  float ff;
};
big endian
union tp_ucpf mk_deffnan={{127,192,0,0}};
union tp_ucpf mk_deffsnan={{255,192,0,0}};
union tp_ucpf mk_deffinf={{127,128,0,0}};
union tp_ucpf mk_deffsinf={{255,128,0,0}};
little endian
union tp_ucpf mk_deffnan={{0,0,192,127}};
union tp_ucpf mk_deffsnan={{0,0,192,255}};
union tp_ucpf mk_deffinf={{0,0,128,127}};
union tp_ucpf mk_deffsinf={{0,0,128,255}};
#define mk_fnan *((const float *)&mk_deffnan.ff)
#define mk_fsnan *((const float *)&mk_deffsnan.ff)
#define mk_finf *((const float *)&mk_deffinf.ff)
#define mk_fsinf *((const float *)&mk_deffsinf.ff)
*/

/* ucd:number as char , dd:number as double */ 
union tp_ucpd {
  unsigned char ucd[8];
  double dd;
};

extern const union tp_ucpd mk_defdnan; 
extern const union tp_ucpd mk_defdsnan;
extern const union tp_ucpd mk_defdinf;
extern const union tp_ucpd mk_defdsinf;

#define mk_dnan mk_defdnan.dd
#define mk_dsnan mk_defdsnan.dd
#define mk_dinf mk_defdinf.dd
#define mk_dsinf mk_defdsinf.dd

extern const mk_lreal mk_i64limit;
extern const mk_ulreal mk_ui64limit;
extern const unsigned int mk_uilimit;
extern const int mk_ilimit;
extern const unsigned short mk_uslimit;
extern const short mk_slimit;
extern const double mk_dlimit;
extern const double mk_derr;
extern const double mk_euler;
extern const double mk_pi;
extern const double mk_rad;
extern const double mk_log210;
extern const double mk_loge10;
extern const double mk_log10e;
extern const unsigned short mk_dmag;
extern const unsigned short mk_i64mag;
extern const unsigned short mk_ui64mag;
extern const unsigned short mk_dprec;
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

/* in bits-to-shift (0...63) , return 1<<bits */
xtern mk_ulreal oswinexp mk_1sh(int);

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
