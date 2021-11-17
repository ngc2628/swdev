
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

extern unsigned char mk_basechar[38];
#define mk_asciispace 32
#define mk_asciihash 35
#define mk_asciiplus 43
#define mk_asciiminus 45
#define mk_asciidec 46
#define mk_asciizero 48
#define mk_asciione 49
#define mk_asciitwo 50
#define mk_asciithree 51
#define mk_asciifour 52
#define mk_asciifive 53
#define mk_asciisix 54
#define mk_asciiseven 55
#define mk_asciieight 56
#define mk_asciinine 57
#define mk_asciiA 65
#define mk_asciiB 66
#define mk_asciiC 67
#define mk_asciiD 68
#define mk_asciiE 69
#define mk_asciiF 70
#define mk_asciiG 71
#define mk_asciiH 72
#define mk_asciiI 73
#define mk_asciiJ 74
#define mk_asciiK 75
#define mk_asciiL 76
#define mk_asciiM 77
#define mk_asciiN 78
#define mk_asciiO 79
#define mk_asciiP 80
#define mk_asciiQ 81
#define mk_asciiR 82
#define mk_asciiS 83
#define mk_asciiT 84
#define mk_asciiU 85
#define mk_asciiV 86
#define mk_asciiW 87
#define mk_asciiX 88
#define mk_asciiY 89
#define mk_asciiZ 90
#define mk_ascii_ 95
#define mk_asciib 98
#define mk_asciie 101
#define mk_asciif 102
#define mk_asciii 105
#define mk_asciix 120
#define mk_asciidel 127

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
