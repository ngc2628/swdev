
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
