
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
#include <mkbase/mkutil.h>

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

/* in string , in string dec=. , in string group=0 , return 0|1 */
xtern int oswinexp mk_removeseparators(char *,char *,char *);

/* in string , in maxlen , in string dec=0 , in string group=0 , return 0|1 */
xtern int oswinexp mk_insertseparators(char *,char *,char *);

/* in string , inout *base=0 , out *sgn=0 , in str group=0 , return number=0 */
xtern mk_ulreal oswinexp mk_parseint(char *,int *,int *,char *);

/* in string , out *type , out *prec , 
   in string dec=. , in string group , return number=nan 
*/
xtern double oswinexp mk_parsefloat(char *,int *,int *,char *,char *);

/* in string , inout *base , out *sign , in string group , return number=0 */
xtern mk_ulreal oswinexp mk_str2ui(char *,int *,int *,char *);

/* in string , inout *base=0 , out *sign=0 , in str group=0 , return number=0 */
xtern mk_ulreal oswinexp mk_a2ui_(int,...);

/* in string , inout *base=0 , in string group=0 , return number=0 */
xtern mk_lreal oswinexp mk_a2i_(int,...);

/* in number , out string , in base=10 , in width=0 , in padzero=1 , 
   in string group=0 , return base=0
*/
xtern int oswinexp mk_ui2str(mk_ulreal,mk_string,int,int,int,char *);

/* in number=0 , out string=0 , in base=10 , in width=0 , 
   in padzero=1 , in string group=0 , return base=0
*/
xtern int oswinexp mk_ui2a_(int,...);

/* in number=0 , out string=0 , in base=10 , in width=0 , 
  in padzero=1 , in string group=0 , return base=0
*/
xtern int oswinexp mk_i2str(mk_lreal,mk_string,int,int,int,char *);

/* in number=0 , out string=0 , in base=10 , in width=0 , 
   in padzero=1 , in str string=0 , return base=0
*/
xtern int oswinexp mk_i2a_(int,...);

/* in string , out *prec , in string dec , in string group , return number|nan */
xtern double oswinexp mk_str2d(char *,int *,char *,char *);

/* in string , out *prec=0 , in string dec=. , in string group=0 , return number|nan */
xtern double oswinexp mk_a2d_(int,...);

/* in number , out string , in prec , in fmt , in pad , return 0,1 */
xtern int oswinexp mk_dconvert(double,mk_string,int,char,int);

/*
  in number , out string , in prec , in fmt , in pad , 
  in string dec=. , in string group=0 , return 0,1
*/
xtern int oswinexp mk_d2str(double,mk_string,int,char,int,char *,char *);

/*
  in number=0 , out string=0 , in prec=15 , in fmt=-1 , in pad=-1 , 
  in string dec=. , in string group=0 , return 0|1
  allocates memory
*/
xtern int oswinexp mk_d2a_(int,...);

/* inout str , return 0,1 */
xtern int oswinexp mk_exp2simple(char *);

#ifdef __cplusplus
}
#endif

#endif
