
#ifndef _mkconv_h_
#define _mkconv_h_

#include <stdint.h>
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <mkbase/defs.h>
#include <mkbase/exportdefs.h>

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

#ifdef __cplusplus
/* #pragma message "cplusplus" */
extern "C" {
#endif

/* in str , in str dec=. , in str group=0 , return 0,1 */
xtern int oswinexp mk_removeseparators(char *,char *,char *);

/* in str , in maxlen , in str dec=0 , in str group=0 , return 0,1 */
xtern int oswinexp mk_insertseparators(char *,char *,char *);

/* in str , inout *base=0 , out *sgn=0 , in str group=0 , return number or 0 */
xtern mk_ulreal oswinexp mk_parseint(char *,int *,int *,char *);

/* in str , out *type=0 , out *prec=0 , in str dec=. , in str group=0 , return number or nan */
xtern double oswinexp mk_parsefloat(char *,int *,int *,char *,char *);

/* in str , inout *base=0 , out *sign=0 , in str group=0 , return number */
xtern mk_ulreal oswinexp mk_a2ui_(int,...);

/* in str , inout *base=0 , in str group=0 , return number */
xtern mk_lreal oswinexp mk_a2i_(int,...);

/* in number , out str , in base=10 , in width=0 , in padzero=1 , in str group=0 , return base */
xtern int oswinexp mk_ui2str(mk_ulreal,char *,int,int,int,char *);

/* in number , in base=10 , in width=0 , in padzero=1 , in str group=0 , return str 
   allocates memory
*/
xtern char * oswinexp mk_ui2a_(int,...);

/* in number , out str , in base=10 , in width=0 , in padzero=1 , in str group=0 , return base */
xtern int oswinexp mk_i2str(mk_lreal,char *,int,int,int,char *);

/* in number , in base=10 , in width=0 , in padzero=1 , in str group=0 , return str 
   allocates memory
*/
xtern char * oswinexp mk_i2a_(int,...);

/* in str , out *prec=0 , in str dec=. , in str group=0 , return number or nan */
xtern double oswinexp mk_a2d_(int,...);

/* in number , out str , in prec , in fmt=-1 , in pad=-1 , return 0,1 */
xtern int oswinexp mk_dconvert(double,char *,int,char,int);

/*
  in number , out str , in prec=15 , in fmt=-1 , in pad=-1 , 
  in str dec=. , in str group=0 , return 0,1
*/
xtern int oswinexp mk_d2str(double,char *,int,char,int,char *,char *);

/*
  in number , out str , in prec=15 , in fmt=-1 , in pad=-1 , 
  in str dec=. , in str group=0 , return str
  allocates memory
*/
xtern char * oswinexp mk_d2a_(int,...);

/* inout str , return 0,1 */
xtern int oswinexp mk_exp2simple(char *);

#ifdef __cplusplus
}
#endif

#endif
